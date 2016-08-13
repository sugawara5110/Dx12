//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@          Dx12Process�N���X                                 **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Dx12Process_Header
#define Class_Dx12Process_Header

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <wrl.h>//Microsoft::WRL
#include "DxStruct.h"
#include "Enum.h"
#include <dxgi1_4.h>
#include <d3d12.h>
#include "d3dx12.h"
#include <d3d10_1.h>
#include <D3Dcompiler.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include "WICTextureLoader.h"
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <unordered_map>
#include <assert.h>

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

#define RELEASE(p)    if(p){p->Release();  p=NULL;}
#define S_DELETE(p)   if(p){delete p;      p=NULL;}
#define ARR_DELETE(p) if(p){delete[] p;    p=NULL;}
#define TEX_PCS 130

//�O���錾
template<typename T>
class UploadBuffer;
class PolygonData;
//�O���錾

class Dx12Process {

private:
	friend PolygonData;

	Microsoft::WRL::ComPtr<IDXGIFactory4> mdxgiFactory;
	Microsoft::WRL::ComPtr<ID3D12Device> md3dDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
	Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
	UINT64 mCurrentFence = 0;

	//MultiSample���x���`�F�b�N
	bool m4xMsaaState = false;
	UINT m4xMsaaQuality = 0;

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;

	static const int SwapChainBufferCount = 2;
	int mCurrBackBuffer = 0;
	Microsoft::WRL::ComPtr<ID3D12Resource> mSwapChainBuffer[SwapChainBufferCount];
	Microsoft::WRL::ComPtr<ID3D12Resource> mDepthStencilBuffer;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mRtvHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDsvHeap;

	//�T���v��
	std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> GetStaticSamplers();

	D3D12_VIEWPORT mScreenViewport;
	D3D12_RECT mScissorRect;

	UINT mRtvDescriptorSize = 0;
	UINT mDsvDescriptorSize = 0;
	UINT mCbvSrvUavDescriptorSize = 0;

	DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	int mClientWidth = WINDOW_WIDTH;
	int mClientHeight = WINDOW_HEIGHT;

	//�e�N�X�`��
	char  **binary_ch;                   //�f�R�[�h��o�C�i��
	int   *binary_size;                 //�o�C�i���T�C�Y

	static Dx12Process *dx;//�N���X���ŃI�u�W�F�N�g�������g���܂킷

	MATRIX mProj;
	MATRIX mView;
	float posX, posY, posZ;

	//�J������p
	float ViewY_theta;
	//�A�X�y�N�g��
	float aspect;
	//near�v���[��
	float NearPlane;
	//far�v���[��
	float FarPlane;

	PointLight plight;
	DirectionLight dlight;
	Fog fog;

	Dx12Process() {}//�O������̃I�u�W�F�N�g�����֎~
	Dx12Process(const Dx12Process &obj) {}     // �R�s�[�R���X�g���N�^�֎~
	void operator=(const Dx12Process& obj) {};// ������Z�q�֎~
	~Dx12Process();
	
	void FlushCommandQueue();
	void TextureBinaryDecode(char *Bpass, int i);//�Í����ς݉摜�o�C�i���f�R�[�h
	void GetTexture(ID3D12GraphicsCommandList *mCommandList, ID3D12Resource **texture, int TexNo); //TextureBinaryDecodeAll();�̌�ɂ�鎖

	Microsoft::WRL::ComPtr<ID3D12Resource> CreateDefaultBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList,
		const void* initData, UINT64 byteSize, Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer);
	Microsoft::WRL::ComPtr<ID3DBlob> CompileShader(LPSTR szFileName, size_t size, LPSTR szFuncName, LPSTR szProfileName);
	void MatrixMap(UploadBuffer<CONSTANT_BUFFER> *mObjectCB, float x, float y, float z,
		float r, float g, float b, float thetaZ, float thetaY, float thetaX, float size, float disp);

public:
	static void InstanceCreate();
	static Dx12Process *GetInstance();
	static void DeleteInstance();
	bool Initialize(HWND hWnd);
	void TextureBinaryDecodeAll();
	void Sclear();
	void Drawscreen();
	void Cameraset(float cx1, float cx2, float cy1, float cy2, float cz1, float cz2);
	void ResetPointLight();
	void P_ShadowBright(float val);
	void PointLightPosSet(int Idx, float x, float y, float z, float r, float g, float b, float a, float range,
		float brightness, float attenuation, bool on_off);//0:���_, 1:���X�{�X, 2:�o�����, 3456:�퓬
	void DirectionLight(float x, float y, float z, float r, float g, float b, float bright, float ShadowBright);
	void SetDirectionLight(bool onoff);
	void Fog(float r, float g, float b, float amount, float density, bool onoff);
	float GetViewY_theta();
	float Getaspect();
	float GetNearPlane();
	float GetFarPlane();
};

struct MeshGeometry {

	//�e�p�����[�^�[�������ŃR�s�[����
	Microsoft::WRL::ComPtr<ID3DBlob> VertexBufferCPU = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> IndexBufferCPU = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> VertexBufferGPU = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> IndexBufferGPU = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> VertexBufferUploader = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> IndexBufferUploader = nullptr;

	//�o�b�t�@�̃T�C�Y��
	UINT VertexByteStride = 0;
	UINT VertexBufferByteSize = 0;
	DXGI_FORMAT IndexFormat = DXGI_FORMAT_R16_UINT;
	UINT IndexBufferByteSize = 0;
	//DrawIndexedInstanced�̈����Ŏg�p
	UINT IndexCount = 0;

	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW VertexBufferView()const
	{
		D3D12_VERTEX_BUFFER_VIEW vbv;
		vbv.BufferLocation = VertexBufferGPU->GetGPUVirtualAddress();
		vbv.StrideInBytes = VertexByteStride;
		vbv.SizeInBytes = VertexBufferByteSize;

		return vbv;
	}

	//�C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW IndexBufferView()const
	{
		D3D12_INDEX_BUFFER_VIEW ibv;
		ibv.BufferLocation = IndexBufferGPU->GetGPUVirtualAddress();
		ibv.Format = IndexFormat;
		ibv.SizeInBytes = IndexBufferByteSize;

		return ibv;
	}

	//GPU�ɃA�b�v���[�h�㎩���ŉ������
	void DisposeUploaders()
	{
		VertexBufferUploader = nullptr;
		IndexBufferUploader = nullptr;
	}
};

template<typename T>
class UploadBuffer {

public:
	UploadBuffer(ID3D12Device *device, UINT elementCount, bool isConstantBuffer) :
		mIsConstantBuffer(isConstantBuffer)
	{
		mElementByteSize = sizeof(T);

		//�R���X�^���g�o�b�t�@�̗v�f��256�o�C�g�ɂ��Ă���(�n�[�h�E�G�A�̓s����)
		// at m*256 byte offsets and of n*256 byte lengths. 
		// typedef struct D3D12_CONSTANT_BUFFER_VIEW_DESC {
		// UINT64 OffsetInBytes; // multiple of 256
		// UINT   SizeInBytes;   // multiple of 256
		// } D3D12_CONSTANT_BUFFER_VIEW_DESC;
		if (isConstantBuffer)
			mElementByteSize = (sizeof(T) + 255) & ~255;

		device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(mElementByteSize * elementCount),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mUploadBuffer));

		mUploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mMappedData));
		//�f�X�g���N�^��Unmap
		//GPU�g�p���������܂Ȃ���
	}

	UploadBuffer(const UploadBuffer& rhs) = delete;
	UploadBuffer& operator=(const UploadBuffer& rhs) = delete;
	~UploadBuffer()
	{
		if (mUploadBuffer != nullptr)
			mUploadBuffer->Unmap(0, nullptr);

		mMappedData = nullptr;
	}

	ID3D12Resource *Resource()const
	{
		return mUploadBuffer.Get();
	}

	void CopyData(int elementIndex, const T& data)
	{
		memcpy(&mMappedData[elementIndex*mElementByteSize], &data, sizeof(T));
	}

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> mUploadBuffer;
	BYTE* mMappedData = nullptr;

	UINT mElementByteSize = 0;
	bool mIsConstantBuffer = false;
};

//*********************************PolygonData�N���X*************************************//

class PolygonData {

private:
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;

	Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mSrvHeap = nullptr;

	//�R���X�^���g�o�b�t�@OBJ
	UploadBuffer<CONSTANT_BUFFER> *mObjectCB = nullptr;

	//���_�o�b�t�@OBJ
	std::unique_ptr<MeshGeometry> mBoxGeo = nullptr;

	//�V�F�[�_�[�o�C�g�R�[�h
	Microsoft::WRL::ComPtr<ID3DBlob> mvsByteCode = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> mpsByteCode = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> mhsByteCode = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> mdsByteCode = nullptr;


	//�e�N�X�`���ێ�
	ID3D12Resource *texture;

	//�C���v�b�g���C�A�E�g
	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

	//�p�C�v���C��OBJ
	Microsoft::WRL::ComPtr<ID3D12PipelineState> mPSO = nullptr;

	//�|�C���^���󂯎��
	Dx12Process *dx;

	Vertex         *d3varray;  //���_�z��
	std::uint16_t  *d3varrayI;//���_�C���f�b�N�X
	int            ver;      //���_��
	int            verI;    //���_�C���f�b�N�X

	D3D12_PRIMITIVE_TOPOLOGY_TYPE  primType_create;
	D3D_PRIMITIVE_TOPOLOGY         primType_draw;

public:
	PolygonData();
	~PolygonData();
	void GetVBarray(PrimitiveType type, bool light, int v);
	void Create();
	void SetVertex(int I1, int I2, int i,
		float vx, float vy, float vz,
		float nx, float ny, float nz,
		float r, float g, float b, float a,
		float u, float v);
	void SetVertex(int I1, int i,
		float vx, float vy, float vz,
		float nx, float ny, float nz,
		float r, float g, float b, float a,
		float u, float v);
	void Draw(float x, float y, float z, float r, float g, float b, float theta, float disp);
};

class T_float {

private:
	static DWORD f, time;
	static DWORD time_fps;//FPS�v���p
	static int frame;    //FPS�v���g�p
	static char str[50];//�E�C���h�E�g�����\���g�p

public:
	static void GetTime(HWND hWnd);//��Ɏ��s
	float Add(float f);
};

#endif
