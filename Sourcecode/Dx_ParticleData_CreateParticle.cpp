//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　         ParticleDataクラス                                 **//
//**                                  CreateParticle関数                                 **//
//*****************************************************************************************//

#include "Dx12Process.h"

ParticleData::ParticleData() {
	dx = Dx12Process::GetInstance();
	mCommandList = dx->dx_sub[0].mCommandList.Get();
	ver = 0;
	P_pos = NULL;
}

void ParticleData::SetCommandList(int no) {
	com_no = no;
	mCommandList = dx->dx_sub[com_no].mCommandList.Get();
}

ParticleData::~ParticleData() {
	S_DELETE(mObjectCB);
	if (P_pos != NULL) {
		free(P_pos);
		P_pos = NULL;
	}
	RELEASE(texture);
	RELEASE(textureUp);
}

void ParticleData::GetShaderByteCode() {
	gsSO = dx->pGeometryShader_PSO.Get();
	gs = dx->pGeometryShader_P.Get();
	vsSO = dx->pVertexShader_PSO.Get();
	vs = dx->pVertexShader_P.Get();
	ps = dx->pPixelShader_P.Get();
}

void ParticleData::MatrixMap(UploadBuffer<CONSTANT_BUFFER_P> *mObjectCB, float x, float y, float z, float theta, float size, bool init, float speed, bool tex) {
	MATRIX mov;
	MATRIX rot;
	MATRIX world;
	CONSTANT_BUFFER_P cb;

	MatrixRotationZ(&rot, theta);
	MatrixTranslation(&mov, x, y, z);
	MatrixMultiply(&world, &rot, &mov);
	MatrixMultiply(&cb.WV, &world, &dx->mView);
	cb.Proj = dx->mProj;
	MatrixTranspose(&cb.WV);
	MatrixTranspose(&cb.Proj);
	cb.size.x = size;
	if (init)cb.size.y = 1.0f; else cb.size.y = 0.0f;
	cb.size.z = speed;
	if (tex)cb.size.w = 1.0f; else cb.size.w = 0.0f;
	mObjectCB->CopyData(0, cb);
}

void ParticleData::GetVbColarray(int texture_no, float size, float density) {
	UINT64  total_bytes = 0;
	dx->md3dDevice->GetCopyableFootprints(&dx->texture[texture_no]->GetDesc(), 0, 1, 0, &footprint, nullptr, nullptr, &total_bytes);

	//テクスチャの横サイズ取得
	float width = (float)dx->texture[texture_no]->GetDesc().Width;
	//テクスチャの縦サイズ取得
	float height = (float)dx->texture[texture_no]->GetDesc().Height;

	//ステップ数
	float step = 1 / size / density;

	//RESOURCE_BARRIERはテクスチャ取得時WICTextureLoader内でD3D12_RESOURCE_STATE_GENERIC_READになってるのでそのまま

	//頂点個数カウント
	D3D12_SUBRESOURCE_DATA texResource;
	dx->textureUp[texture_no]->Map(0, nullptr, reinterpret_cast<void**>(&texResource));

	texResource.RowPitch = footprint.Footprint.RowPitch;
	UCHAR *ptex = (UCHAR*)texResource.pData;
	for (float j = 0; j < height; j += step) {
		UINT j1 = (UINT)j * texResource.RowPitch;//RowPitchデータの行ピッチ、行幅、または物理サイズ (バイト単位)
		for (float i = 0; i < width; i += step) {
			UINT ptexI = (UINT)i * 4 + j1;
			if (ptex[ptexI + 3] > 0)ver++;//アルファ値0より高い場合カウント
		}
	}

	dx->textureUp[texture_no]->Unmap(0, nullptr);

	//パーティクル配列確保
	P_pos = (PartPos*)malloc(sizeof(PartPos) * ver);

	//ピクセルデータ読み込み
	dx->textureUp[texture_no]->Map(0, nullptr, reinterpret_cast<void**>(&texResource));

	texResource.RowPitch = footprint.Footprint.RowPitch;
	ptex = (UCHAR*)texResource.pData;
	int P_no = 0;
	float ws = width * size / 2;//中心を0,0,0にする為
	float hs = height * size / 2;
	for (float j = 0; j < height; j += step) {
		UINT j1 = (UINT)j * texResource.RowPitch;//RowPitchデータの行ピッチ、行幅、または物理サイズ (バイト単位)
		for (float i = 0; i < width; i += step) {
			UINT ptexI = (UINT)i * 4 + j1;
			float yp = (float)(j * size - hs);
			if (ptex[ptexI + 3] > 0) {
				float xp = (float)(i * size - ws);
				P_pos[P_no].Col.as(ptex[ptexI + 0], ptex[ptexI + 1], ptex[ptexI + 2], ptex[ptexI + 3]);//色
				float xst = xp + ((rand() % 500) * size) - ((rand() % 500) * size);
				float yst = yp + ((rand() % 500) * size) - ((rand() % 500) * size);
				float zst = (rand() % 500) * size;
				P_pos[P_no].CurrentPos.as(xst, yst, zst);
				P_pos[P_no].PosSt.as(xst, yst, zst);
				P_pos[P_no].PosEnd.as(xp, yp, 0.0f);//0,0,0を中心にする
				P_no++;
			}
		}
	}

	dx->textureUp[texture_no]->Unmap(0, nullptr);
}

void ParticleData::GetVBarray(int v) {
	ver = v;
	P_pos = (PartPos*)malloc(sizeof(PartPos) * ver);
}

void ParticleData::CreateVbObj() {
	const UINT vbByteSize = ver * sizeof(PartPos);
	Vview = std::make_unique<VertexView>();

	D3DCreateBlob(vbByteSize, &Vview->VertexBufferCPU);
	CopyMemory(Vview->VertexBufferCPU->GetBufferPointer(), P_pos, vbByteSize);

	Vview->VertexBufferGPU = dx->CreateDefaultBuffer(dx->md3dDevice.Get(),
		mCommandList, P_pos, vbByteSize, Vview->VertexBufferUploader);

	Vview->VertexByteStride = sizeof(PartPos);
	Vview->VertexBufferByteSize = vbByteSize;

	Sview1 = std::make_unique<StreamView[]>(2);
	Sview2 = std::make_unique<StreamView[]>(2);
	for (int i = 0; i < 2; i++) {
		Sview1[i].StreamBufferGPU = dx->CreateStreamBuffer(dx->md3dDevice.Get(), vbByteSize);
		Sview2[i].StreamBufferGPU = dx->CreateStreamBuffer(dx->md3dDevice.Get(), vbByteSize);

		Sview1[i].StreamByteStride = Sview2[i].StreamByteStride = sizeof(PartPos);
		Sview1[i].StreamBufferByteSize = Sview2[i].StreamBufferByteSize = vbByteSize;
		Sview1[i].BufferFilledSizeLocation = Sview2[i].StreamBufferGPU->GetGPUVirtualAddress();
	}
}

void ParticleData::CreatePartsCom() {

	CD3DX12_ROOT_PARAMETER slotRootParameter_com[1];
	slotRootParameter_com[0].InitAsConstantBufferView(0);

	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc_com(1, slotRootParameter_com,
		NULL, NULL,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_STREAM_OUTPUT | D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	Microsoft::WRL::ComPtr<ID3DBlob> serializedRootSig_com = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob_com = nullptr;
	D3D12SerializeRootSignature(&rootSigDesc_com, D3D_ROOT_SIGNATURE_VERSION_1,
		serializedRootSig_com.GetAddressOf(), errorBlob_com.GetAddressOf());

	if (errorBlob_com != nullptr)
	{
		::OutputDebugStringA((char*)errorBlob_com->GetBufferPointer());
	}

	dx->md3dDevice->CreateRootSignature(
		0,
		serializedRootSig_com->GetBufferPointer(),
		serializedRootSig_com->GetBufferSize(),
		IID_PPV_ARGS(mRootSignature_com.GetAddressOf()));

	//パイプラインステートオブジェクト生成(STREAM_OUTPUT)
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	psoDesc.InputLayout = { dx->pVertexLayout_P.data(), (UINT)dx->pVertexLayout_P.size() };
	psoDesc.pRootSignature = mRootSignature_com.Get();
	psoDesc.VS =
	{
		reinterpret_cast<BYTE*>(vsSO->GetBufferPointer()),
		vsSO->GetBufferSize()
	};
	psoDesc.GS =
	{
		reinterpret_cast<BYTE*>(gsSO->GetBufferPointer()),
		gsSO->GetBufferSize()
	};
	//ストリーム
	psoDesc.StreamOutput.pSODeclaration = dx->pDeclaration_PSO.data();
	psoDesc.StreamOutput.NumEntries = 4;  //D3D12_SO_DECLARATION_ENTRYの個数
	psoDesc.StreamOutput.pBufferStrides = &Sview1[0].StreamByteStride;
	psoDesc.StreamOutput.NumStrides = 1;//バッファの数？
	psoDesc.StreamOutput.RasterizedStream = D3D12_SO_NO_RASTERIZED_STREAM;

	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.BlendState.IndependentBlendEnable = FALSE;
	psoDesc.BlendState.AlphaToCoverageEnable = TRUE;//アルファテストon/off
	psoDesc.BlendState.RenderTarget[0].BlendEnable = TRUE;//ブレンドon/off
	psoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	psoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = dx->mBackBufferFormat;
	psoDesc.SampleDesc.Count = dx->m4xMsaaState ? 4 : 1;
	psoDesc.SampleDesc.Quality = dx->m4xMsaaState ? (dx->m4xMsaaQuality - 1) : 0;
	psoDesc.DSVFormat = dx->mDepthStencilFormat;
	dx->md3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPSO_com));
}

void ParticleData::CreatePartsDraw(int texpar) {

	if (texpar != -1)texpar_on = TRUE;

	CD3DX12_DESCRIPTOR_RANGE texTable;
	texTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	CD3DX12_ROOT_PARAMETER slotRootParameter_draw[2];
	slotRootParameter_draw[0].InitAsDescriptorTable(1, &texTable, D3D12_SHADER_VISIBILITY_ALL);// DescriptorRangeの数は1つ, DescriptorRangeの先頭アドレス
	slotRootParameter_draw[1].InitAsConstantBufferView(0);

	auto staticSamplers = dx->GetStaticSamplers();

	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc_draw(2, slotRootParameter_draw,
		(UINT)staticSamplers.size(), staticSamplers.data(),
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	Microsoft::WRL::ComPtr<ID3DBlob> serializedRootSig_draw = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob_draw = nullptr;
	D3D12SerializeRootSignature(&rootSigDesc_draw, D3D_ROOT_SIGNATURE_VERSION_1,
		serializedRootSig_draw.GetAddressOf(), errorBlob_draw.GetAddressOf());

	if (errorBlob_draw != nullptr)
	{
		::OutputDebugStringA((char*)errorBlob_draw->GetBufferPointer());
	}

	dx->md3dDevice->CreateRootSignature(
		0,
		serializedRootSig_draw->GetBufferPointer(),
		serializedRootSig_draw->GetBufferSize(),
		IID_PPV_ARGS(mRootSignature_draw.GetAddressOf()));

	//シェーダーリソースビュー(directshow用)
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = 1;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	dx->md3dDevice->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&mSrvHeap));

	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(mSrvHeap->GetCPUDescriptorHandleForHeapStart());

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	if (!m_on && texpar != -1) {
		srvDesc.Format = dx->texture[texpar]->GetDesc().Format;
		srvDesc.Texture2D.MipLevels = dx->texture[texpar]->GetDesc().MipLevels;
		dx->md3dDevice->CreateShaderResourceView(dx->texture[texpar], &srvDesc, hDescriptor);
	}
	if (m_on) {
		srvDesc.Format = texture->GetDesc().Format;
		srvDesc.Texture2D.MipLevels = texture->GetDesc().MipLevels;
		dx->md3dDevice->CreateShaderResourceView(texture, &srvDesc, hDescriptor);
	}

	//パイプラインステートオブジェクト生成(Draw)
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	psoDesc.InputLayout = { dx->pVertexLayout_P.data(), (UINT)dx->pVertexLayout_P.size() };
	psoDesc.pRootSignature = mRootSignature_draw.Get();
	psoDesc.VS =
	{
		reinterpret_cast<BYTE*>(vs->GetBufferPointer()),
		vs->GetBufferSize()
	};
	psoDesc.GS =
	{
		reinterpret_cast<BYTE*>(gs->GetBufferPointer()),
		gs->GetBufferSize()
	};
	psoDesc.PS =
	{
		reinterpret_cast<BYTE*>(ps->GetBufferPointer()),
		ps->GetBufferSize()
	};

	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.BlendState.IndependentBlendEnable = FALSE;
	psoDesc.BlendState.AlphaToCoverageEnable = TRUE;//アルファテストon/off
	psoDesc.BlendState.RenderTarget[0].BlendEnable = TRUE;//ブレンドon/off
	psoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	psoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = dx->mBackBufferFormat;
	psoDesc.SampleDesc.Count = dx->m4xMsaaState ? 4 : 1;
	psoDesc.SampleDesc.Quality = dx->m4xMsaaState ? (dx->m4xMsaaQuality - 1) : 0;
	psoDesc.DSVFormat = dx->mDepthStencilFormat;
	dx->md3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPSO_draw));
}

void ParticleData::CreateParticle(int texture_no, int texpar, float size, float density) {
	GetShaderByteCode();
	mObjectCB = new UploadBuffer<CONSTANT_BUFFER_P>(dx->md3dDevice.Get(), 1, true);
	GetVbColarray(texture_no, size, density);
	CreateVbObj();
	CreatePartsCom();
	CreatePartsDraw(texpar);
}

void ParticleData::CreateBillboard() {
	GetShaderByteCode();
	mObjectCB = new UploadBuffer<CONSTANT_BUFFER_P>(dx->md3dDevice.Get(), 1, true);
	CreateVbObj();
	CreatePartsDraw(-1);
}

void ParticleData::DrawParts0(float x, float y, float z, float theta, float size, bool init, float speed, bool tex) {
	MatrixMap(mObjectCB, x, y, z, theta, size, init, speed, tex);

	mCommandList->RSSetViewports(1, &dx->mScreenViewport);
	mCommandList->RSSetScissorRects(1, &dx->mScissorRect);

	//mSwapChainBuffer PRESENT→RENDER_TARGET
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(dx->mSwapChainBuffer[dx->mCurrBackBuffer].Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	//レンダーターゲットのセット
	mCommandList->OMSetRenderTargets(1, &CD3DX12_CPU_DESCRIPTOR_HANDLE(
		dx->mRtvHeap->GetCPUDescriptorHandleForHeapStart(),
		dx->mCurrBackBuffer,
		dx->mRtvDescriptorSize), true, &dx->mDsvHeap->GetCPUDescriptorHandleForHeapStart());
}

void ParticleData::DrawParts1() {
	mCommandList->SetPipelineState(mPSO_com.Get());

	mCommandList->SetGraphicsRootSignature(mRootSignature_com.Get());

	mCommandList->IASetVertexBuffers(0, 1, &Vview->VertexBufferView());
	mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	mCommandList->SetGraphicsRootConstantBufferView(0, mObjectCB->Resource()->GetGPUVirtualAddress());

	mCommandList->SOSetTargets(0, 1, &Sview1[svInd].StreamBufferView());

	mCommandList->DrawInstanced(ver, 1, 0, 0);

	svInd = 1 - svInd;
	if (firstDraw) {
		mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(Vview->VertexBufferGPU.Get(),
			D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_COPY_DEST));
		mCommandList->CopyResource(Vview->VertexBufferGPU.Get(), Sview1[svInd].StreamBufferGPU.Get());
		mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(Vview->VertexBufferGPU.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ));
	}
	mCommandList->SOSetTargets(0, 1, nullptr);
}

void ParticleData::DrawParts2() {
	mCommandList->SetPipelineState(mPSO_draw.Get());

	ID3D12DescriptorHeap* descriptorHeaps[] = { mSrvHeap.Get() };
	mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	mCommandList->SetGraphicsRootSignature(mRootSignature_draw.Get());

	mCommandList->IASetVertexBuffers(0, 1, &Vview->VertexBufferView());
	mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	mCommandList->SetGraphicsRootDescriptorTable(0, mSrvHeap->GetGPUDescriptorHandleForHeapStart());
	mCommandList->SetGraphicsRootConstantBufferView(1, mObjectCB->Resource()->GetGPUVirtualAddress());

	mCommandList->DrawInstanced(ver, 1, 0, 0);
}

void ParticleData::Draw(float x, float y, float z, float theta, float size, bool init, float speed) {

	//一回のinit == TRUE で二つのstreamOutを初期化
	if (init == TRUE) { streamInitcount = 1; }
	else {
		if (streamInitcount > 2) { streamInitcount = 0; }
		if (streamInitcount != 0) { init = TRUE; streamInitcount++; }
	}

	DrawParts0(x, y, z, theta, size, init, speed, texpar_on | m_on);
	DrawParts1();
	if (firstDraw)DrawParts2();
	//mSwapChainBuffer RENDER_TARGET→PRESENT
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(dx->mSwapChainBuffer[dx->mCurrBackBuffer].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
	firstDraw = TRUE;
}

void ParticleData::DrawBillboard(float size) {
	DrawParts0(0, 0, 0, 0, size, TRUE, 1.0f, texpar_on | m_on);
	DrawParts2();
	//mSwapChainBuffer RENDER_TARGET→PRESENT
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(dx->mSwapChainBuffer[dx->mCurrBackBuffer].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
}

void ParticleData::TextureInit(int width, int height) {

	D3D12_RESOURCE_DESC texDesc = {};
	texDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.DepthOrArraySize = 1;
	texDesc.MipLevels = 1;
	texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	texDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	D3D12_HEAP_PROPERTIES HeapProps;
	HeapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
	HeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	HeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	HeapProps.CreationNodeMask = 1;
	HeapProps.VisibleNodeMask = 1;
	HRESULT hr;
	hr = dx->md3dDevice->CreateCommittedResource(&HeapProps, D3D12_HEAP_FLAG_NONE, &texDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&texture));

	//upload
	UINT64 uploadBufferSize = GetRequiredIntermediateSize(texture, 0, 1);
	D3D12_HEAP_PROPERTIES HeapPropsUp;
	HeapPropsUp.Type = D3D12_HEAP_TYPE_UPLOAD;
	HeapPropsUp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	HeapPropsUp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	HeapPropsUp.CreationNodeMask = 1;
	HeapPropsUp.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC BufferDesc;
	BufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	BufferDesc.Alignment = 0;
	BufferDesc.Width = uploadBufferSize;
	BufferDesc.Height = 1;
	BufferDesc.DepthOrArraySize = 1;
	BufferDesc.MipLevels = 1;
	BufferDesc.Format = DXGI_FORMAT_UNKNOWN;
	BufferDesc.SampleDesc.Count = 1;
	BufferDesc.SampleDesc.Quality = 0;
	BufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	BufferDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	dx->md3dDevice->CreateCommittedResource(&HeapPropsUp, D3D12_HEAP_FLAG_NONE,
		&BufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&textureUp));

	UINT64  total_bytes = 0;
	dx->md3dDevice->GetCopyableFootprints(&texDesc, 0, 1, 0, &footprint, nullptr, nullptr, &total_bytes);

	memset(&dest, 0, sizeof(dest));
	dest.pResource = texture;
	dest.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	dest.SubresourceIndex = 0;

	memset(&src, 0, sizeof(src));
	src.pResource = textureUp;
	src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	src.PlacedFootprint = footprint;

	m_on = TRUE;
}

void ParticleData::SetTextureMPixel(int **m_pix, BYTE r, BYTE g, BYTE b, int a) {

	D3D12_RESOURCE_DESC texdesc;
	texdesc = texture->GetDesc();
	//テクスチャの横サイズ取得
	int width = (int)texdesc.Width;
	//テクスチャの縦サイズ取得
	int height = texdesc.Height;

	D3D12_SUBRESOURCE_DATA texResource;

	D3D12_RESOURCE_BARRIER BarrierDesc;
	BarrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	BarrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	BarrierDesc.Transition.pResource = texture;
	BarrierDesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	BarrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_GENERIC_READ;
	BarrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;
	mCommandList->ResourceBarrier(1, &BarrierDesc);

	textureUp->Map(0, nullptr, reinterpret_cast<void**>(&texResource));

	UCHAR *ptex = (UCHAR*)texResource.pData;
	texResource.RowPitch = footprint.Footprint.RowPitch;

	for (int j = 0; j < height; j++) {
		UINT j1 = j * texResource.RowPitch;//RowPitchデータの行ピッチ、行幅、または物理サイズ (バイト単位)
		for (int i = 0; i < width; i++) {
			UINT ptexI = i * 4 + j1;
			ptex[ptexI + 0] = m_pix[j][i] >> 16 & r;
			ptex[ptexI + 1] = m_pix[j][i] >> 8 & g;
			ptex[ptexI + 2] = m_pix[j][i] & b;

			if ((m_pix[j][i] >> 16 & b) < 50 && (m_pix[j][i] >> 8 & g) < 50 && (m_pix[j][i] & r) < 50) {
				ptex[ptexI + 3] = 0;
			}
			else {
				ptex[ptexI + 3] = a;
			}
		}
	}
	textureUp->Unmap(0, nullptr);

	mCommandList->CopyTextureRegion(&dest, 0, 0, 0, &src, nullptr);

	BarrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	BarrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	mCommandList->ResourceBarrier(1, &BarrierDesc);
}

void ParticleData::SetVertex(int i,
	float vx, float vy, float vz,
	float r, float g, float b, float a) {
	P_pos[i].CurrentPos.as(vx, vy, vz);
	P_pos[i].Col.as(r, g, b, a);
}