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
#include "WICTextureLoader12.h"
#include <memory>
#include <stdlib.h>
#include <string>
#include <vector>
#include <array>
#include <unordered_map>
#include <assert.h>
#include <Process.h>
#include <fbxsdk.h>
#include <mutex>
#include <new>

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

#define COM_NO        7
#define TEX_PCS       130

//�O���錾
template<typename T>
class UploadBuffer;
class Dx12Process;
class MeshData;
class PolygonData;
class PolygonData2D;
class ParticleData;
class SkinMesh;
class DxText;
//�O���錾

class Dx12Process_sub {

private:
	friend Dx12Process;
	friend MeshData;
	friend PolygonData;
	friend PolygonData2D;
	friend ParticleData;
	friend SkinMesh;

	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mCmdListAlloc[2];
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;
	int mAloc_Num = 0;
	volatile ComListState mComState;

	void ListCreate();
	void Bigin();
	void End();
};

class Dx12Process {

private:
	friend MeshData;
	friend PolygonData;
	friend PolygonData2D;
	friend ParticleData;
	friend SkinMesh;
	friend Dx12Process_sub;

	Microsoft::WRL::ComPtr<IDXGIFactory4> mdxgiFactory;
	Microsoft::WRL::ComPtr<ID3D12Device> md3dDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
	Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
	UINT64 mCurrentFence = 0;

	//MultiSample���x���`�F�b�N
	bool m4xMsaaState = false;
	UINT m4xMsaaQuality = 0;

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;
	Dx12Process_sub dx_sub[COM_NO];
	volatile bool requestSync, replySync, syncFin;

	static const int SwapChainBufferCount = 2;
	int mCurrBackBuffer = 0;
	Microsoft::WRL::ComPtr<ID3D12Resource> mSwapChainBuffer[SwapChainBufferCount];
	Microsoft::WRL::ComPtr<ID3D12Resource> mDepthStencilBuffer;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mRtvHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDsvHeap;

	//�V�F�[�_�[�o�C�g�R�[�h
	Microsoft::WRL::ComPtr<ID3DBlob> pGeometryShader_PSO = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pGeometryShader_P = nullptr;

	Microsoft::WRL::ComPtr<ID3DBlob> pHullShader_MESH_D = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pHullShader_DISPL = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pHullShader_DISP = nullptr;

	Microsoft::WRL::ComPtr<ID3DBlob> pDomainShader_MESH_D = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pDomainShader_DISPL = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pDomainShader_DISP = nullptr;

	std::vector<D3D12_INPUT_ELEMENT_DESC> pVertexLayout_SKIN;
	std::vector<D3D12_SO_DECLARATION_ENTRY> pDeclaration_PSO;
	std::vector<D3D12_INPUT_ELEMENT_DESC> pVertexLayout_P;
	std::vector<D3D12_INPUT_ELEMENT_DESC> pVertexLayout_MESH;
	std::vector<D3D12_INPUT_ELEMENT_DESC> pVertexLayout_3D;
	std::vector<D3D12_INPUT_ELEMENT_DESC> pVertexLayout_2D;

	Microsoft::WRL::ComPtr<ID3DBlob> pVertexShader_SKIN = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pVertexShader_PSO = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pVertexShader_P = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pVertexShader_MESH_D = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pVertexShader_MESH = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pVertexShader_DISPL = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pVertexShader_DISP = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pVertexShader_TCL = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pVertexShader_TC = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pVertexShader_BC = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pVertexShader_2D = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pVertexShader_2DTC = nullptr;

	Microsoft::WRL::ComPtr<ID3DBlob> pPixelShader_SKIN = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pPixelShader_P = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pPixelShader_MESH_D = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pPixelShader_MESH = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pPixelShader_DISPL = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pPixelShader_DISP = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pPixelShader_TCL = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pPixelShader_TC = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pPixelShader_BC = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pPixelShader_2D = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pPixelShader_2DTC = nullptr;

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
	char  *binary_ch[TEX_PCS] = { 0 };    //�f�R�[�h��o�C�i��
	int   binary_size[TEX_PCS] = { 0 };  //�o�C�i���T�C�Y
	char  *texName[TEX_PCS] = { 0 };
	ID3D12Resource *texture[TEX_PCS];
	ID3D12Resource *textureUp[TEX_PCS];

	static Dx12Process *dx;//�N���X���ŃI�u�W�F�N�g�������g���܂킷

	MATRIX      mProj;
	MATRIX      mView;
	MATRIX      Vp;    //�r���[�|�[�g�s��(3D���W��2D���W�ϊ����g�p)
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

	CONSTANT_BUFFER cb;
	int  ins_no = 0;

	static std::mutex mtx;

	Dx12Process() {}//�O������̃I�u�W�F�N�g�����֎~
	Dx12Process(const Dx12Process &obj) {}     // �R�s�[�R���X�g���N�^�֎~
	void operator=(const Dx12Process& obj) {};// ������Z�q�֎~
	~Dx12Process();
	
	void CreateShaderByteCode();

	Microsoft::WRL::ComPtr<ID3D12Resource> CreateDefaultBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList,
		const void* initData, UINT64 byteSize, Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer);

	Microsoft::WRL::ComPtr<ID3D12Resource> CreateStreamBuffer(ID3D12Device* device, UINT64 byteSize);

	Microsoft::WRL::ComPtr<ID3DBlob> CompileShader(LPSTR szFileName, size_t size, LPSTR szFuncName, LPSTR szProfileName);

	void InstancedMap(float x, float y, float z, float thetaZ, float thetaY, float thetaX, float size);

	void InstancedMapSize3(float x, float y, float z, float thetaZ, float thetaY, float thetaX, float sizeX, float sizeY, float sizeZ);

	void MatrixMap2(UploadBuffer<CONSTANT_BUFFER> *mObjectCB, float r, float g, float b, float disp, float px, float py, float mx, float my);

	void MatrixMap(UploadBuffer<CONSTANT_BUFFER> *mObjectCB, float x, float y, float z,
		float r, float g, float b, float thetaZ, float thetaY, float thetaX, float size, float disp, float px, float py, float mx, float my);

	void MatrixMapSize3(UploadBuffer<CONSTANT_BUFFER> *mObjectCB, float x, float y, float z,
		float r, float g, float b, float thetaZ, float thetaY, float thetaX, float sizeX, float sizeY, float sizeZ, float disp, float px, float py, float mx, float my);
	void WaitFence(int fence);
	char *GetNameFromPass(char *pass);
	
public:
	static void InstanceCreate();
	static Dx12Process *GetInstance();
	static void DeleteInstance();
	static void Lock() { mtx.lock(); }
	static void Unlock() { mtx.unlock(); }

	bool Initialize(HWND hWnd);
	void TextureGetBuffer(char *Bpass, int i);
	void TextureBinaryDecode(char *Bpass, int i);//�Í����ς݉摜�o�C�i���f�R�[�h
	void GetTexture();
	void Sclear();
	void Bigin(int com_no);
	void End(int com_no);
	void WaitFenceCurrent();//GPU�������̂܂ܑ҂�
	void WaitFencePast();//�O��GPU���������̏ꍇ�҂�
	void RequestSync();//�����v��
	void ReplySync();//�����ԓ�
	void SyncFin(bool on);//TRUE:�����I��
	bool SyncFin();
	void DrawScreen();
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

struct VertexView {

	//�e�p�����[�^�[�������ŃR�s�[����
	Microsoft::WRL::ComPtr<ID3DBlob> VertexBufferCPU = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> VertexBufferGPU = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> VertexBufferUploader = nullptr;

	//�o�b�t�@�̃T�C�Y��
	UINT VertexByteStride = 0;
	UINT VertexBufferByteSize = 0;

	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW VertexBufferView()const
	{
		D3D12_VERTEX_BUFFER_VIEW vbv;
		vbv.BufferLocation = VertexBufferGPU->GetGPUVirtualAddress();
		vbv.StrideInBytes = VertexByteStride;
		vbv.SizeInBytes = VertexBufferByteSize;

		return vbv;
	}
};

struct IndexView {

	//�e�p�����[�^�[�������ŃR�s�[����
	Microsoft::WRL::ComPtr<ID3DBlob> IndexBufferCPU = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> IndexBufferGPU = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> IndexBufferUploader = nullptr;

	//�o�b�t�@�̃T�C�Y��
	DXGI_FORMAT IndexFormat = DXGI_FORMAT_R16_UINT;
	UINT IndexBufferByteSize = 0;
	//DrawIndexedInstanced�̈����Ŏg�p
	UINT IndexCount = 0;

	//�C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW IndexBufferView()const
	{
		D3D12_INDEX_BUFFER_VIEW ibv;
		ibv.BufferLocation = IndexBufferGPU->GetGPUVirtualAddress();
		ibv.Format = IndexFormat;
		ibv.SizeInBytes = IndexBufferByteSize;

		return ibv;
	}
};

struct StreamView {

	Microsoft::WRL::ComPtr<ID3D12Resource> StreamBufferGPU = nullptr;

	//�o�b�t�@�̃T�C�Y��
	UINT StreamByteStride = 0;
	UINT StreamBufferByteSize = 0;
	D3D12_GPU_VIRTUAL_ADDRESS BufferFilledSizeLocation;

	//�X�g���[���o�b�t�@�r���[
	D3D12_STREAM_OUTPUT_BUFFER_VIEW StreamBufferView()const
	{
		D3D12_STREAM_OUTPUT_BUFFER_VIEW sbv;
		sbv.BufferLocation = StreamBufferGPU->GetGPUVirtualAddress();
		sbv.SizeInBytes = StreamBufferByteSize;
		sbv.BufferFilledSizeLocation = BufferFilledSizeLocation;//�ȑO�̃T�C�Y?
		return sbv;
	}
};

template<typename T>
class UploadBuffer {

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> mUploadBuffer;
	BYTE *mMappedData = nullptr;
	UINT mElementByteSize = 0;

public:
	UploadBuffer(ID3D12Device *device, UINT elementCount, bool isConstantBuffer) {

		mElementByteSize = sizeof(T);

		//�R���X�^���g�o�b�t�@�T�C�Y��256�o�C�g�P�ʂɂ��Ă���(�A���C�����g)
		// at m*256 byte offsets and of n*256 byte lengths. 
		// typedef struct D3D12_CONSTANT_BUFFER_VIEW_DESC {
		// UINT64 OffsetInBytes; // multiple of 256
		// UINT   SizeInBytes;   // multiple of 256
		// } D3D12_CONSTANT_BUFFER_VIEW_DESC;
		if (isConstantBuffer)//�R���X�^���g�o�b�t�@�̏ꍇ
			mElementByteSize = (sizeof(T) + 255) & ~255;//255�𑫂���255�̕␔�̘_���ς����B(255�P�ʂɕϊ�)

		device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(mElementByteSize * elementCount),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mUploadBuffer));

		mUploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mMappedData));
	}

	UploadBuffer(const UploadBuffer& rhs) = delete;
	UploadBuffer& operator=(const UploadBuffer& rhs) = delete;

	~UploadBuffer() {

		if (mUploadBuffer != nullptr)
			mUploadBuffer->Unmap(0, nullptr);

		mMappedData = nullptr;
	}

	ID3D12Resource *Resource()const {
		return mUploadBuffer.Get();
	}

	void CopyData(int elementIndex, const T& data) {
		memcpy(&mMappedData[elementIndex*mElementByteSize], &data, sizeof(T));
	}
};

//*********************************MeshData�N���X*************************************//

class MeshData {

private:
	Dx12Process                *dx;
	ID3D12GraphicsCommandList  *mCommandList;
	int                        com_no = 0;
	ID3DBlob                   *vs = nullptr;
	ID3DBlob                   *ps = nullptr;
	ID3DBlob                   *hs = nullptr;
	ID3DBlob                   *ds = nullptr;

	Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mSrvHeap = nullptr;

	//�R���X�^���g�o�b�t�@OBJ
	UploadBuffer<CONSTANT_BUFFER> *mObjectCB = nullptr;
	UploadBuffer<CONSTANT_BUFFER_MESH> *mObject_MESHCB = nullptr;//�}�e���A���n���p

	//���_�o�b�t�@OBJ
	std::unique_ptr<VertexView> Vview = nullptr;
	std::unique_ptr<IndexView[]> Iview = nullptr;
			
	Microsoft::WRL::ComPtr<ID3D12PipelineState> mPSO = nullptr;//�p�C�v���C��OBJ

	int              MaterialCount = 0;//�}�e���A����
	int              *piFaceBuffer;
	MY_VERTEX_MESH   *pvVertexBuffer;
	int              FaceCount;  //�|���S�����J�E���^�[
	char             mFileName[255];
	//�ꎞ�ۊ�
	VECTOR3 *pvCoord;
	VECTOR3 *pvNormal;
	VECTOR2 *pvTexture;

	struct MY_MATERIAL {
		CHAR MaterialName[110];//�}�e���A���t�@�C�����̃}�e���A����������
		VECTOR4 Kd;           //�f�B�t���[�Y
		CHAR TextureName[110];//�}�e���A���t�@�C�����̃e�N�X�`�����̕����񂪓���
		int tex_no;

		MY_MATERIAL()
		{
			ZeroMemory(this, sizeof(MY_MATERIAL));
			tex_no = -1;
		}
		~MY_MATERIAL(){}
	};
	MY_MATERIAL* pMaterial;

	bool alpha = FALSE;
	bool blend = FALSE;
	bool disp = FALSE;//�e�Z���[�^�t���O
	D3D12_PRIMITIVE_TOPOLOGY_TYPE  primType_create;
	D3D_PRIMITIVE_TOPOLOGY         primType_draw;

	void LoadMaterialFromFile(char *FileName, MY_MATERIAL **ppMaterial);
	void GetShaderByteCode(bool disp);
	void DrawParts();

public:
	MeshData();
	~MeshData();
	void SetCommandList(int no);
	void SetState(bool alpha, bool blend, bool disp);
	void GetBuffer(char *FileName);
	void SetVertex();
	void CreateMesh();
	void GetTexture();
	ID3D12PipelineState *GetPipelineState();
	//��./dat/mesh/tree.obj
	void InstancedMap(float x, float y, float z, float thetaZ, float thetaY, float thetaX, float size);
	void InstanceDraw(float r, float g, float b, float disp);
	void Draw(float x, float y, float z, float r, float g, float b, float thetaZ, float thetaY, float thetaX, float size, float disp);
};

//*********************************PolygonData�N���X*************************************//

class PolygonData {

private:
	//�|�C���^�Ŏ󂯎��
	Dx12Process *dx;
	ID3D12GraphicsCommandList  *mCommandList;   
	int                        com_no = 0;
	ID3DBlob                   *vs = nullptr;
	ID3DBlob                   *ps = nullptr;
	ID3DBlob                   *hs = nullptr;
	ID3DBlob                   *ds = nullptr;

	Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mSrvHeap = nullptr;

	//�R���X�^���g�o�b�t�@OBJ
	UploadBuffer<CONSTANT_BUFFER> *mObjectCB = nullptr;

	//���_�o�b�t�@OBJ
	std::unique_ptr<VertexView> Vview = nullptr;
	std::unique_ptr<IndexView> Iview = nullptr;

	//�e�N�X�`���ێ�(directshow�p)
	ID3D12Resource *texture = NULL;
	ID3D12Resource *textureUp = NULL;
	//movie_on
	bool           m_on = FALSE;
	//�e�N�X�`���ԍ�(�ʏ�e�N�X�`���p)
	int            t_no = -1;
	
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
	D3D12_TEXTURE_COPY_LOCATION dest, src;

	//�p�C�v���C��OBJ
	Microsoft::WRL::ComPtr<ID3D12PipelineState> mPSO = nullptr;

	Vertex         *d3varray;  //���_�z��
	std::uint16_t  *d3varrayI;//���_�C���f�b�N�X
	int            ver;      //���_��
	int            verI;    //���_�C���f�b�N�X

	D3D12_PRIMITIVE_TOPOLOGY_TYPE  primType_create;
	D3D_PRIMITIVE_TOPOLOGY         primType_draw;

	void GetShaderByteCode(bool light, int tNo);
	void DrawParts();

public:
	PolygonData();
	void SetCommandList(int no);
	~PolygonData();
	ID3D12PipelineState *GetPipelineState();
	void GetVBarray(PrimitiveType type, int v);
	void TextureInit(int width, int height);
	void SetTextureMPixel(int **m_pix, BYTE r, BYTE g, BYTE b, int a);
	void Create(bool light, int tNo, bool blend, bool alpha);
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
	void InstancedMap(float x, float y, float z, float theta);
	void InstancedMap(float x, float y, float z, float theta, float size);
	void InstancedMapSize3(float x, float y, float z, float theta, float sizeX, float sizeY, float sizeZ);
	void InstanceDraw(float r, float g, float b, float disp);
	void InstanceDraw(float r, float g, float b, float disp, float px, float py, float mx, float my);
	void Draw(float x, float y, float z, float r, float g, float b, float theta, float disp);
	void Draw(float x, float y, float z, float r, float g, float b, float theta, float disp, float size);
	void Draw(float x, float y, float z, float r, float g, float b, float theta, float disp, float size, float px, float py, float mx, float my);
};

//*********************************PolygonData2D�N���X*************************************//

class PolygonData2D {

private:
	friend DxText;

	Dx12Process                *dx;
	ID3D12GraphicsCommandList  *mCommandList;
	int                        com_no = 0;
	ID3DBlob                   *vs = nullptr;
	ID3DBlob                   *ps = nullptr;

	int                        ver;//���_��

	Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mSrvHeap = nullptr;

	//�R���X�^���g�o�b�t�@OBJ
	UploadBuffer<CONSTANT_BUFFER2D> *mObjectCB = nullptr;

	//���_�o�b�t�@OBJ
	std::unique_ptr<VertexView> Vview = nullptr;
	std::unique_ptr<IndexView> Iview = nullptr;

	Microsoft::WRL::ComPtr<ID3D12PipelineState> mPSO = nullptr;

	//�e�N�X�`���ێ�,DxText class�ł����g��Ȃ�
	ID3D12Resource *texture = NULL;
	ID3D12Resource *textureUp = NULL;
	bool tex_on = FALSE;

	CONSTANT_BUFFER2D cb;
	int               ins_no = 0;

	void GetShaderByteCode();
	void SetConstBf(UploadBuffer<CONSTANT_BUFFER2D> *mObjectCB, float x, float y, float z, float r, float g, float b, float a, float sizeX, float sizeY);
	void SetText(int width, int height, int textCount, TEXTMETRIC **TM, GLYPHMETRICS **GM, BYTE **ptr, DWORD **allsize);//DxText class�ł����g��Ȃ�
	void DrawParts();

public:
	MY_VERTEX2         *d2varray;  //���_�z��
	std::uint16_t      *d2varrayI;//���_�C���f�b�N�X

	static void Pos2DCompute(VECTOR3 *p);//3D���W��2D���W�ϊ�

	PolygonData2D();
	~PolygonData2D();
	void SetCommandList(int no);
	ID3D12PipelineState *GetPipelineState();
	void GetVBarray2D(int pcs);
	void TexOn();
	void CreateBox(float x, float y, float z, float sizex, float sizey, float r, float g, float b, float a, bool blend, bool alpha);
	void Create(bool blend, bool alpha);
	void InstancedSetConstBf(float x, float y, float r, float g, float b, float a, float sizeX, float sizeY);
	void InstancedSetConstBf(float x, float y, float z, float r, float g, float b, float a, float sizeX, float sizeY);
	void InstanceDraw();
	void Draw(float x, float y, float r, float g, float b, float a, float sizeX, float sizeY);
	void Draw(float x, float y, float z, float r, float g, float b, float a, float sizeX, float sizeY);
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

//*********************************ParticleData�N���X*************************************//

class ParticleData {

private:
	Dx12Process                *dx;
	ID3D12GraphicsCommandList  *mCommandList;
	int                        com_no = 0;
	ID3DBlob                   *gsSO;
	ID3DBlob                   *vsSO;
	ID3DBlob                   *gs;
	ID3DBlob                   *vs;
	ID3DBlob                   *ps;
	int                        ver;//���_��

	PartPos                    *P_pos;//�p�[�e�B�N���f�[�^�z��

	Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature_com = nullptr;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature_draw = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mSrvHeap = nullptr;

	//�R���X�^���g�o�b�t�@OBJ
	UploadBuffer<CONSTANT_BUFFER_P> *mObjectCB = nullptr;

	D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
	D3D12_TEXTURE_COPY_LOCATION dest, src;

	//�e�N�X�`���ێ�(directshow�p)
	ID3D12Resource *texture = NULL;
	ID3D12Resource *textureUp = NULL;
	//movie_on
	bool           m_on = FALSE;
	bool           texpar_on = FALSE;

	//���_�o�b�t�@OBJ
	std::unique_ptr<VertexView> Vview = nullptr;
	//�X�g���[���o�b�t�@OBJ
	std::unique_ptr<StreamView[]> Sview1 = nullptr;
	std::unique_ptr<StreamView[]> Sview2 = nullptr;//BufferFilledSizeLocation�̑����
	int svInd = 0;
	bool firstDraw = FALSE;
	int  streamInitcount = 0;

	Microsoft::WRL::ComPtr<ID3D12PipelineState> mPSO_com = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> mPSO_draw = nullptr;

	void GetShaderByteCode();
	void MatrixMap(UploadBuffer<CONSTANT_BUFFER_P> *mObjectCB, float x, float y, float z, float theta, float size, bool init, float speed, bool tex);
	void GetVbColarray(int texture_no, float size, float density);
	void CreateVbObj();
	void CreatePartsCom();
	void CreatePartsDraw(int texpar);
	void DrawParts0(float x, float y, float z, float theta, float size, bool init, float speed, bool tex);
	void DrawParts1();
	void DrawParts2();

public:
	ParticleData();
	~ParticleData();
	void SetCommandList(int no);
	void TextureInit(int width, int height);
	void GetBufferParticle(int texture_no, float size, float density);//�e�N�X�`�������Ƀp�[�e�B�N���f�[�^����, �S�̃T�C�Y�{��, ���x
	void GetBufferBill(int v);
	void SetVertex(int i,
		float vx, float vy, float vz,
		float r, float g, float b, float a);
	void CreateParticle(int texpar);//�p�[�e�B�N��1�̃e�N�X�`���i���o�[
	void CreateBillboard();//ver�̎l�p�`�𐶐�
	void Draw(float x, float y, float z, float theta, float size, bool init, float speed);//size�p�[�e�B�N��1�̃T�C�Y
	void SetTextureMPixel(int **m_pix, BYTE r, BYTE g, BYTE b, int a);
	void DrawBillboard(float size);
};

//*********************************SkinMesh�N���X*************************************//

class SkinMesh_sub {

private:
	friend SkinMesh;
	
	FbxScene *m_pmyScene = NULL;
	float end_frame, current_frame;

	bool centering, offset;
	float cx, cy, cz;
	MATRIX rotZYX;
	float connect_step;

	SkinMesh_sub();
	~SkinMesh_sub();
	bool Create(CHAR *szFileName);
};

class SkinMesh {

private:
	friend SkinMesh_sub;
	friend Dx12Process;
	static FbxManager *m_pSdkManager;
	static FbxImporter *m_pImporter;
	//InitFBX�r�������p
	static volatile bool stInitFBX_ON, stSetNewPose_ON;//����ǂݍ��݂Ɍ�ŉ������鎞�����ς���

	Dx12Process                *dx;
	ID3D12GraphicsCommandList  *mCommandList;
	int                        com_no = 0;
	ID3DBlob                   *vs = nullptr;
	ID3DBlob                   *ps = nullptr;
	bool alpha = FALSE;
	bool blend = FALSE;

	Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mSrvHeap = nullptr;

	//�R���X�^���g�o�b�t�@OBJ
	UploadBuffer<CONSTANT_BUFFER> *mObjectCB0 = nullptr;
	UploadBuffer<SHADER_GLOBAL1> *mObjectCB1 = nullptr;
	UploadBuffer<SHADER_GLOBAL_BONES> *mObject_BONES = nullptr;

	std::unique_ptr<VertexView> Vview = nullptr;
	std::unique_ptr<IndexView[]> Iview = nullptr;

	Microsoft::WRL::ComPtr<ID3D12PipelineState> mPSO = nullptr;//�p�C�v���C��OBJ

	//���b�V���֘A	
	DWORD *m_pdwNumVert;//���b�V�����̒��_��
	DWORD VerAllpcs;    //�S���_��
	MY_VERTEX_S *pvVB;//�g�p��ێ����邩�j�����邩�t���O�Ō��߂�,�ʏ�͔j��
	bool pvVB_delete_f;

	int MateAllpcs;  //�S�}�e���A����
	MY_MATERIAL_S *m_pMaterial;

	//�ꎞ�i�[�p
	DWORD *m_pMaterialCount;//���b�V�����̃}�e���A���J�E���g
	DWORD *pdwNumFace; //���b�V�����̃|���S����
	int *IndexCount34Me;  //4���_�|���S�������O�̃��b�V�����̃C���f�b�N�X��
	int *IndexCount3M;  //4���_�|���S��������̃}�e���A�����̃C���f�b�N�X��
	int **piFaceBuffer;//���b�V�����̒��_�C���f�b�N�X�z��
	int **pIndex;      //���b�V������4���_������C���f�b�N�X�z��

	//�{�[��
	int m_iNumBone;
	BONE *m_BoneArray;
	
	//FBX
	SkinMesh_sub *fbx;
	FbxCluster **m_ppCluster;//�{�[�����
	char *m_pClusterName;
	FbxNode **m_ppNodeArray;//�eNode�ւ̃|�C���^�z��
	int NodeArraypcs;
	FbxNode **m_ppSubAnimationBone;//���̑��A�j���[�V�����{�[���|�C���^�z��
	MATRIX *m_pLastBoneMatrix;
	int AnimLastInd;
	float BoneConnect;


	//���ʂŎg���}�l�[�W���[����(Dx12Process�Ő���������s��)
	static void CreateManager();
	static void DeleteManager();

	void DestroyFBX();
	FbxScene* GetScene(int p);
	int SearchNodeCount(FbxNode *pnode, FbxNodeAttribute::EType SearchType);
	FbxNode *SearchNode(FbxNode *pnode, FbxNodeAttribute::EType SearchType, int Ind);
	HRESULT InitFBX(CHAR* szFileName, int p);
	void CreateIndexBuffer(int cnt, int *pIndex, int IviewInd);
	HRESULT ReadSkinInfo(MY_VERTEX_S *pvVB);
	MATRIX GetCurrentPoseMatrix(int index);
	void MatrixMap_Bone(UploadBuffer<SHADER_GLOBAL_BONES> *CB);
	void GetTexture();
	int GetTexNumber(CHAR *fileName);
	bool SetNewPoseMatrices(float time, int ind);
	void CreateRotMatrix(float thetaZ, float thetaY, float thetaX, int ind);

public:
	SkinMesh();
	~SkinMesh();

	void SetCommandList(int no);
	void SetState(bool alpha, bool blend);
	void ObjCentering(bool f, int ind);
	void ObjCentering(float x, float y, float z, float thetaZ, float thetaY, float thetaX, int ind);
	void ObjOffset(float x, float y, float z, float thetaZ, float thetaY, float thetaX, int ind);
	void SetConnectStep(int ind, float step);
	void Vertex_hold();
	HRESULT GetBuffer(CHAR* szFileName, float end_frame);
	void SetVertex();
	void CreateFromFBX();
	HRESULT GetBuffer_Sub(CHAR* szFileName, int ind, float end_frame);
	void CreateFromFBX_SubAnimation(int ind);
	bool Draw(float time, float x, float y, float z, float r, float g, float b, float thetaZ, float thetaY, float thetaX, float size);
	bool Draw(int ind, float time, float x, float y, float z, float r, float g, float b, float thetaZ, float thetaY, float thetaX, float size);
	VECTOR3 GetVertexPosition(int verNum, float adjustZ, float adjustY, float adjustX, float thetaZ, float thetaY, float thetaX, float scale);
};

//�G���[���b�Z�[�W
void ErrorMessage(char *E_mes);

#endif