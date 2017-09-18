//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　          Waveクラス                                        **//
//**                                                                                     **//
//*****************************************************************************************//

#include "Dx12Process.h"

std::mutex Wave::mtx;

Wave::Wave() {
	dx = Dx12Process::GetInstance();
	mCommandList = dx->dx_sub[0].mCommandList.Get();
	d3varray = NULL;
	d3varrayI = NULL;
	texNum = 1;
}

void Wave::SetCommandList(int no) {
	com_no = no;
	mCommandList = dx->dx_sub[com_no].mCommandList.Get();
}

Wave::~Wave() {
	free(d3varray);
	d3varray = NULL;
	free(d3varrayI);
	d3varrayI = NULL;
	S_DELETE(mObjectCB);
	S_DELETE(mObjectCB_WAVE);
}

void Wave::SetVertex(int i,
	float vx, float vy, float vz,
	float nx, float ny, float nz,
	float u, float v) {
	d3varrayI[i] = i;
	d3varray[i].Pos.as(vx, vy, vz);
	d3varray[i].normal.as(nx, ny, nz);
	d3varray[i].tex.as(u, v);
}

void Wave::GetVBarray(int v) {
	ver = verI = v;
	d3varray = (Vertex*)malloc(sizeof(Vertex) * ver);
	d3varrayI = (std::uint16_t*)malloc(sizeof(std::uint16_t) * verI);
	mObjectCB = new UploadBuffer<CONSTANT_BUFFER>(dx->md3dDevice.Get(), 1, true);
	mObjectCB_WAVE = new UploadBuffer<CONSTANT_BUFFER_WAVE>(dx->md3dDevice.Get(), 1, true);
	Vview = std::make_unique<VertexView>();
	Iview = std::make_unique<IndexView>();
}

void Wave::GetShaderByteCode(int texNum) {
	cs = dx->pComputeShader_Wave.Get();
	vs = dx->pVertexShader_Wave.Get();
	if (texNum <= 1)
		ps = dx->pPixelShader_Wave.Get();
	else
		ps = dx->pPixelShader_WaveBump.Get();
	hs = dx->pHullShader_Wave.Get();
	ds = dx->pDomainShader_Wave.Get();
}

void Wave::ComCreate() {

	//CSからDSへの受け渡し用
	int divide = cbw.wHei_divide.y * cbw.wHei_divide.y;
	div = cbw.wHei_divide.y / 32.0f;//32はCS内スレッド数
	std::vector<WaveData> wdata(divide);
	for (int i = 0; i < divide; ++i)
	{
		wdata[i].sinWave = 0;
		wdata[i].theta = i % 360;
	}

	UINT64 byteSize = wdata.size() * sizeof(WaveData);
	//RWStructuredBuffer用
	dx->md3dDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(byteSize, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS),
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(&mOutputBuffer));

	dx->md3dDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(byteSize, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS),
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(&mInputBuffer));
	//UpLoad用
	dx->md3dDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(byteSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&mInputUploadBuffer));

	D3D12_SUBRESOURCE_DATA subResourceData = {};
	subResourceData.pData = wdata.data();
	subResourceData.RowPitch = wdata.size();
	subResourceData.SlicePitch = subResourceData.RowPitch;
	//wdata,UpLoad
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mInputBuffer.Get(),
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));
	UpdateSubresources(mCommandList, mInputBuffer.Get(), mInputUploadBuffer.Get(), 0, 0, 1, &subResourceData);
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mInputBuffer.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_UNORDERED_ACCESS));

	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mOutputBuffer.Get(),
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_UNORDERED_ACCESS));

	//ルートシグネチャ
	CD3DX12_ROOT_PARAMETER slotRootParameter[3];
	slotRootParameter[0].InitAsUnorderedAccessView(0);//RWStructuredBuffer(u0)
	slotRootParameter[1].InitAsUnorderedAccessView(1);//RWStructuredBuffer(u1)
	slotRootParameter[2].InitAsConstantBufferView(0);//mObjectCB_WAVE

	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(3, slotRootParameter,
		0, nullptr,
		D3D12_ROOT_SIGNATURE_FLAG_NONE);

	Microsoft::WRL::ComPtr<ID3DBlob> serializedRootSig = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

	if (errorBlob != nullptr)
	{
		::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}

	dx->md3dDevice->CreateRootSignature(
		0,
		serializedRootSig->GetBufferPointer(),
		serializedRootSig->GetBufferSize(),
		IID_PPV_ARGS(mRootSignatureCom.GetAddressOf()));

	//PSO
	D3D12_COMPUTE_PIPELINE_STATE_DESC PsoDesc = {};
	PsoDesc.pRootSignature = mRootSignatureCom.Get();
	PsoDesc.CS =
	{
		reinterpret_cast<BYTE*>(cs->GetBufferPointer()),
		cs->GetBufferSize()
	};
	PsoDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	dx->md3dDevice->CreateComputePipelineState(&PsoDesc, IID_PPV_ARGS(&mPSOCom));
}

void Wave::DrawCreate(int texNo, int nortNo, bool blend, bool alpha) {

	CD3DX12_DESCRIPTOR_RANGE texTable, nortexTable;
	texTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);//このDescriptorRangeはシェーダーリソースビュー,Descriptor 1個, 開始Index 0番
	nortexTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);

	CD3DX12_ROOT_PARAMETER slotRootParameter[5];
	slotRootParameter[0].InitAsDescriptorTable(1, &texTable, D3D12_SHADER_VISIBILITY_ALL);//(t0)DescriptorRangeの数は1つ, DescriptorRangeの先頭アドレス
	slotRootParameter[1].InitAsDescriptorTable(1, &nortexTable, D3D12_SHADER_VISIBILITY_ALL);//(t1)
	slotRootParameter[2].InitAsConstantBufferView(0);//(b0)
	slotRootParameter[3].InitAsConstantBufferView(1);//mObjectCB_WAVE(b1)
	slotRootParameter[4].InitAsShaderResourceView(2);//StructuredBuffer(t2)

	auto staticSamplers = dx->GetStaticSamplers();

	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(5, slotRootParameter,
		(UINT)staticSamplers.size(), staticSamplers.data(),
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

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
		IID_PPV_ARGS(mRootSignatureDraw.GetAddressOf()));

	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = texNum;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	dx->md3dDevice->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&mSrvHeap));

	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(mSrvHeap->GetCPUDescriptorHandleForHeapStart());

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	srvDesc.Format = dx->texture[texNo]->GetDesc().Format;
	srvDesc.Texture2D.MipLevels = dx->texture[texNo]->GetDesc().MipLevels;
	dx->md3dDevice->CreateShaderResourceView(dx->texture[texNo], &srvDesc, hDescriptor);
	if (nortNo != -1) {
		hDescriptor.Offset(1, dx->mCbvSrvUavDescriptorSize);
		srvDesc.Format = dx->texture[nortNo]->GetDesc().Format;
		srvDesc.Texture2D.MipLevels = dx->texture[nortNo]->GetDesc().MipLevels;
		dx->md3dDevice->CreateShaderResourceView(dx->texture[nortNo], &srvDesc, hDescriptor);
	}

	const UINT vbByteSize = ver * sizeof(Vertex);
	const UINT ibByteSize = verI * sizeof(std::uint16_t);

	D3DCreateBlob(vbByteSize, &Vview->VertexBufferCPU);
	CopyMemory(Vview->VertexBufferCPU->GetBufferPointer(), d3varray, vbByteSize);

	D3DCreateBlob(ibByteSize, &Iview->IndexBufferCPU);
	CopyMemory(Iview->IndexBufferCPU->GetBufferPointer(), d3varrayI, ibByteSize);

	Vview->VertexBufferGPU = dx->CreateDefaultBuffer(dx->md3dDevice.Get(),
		mCommandList, d3varray, vbByteSize, Vview->VertexBufferUploader);

	Iview->IndexBufferGPU = dx->CreateDefaultBuffer(dx->md3dDevice.Get(),
		mCommandList, d3varrayI, ibByteSize, Iview->IndexBufferUploader);

	Vview->VertexByteStride = sizeof(Vertex);
	Vview->VertexBufferByteSize = vbByteSize;
	Iview->IndexFormat = DXGI_FORMAT_R16_UINT;
	Iview->IndexBufferByteSize = ibByteSize;
	Iview->IndexCount = verI;

	//パイプラインステートオブジェクト生成
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	psoDesc.InputLayout = { dx->pVertexLayout_3D.data(), (UINT)dx->pVertexLayout_3D.size() };
	psoDesc.pRootSignature = mRootSignatureDraw.Get();
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
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = dx->mBackBufferFormat;
	psoDesc.SampleDesc.Count = dx->m4xMsaaState ? 4 : 1;
	psoDesc.SampleDesc.Quality = dx->m4xMsaaState ? (dx->m4xMsaaQuality - 1) : 0;
	psoDesc.DSVFormat = dx->mDepthStencilFormat;
	dx->md3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPSODraw));
}

void Wave::Create(int texNo, bool blend, bool alpha, float waveHeight, float divide) {
	Create(texNo, -1, blend, alpha, waveHeight, divide);
}

void Wave::Create(int texNo, int nortNo, bool blend, bool alpha, float waveHeight, float divide) {
	cbw.wHei_divide.as(waveHeight, divide, 0.0f, 0.0f);
	mObjectCB_WAVE->CopyData(0, cbw);
	t_no = texNo;
	if (nortNo != -1)texNum = 2;
	GetShaderByteCode(texNum);
	ComCreate();
	DrawCreate(texNo, nortNo, blend, alpha);
}

void Wave::InstancedMap(float x, float y, float z, float theta) {
	dx->InstancedMap(&cb[sw], x, y, z, theta, 0, 0, 1.0f);
}

void Wave::InstancedMap(float x, float y, float z, float theta, float size) {
	dx->InstancedMap(&cb[sw], x, y, z, theta, 0, 0, size);
}

void Wave::InstancedMapSize3(float x, float y, float z, float theta, float sizeX, float sizeY, float sizeZ) {
	dx->InstancedMapSize3(&cb[sw], x, y, z, theta, 0, 0, sizeX, sizeY, sizeZ);
}

void Wave::InstanceUpdate(float r, float g, float b, float a, float disp) {
	InstanceUpdate(r, g, b, a, disp, 1.0f, 1.0f, 1.0f, 1.0f);
}

void Wave::CbSwap() {
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

void Wave::InstanceUpdate(float r, float g, float b, float a, float disp, float px, float py, float mx, float my) {
	dx->MatrixMap2(&cb[sw], r, g, b, a, disp, px, py, mx, my);
	CbSwap();
}

void Wave::Update(float x, float y, float z, float r, float g, float b, float a, float theta, float disp) {
	Update(x, y, z, r, g, b, a, theta, disp, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
}

void Wave::Update(float x, float y, float z, float r, float g, float b, float a, float theta, float disp, float size) {
	Update(x, y, z, r, g, b, a, theta, disp, size, 1.0f, 1.0f, 1.0f, 1.0f);
}

void Wave::Update(float x, float y, float z, float r, float g, float b, float a, float theta, float disp, float size, float px, float py, float mx, float my) {
	dx->MatrixMap(&cb[sw], x, y, z, r, g, b, a, theta, 0, 0, size, disp, px, py, mx, my);
	CbSwap();
}

void Wave::DrawOff() {
	DrawOn = FALSE;
}

void Wave::Compute() {

	mCommandList->SetPipelineState(mPSOCom.Get());

	mCommandList->SetComputeRootSignature(mRootSignatureCom.Get());

	mCommandList->SetComputeRootUnorderedAccessView(0, mInputBuffer->GetGPUVirtualAddress());
	mCommandList->SetComputeRootUnorderedAccessView(1, mOutputBuffer->GetGPUVirtualAddress());
	mCommandList->SetComputeRootConstantBufferView(2, mObjectCB_WAVE->Resource()->GetGPUVirtualAddress());

	mCommandList->Dispatch(div, div, 1);

	auto tmp = mInputBuffer;
	mInputBuffer = mOutputBuffer;
	mOutputBuffer = tmp;

	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mInputBuffer.Get(),
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_GENERIC_READ));
}

void Wave::DrawSub() {

	mCommandList->SetPipelineState(mPSODraw.Get());

	//mSwapChainBuffer PRESENT→RENDER_TARGET
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(dx->mSwapChainBuffer[dx->mCurrBackBuffer].Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	ID3D12DescriptorHeap* descriptorHeaps[] = { mSrvHeap.Get() };
	mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);//テクスチャ無しの場合このままで良いのやら・・エラーは無し

	mCommandList->SetGraphicsRootSignature(mRootSignatureDraw.Get());

	mCommandList->IASetVertexBuffers(0, 1, &Vview->VertexBufferView());
	mCommandList->IASetIndexBuffer(&Iview->IndexBufferView());
	mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);

	CD3DX12_GPU_DESCRIPTOR_HANDLE tex(mSrvHeap->GetGPUDescriptorHandleForHeapStart());
	mCommandList->SetGraphicsRootDescriptorTable(0, tex);
	if (texNum == 2) {
		tex.Offset(1, dx->mCbvSrvUavDescriptorSize);
		mCommandList->SetGraphicsRootDescriptorTable(1, tex);
	}
	mCommandList->SetGraphicsRootConstantBufferView(2, mObjectCB->Resource()->GetGPUVirtualAddress());
	mCommandList->SetGraphicsRootConstantBufferView(3, mObjectCB_WAVE->Resource()->GetGPUVirtualAddress());
	mCommandList->SetGraphicsRootShaderResourceView(4, mOutputBuffer->GetGPUVirtualAddress());

	mCommandList->DrawIndexedInstanced(Iview->IndexCount, insNum, 0, 0, 0);

	//mSwapChainBuffer RENDER_TARGET→PRESENT
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(dx->mSwapChainBuffer[dx->mCurrBackBuffer].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
}

void Wave::Draw() {

	if (!UpOn | !DrawOn)return;

	Lock();
	mObjectCB->CopyData(0, cb[1 - sw]);
	Unlock();

	Compute();
	DrawSub();
}



