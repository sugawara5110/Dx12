//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　          PolygonData2Dクラス                               **//
//**                                    GetVBarray2D関数                                 **//
//*****************************************************************************************//

#include "Dx12Process.h"

void PolygonData2D::Pos2DCompute(VECTOR3 *p){
	MATRIX VP, VP_VP;
	//入力3D座標から変換行列取得
	MatrixMultiply(&VP, &Dx12Process::GetInstance()->mView, &Dx12Process::GetInstance()->mProj);
	MatrixMultiply(&VP_VP, &VP, &Dx12Process::GetInstance()->Vp);
	//変換後の座標取得
	VectorMatrixMultiply(p, &VP_VP);
}

PolygonData2D::PolygonData2D() {
	dx = Dx12Process::GetInstance();
	mCommandList = dx->dx_sub[0].mCommandList.Get();
	com_no = 0;
	d2varray = NULL;
	d2varrayI = NULL;
}

void PolygonData2D::SetCommandList(int no) {
	com_no = no;
	mCommandList = dx->dx_sub[com_no].mCommandList.Get();
}

PolygonData2D::~PolygonData2D() {
	free(d2varray);
	d2varray = NULL;
	free(d2varrayI);
	d2varrayI = NULL;
	S_DELETE(mObjectCB);
	RELEASE(texture);
	RELEASE(textureUp);
}

void PolygonData2D::InstancedSetConstBf(float x, float y, float r, float g, float b, float a, float sizeX, float sizeY) {
	InstancedSetConstBf(x, y, 0.0f, r, g, b, a, sizeX, sizeY);
}

void PolygonData2D::InstancedSetConstBf(float x, float y, float z, float r, float g, float b, float a, float sizeX, float sizeY) {

	cb.Pos[ins_no].as(x, y, z, 0.0f);
	cb.Color[ins_no].as(r, g, b, a);
	cb.sizeXY[ins_no].as(sizeX, sizeY, 0.0f, 0.0f);
	ins_no++;
}

void PolygonData2D::SetConstBf(UploadBuffer<CONSTANT_BUFFER2D> *mObjectCB, float x, float y, float z, float r, float g, float b, float a, float sizeX, float sizeY) {

	cb.Pos[ins_no].as(x, y, z, 0.0f);
	cb.Color[ins_no].as(r, g, b, a);
	cb.sizeXY[ins_no].as(sizeX, sizeY, 0.0f, 0.0f);
	mObjectCB->CopyData(0, cb);
	ins_no++;
}

void PolygonData2D::SetText(int width, int height, int textCount, TEXTMETRIC **TM, GLYPHMETRICS **GM, BYTE **ptr, DWORD **allsize) {

	RELEASE(texture);
	RELEASE(textureUp);

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
		D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&texture));

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

	D3D12_PLACED_SUBRESOURCE_FOOTPRINT  footprint;
	UINT64  total_bytes = 0;
	dx->md3dDevice->GetCopyableFootprints(&texDesc, 0, 1, 0, &footprint, nullptr, nullptr, &total_bytes);

	D3D12_TEXTURE_COPY_LOCATION dest, src;

	memset(&dest, 0, sizeof(dest));
	dest.pResource = texture;
	dest.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	dest.SubresourceIndex = 0;

	memset(&src, 0, sizeof(src));
	src.pResource = textureUp;
	src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	src.PlacedFootprint = footprint;

	D3D12_RESOURCE_BARRIER BarrierDesc;
	BarrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	BarrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	BarrierDesc.Transition.pResource = texture;
	BarrierDesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	BarrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
	BarrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;
	mCommandList->ResourceBarrier(1, &BarrierDesc);

	D3D12_SUBRESOURCE_DATA texResource;
	textureUp->Map(0, nullptr, reinterpret_cast<void**>(&texResource));
	BYTE* pBits = (BYTE*)texResource.pData;
	texResource.RowPitch = footprint.Footprint.RowPitch;
	memset(pBits, 0, texResource.RowPitch * height);//0埋め
	for (int cnt = 0; cnt < textCount; cnt++) {

		UINT temp = texResource.RowPitch / textCount / 4;
		UINT s_rowPitch = temp * 4;
		UINT offset1 = s_rowPitch * cnt;//4の倍数になっている事

		// フォント情報の書き込み
		// iOfs_x, iOfs_y : 書き出し位置(左上)
		// iBmp_w, iBmp_h : フォントビットマップの幅高
		// Level : α値の段階 (GGO_GRAY4_BITMAPなので17段階)
		int iOfs_x = (*GM)[cnt].gmptGlyphOrigin.x;
		int iOfs_y = (*TM)[cnt].tmAscent - (*GM)[cnt].gmptGlyphOrigin.y;
		int iBmp_w = (*GM)[cnt].gmBlackBoxX + (4 - ((*GM)[cnt].gmBlackBoxX % 4)) % 4;
		int iBmp_h = (*GM)[cnt].gmBlackBoxY;
		int Level = 17;
		int x, y;
		DWORD Alpha, Color;

		for (y = iOfs_y; y < iOfs_y + iBmp_h; y++) {
			for (x = iOfs_x; x < iOfs_x + iBmp_w; x++) {
				int offset2;
				if (cnt == 0)offset2 = 0; else {
					offset2 = (*allsize)[cnt - 1];
				}
				Alpha = (255 * (*ptr)[(x - iOfs_x + iBmp_w * (y - iOfs_y)) + offset2]) / (Level - 1);
				Color = 0x00ffffff | (Alpha << 24);
				memcpy((BYTE*)pBits + texResource.RowPitch * y + 4 * x + offset1, &Color, sizeof(DWORD));
			}
		}
	}
	textureUp->Unmap(0, nullptr);

	mCommandList->CopyTextureRegion(&dest, 0, 0, 0, &src, nullptr);

	BarrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	BarrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	mCommandList->ResourceBarrier(1, &BarrierDesc);

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
	srvDesc.Format = texture->GetDesc().Format;
	srvDesc.Texture2D.MipLevels = texture->GetDesc().MipLevels;
	dx->md3dDevice->CreateShaderResourceView(texture, &srvDesc, hDescriptor);
}

ID3D12PipelineState *PolygonData2D::GetPipelineState() {
	return mPSO.Get();
}

void PolygonData2D::GetVBarray2D(int pcs) {
	ver = pcs * 4;
	d2varray = (MY_VERTEX2*)malloc(sizeof(MY_VERTEX2) * ver);
	d2varrayI = (std::uint16_t*)malloc(sizeof(std::uint16_t) * (int)(ver * 1.5));
	mObjectCB = new UploadBuffer<CONSTANT_BUFFER2D>(dx->md3dDevice.Get(), 1, true);
	Vview = std::make_unique<VertexView>();
	Iview = std::make_unique<IndexView>();
}

void PolygonData2D::TexOn() {
	tex_on = TRUE;
}

void PolygonData2D::GetShaderByteCode() {

	if (!tex_on) {
		vs = dx->pVertexShader_2D.Get();
		ps = dx->pPixelShader_2D.Get();
	}
	else {
		vs = dx->pVertexShader_2DTC.Get();
		ps = dx->pPixelShader_2DTC.Get();
	}
}

void PolygonData2D::CreateBox(float x, float y, float z, float sizex, float sizey, float r, float g, float b, float a, bool blend, bool alpha) {

	//verが4の時のみ実行可
	if (ver != 4)return;
	d2varray[0].Pos.as(x, y, z);
	d2varray[0].color.as(r, g, b, a);
	d2varray[0].tex.as(0.0f, 0.0f);

	d2varray[1].Pos.as(x, y + sizey, z);
	d2varray[1].color.as(r, g, b, a);
	d2varray[1].tex.as(0.0f, 1.0f);

	d2varray[2].Pos.as(x + sizex, y + sizey, z);
	d2varray[2].color.as(r, g, b, a);
	d2varray[2].tex.as(1.0f, 1.0f);

	d2varray[3].Pos.as(x + sizex, y, z);
	d2varray[3].color.as(r, g, b, a);
	d2varray[3].tex.as(1.0f, 0.0f);

	d2varrayI[0] = 0;
	d2varrayI[1] = 3;
	d2varrayI[2] = 1;
	d2varrayI[3] = 1;
	d2varrayI[4] = 3;
	d2varrayI[5] = 2;

	Create(blend, alpha);
}

void PolygonData2D::Create(bool blend, bool alpha) {

	GetShaderByteCode();

	CD3DX12_DESCRIPTOR_RANGE texTable;
	texTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	CD3DX12_ROOT_PARAMETER slotRootParameter[2];
	slotRootParameter[0].InitAsDescriptorTable(1, &texTable, D3D12_SHADER_VISIBILITY_ALL);
	slotRootParameter[1].InitAsConstantBufferView(0);

	auto staticSamplers = dx->GetStaticSamplers();

	// A root signature is an array of root parameters.
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(2, slotRootParameter,
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

	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = 1;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	dx->md3dDevice->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&mSrvHeap));

	const UINT vbByteSize = ver * sizeof(MY_VERTEX2);
	const UINT ibByteSize = (int)(ver * 1.5) * sizeof(std::uint16_t);

	D3DCreateBlob(vbByteSize, &Vview->VertexBufferCPU);
	CopyMemory(Vview->VertexBufferCPU->GetBufferPointer(), d2varray, vbByteSize);

	D3DCreateBlob(ibByteSize, &Iview->IndexBufferCPU);
	CopyMemory(Iview->IndexBufferCPU->GetBufferPointer(), d2varrayI, ibByteSize);

	Vview->VertexBufferGPU = dx->CreateDefaultBuffer(dx->md3dDevice.Get(),
		mCommandList, d2varray, vbByteSize, Vview->VertexBufferUploader);

	Iview->IndexBufferGPU = dx->CreateDefaultBuffer(dx->md3dDevice.Get(),
		mCommandList, d2varrayI, ibByteSize, Iview->IndexBufferUploader);

	Vview->VertexByteStride = sizeof(MY_VERTEX2);
	Vview->VertexBufferByteSize = vbByteSize;
	Iview->IndexFormat = DXGI_FORMAT_R16_UINT;
	Iview->IndexBufferByteSize = ibByteSize;
	Iview->IndexCount = (int)(ver * 1.5);

	//パイプラインステートオブジェクト生成
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	psoDesc.InputLayout = { dx->pVertexLayout_2D.data(), (UINT)dx->pVertexLayout_2D.size() };
	psoDesc.pRootSignature = mRootSignature.Get();
	psoDesc.VS =
	{
		reinterpret_cast<BYTE*>(vs->GetBufferPointer()),
		vs->GetBufferSize()
	};
	psoDesc.PS =
	{
		reinterpret_cast<BYTE*>(ps->GetBufferPointer()),
		ps->GetBufferSize()
	};
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	//psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//両面
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.BlendState.IndependentBlendEnable = FALSE;
	psoDesc.BlendState.AlphaToCoverageEnable = alpha;//アルファテストon/off
	psoDesc.BlendState.RenderTarget[0].BlendEnable = blend;//ブレンドon/off
	psoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	psoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = dx->mBackBufferFormat;
	psoDesc.SampleDesc.Count = dx->m4xMsaaState ? 4 : 1;
	psoDesc.SampleDesc.Quality = dx->m4xMsaaState ? (dx->m4xMsaaQuality - 1) : 0;
	psoDesc.DSVFormat = dx->mDepthStencilFormat;
	dx->md3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPSO));
}

void PolygonData2D::InstanceDraw() {
	mObjectCB->CopyData(0, cb);
	DrawParts();
}

void PolygonData2D::Draw(float x, float y, float r, float g, float b, float a, float sizeX, float sizeY) {
	Draw(x, y, 0.0f, r, g, b, a, sizeX, sizeY);
}

void PolygonData2D::Draw(float x, float y, float z, float r, float g, float b, float a, float sizeX, float sizeY) {
	SetConstBf(mObjectCB, x, y, z, r, g, b, a, sizeX, sizeY);
	DrawParts();
}

void PolygonData2D::DrawParts() {

	mCommandList->SetPipelineState(mPSO.Get());

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

	ID3D12DescriptorHeap* descriptorHeaps[] = { mSrvHeap.Get() };
	mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);//テクスチャ無しの場合このままで良いのやら・・エラーは無し

	mCommandList->SetGraphicsRootSignature(mRootSignature.Get());

	mCommandList->IASetVertexBuffers(0, 1, &Vview->VertexBufferView());
	mCommandList->IASetIndexBuffer(&Iview->IndexBufferView());
	mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	mCommandList->SetGraphicsRootDescriptorTable(0, mSrvHeap->GetGPUDescriptorHandleForHeapStart());
	mCommandList->SetGraphicsRootConstantBufferView(1, mObjectCB->Resource()->GetGPUVirtualAddress());

	mCommandList->DrawIndexedInstanced(Iview->IndexCount, ins_no, 0, 0, 0);

	//mSwapChainBuffer RENDER_TARGET→PRESENT
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(dx->mSwapChainBuffer[dx->mCurrBackBuffer].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
	ins_no = 0;
}



