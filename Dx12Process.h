//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　          Dx12Processクラス                                 **//
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

//前方宣言
template<typename T>
class UploadBuffer;
class PolygonData;
//前方宣言

class Dx12Process {

private:
	friend PolygonData;

	Microsoft::WRL::ComPtr<IDXGIFactory4> mdxgiFactory;
	Microsoft::WRL::ComPtr<ID3D12Device> md3dDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
	Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
	UINT64 mCurrentFence = 0;

	//MultiSampleレベルチェック
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

	//サンプラ
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

	//テクスチャ
	char  **binary_ch;                   //デコード後バイナリ
	int   *binary_size;                 //バイナリサイズ

	static Dx12Process *dx;//クラス内でオブジェクト生成し使いまわす

	MATRIX mProj;
	MATRIX mView;
	float posX, posY, posZ;

	//カメラ画角
	float ViewY_theta;
	//アスペクト比
	float aspect;
	//nearプレーン
	float NearPlane;
	//farプレーン
	float FarPlane;

	PointLight plight;
	DirectionLight dlight;
	Fog fog;

	Dx12Process() {}//外部からのオブジェクト生成禁止
	Dx12Process(const Dx12Process &obj) {}     // コピーコンストラクタ禁止
	void operator=(const Dx12Process& obj) {};// 代入演算子禁止
	~Dx12Process();
	
	void FlushCommandQueue();
	void TextureBinaryDecode(char *Bpass, int i);//暗号化済み画像バイナリデコード
	void GetTexture(ID3D12GraphicsCommandList *mCommandList, ID3D12Resource **texture, int TexNo); //TextureBinaryDecodeAll();の後にやる事

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
		float brightness, float attenuation, bool on_off);//0:視点, 1:ラスボス, 2:出入り口, 3456:戦闘
	void DirectionLight(float x, float y, float z, float r, float g, float b, float bright, float ShadowBright);
	void SetDirectionLight(bool onoff);
	void Fog(float r, float g, float b, float amount, float density, bool onoff);
	float GetViewY_theta();
	float Getaspect();
	float GetNearPlane();
	float GetFarPlane();
};

struct MeshGeometry {

	//各パラメーターを自分でコピーする
	Microsoft::WRL::ComPtr<ID3DBlob> VertexBufferCPU = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> IndexBufferCPU = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> VertexBufferGPU = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> IndexBufferGPU = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> VertexBufferUploader = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> IndexBufferUploader = nullptr;

	//バッファのサイズ等
	UINT VertexByteStride = 0;
	UINT VertexBufferByteSize = 0;
	DXGI_FORMAT IndexFormat = DXGI_FORMAT_R16_UINT;
	UINT IndexBufferByteSize = 0;
	//DrawIndexedInstancedの引数で使用
	UINT IndexCount = 0;

	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW VertexBufferView()const
	{
		D3D12_VERTEX_BUFFER_VIEW vbv;
		vbv.BufferLocation = VertexBufferGPU->GetGPUVirtualAddress();
		vbv.StrideInBytes = VertexByteStride;
		vbv.SizeInBytes = VertexBufferByteSize;

		return vbv;
	}

	//インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW IndexBufferView()const
	{
		D3D12_INDEX_BUFFER_VIEW ibv;
		ibv.BufferLocation = IndexBufferGPU->GetGPUVirtualAddress();
		ibv.Format = IndexFormat;
		ibv.SizeInBytes = IndexBufferByteSize;

		return ibv;
	}

	//GPUにアップロード後自分で解放する
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

		//コンスタントバッファの要素は256バイトにしておく(ハードウエアの都合上)
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
		//デストラクタでUnmap
		//GPU使用中書き込まない事
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

//*********************************PolygonDataクラス*************************************//

class PolygonData {

private:
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;

	Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mSrvHeap = nullptr;

	//コンスタントバッファOBJ
	UploadBuffer<CONSTANT_BUFFER> *mObjectCB = nullptr;

	//頂点バッファOBJ
	std::unique_ptr<MeshGeometry> mBoxGeo = nullptr;

	//シェーダーバイトコード
	Microsoft::WRL::ComPtr<ID3DBlob> mvsByteCode = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> mpsByteCode = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> mhsByteCode = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> mdsByteCode = nullptr;


	//テクスチャ保持
	ID3D12Resource *texture;

	//インプットレイアウト
	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

	//パイプラインOBJ
	Microsoft::WRL::ComPtr<ID3D12PipelineState> mPSO = nullptr;

	//ポインタを受け取る
	Dx12Process *dx;

	Vertex         *d3varray;  //頂点配列
	std::uint16_t  *d3varrayI;//頂点インデックス
	int            ver;      //頂点個数
	int            verI;    //頂点インデックス

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
	static DWORD time_fps;//FPS計測用
	static int frame;    //FPS計測使用
	static char str[50];//ウインドウ枠文字表示使用

public:
	static void GetTime(HWND hWnd);//常に実行
	float Add(float f);
};

#endif
