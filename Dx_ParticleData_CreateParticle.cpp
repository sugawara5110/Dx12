//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　         ParticleDataクラス                                 **//
//**                                  CreateParticle関数                                 **//
//*****************************************************************************************//

#include "Dx12Process.h"

ParticleData::ParticleData() {
	dx = Dx12Process::GetInstance();
	mDirectCmdListAlloc = dx->mDirectCmdListAlloc[0].Get();
	mCommandList = dx->mCommandList[0].Get();
	ver = 0;
	Drawfirst = TRUE;
	P_pos = NULL;
}

void ParticleData::SetCommandList(int no) {
	com_no = no;
	mDirectCmdListAlloc = dx->mDirectCmdListAlloc[com_no].Get();
	mCommandList = dx->mCommandList[com_no].Get();
}

ParticleData::~ParticleData() {
	S_DELETE(mObjectCB);
	if (P_pos != NULL) {
		free(P_pos);
		P_pos = NULL;
	}
}

void ParticleData::GetShaderByteCode() {
	gsSO = dx->pGeometryShader_PSO.Get();
	gs = dx->pGeometryShader_P.Get();
	vsSO = dx->pVertexShader_PSO.Get();
	vs = dx->pVertexShader_P.Get();
	ps = dx->pPixelShader_P.Get();
}

void ParticleData::MatrixMap(UploadBuffer<CONSTANT_BUFFER_P> *mObjectCB, float x, float y, float z, float theta, float size, bool init, float speed) {
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
	mObjectCB->CopyData(0, cb);
}

void ParticleData::CreateParticle(int texture_no, float size, float density) {

	GetShaderByteCode();

	mObjectCB = new UploadBuffer<CONSTANT_BUFFER_P>(dx->md3dDevice.Get(), 1, true);

	CD3DX12_ROOT_PARAMETER slotRootParameter[1];
	slotRootParameter[0].InitAsConstantBufferView(0);

	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc_com(1, slotRootParameter,
		NULL, NULL,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_STREAM_OUTPUT | D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc_draw(1, slotRootParameter,
		NULL, NULL,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

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

	D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
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

	const UINT vbByteSize = ver * sizeof(PartPos);
	Vview = std::make_unique<VertexView>();

	D3DCreateBlob(vbByteSize, &Vview->VertexBufferCPU);
	CopyMemory(Vview->VertexBufferCPU->GetBufferPointer(), P_pos, vbByteSize);

	Vview->VertexBufferGPU = dx->CreateDefaultBuffer(dx->md3dDevice.Get(),
		mCommandList, P_pos, vbByteSize, Vview->VertexBufferUploader);

	Vview->VertexByteStride = sizeof(PartPos);
	Vview->VertexBufferByteSize = vbByteSize;

	Sview1 = std::make_unique<StreamView>();
	Sview2 = std::make_unique<StreamView>();

	Sview1->StreamBufferGPU = dx->CreateStreamBuffer(dx->md3dDevice.Get(), vbByteSize);
	Sview2->StreamBufferGPU = dx->CreateStreamBuffer(dx->md3dDevice.Get(), vbByteSize);

	Sview1->StreamByteStride = Sview2->StreamByteStride = sizeof(PartPos);
	Sview1->StreamBufferByteSize = Sview2->StreamBufferByteSize = vbByteSize;
	Sview1->BufferFilledSizeLocation = Sview2->StreamBufferGPU->GetGPUVirtualAddress();

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
	psoDesc.StreamOutput.pBufferStrides = &Sview1->StreamByteStride;
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

	//パイプラインステートオブジェクト生成(Draw)
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

void ParticleData::Draw(float x, float y, float z, float theta, float size, bool init, float speed) {

	mCommandList->SetPipelineState(mPSO_com.Get());

	MatrixMap(mObjectCB, x, y, z, theta, size, init, speed);

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

	mCommandList->SetGraphicsRootSignature(mRootSignature_com.Get());

	mCommandList->IASetVertexBuffers(0, 1, &Vview->VertexBufferView());
	mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	mCommandList->SetGraphicsRootConstantBufferView(0, mObjectCB->Resource()->GetGPUVirtualAddress());

	mCommandList->SOSetTargets(0, 1, &Sview1->StreamBufferView());

	mCommandList->DrawInstanced(ver, 1, 0, 0);

	mCommandList->CopyResource(Vview->VertexBufferGPU.Get(), Sview1->StreamBufferGPU.Get());

	mCommandList->SOSetTargets(0, 1, nullptr);

	mCommandList->SetPipelineState(mPSO_draw.Get());

	mCommandList->SetGraphicsRootSignature(mRootSignature_draw.Get());

	mCommandList->IASetVertexBuffers(0, 1, &Vview->VertexBufferView());
	mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	mCommandList->SetGraphicsRootConstantBufferView(0, mObjectCB->Resource()->GetGPUVirtualAddress());

	mCommandList->DrawInstanced(ver, 1, 0, 0);

	//mSwapChainBuffer RENDER_TARGET→PRESENT
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(dx->mSwapChainBuffer[dx->mCurrBackBuffer].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
}