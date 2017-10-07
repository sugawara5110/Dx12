//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　          PolygonDataクラス                                 **//
//**                                   GetVBarray関数                                    **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include "Dx12Process.h"

std::mutex PolygonData::mtx;

PolygonData::PolygonData() {
	dx = Dx12Process::GetInstance();
	mCommandList = dx->dx_sub[0].mCommandList.Get();
	d3varray = NULL;
	d3varrayBC = NULL;
	d3varrayI = NULL;
}

void PolygonData::SetCommandList(int no) {
	com_no = no;
	mCommandList = dx->dx_sub[com_no].mCommandList.Get();
}

PolygonData::~PolygonData() {
	free(d3varray);
	d3varray = NULL;
	free(d3varrayBC);
	d3varrayBC = NULL;
	free(d3varrayI);
	d3varrayI = NULL;
	S_DELETE(mObjectCB);
	RELEASE(texture);
	RELEASE(textureUp);
}

ID3D12PipelineState *PolygonData::GetPipelineState() {
	return mPSO.Get();
}

void PolygonData::SetVertex(int I1, int I2, int i,
	float vx, float vy, float vz,
	float nx, float ny, float nz,
	float u, float v) {
	d3varrayI[I1] = i;
	d3varrayI[I2] = i;
	d3varray[i].Pos.as(vx, vy, vz);
	d3varray[i].normal.as(nx, ny, nz);
	d3varray[i].tex.as(u, v);
}

void PolygonData::SetVertex(int I1, int i,
	float vx, float vy, float vz,
	float nx, float ny, float nz,
	float u, float v) {
	d3varrayI[I1] = i;
	d3varray[i].Pos.as(vx, vy, vz);
	d3varray[i].normal.as(nx, ny, nz);
	d3varray[i].tex.as(u, v);
}

void PolygonData::SetVertexBC(int I1, int I2, int i,
	float vx, float vy, float vz,
	float r, float g, float b, float a) {
	d3varrayI[I1] = i;
	d3varrayI[I2] = i;
	d3varrayBC[i].Pos.as(vx, vy, vz);
	d3varrayBC[i].color.as(r, g, b, a);
}

void PolygonData::SetVertexBC(int I1, int i,
	float vx, float vy, float vz,
	float r, float g, float b, float a) {
	d3varrayI[I1] = i;
	d3varrayBC[i].Pos.as(vx, vy, vz);
	d3varrayBC[i].color.as(r, g, b, a);
}

void PolygonData::GetVBarray(PrimitiveType type, int v) {

	if (type == SQUARE) {
		primType_create = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		primType_draw = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		ver = v * 4;//v==四角形の個数
		verI = v * 6;
	}
	if (type == POINt) {
		primType_create = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
		primType_draw = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
		ver = v;//v==点の個数
		verI = v;
	}
	if (type == LINE_L) {
		primType_create = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
		primType_draw = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		ver = v * 2;//v==線の個数
		verI = v * 2;
	}
	if (type == LINE_S) {
		primType_create = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
		primType_draw = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
		ver = v * 2;//v==線の個数
		verI = v * 2;
	}
	if (type == CONTROL_POINT) {
		primType_create = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
		primType_draw = D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST;
		ver = v * 4;//v==パッチの個数
		verI = v * 4;
	}

	d3varray = (Vertex*)malloc(sizeof(Vertex) * ver);
	d3varrayBC = (VertexBC*)malloc(sizeof(VertexBC) * ver);
	d3varrayI = (std::uint16_t*)malloc(sizeof(std::uint16_t) * verI);
	mObjectCB = new UploadBuffer<CONSTANT_BUFFER>(dx->md3dDevice.Get(), 1, true);
	mObjectCB1 = new UploadBuffer<CONSTANT_BUFFER2>(dx->md3dDevice.Get(), 1, true);
	Vview = std::make_unique<VertexView>();
	Iview = std::make_unique<IndexView>();
}

void PolygonData::TextureInit(int width, int height) {

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

void PolygonData::SetTextureMPixel(int **m_pix, BYTE r, BYTE g, BYTE b, int a) {

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
		UINT j1 = (UINT)(j * texResource.RowPitch);//RowPitchデータの行ピッチ、行幅、または物理サイズ (バイト単位)
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

void PolygonData::GetShaderByteCode(bool light, int tNo, int nortNo) {
	t_no = tNo;
	bool disp = FALSE;
	if (primType_create == D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH)disp = TRUE;
	if (tNo == -1 && m_on == FALSE) {
		vs = dx->pVertexShader_BC.Get();
		ps = dx->pPixelShader_BC.Get();
		return;
	}
	if (!disp && light) {
		vs = dx->pVertexShader_TC.Get();
		ps = dx->pPixelShader_3D.Get();
		return;
	}
	if (!disp && !light) {
		vs = dx->pVertexShader_TC.Get();
		ps = dx->pPixelShader_Emissive.Get();
		return;
	}
	if (disp && light) {
		vs = dx->pVertexShader_DISP.Get();
		if (nortNo == -1)
			ps = dx->pPixelShader_3D.Get();
		else
			ps = dx->pPixelShader_Bump.Get();
		hs = dx->pHullShader_DISP.Get();
		ds = dx->pDomainShader_DISP.Get();
		return;
	}
	if (disp && !light) {
		vs = dx->pVertexShader_DISP.Get();
		ps = dx->pPixelShader_Emissive.Get();
		hs = dx->pHullShader_DISP.Get();
		ds = dx->pDomainShader_DISP.Get();
		return;
	}
}

void PolygonData::Create(bool light, int tNo, bool blend, bool alpha) {
	Create(light, tNo, -1, blend, alpha);
}

void PolygonData::Create(bool light, int tNo, int nortNo, bool blend, bool alpha) {

	GetShaderByteCode(light, tNo, nortNo);

	CONSTANT_BUFFER2 sg;
	sg.vDiffuse.x = 1.0f;
	sg.vDiffuse.y = 1.0f;
	sg.vDiffuse.z = 1.0f;
	sg.vDiffuse.w = 1.0f;
	sg.vSpeculer.x = 1.0f;
	sg.vSpeculer.y = 1.0f;
	sg.vSpeculer.z = 1.0f;
	sg.vSpeculer.w = 1.0f;
	mObjectCB1->CopyData(0, sg);

	//BuildRootSignature
	CD3DX12_DESCRIPTOR_RANGE texTable, nortexTable;
	texTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);//このDescriptorRangeはシェーダーリソースビュー,Descriptor 1個, 開始Index 0番
	nortexTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);

	//BuildRootSignatureParameter
	CD3DX12_ROOT_PARAMETER slotRootParameter[4];
	slotRootParameter[0].InitAsDescriptorTable(1, &texTable, D3D12_SHADER_VISIBILITY_ALL);// DescriptorRangeの数は1つ, DescriptorRangeの先頭アドレス
	slotRootParameter[1].InitAsDescriptorTable(1, &nortexTable, D3D12_SHADER_VISIBILITY_ALL);
	slotRootParameter[2].InitAsConstantBufferView(0);
	slotRootParameter[3].InitAsConstantBufferView(1);

	auto staticSamplers = dx->GetStaticSamplers();

	// A root signature is an array of root parameters.
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(4, slotRootParameter,
		(UINT)staticSamplers.size(), staticSamplers.data(),
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	//RootSignatureを作成するのに必要なバッファを確保し、Tableの情報をシリアライズします
	//このバッファを指定して、CreateRootSignature() でRootSignatureを作成します
	Microsoft::WRL::ComPtr<ID3DBlob> serializedRootSig = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
	D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

	if (errorBlob != nullptr)
	{
		::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}
	//RootSignature生成
	dx->md3dDevice->CreateRootSignature(
		0,
		serializedRootSig->GetBufferPointer(),
		serializedRootSig->GetBufferSize(),
		IID_PPV_ARGS(mRootSignature.GetAddressOf()));

	//SRVのデスクリプターヒープ生成
	texNum = 1;
	if (nortNo != -1)texNum = 2;//normalMap有りの場合2個生成
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = texNum;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	dx->md3dDevice->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&mSrvHeap));

	//テクスチャ設定
	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(mSrvHeap->GetCPUDescriptorHandleForHeapStart());
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;

	//diffuseテクスチャ
	if (!m_on && tNo != -1) {
		srvDesc.Format = dx->texture[tNo]->GetDesc().Format;
		srvDesc.Texture2D.MipLevels = dx->texture[tNo]->GetDesc().MipLevels;
		dx->md3dDevice->CreateShaderResourceView(dx->texture[tNo], &srvDesc, hDescriptor);
	}
	//動画テクスチャ
	if (m_on) {
		srvDesc.Format = texture->GetDesc().Format;
		srvDesc.Texture2D.MipLevels = texture->GetDesc().MipLevels;
		dx->md3dDevice->CreateShaderResourceView(texture, &srvDesc, hDescriptor);
	}
	//normalMap
	if (nortNo != -1) {
		hDescriptor.Offset(1, dx->mCbvSrvUavDescriptorSize);//normalMapは二個目のテクスチャになるのでオフセット
		srvDesc.Format = dx->texture[nortNo]->GetDesc().Format;
		srvDesc.Texture2D.MipLevels = dx->texture[nortNo]->GetDesc().MipLevels;
		dx->md3dDevice->CreateShaderResourceView(dx->texture[nortNo], &srvDesc, hDescriptor);
	}

	UINT VertexSize;
	if (tNo == -1 && !m_on)
		VertexSize = sizeof(VertexBC);
	else
		VertexSize = sizeof(Vertex);

	const UINT vbByteSize = VertexSize * ver;
	const UINT ibByteSize = verI * sizeof(std::uint16_t);

	D3DCreateBlob(vbByteSize, &Vview->VertexBufferCPU);
	if (tNo == -1 && !m_on)
		CopyMemory(Vview->VertexBufferCPU->GetBufferPointer(), d3varrayBC, vbByteSize);
	else
		CopyMemory(Vview->VertexBufferCPU->GetBufferPointer(), d3varray, vbByteSize);

	D3DCreateBlob(ibByteSize, &Iview->IndexBufferCPU);
	CopyMemory(Iview->IndexBufferCPU->GetBufferPointer(), d3varrayI, ibByteSize);

	if (tNo == -1 && !m_on)
		Vview->VertexBufferGPU = dx->CreateDefaultBuffer(dx->md3dDevice.Get(),
			mCommandList, d3varrayBC, vbByteSize, Vview->VertexBufferUploader);
	else
		Vview->VertexBufferGPU = dx->CreateDefaultBuffer(dx->md3dDevice.Get(),
			mCommandList, d3varray, vbByteSize, Vview->VertexBufferUploader);

	Iview->IndexBufferGPU = dx->CreateDefaultBuffer(dx->md3dDevice.Get(),
		mCommandList, d3varrayI, ibByteSize, Iview->IndexBufferUploader);

	Vview->VertexByteStride = VertexSize;
	Vview->VertexBufferByteSize = vbByteSize;
	Iview->IndexFormat = DXGI_FORMAT_R16_UINT;
	Iview->IndexBufferByteSize = ibByteSize;
	Iview->IndexCount = verI;

	//パイプラインステートオブジェクト生成
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	if (tNo == -1 && !m_on)
		psoDesc.InputLayout = { dx->pVertexLayout_3DBC.data(), (UINT)dx->pVertexLayout_3DBC.size() };
	else
		psoDesc.InputLayout = { dx->pVertexLayout_3D.data(), (UINT)dx->pVertexLayout_3D.size() };
	psoDesc.pRootSignature = mRootSignature.Get();
	psoDesc.VS =
	{
		reinterpret_cast<BYTE*>(vs->GetBufferPointer()),
		vs->GetBufferSize()
	};
	if (hs != nullptr) {
		psoDesc.HS =
		{
			reinterpret_cast<BYTE*>(hs->GetBufferPointer()),
			hs->GetBufferSize()
		};
	}
	if (ds != nullptr) {
		psoDesc.DS =
		{
			reinterpret_cast<BYTE*>(ds->GetBufferPointer()),
			ds->GetBufferSize()
		};
	}
	psoDesc.PS =
	{
		reinterpret_cast<BYTE*>(ps->GetBufferPointer()),
		ps->GetBufferSize()
	};
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.BlendState.IndependentBlendEnable = FALSE;
	psoDesc.BlendState.AlphaToCoverageEnable = alpha;//アルファテストon/off
	psoDesc.BlendState.RenderTarget[0].BlendEnable = blend;//ブレンドon/off
	psoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	psoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = primType_create;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = dx->mBackBufferFormat;
	psoDesc.SampleDesc.Count = dx->m4xMsaaState ? 4 : 1;
	psoDesc.SampleDesc.Quality = dx->m4xMsaaState ? (dx->m4xMsaaQuality - 1) : 0;
	psoDesc.DSVFormat = dx->mDepthStencilFormat;
	dx->md3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPSO));
}

void PolygonData::InstancedMap(float x, float y, float z, float theta) {
	dx->InstancedMap(&cb[sw], x, y, z, theta, 0, 0, 1.0f);
}

void PolygonData::InstancedMap(float x, float y, float z, float theta, float size) {
	dx->InstancedMap(&cb[sw], x, y, z, theta, 0, 0, size);
}

void PolygonData::InstancedMapSize3(float x, float y, float z, float theta, float sizeX, float sizeY, float sizeZ) {
	dx->InstancedMapSize3(&cb[sw], x, y, z, theta, 0, 0, sizeX, sizeY, sizeZ);
}

void PolygonData::InstanceUpdate(float r, float g, float b, float a, float disp) {
	InstanceUpdate(r, g, b, a, disp, 1.0f, 1.0f, 1.0f, 1.0f);
}

void PolygonData::CbSwap() {
	Lock();
	if (!UpOn) {
		upCount++;
		if (upCount > 1)UpOn = TRUE;//cb,2要素初回更新終了
	}
	sw = 1 - sw;//cbスワップ
	insNum = dx->ins_no;
	dx->ins_no = 0;
	Unlock();
	DrawOn = TRUE;
}

void PolygonData::InstanceUpdate(float r, float g, float b, float a, float disp, float px, float py, float mx, float my) {
	dx->MatrixMap2(&cb[sw], r, g, b, a, disp, px, py, mx, my);
	CbSwap();
}

void PolygonData::Update(float x, float y, float z, float r, float g, float b, float a, float theta, float disp) {
	Update(x, y, z, r, g, b, a, theta, disp, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
}

void  PolygonData::Update(float x, float y, float z, float r, float g, float b, float a, float theta, float disp, float size) {
	Update(x, y, z, r, g, b, a, theta, disp, size, 1.0f, 1.0f, 1.0f, 1.0f);
}

void PolygonData::Update(float x, float y, float z, float r, float g, float b, float a, float theta, float disp, float size, float px, float py, float mx, float my) {
	dx->MatrixMap(&cb[sw], x, y, z, r, g, b, a, theta, 0, 0, size, disp, px, py, mx, my);
	CbSwap();
}

void PolygonData::DrawOff() {
	DrawOn = FALSE;
}

void PolygonData::Draw() {

	if (!UpOn | !DrawOn)return;

	Lock();
	mObjectCB->CopyData(0, cb[1 - sw]);
	Unlock();

	mCommandList->SetPipelineState(mPSO.Get());

	//mSwapChainBuffer PRESENT→RENDER_TARGET
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(dx->mSwapChainBuffer[dx->mCurrBackBuffer].Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	ID3D12DescriptorHeap* descriptorHeaps[] = { mSrvHeap.Get() };
	mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);//テクスチャ無しの場合このままで良いのやら・・エラーは無し

	mCommandList->SetGraphicsRootSignature(mRootSignature.Get());

	mCommandList->IASetVertexBuffers(0, 1, &Vview->VertexBufferView());
	mCommandList->IASetIndexBuffer(&Iview->IndexBufferView());
	mCommandList->IASetPrimitiveTopology(primType_draw);

	CD3DX12_GPU_DESCRIPTOR_HANDLE tex(mSrvHeap->GetGPUDescriptorHandleForHeapStart());
	mCommandList->SetGraphicsRootDescriptorTable(0, tex);
	if (texNum == 2) {
		tex.Offset(1, dx->mCbvSrvUavDescriptorSize);
		mCommandList->SetGraphicsRootDescriptorTable(1, tex);
	}
	mCommandList->SetGraphicsRootConstantBufferView(2, mObjectCB->Resource()->GetGPUVirtualAddress());
	mCommandList->SetGraphicsRootConstantBufferView(3, mObjectCB1->Resource()->GetGPUVirtualAddress());

	mCommandList->DrawIndexedInstanced(Iview->IndexCount, insNum, 0, 0, 0);

	//mSwapChainBuffer RENDER_TARGET→PRESENT
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(dx->mSwapChainBuffer[dx->mCurrBackBuffer].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
}