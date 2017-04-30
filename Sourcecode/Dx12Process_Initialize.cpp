//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@          Dx12Process�N���X                                 **//
//**                                   Initialize�֐�                                    **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include "Dx12Process.h"
#include <WindowsX.h>
#include "Shader2D.h"
#include "Shader3D.h"
#include "ShaderDisp.h"
#include "ShaderMesh.h"
#include "ShaderMesh_D.h"
#include "ShaderParticle.h"
#include "ShaderSkinMesh.h"

using Microsoft::WRL::ComPtr;

void Dx12Process_sub::ListCreate() {
	for (int i = 0; i < 2; i++) {
		//�R�}���h�A���P�[�^����(�R�}���h���X�g�ɐςރo�b�t�@���m�ۂ���Obj)
		if (FAILED(Dx12Process::dx->md3dDevice->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(mCmdListAlloc[i].GetAddressOf()))))FALSE;
	}

	//�R�}���h���X�g����
	if (FAILED(Dx12Process::dx->md3dDevice->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		mCmdListAlloc[0].Get(),
		nullptr,
		IID_PPV_ARGS(mCommandList.GetAddressOf()))))FALSE;

	//�ŏ��͕��������ǂ�
	mCommandList->Close();
	mCommandList->Reset(mCmdListAlloc[0].Get(), nullptr);
	mCommandList->Close();
	mComState = USED;
}

void Dx12Process_sub::Bigin() {
	mComState = OPEN;
	mAloc_Num = 1 - mAloc_Num;
	mCmdListAlloc[mAloc_Num]->Reset();
	mCommandList->Reset(mCmdListAlloc[mAloc_Num].Get(), nullptr);
}

void Dx12Process_sub::End() {
	//�R�}���h�N���[�Y
	mCommandList->Close();
	mComState = CLOSE;
}

Dx12Process *Dx12Process::dx = NULL;
std::mutex Dx12Process::mtx;

void Dx12Process::InstanceCreate() {

	if (dx == NULL)dx = new Dx12Process();
}

Dx12Process *Dx12Process::GetInstance() {

	if (dx != NULL)return dx;
	return NULL;
}

void Dx12Process::DeleteInstance() {

	if (dx != NULL) {
		delete dx;
		dx = NULL;
	}
}

Dx12Process::~Dx12Process() {

	WaitFenceCurrent();

	SkinMesh::DeleteManager();

	for (int i = 0; i < TEX_PCS; i++) {
		if (binary_ch[i] == NULL)continue;
		free(binary_ch[i]);
		binary_ch[i] = NULL;
		RELEASE(texture[i]);
		RELEASE(textureUp[i]);
	}
}

void Dx12Process::WaitFence(int fence) {
	//�N���[�Y�ナ�X�g�ɉ�����
	for (int i = 0; i < COM_NO; i++) {
		if (dx_sub[i].mComState != CLOSE)continue;
		ID3D12CommandList* cmdsLists[] = { dx_sub[i].mCommandList.Get() };
		mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
		dx_sub[i].mComState = USED;
	}

	//�C���N�������g���ꂽ���Ƃŕ`�抮���Ɣ��f
	mCurrentFence++;
	//GPU��Ŏ��s����Ă���R�}���h������,�����I�Ƀt�F���X��mCurrentFence����������
	//(mFence->GetCompletedValue()�œ�����l��mCurrentFence�Ɠ����ɂȂ�)
	mCommandQueue->Signal(mFence.Get(), mCurrentFence);

	//�����܂łŃR�}���h�L���[���I�����Ă��܂���
	//���̃C�x���g����������������Ȃ��\���L��ׁ�if�Ń`�F�b�N���Ă���
	//GetCompletedValue():Fence����UINT64�̃J�E���^�擾(�����l0)
	//�R�}���h�A���P�[�^���_�u���o�b�t�@�ɂ��Ă���̂�Fence�l1��O�܂ł�GPU�����������̏ꍇ�̂ݑ҂��������s��
	if (mFence->GetCompletedValue() < mCurrentFence - fence) {

		HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
		//����Fence�ɂ�����,mCurrentFence �̒l�ɂȂ�����C�x���g�𔭉΂�����
		//����L��if���������Ɗ���mCurrentFence�Ɠ��l�ɂȂ��Ă��܂���������̂ł��̍ۂ́�WaitForSingleObject�Œ�~�����܂܂ɂȂ�
		mFence->SetEventOnCompletion(mCurrentFence, eventHandle);
		//�C�x���g�����΂���܂ő҂�(GPU�̏����҂�)����ɂ��GPU��̑S�R�}���h���s�I���܂ő҂����鎖���o����
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}
}

void Dx12Process::WaitFenceCurrent() {
	WaitFence(0);
}

void Dx12Process::WaitFencePast() {
	WaitFence(1);
}

void Dx12Process::RequestSync() {
	while (requestSync);
	WaitFenceCurrent();
	requestSync = TRUE;
	while (!replySync);
	replySync = FALSE;
}

void Dx12Process::ReplySync() {
	while (!requestSync && !syncFin);
	WaitFenceCurrent();
	replySync = TRUE;
	requestSync = FALSE;
}

void Dx12Process::SyncFin(bool on) {
	syncFin = on;
	requestSync = replySync = FALSE;
}

bool Dx12Process::SyncFin() {
	return syncFin;
}

void Dx12Process::CreateShaderByteCode() {

	//�X�L�����b�V��
	pVertexLayout_SKIN =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BONE_INDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BONE_WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};
	pVertexShader_SKIN = dx->CompileShader(ShaderSkinMesh, strlen(ShaderSkinMesh), "VSSkin", "vs_5_0");
	pPixelShader_SKIN = dx->CompileShader(ShaderSkinMesh, strlen(ShaderSkinMesh), "PSSkin", "ps_5_0");

	//�X�g���[���o�̓f�[�^��`(�p�[�e�B�N���p)
	pDeclaration_PSO =
	{
		{ 0, "POSITION", 0, 0, 3, 0 }, //�ux,y,z�v���X���b�g�u0�v�́uPOSITION�v�ɏo��
		{ 0, "POSITION", 1, 0, 3, 0 },
		{ 0, "POSITION", 2, 0, 3, 0 },
		{ 0, "COLOR", 0, 0, 4, 0 }
	};
	//�X�g���[���o��
	pVertexShader_PSO = dx->CompileShader(ShaderParticle, strlen(ShaderParticle), "VS_SO", "vs_5_0");
	pGeometryShader_PSO = dx->CompileShader(ShaderParticle, strlen(ShaderParticle), "GS_Point_SO", "gs_5_0");

	//�p�[�e�B�N�����_�C���v�b�g���C�A�E�g���`
	pVertexLayout_P = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "POSITION", 1, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "POSITION", 2, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3 * 2, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 3 * 3, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
	//�p�[�e�B�N��
	pVertexShader_P = dx->CompileShader(ShaderParticle, strlen(ShaderParticle), "VS", "vs_5_0");
	pGeometryShader_P = dx->CompileShader(ShaderParticle, strlen(ShaderParticle), "GS_Point", "gs_5_0");
	pPixelShader_P = dx->CompileShader(ShaderParticle, strlen(ShaderParticle), "PS", "ps_5_0");

	//���b�V�����C�A�E�g
	pVertexLayout_MESH =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3 * 2, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
	//���b�V��
	pVertexShader_MESH = dx->CompileShader(ShaderMesh, strlen(ShaderMesh), "VSMesh", "vs_5_0");
	pPixelShader_MESH = dx->CompileShader(ShaderMesh, strlen(ShaderMesh), "PSMesh", "ps_5_0");
	//�e�Z���[�^�[�L���b�V��
	pVertexShader_MESH_D = dx->CompileShader(ShaderMesh_D, strlen(ShaderMesh_D), "VSMesh", "vs_5_0");
	pPixelShader_MESH_D = dx->CompileShader(ShaderMesh_D, strlen(ShaderMesh_D), "PSMesh", "ps_5_0");
	pHullShader_MESH_D = dx->CompileShader(ShaderMesh_D, strlen(ShaderMesh_D), "HSMesh", "hs_5_0");
	pDomainShader_MESH_D = dx->CompileShader(ShaderMesh_D, strlen(ShaderMesh_D), "DSMesh", "ds_5_0");

	//3D���C�A�E�g
	pVertexLayout_3D =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 3 * 2, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3 * 2 + 4 * 4, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
	//�e�N�X�`��3D���C�g��
	pVertexShader_TC = dx->CompileShader(Shader3D, strlen(Shader3D), "VSTextureColor", "vs_5_0");
	pPixelShader_TC = dx->CompileShader(Shader3D, strlen(Shader3D), "PSTextureColor", "ps_5_0");
	//�e�N�X�`��3D���C�g�L
	pVertexShader_TCL = dx->CompileShader(Shader3D, strlen(Shader3D), "VSTextureColorL", "vs_5_0");
	pPixelShader_TCL = dx->CompileShader(Shader3D, strlen(Shader3D), "PSTextureColorL", "ps_5_0");
	//��{�F3D
	pVertexShader_BC = dx->CompileShader(Shader3D, strlen(Shader3D), "VSBaseColor", "vs_5_0");
	pPixelShader_BC = dx->CompileShader(Shader3D, strlen(Shader3D), "PSBaseColor", "ps_5_0");
	//�e�Z���[�^���C�g��
	pVertexShader_DISP = dx->CompileShader(ShaderDisp, strlen(ShaderDisp), "VSDisp", "vs_5_0");
	pPixelShader_DISP = dx->CompileShader(ShaderDisp, strlen(ShaderDisp), "PSDisp", "ps_5_0");
	pHullShader_DISP = dx->CompileShader(ShaderDisp, strlen(ShaderDisp), "HSDisp", "hs_5_0");
	pDomainShader_DISP = dx->CompileShader(ShaderDisp, strlen(ShaderDisp), "DSDisp", "ds_5_0");
	//�e�Z���[�^���C�g�L
	pVertexShader_DISPL = dx->CompileShader(ShaderDisp, strlen(ShaderDisp), "VSDispL", "vs_5_0");
	pPixelShader_DISPL = dx->CompileShader(ShaderDisp, strlen(ShaderDisp), "PSDispL", "ps_5_0");
	pHullShader_DISPL = dx->CompileShader(ShaderDisp, strlen(ShaderDisp), "HSDispL", "hs_5_0");
	pDomainShader_DISPL = dx->CompileShader(ShaderDisp, strlen(ShaderDisp), "DSDispL", "ds_5_0");

	//2D���C�A�E�g
	pVertexLayout_2D =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 3, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3 + 4 * 4, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
	//�e�N�X�`��2D
	pVertexShader_2DTC = dx->CompileShader(Shader2D, strlen(Shader2D), "VSTextureColor", "vs_5_0");
	pPixelShader_2DTC = dx->CompileShader(Shader2D, strlen(Shader2D), "PSTextureColor", "ps_5_0");
	//2D
	pVertexShader_2D = dx->CompileShader(Shader2D, strlen(Shader2D), "VSBaseColor", "vs_5_0");
	pPixelShader_2D = dx->CompileShader(Shader2D, strlen(Shader2D), "PSBaseColor", "ps_5_0");
}

void Dx12Process::TextureGetBuffer(char *Bpass, int i) {
	BGetBuffer(Bpass, &binary_ch[i], &binary_size[i]);
}

void Dx12Process::TextureBinaryDecode(char *Bpass, int i) {
	Bdecode(Bpass, &binary_ch[i], &binary_size[i]);
	texName[i] = GetNameFromPass(Bpass);
}

void Dx12Process::GetTexture() {

	std::unique_ptr<uint8_t[]> decodedData = nullptr;
	D3D12_SUBRESOURCE_DATA subresource;
	Microsoft::WRL::ComPtr<ID3D12Resource> t = nullptr;

	char str[50];
	const int COM = 2;

	Bigin(COM);
	for (int i = 0; i < TEX_PCS; i++) {
		if (binary_size[i] == 0)continue;

		if (FAILED(DirectX::LoadWICTextureFromMemory(md3dDevice.Get(),
			(uint8_t*)binary_ch[i], binary_size[i], &t, decodedData, subresource))) {
			sprintf(str, "�e�N�X�`����%d�ǂݍ��݃G���[", (i));
			throw str;
		}
		D3D12_RESOURCE_DESC texDesc;
		texDesc = t->GetDesc();

		D3D12_HEAP_PROPERTIES HeapProps;
		HeapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
		HeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		HeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		HeapProps.CreationNodeMask = 1;
		HeapProps.VisibleNodeMask = 1;
		HRESULT hr;
		hr = dx->md3dDevice->CreateCommittedResource(&HeapProps, D3D12_HEAP_FLAG_NONE, &texDesc,
			D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&texture[i]));

		//upload
		UINT64 uploadBufferSize = GetRequiredIntermediateSize(texture[i], 0, 1);
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
			nullptr, IID_PPV_ARGS(&textureUp[i]));

		D3D12_RESOURCE_BARRIER BarrierDesc;
		BarrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		BarrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		BarrierDesc.Transition.pResource = texture[i];
		BarrierDesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		BarrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
		BarrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;
		dx_sub[COM].mCommandList->ResourceBarrier(1, &BarrierDesc);

		UpdateSubresources(dx_sub[COM].mCommandList.Get(), texture[i], textureUp[i], 0, 0, 1, &subresource);

		BarrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
		BarrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
		dx_sub[COM].mCommandList->ResourceBarrier(1, &BarrierDesc);
	}
	End(COM);
	WaitFenceCurrent();
}

bool Dx12Process::Initialize(HWND hWnd) {
#if defined(DEBUG) || defined(_DEBUG) 
	//�f�o�b�O���̓f�o�b�O���C���[��L��������
	{
		ComPtr<ID3D12Debug> debugController;
		if (FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))FALSE;
		debugController->EnableDebugLayer();
	}
#endif

	//�t�@�N�g������
	//�A�_�v�^�[�̗񋓁A�X���b�v �`�F�[���̍쐬�A
	//����ёS��ʕ\�����[�h�Ƃ̊Ԃ̐؂�ւ��Ɏg�p����� Alt + 
	//Enter �L�[ �V�[�P���X�Ƃ̃E�B���h�E�̊֘A�t�����s���I�u�W�F�N�g�𐶐����邽�߂Ɏg�p
	if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&mdxgiFactory))))FALSE;

	//�n�[�h�E�G�A�����\��,�n�[�h�E�G�A�����f�o�C�X����
	HRESULT hardwareResult = D3D12CreateDevice(
		nullptr,             // default adapter
		D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS(&md3dDevice));

	//���n�[�h�E�G�A�����s�̏ꍇ�\�t�g�E�G�A�����ɂ���,�\�t�g�E�G�A�����f�o�C�X����
	if (FAILED(hardwareResult))
	{
		ComPtr<IDXGIAdapter> pWarpAdapter;
		if (FAILED(mdxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter))))FALSE;

		if (FAILED(D3D12CreateDevice(
			pWarpAdapter.Get(),
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&md3dDevice))))FALSE;
	}

	//�t�F���X����
	//Command Queue�ɑ��M����Command List�̊��������m���邽�߂Ɏg�p
	if (FAILED(md3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS(&mFence))))FALSE;

	//Descriptor �̃A�h���X�v�Z�Ŏg�p
	mRtvDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	mDsvDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	mCbvSrvUavDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//MultiSample���x���`�F�b�N
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
	msQualityLevels.Format = mBackBufferFormat;
	msQualityLevels.SampleCount = 4;
	msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQualityLevels.NumQualityLevels = 0;
	if (FAILED(md3dDevice->CheckFeatureSupport(
		D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
		&msQualityLevels,     //�@�\�̃T�|�[�g�������f�[�^���i�[
		sizeof(msQualityLevels))))FALSE;

	m4xMsaaQuality = msQualityLevels.NumQualityLevels;
	assert(m4xMsaaQuality > 0 && "Unexpected MSAA quality level.");

	//�R�}���h�L���[�f�X�N
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT; //���ڃR�}���h�L���[
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE; //GPU�^�C���A�E�g���L��
	//�R�}���h�҂��s�񐶐�
	if (FAILED(md3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mCommandQueue))))FALSE;

	for (int i = 0; i < COM_NO; i++) {
		//�R�}���h�A���P�[�^,�R�}���h���X�g����
		dx_sub[i].ListCreate();
	}
	dx_sub[0].Bigin();
	requestSync = replySync = syncFin = FALSE;

	//������
	mSwapChain.Reset();

	//�X���b�v�`�F�C������
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = mClientWidth;
	sd.BufferDesc.Height = mClientHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = mBackBufferFormat;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = m4xMsaaState ? 4 : 1;
	sd.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = SwapChainBufferCount;
	sd.OutputWindow = hWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	if (FAILED(mdxgiFactory->CreateSwapChain(
		mCommandQueue.Get(),
		&sd,
		mSwapChain.GetAddressOf())))FALSE;

	//Descriptor:���̃o�b�t�@�����L�q�����
	//�X���b�v�`�F�C����RenderTarget�Ƃ��Ďg�p���邽�߂�DescriptorHeap���쐬(Descriptor�̋L�^�ꏊ)
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
	rtvHeapDesc.NumDescriptors = SwapChainBufferCount;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;   //RenderTargetView
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; //�V�F�[�_����A�N�Z�X���Ȃ��̂�NONE��OK
	rtvHeapDesc.NodeMask = 0;
	if (FAILED(md3dDevice->CreateDescriptorHeap(
		&rtvHeapDesc, IID_PPV_ARGS(mRtvHeap.GetAddressOf()))))FALSE;

	//�[�x�X�e���V���r��-DescriptorHeap���쐬
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dsvHeapDesc.NodeMask = 0;
	if (FAILED(md3dDevice->CreateDescriptorHeap(
		&dsvHeapDesc, IID_PPV_ARGS(mDsvHeap.GetAddressOf()))))FALSE;

	//�����_�[�^�[�Q�b�g�r���[�f�X�N���v�^�[�q�[�v�̊J�n�n���h���擾
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(mRtvHeap->GetCPUDescriptorHandleForHeapStart());
	for (UINT i = 0; i < SwapChainBufferCount; i++) {
		//�X���b�v�`�F�C���o�b�t�@�擾
		if (FAILED(mSwapChain->GetBuffer(i, IID_PPV_ARGS(&mSwapChainBuffer[i]))))FALSE;
		//�����_�[�^�[�Q�b�g�r���[(Descriptor)����,DescriptorHeap��Descriptor���L�^�����
		md3dDevice->CreateRenderTargetView(mSwapChainBuffer[i].Get(), nullptr, rtvHeapHandle);
		//�q�[�v�ʒu�I�t�Z�b�g(2����̂�2�ڋL�^�ʒu��DescriptorSize���I�t�Z�b�g)
		rtvHeapHandle.Offset(1, mRtvDescriptorSize);
	}

	//�f�v�X�X�e���V���r���[DESC
	D3D12_RESOURCE_DESC depthStencilDesc;
	depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthStencilDesc.Alignment = 0;
	depthStencilDesc.Width = mClientWidth;
	depthStencilDesc.Height = mClientHeight;
	depthStencilDesc.DepthOrArraySize = 1;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.Format = mDepthStencilFormat;
	depthStencilDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
	depthStencilDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
	depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE optClear;
	optClear.Format = mDepthStencilFormat;
	optClear.DepthStencil.Depth = 1.0f;
	optClear.DepthStencil.Stencil = 0;
	//�[�x�X�e���V���o�b�t�@�̈�m��
	if (FAILED(md3dDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthStencilDesc,
		D3D12_RESOURCE_STATE_COMMON,
		&optClear,
		IID_PPV_ARGS(mDepthStencilBuffer.GetAddressOf()))))FALSE;

	//�[�x�X�e���V���r���[�f�X�N���v�^�[�q�[�v�̊J�n�n���h���擾
	CD3DX12_CPU_DESCRIPTOR_HANDLE mDsvHeapHeapHandle(mDsvHeap->GetCPUDescriptorHandleForHeapStart());
	//�[�x�X�e���V���r���[����
	md3dDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), nullptr, mDsvHeapHeapHandle);

	//�[�x�X�e���V���o�b�t�@,���\�[�X�o���A���L���[�x��������
	dx_sub[0].mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mDepthStencilBuffer.Get(),
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));

	//�r���[�|�[�g
	mScreenViewport.TopLeftX = 0;
	mScreenViewport.TopLeftY = 0;
	mScreenViewport.Width = static_cast<float>(mClientWidth);
	mScreenViewport.Height = static_cast<float>(mClientHeight);
	mScreenViewport.MinDepth = 0.0f;
	mScreenViewport.MaxDepth = 1.0f;

	mScissorRect = { 0, 0, mClientWidth, mClientHeight };

	MatrixIdentity(&mProj);
	MatrixIdentity(&mView);
	//�J������p
	ViewY_theta = 55.0f;
	// �A�X�y�N�g��̌v�Z
	aspect = (float)mScreenViewport.Width / (float)mScreenViewport.Height;
	//near�v���[��
	NearPlane = 1.0f;
	//far�v���[��
	FarPlane = 10000.0f;
	MatrixPerspectiveFovLH(&mProj, ViewY_theta, aspect, NearPlane, FarPlane);

	//�r���[�|�[�g�s��쐬(3D���W��2D���W�ϊ��Ɏg�p)
	MatrixViewPort(&Vp);

	dx_sub[0].End();
	WaitFenceCurrent();

	//�|�C���g���C�g�\���̏�����
	ResetPointLight();

	//���s����������
	dlight.Direction.as(0.0f, 0.0f, 0.0f, 0.0f);
	dlight.LightColor.as(1.0f, 1.0f, 1.0f, 1.0f);
	dlight.Lightst.as(1.0f, 0.0f, 0.3f, 0.0f);

	//�t�H�O������
	fog.FogColor.as(1.0f, 1.0f, 1.0f, 1.0f);
	fog.Amount = 0.0f;
	fog.Density = 0.0f;
	fog.on_off = 0.0f;

	CreateShaderByteCode();

	SkinMesh::CreateManager();

	return TRUE;
}

void Dx12Process::Sclear() {

	dx_sub[0].Bigin();

	dx_sub[0].mCommandList->RSSetViewports(1, &mScreenViewport);
	dx_sub[0].mCommandList->RSSetScissorRects(1, &mScissorRect);

	dx_sub[0].mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mSwapChainBuffer[mCurrBackBuffer].Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	dx_sub[0].mCommandList->ClearRenderTargetView(CD3DX12_CPU_DESCRIPTOR_HANDLE(
		mRtvHeap->GetCPUDescriptorHandleForHeapStart(),
		mCurrBackBuffer,
		mRtvDescriptorSize), DirectX::Colors::Black, 0, nullptr);
	dx_sub[0].mCommandList->ClearDepthStencilView(mDsvHeap->GetCPUDescriptorHandleForHeapStart(),
		D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	dx_sub[0].mCommandList->OMSetRenderTargets(1, &CD3DX12_CPU_DESCRIPTOR_HANDLE(
		mRtvHeap->GetCPUDescriptorHandleForHeapStart(),
		mCurrBackBuffer,
		mRtvDescriptorSize), true, &mDsvHeap->GetCPUDescriptorHandleForHeapStart());

	dx_sub[0].mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(dx->mSwapChainBuffer[dx->mCurrBackBuffer].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	dx_sub[0].End();
}

void Dx12Process::Bigin(int com_no) {
	dx_sub[com_no].Bigin();
}

void Dx12Process::End(int com_no) {
	dx_sub[com_no].End();
}

void Dx12Process::DrawScreen() {
	// swap the back and front buffers
	mSwapChain->Present(0, 0);
	mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;
}

void Dx12Process::Cameraset(float cx1, float cx2, float cy1, float cy2, float cz1, float cz2) {

	//�J�����̈ʒu�ƕ�����ݒ�
	MatrixLookAtLH(&mView,
		cx1, cy1, cz1,   //�J�����̈ʒu
		cx2, cy2, cz2,   //�J�����̕�����������_
		0.0f, 0.0f, 1.0f); //�J�����̏�̕���(�ʏ펋�_�ł̏������1.0f�ɂ���)
	//�V�F�[�_�[�v�Z�p���W�o�^(���_����̋����Ŏg��)
	posX = cx1;
	posY = cy1;
	posZ = cz1;
}

void Dx12Process::ResetPointLight() {
	for (int i = 0; i < LIGHT_PCS; i++) {
		plight.LightPos[i].as(0.0f, 0.0f, 0.0f, 0.0f);
		plight.LightColor[i].as(0.0f, 0.0f, 0.0f, 0.0f);
		plight.Lightst[i].as(0.0f, 0.0f, 0.0f, 0.0f);
	}
	plight.ShadowLow_val = 0.0f;
}

void Dx12Process::P_ShadowBright(float val) {
	plight.ShadowLow_val = val;
}

void Dx12Process::PointLightPosSet(int Idx, float x, float y, float z, float r, float g, float b, float a, float range,
	float brightness, float attenuation, bool on_off) {

	static int pcs = LIGHT_PCS_init;//�������l(�\��ς�)

	if (Idx > LIGHT_PCS - 1 || Idx < 0)return;//�G���[�h�~

	if (Idx > LIGHT_PCS_init - 1 && on_off == TRUE)pcs = Idx + 1;

	float onoff;
	if (on_off == TRUE)onoff = 1.0f; else onoff = 0.0f;
	plight.LightPos[Idx].as(x, y, z, 1.0f);
	plight.LightColor[Idx].as(r, g, b, a);
	plight.Lightst[Idx].as(range, brightness, attenuation, onoff);
	plight.LightPcs = pcs;
}

void Dx12Process::DirectionLight(float x, float y, float z, float r, float g, float b, float bright, float ShadowBright) {
	dlight.Direction.as(x, y, z, 0.0f);
	dlight.LightColor.as(r, g, b, 0.0f);
	dlight.Lightst.x = bright;
	dlight.Lightst.z = ShadowBright;
	dlight.Lightst.w = 0.0f;
}

void Dx12Process::SetDirectionLight(bool onoff) {
	float f = 0.0f;
	if (onoff == TRUE)f = 1.0f;
	dlight.Lightst.y = f;
}

void Dx12Process::Fog(float r, float g, float b, float amount, float density, bool onoff) {

	if (onoff == FALSE) {
		fog.on_off = 0.0f;
		return;
	}
	fog.on_off = 1.0f;
	fog.FogColor.as(r, g, b, 1.0f);
	fog.Amount = amount;
	fog.Density = density;
}

Microsoft::WRL::ComPtr<ID3D12Resource> Dx12Process::CreateDefaultBuffer(
	ID3D12Device* device,
	ID3D12GraphicsCommandList* cmdList,
	const void* initData,
	UINT64 byteSize,
	Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer)
{
	ComPtr<ID3D12Resource> defaultBuffer;

	//�f�t�H���g�o�b�t�@����
	device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(byteSize),
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(defaultBuffer.GetAddressOf()));

	//�f�t�H���g�o�b�t�@��CPU�������f�[�^���R�s�[���邽�߂̒��ԃo�b�t�@����
	device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(byteSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(uploadBuffer.GetAddressOf()));

	//�f�t�H���g�o�b�t�@�ɃR�s�[�������f�[�^�̓��e
	D3D12_SUBRESOURCE_DATA subResourceData = {};
	subResourceData.pData = initData;
	subResourceData.RowPitch = (LONG_PTR)byteSize;
	subResourceData.SlicePitch = subResourceData.RowPitch;

	//�f�t�H���g�o�b�t�@,���\�[�X�o���A���L���R�s�[����鑤
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(),
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));
	//UpdateSubresources:���ԃo�b�t�@��CPU���������R�s�[����
	//���̌�CommandList���g���ƒ��ԃo�b�t�@�̓��e���A�b�v���[�h�����
	UpdateSubresources<1>(cmdList, defaultBuffer.Get(), uploadBuffer.Get(), 0, 0, 1, &subResourceData);
	//�f�t�H���g�o�b�t�@,���\�[�X�o���A�R�s�[����鑤���A�b�v���[�h
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ));

	//�R�}���h���X�g���쐬����Ȃ��ꍇ�ł��R�s�[�͎��s�����
	//�R�s�[�����s���ꂽ��uploadBuffer�̓����[�X�����

	return defaultBuffer;
}

Microsoft::WRL::ComPtr<ID3D12Resource> Dx12Process::CreateStreamBuffer(ID3D12Device* device, UINT64 byteSize)
{
	ComPtr<ID3D12Resource> defaultBuffer;

	//�X�g���[���o�b�t�@����
	device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(byteSize),
		D3D12_RESOURCE_STATE_STREAM_OUT,
		nullptr,
		IID_PPV_ARGS(defaultBuffer.GetAddressOf()));

	return defaultBuffer;
}

ComPtr<ID3DBlob> Dx12Process::CompileShader(LPSTR szFileName, size_t size, LPSTR szFuncName, LPSTR szProfileName) {

	HRESULT hr = S_OK;

	ComPtr<ID3DBlob> byteCode = nullptr;
	ComPtr<ID3DBlob> errors;
	hr = D3DCompile(szFileName, size, NULL, NULL, NULL, szFuncName, szProfileName,
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, &byteCode, &errors);

	//�G���[�������ǂ����邩

	return byteCode;
}

void  Dx12Process::InstancedMap(float x, float y, float z, float thetaZ, float thetaY, float thetaX, float size) {
	InstancedMapSize3(x, y, z, thetaZ, thetaY, thetaX, size, size, size);
}

void Dx12Process::InstancedMapSize3(float x, float y, float z, float thetaZ, float thetaY, float thetaX, float sizeX, float sizeY, float sizeZ) {

	if (ins_no > INSTANCE_PCS_3D - 1)ins_no--;
	MATRIX mov;
	MATRIX rotZ, rotY, rotX, rotZY, rotZYX;
	MATRIX scale;
	MATRIX scro;
	MATRIX world;
	MATRIX WV;

	//�g��k��
	MatrixScaling(&scale, sizeX, sizeY, sizeZ);
	//�\���ʒu
	MatrixRotationZ(&rotZ, thetaZ);
	MatrixRotationY(&rotY, thetaY);
	MatrixRotationX(&rotX, thetaX);
	MatrixMultiply(&rotZY, &rotZ, &rotY);
	MatrixMultiply(&rotZYX, &rotZY, &rotX);
	MatrixTranslation(&mov, x, y, z);
	MatrixMultiply(&scro, &rotZYX, &scale);
	MatrixMultiply(&world, &scro, &mov);

	//���[���h�A�J�����A�ˉe�s��A��
	cb.World[ins_no] = world;
	MatrixMultiply(&WV, &world, &mView);
	MatrixMultiply(&cb.WVP[ins_no], &WV, &mProj);
	MatrixTranspose(&cb.World[ins_no]);
	MatrixTranspose(&cb.WVP[ins_no]);
	ins_no++;
}

void Dx12Process::MatrixMap2(UploadBuffer<CONSTANT_BUFFER> *mObjectCB, float r, float g, float b, float disp, float px, float py, float mx, float my) {

	cb.C_Pos.as(posX, posY, posZ, 0.0f);
	cb.AddObjColor.as(r, g, b, 0.0f);
	cb.pShadowLow_Lpcs.as(plight.ShadowLow_val, (float)plight.LightPcs, 0.0f, 0.0f);
	memcpy(cb.pLightPos, plight.LightPos, sizeof(VECTOR4) * LIGHT_PCS);
	memcpy(cb.pLightColor, plight.LightColor, sizeof(VECTOR4) * LIGHT_PCS);
	memcpy(cb.pLightst, plight.Lightst, sizeof(VECTOR4) * LIGHT_PCS);
	cb.dDirection = dlight.Direction;
	cb.dLightColor = dlight.LightColor;
	cb.dLightst = dlight.Lightst;
	cb.FogAmo_Density.as(fog.Amount, fog.Density, fog.on_off, 0.0f);
	cb.FogColor = fog.FogColor;
	if (disp == 0.0f)disp = 3.0f;
	cb.DispAmount.as(disp, 0.0f, 0.0f, 0.0f);
	cb.pXpYmXmY.as(px, py, mx, my);
	mObjectCB->CopyData(0, cb);
}

void Dx12Process::MatrixMap(UploadBuffer<CONSTANT_BUFFER> *mObjectCB, float x, float y, float z,
	float r, float g, float b, float thetaZ, float thetaY, float thetaX, float size, float disp, float px, float py, float mx, float my) {

	MatrixMapSize3(mObjectCB, x, y, z, r, g, b, thetaZ, thetaY, thetaX,
		size, size, size, disp, px, py, mx, my);
}

void Dx12Process::MatrixMapSize3(UploadBuffer<CONSTANT_BUFFER> *mObjectCB, float x, float y, float z,
	float r, float g, float b, float thetaZ, float thetaY, float thetaX,
	float sizeX, float sizeY, float sizeZ, float disp, float px, float py, float mx, float my) {

	if (ins_no > INSTANCE_PCS_3D - 1)ins_no--;
	MATRIX mov;
	MATRIX rotZ, rotY, rotX, rotZY, rotZYX;
	MATRIX scale;
	MATRIX scro;
	MATRIX world;
	MATRIX WV;

	//�g��k��
	MatrixScaling(&scale, sizeX, sizeY, sizeZ);
	//�\���ʒu
	MatrixRotationZ(&rotZ, thetaZ);
	MatrixRotationY(&rotY, thetaY);
	MatrixRotationX(&rotX, thetaX);
	MatrixMultiply(&rotZY, &rotZ, &rotY);
	MatrixMultiply(&rotZYX, &rotZY, &rotX);
	MatrixTranslation(&mov, x, y, z);
	MatrixMultiply(&scro, &rotZYX, &scale);
	MatrixMultiply(&world, &scro, &mov);

	cb.World[ins_no] = world;
	MatrixMultiply(&WV, &world, &mView);
	MatrixMultiply(&cb.WVP[ins_no], &WV, &mProj);
	MatrixTranspose(&cb.World[ins_no]);
	MatrixTranspose(&cb.WVP[ins_no]);
	MatrixMap2(mObjectCB, r, g, b, disp, px, py, mx, my);
	ins_no++;
}

std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> Dx12Process::GetStaticSamplers()
{
	// Applications usually only need a handful of samplers.  So just define them all up front
	// and keep them available as part of the root signature.  

	const CD3DX12_STATIC_SAMPLER_DESC pointWrap(
		0, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC pointClamp(
		1, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC linearWrap(
		2, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC linearClamp(
		3, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC anisotropicWrap(
		4, // shaderRegister
		D3D12_FILTER_ANISOTROPIC, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressW
		0.0f,                             // mipLODBias
		8);                               // maxAnisotropy

	const CD3DX12_STATIC_SAMPLER_DESC anisotropicClamp(
		5, // shaderRegister
		D3D12_FILTER_ANISOTROPIC, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressW
		0.0f,                              // mipLODBias
		8);                                // maxAnisotropy

	return{
		pointWrap, pointClamp,
		linearWrap, linearClamp,
		anisotropicWrap, anisotropicClamp };
}

float Dx12Process::GetViewY_theta() {
	return ViewY_theta;
}

float Dx12Process::Getaspect() {
	return aspect;
}

float Dx12Process::GetNearPlane() {
	return NearPlane;
}

float Dx12Process::GetFarPlane() {
	return FarPlane;
}

char *Dx12Process::GetNameFromPass(char *pass) {

	CHAR temp[255];
	strcpy_s(temp, pass);

	bool f = FALSE;

	for (int i = 0; temp[i] != '\0' && i < 255; i++) {
		if (temp[i] == '\\' || temp[i] == '/') { f = TRUE; break; }
	}

	if (f) {
		//�t�@�C�����݂̂ł͖����ꍇ�̏���
		while (*pass != '\0') pass++;//�I�[�����܂Ń|�C���^��i�߂�
		while (*pass != '\\' && *pass != '/')pass--;//�t�@�C�����擪��'\'��'/'�܂Ń|�C���^��i�߂�
		pass++;//'\'�܂���'/'�̎�(�t�@�C�����擪����)�܂Ń|�C���^��i�߂�
	}

	return pass;//�|�C���^���삵�Ă�̂ŕԂ�l���g�p������
}

//�ړ��ʈ�艻
DWORD T_float::f = timeGetTime();
DWORD T_float::time = 0;
DWORD T_float::time_fps = 0;//FPS�v���p
int T_float::frame = 0;     //FPS�v���g�p
char T_float::str[50];     //�E�C���h�E�g�����\���g�p

void T_float::GetTime(HWND hWnd) {
	time = timeGetTime() - f;
	f = timeGetTime();

	//FPS�v��
	frame++;
	sprintf(str, "     Ctrl:����  Delete:�L�����Z��  fps=%d", frame);
	if (timeGetTime() - time_fps > 1000)
	{
		time_fps = timeGetTime();
		frame = 0;
		char Name[100] = { 0 };
		GetClassNameA(hWnd, Name, sizeof(Name));
		strcat(Name, str);
		SetWindowTextA(hWnd, Name);
	}
}

float T_float::Add(float f) {
	float r = ((float)time * f) / 2.0f;
	if (r <= 0.0f)return 0.01f;
	return r;
}

//�G���[���b�Z�[�W
void ErrorMessage(char *E_mes) {
	MessageBoxA(0, E_mes, 0, MB_OK);
}