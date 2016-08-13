//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@          PolygonData�N���X                                 **//
//**                                   GetVBarray�֐�                                    **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include "Dx12Process.h"
#include "Shader3D.h"
#include "ShaderDisp.h"

PolygonData::PolygonData() {
	dx = Dx12Process::GetInstance();
	d3varray = NULL;
	d3varrayI = NULL;
	//�R�}���h�A���P�[�^����(�R�}���h���X�g�ɐςރo�b�t�@���m�ۂ���Obj)
	dx->md3dDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(mDirectCmdListAlloc.GetAddressOf()));

	//�R�}���h���X�g����
	dx->md3dDevice->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		mDirectCmdListAlloc.Get(),
		nullptr,
		IID_PPV_ARGS(mCommandList.GetAddressOf()));

	//�ŏ��͕��������ǂ�
	mCommandList->Close();
}

PolygonData::~PolygonData() {
	free(d3varray);
	d3varray = NULL;
	free(d3varrayI);
	d3varrayI = NULL;
	S_DELETE(mObjectCB);
	RELEASE(texture);
}

void PolygonData::SetVertex(int I1, int I2, int i,
	float vx, float vy, float vz,
	float nx, float ny, float nz,
	float r, float g, float b, float a,
	float u, float v) {
	d3varrayI[I1] = i;
	d3varrayI[I2] = i;
	d3varray[i].Pos.as(vx, vy, vz);
	d3varray[i].normal.as(nx, ny, nz);
	d3varray[i].color.as((FLOAT)r, (FLOAT)g, (FLOAT)b, (FLOAT)a);
	d3varray[i].tex.as(u, v);
}

void PolygonData::SetVertex(int I1, int i,
	float vx, float vy, float vz,
	float nx, float ny, float nz,
	float r, float g, float b, float a,
	float u, float v) {
	d3varrayI[I1] = i;
	d3varray[i].Pos.as(vx, vy, vz);
	d3varray[i].normal.as(nx, ny, nz);
	d3varray[i].color.as((FLOAT)r, (FLOAT)g, (FLOAT)b, (FLOAT)a);
	d3varray[i].tex.as(u, v);
}

void PolygonData::GetVBarray(PrimitiveType type, bool light, int v) {

	if (type == SQUARE) {
		primType_create = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		primType_draw = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		ver = v * 4;//v==�l�p�`�̌�
		verI = v * 6;
	}
	if (type == POINt) {
		primType_create = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
		primType_draw = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
		ver = v;//v==�_�̌�
		verI = v;
	}
	if (type == LINE_L) {
		primType_create = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
		primType_draw = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		ver = v * 2;//v==���̌�
		verI = v * 2;
	}
	if (type == LINE_S) {
		primType_create = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
		primType_draw = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
		ver = v * 2;//v==���̌�
		verI = v * 2;
	}
	if (type == CONTROL_POINT) {
		primType_create = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
		primType_draw = D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST;
		ver = v * 4;//v==�p�b�`�̌�
		verI = v * 4;
	}

	d3varray = (Vertex*)malloc(sizeof(Vertex) * ver);
	d3varrayI = (std::uint16_t*)malloc(sizeof(std::uint16_t) * verI);
}

void PolygonData::Create() {

	mDirectCmdListAlloc->Reset();
	mCommandList->Reset(mDirectCmdListAlloc.Get(), mPSO.Get());

	//BuildConstantBuffers
	mObjectCB = new UploadBuffer<CONSTANT_BUFFER>(dx->md3dDevice.Get(), 1, true);

	//BuildRootSignature
	CD3DX12_DESCRIPTOR_RANGE texTable;
	texTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);// ����DescriptorRange�̓V�F�[�_�[���\�[�X�r���[,Descriptor 1��, �J�nIndex 0��

	//BuildRootSignatureParameter
	CD3DX12_ROOT_PARAMETER slotRootParameter[2];
	slotRootParameter[0].InitAsDescriptorTable(1, &texTable, D3D12_SHADER_VISIBILITY_ALL);// DescriptorRange�̐���1��, DescriptorRange�̐擪�A�h���X
	slotRootParameter[1].InitAsConstantBufferView(0);

	auto staticSamplers = dx->GetStaticSamplers();

	// A root signature is an array of root parameters.
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(2, slotRootParameter,
		(UINT)staticSamplers.size(), staticSamplers.data(),
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	//RootSignature���쐬����̂ɕK�v�ȃo�b�t�@���m�ۂ��ATable�̏����V���A���C�Y���܂�
	//���̃o�b�t�@���w�肵�āACreateRootSignature() ��RootSignature���쐬���܂�
	Microsoft::WRL::ComPtr<ID3DBlob> serializedRootSig = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
	D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

	if (errorBlob != nullptr)
	{
		::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}
	//RootSignature����
	dx->md3dDevice->CreateRootSignature(
		0,
		serializedRootSig->GetBufferPointer(),
		serializedRootSig->GetBufferSize(),
		IID_PPV_ARGS(mRootSignature.GetAddressOf()));

	//BuildDescriptorHeaps
	//
	// Create the SRV heap.
	//
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = 1;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	dx->md3dDevice->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&mSrvHeap));

	//
	// Fill out the heap with actual descriptors.
	//
	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(mSrvHeap->GetCPUDescriptorHandleForHeapStart());

	dx->GetTexture(mCommandList.Get(), &texture, 0);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = texture->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = texture->GetDesc().MipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;

	dx->md3dDevice->CreateShaderResourceView(texture, &srvDesc, hDescriptor);

	//BuildShadersAndInputLayout
	mvsByteCode = dx->CompileShader(ShaderDisp, strlen(ShaderDisp), "VSDispL", "vs_5_0");
	mpsByteCode = dx->CompileShader(ShaderDisp, strlen(ShaderDisp), "PSDispL", "ps_5_0");
	mhsByteCode = dx->CompileShader(ShaderDisp, strlen(ShaderDisp), "HSDispL", "hs_5_0");
	mdsByteCode = dx->CompileShader(ShaderDisp, strlen(ShaderDisp), "DSDispL", "ds_5_0");

	mInputLayout =
	{
	   { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	   { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	   { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 3 * 2, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	   { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3 * 2 + 4 * 4, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	const UINT vbByteSize = ver * sizeof(Vertex);
	const UINT ibByteSize = verI * sizeof(std::uint16_t);

	mBoxGeo = std::make_unique<MeshGeometry>();

	D3DCreateBlob(vbByteSize, &mBoxGeo->VertexBufferCPU);
	CopyMemory(mBoxGeo->VertexBufferCPU->GetBufferPointer(), d3varray, vbByteSize);

	D3DCreateBlob(ibByteSize, &mBoxGeo->IndexBufferCPU);
	CopyMemory(mBoxGeo->IndexBufferCPU->GetBufferPointer(), d3varrayI, ibByteSize);

	mBoxGeo->VertexBufferGPU = dx->CreateDefaultBuffer(dx->md3dDevice.Get(),
		mCommandList.Get(), d3varray, vbByteSize, mBoxGeo->VertexBufferUploader);

	mBoxGeo->IndexBufferGPU = dx->CreateDefaultBuffer(dx->md3dDevice.Get(),
		mCommandList.Get(), d3varrayI, ibByteSize, mBoxGeo->IndexBufferUploader);

	mBoxGeo->VertexByteStride = sizeof(Vertex);
	mBoxGeo->VertexBufferByteSize = vbByteSize;
	mBoxGeo->IndexFormat = DXGI_FORMAT_R16_UINT;
	mBoxGeo->IndexBufferByteSize = ibByteSize;
	mBoxGeo->IndexCount = verI;

	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g����
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	psoDesc.InputLayout = { mInputLayout.data(), (UINT)mInputLayout.size() };
	psoDesc.pRootSignature = mRootSignature.Get();
	psoDesc.VS =
	{
		reinterpret_cast<BYTE*>(mvsByteCode->GetBufferPointer()),
		mvsByteCode->GetBufferSize()
	};
	psoDesc.HS =
	{
		reinterpret_cast<BYTE*>(mhsByteCode->GetBufferPointer()),
		mhsByteCode->GetBufferSize()
	};
	psoDesc.DS =
	{
		reinterpret_cast<BYTE*>(mdsByteCode->GetBufferPointer()),
		mdsByteCode->GetBufferSize()
	};
	psoDesc.PS =
	{
		reinterpret_cast<BYTE*>(mpsByteCode->GetBufferPointer()),
		mpsByteCode->GetBufferSize()
	};
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = primType_create;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = dx->mBackBufferFormat;
	psoDesc.SampleDesc.Count = dx->m4xMsaaState ? 4 : 1;
	psoDesc.SampleDesc.Quality = dx->m4xMsaaState ? (dx->m4xMsaaQuality - 1) : 0;
	psoDesc.DSVFormat = dx->mDepthStencilFormat;
	dx->md3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPSO));

	//�R�}���h�N���[�Y
	mCommandList->Close();
	//�N���[�Y�ナ�X�g�ɉ�����
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	dx->mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	//�R�}���h�����҂�
	dx->FlushCommandQueue();
}

void PolygonData::Draw(float x, float y, float z, float r, float g, float b, float theta, float disp) {

	dx->MatrixMap(mObjectCB, x, y, z, r, g, b, theta, 0, 0, 1.0f, disp);

	mDirectCmdListAlloc->Reset();
	mCommandList->Reset(mDirectCmdListAlloc.Get(), mPSO.Get());

	mCommandList->RSSetViewports(1, &dx->mScreenViewport);
	mCommandList->RSSetScissorRects(1, &dx->mScissorRect);

	//mSwapChainBuffer PRESENT��RENDER_TARGET
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(dx->mSwapChainBuffer[dx->mCurrBackBuffer].Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	//�����_�[�^�[�Q�b�g�̃Z�b�g
	mCommandList->OMSetRenderTargets(1, &CD3DX12_CPU_DESCRIPTOR_HANDLE(
		dx->mRtvHeap->GetCPUDescriptorHandleForHeapStart(),
		dx->mCurrBackBuffer,
		dx->mRtvDescriptorSize), true, &dx->mDsvHeap->GetCPUDescriptorHandleForHeapStart());

	ID3D12DescriptorHeap* descriptorHeaps[] = { mSrvHeap.Get() };
	mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	mCommandList->SetGraphicsRootSignature(mRootSignature.Get());

	mCommandList->IASetVertexBuffers(0, 1, &mBoxGeo->VertexBufferView());
	mCommandList->IASetIndexBuffer(&mBoxGeo->IndexBufferView());
	mCommandList->IASetPrimitiveTopology(primType_draw);

	mCommandList->SetGraphicsRootDescriptorTable(0, mSrvHeap->GetGPUDescriptorHandleForHeapStart());
	mCommandList->SetGraphicsRootConstantBufferView(1, mObjectCB->Resource()->GetGPUVirtualAddress());

	mCommandList->DrawIndexedInstanced(
		mBoxGeo->IndexCount,
		1, 0, 0, 0);

	//mSwapChainBuffer RENDER_TARGET��PRESENT
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(dx->mSwapChainBuffer[dx->mCurrBackBuffer].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	mCommandList->Close();
	ID3D12CommandList* cmdsLists1[] = { mCommandList.Get() };//ID3D12GraphicsCommandList��ID3D12CommandList�֕ϊ��K�v
	dx->mCommandQueue->ExecuteCommandLists(_countof(cmdsLists1), cmdsLists1);
}