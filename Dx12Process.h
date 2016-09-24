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
#include <stdlib.h>
#include <string>
#include <vector>
#include <array>
#include <unordered_map>
#include <assert.h>
#include <Process.h>

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

#define COM_NO        7
#define RELEASE(p)    if(p){p->Release();  p=NULL;}
#define S_DELETE(p)   if(p){delete p;      p=NULL;}
#define ARR_DELETE(p) if(p){delete[] p;    p=NULL;}
#define TEX_PCS       130

//前方宣言
template<typename T>
class UploadBuffer;
class MeshData;
class PolygonData;
class PolygonData2D;
class ParticleData;
class DxText;
//前方宣言

class Dx12Process {

private:
	friend MeshData;
	friend PolygonData;
	friend PolygonData2D;
	friend ParticleData;

	Microsoft::WRL::ComPtr<IDXGIFactory4> mdxgiFactory;
	Microsoft::WRL::ComPtr<ID3D12Device> md3dDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
	Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
	UINT64 mCurrentFence = 0;

	//MultiSampleレベルチェック
	bool m4xMsaaState = false;
	UINT m4xMsaaQuality = 0;

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc[COM_NO];
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList[COM_NO];

	static const int SwapChainBufferCount = 2;
	int mCurrBackBuffer = 0;
	Microsoft::WRL::ComPtr<ID3D12Resource> mSwapChainBuffer[SwapChainBufferCount];
	Microsoft::WRL::ComPtr<ID3D12Resource> mDepthStencilBuffer;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mRtvHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDsvHeap;

	//シェーダーバイトコード
	Microsoft::WRL::ComPtr<ID3DBlob> pGeometryShader_PSO = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pGeometryShader_P = nullptr;

	Microsoft::WRL::ComPtr<ID3DBlob> pHullShader_MESH_D = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pHullShader_DISPL = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pHullShader_DISP = nullptr;

	Microsoft::WRL::ComPtr<ID3DBlob> pDomainShader_MESH_D = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pDomainShader_DISPL = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pDomainShader_DISP = nullptr;

	std::vector<D3D12_SO_DECLARATION_ENTRY> pDeclaration_PSO;
	std::vector<D3D12_INPUT_ELEMENT_DESC> pVertexLayout_P;
	std::vector<D3D12_INPUT_ELEMENT_DESC> pVertexLayout_MESH;
	std::vector<D3D12_INPUT_ELEMENT_DESC> pVertexLayout_3D;
	std::vector<D3D12_INPUT_ELEMENT_DESC> pVertexLayout_2D;

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
	//テクスチャ保持
	ID3D12Resource *texture[TEX_PCS];
	ID3D12Resource *textureUp[TEX_PCS];

	static Dx12Process *dx;//クラス内でオブジェクト生成し使いまわす

	MATRIX      mProj;
	MATRIX      mView;
	MATRIX      Vp;    //ビューポート行列(3D座標→2D座標変換時使用)
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

	CONSTANT_BUFFER cb;
	int             ins_no = 0;

	Dx12Process() {}//外部からのオブジェクト生成禁止
	Dx12Process(const Dx12Process &obj) {}     // コピーコンストラクタ禁止
	void operator=(const Dx12Process& obj) {};// 代入演算子禁止
	~Dx12Process();
	
	void CreateShaderByteCode();
	void TextureBinaryDecode(char *Bpass, int i);//暗号化済み画像バイナリデコード

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

public:
	static void InstanceCreate();
	static Dx12Process *GetInstance();
	static void DeleteInstance();
	bool Initialize(HWND hWnd);
	void TextureBinaryDecodeAll();
	void GetTexture();
	void Sclear();
	void Bigin(int com_no, ID3D12PipelineState *pso);
	void End(int com_no);
	void FlushCommandQueue();
	void DrawScreen();
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

struct VertexView {

	//各パラメーターを自分でコピーする
	Microsoft::WRL::ComPtr<ID3DBlob> VertexBufferCPU = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> VertexBufferGPU = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> VertexBufferUploader = nullptr;

	//バッファのサイズ等
	UINT VertexByteStride = 0;
	UINT VertexBufferByteSize = 0;

	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW VertexBufferView()const
	{
		D3D12_VERTEX_BUFFER_VIEW vbv;
		vbv.BufferLocation = VertexBufferGPU->GetGPUVirtualAddress();
		vbv.StrideInBytes = VertexByteStride;
		vbv.SizeInBytes = VertexBufferByteSize;

		return vbv;
	}

	//GPUにアップロード後自分で解放する
	void DisposeUploaders()
	{
		VertexBufferUploader = nullptr;
	}
};

struct IndexView {

	//各パラメーターを自分でコピーする
	Microsoft::WRL::ComPtr<ID3DBlob> IndexBufferCPU = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> IndexBufferGPU = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> IndexBufferUploader = nullptr;

	//バッファのサイズ等
	DXGI_FORMAT IndexFormat = DXGI_FORMAT_R16_UINT;
	UINT IndexBufferByteSize = 0;
	//DrawIndexedInstancedの引数で使用
	UINT IndexCount = 0;

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
		IndexBufferUploader = nullptr;
	}
};

struct StreamView {

	Microsoft::WRL::ComPtr<ID3D12Resource> StreamBufferGPU = nullptr;

	//バッファのサイズ等
	UINT StreamByteStride = 0;
	UINT StreamBufferByteSize = 0;
	D3D12_GPU_VIRTUAL_ADDRESS BufferFilledSizeLocation;

	//ストリームバッファビュー
	D3D12_STREAM_OUTPUT_BUFFER_VIEW StreamBufferView()const
	{
		D3D12_STREAM_OUTPUT_BUFFER_VIEW sbv;
		sbv.BufferLocation = StreamBufferGPU->GetGPUVirtualAddress();
		sbv.SizeInBytes = StreamBufferByteSize;
		sbv.BufferFilledSizeLocation = BufferFilledSizeLocation;//分からないとりあえずこれで動いているVertexにおくるとhungする
		return sbv;
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

//*********************************MeshDataクラス*************************************//

class MeshData {

private:
	Dx12Process                *dx;
	ID3D12CommandAllocator     *mDirectCmdListAlloc;
	ID3D12GraphicsCommandList  *mCommandList;
	int                        com_no = 0;
	ID3DBlob                   *vs = nullptr;
	ID3DBlob                   *ps = nullptr;
	ID3DBlob                   *hs = nullptr;
	ID3DBlob                   *ds = nullptr;

	Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mSrvHeap = nullptr;

	//コンスタントバッファOBJ
	UploadBuffer<CONSTANT_BUFFER> *mObjectCB = nullptr;
	UploadBuffer<CONSTANT_BUFFER_MESH> *mObject_MESHCB = nullptr;//マテリアル渡し用

	//頂点バッファOBJ
	std::unique_ptr<VertexView> Vview = nullptr;
	std::unique_ptr<IndexView[]> Iview = nullptr;
			
	Microsoft::WRL::ComPtr<ID3D12PipelineState> mPSO = nullptr;//パイプラインOBJ

	int              MaterialCount = 0;//マテリアル数
	int              *piFaceBuffer;
	MY_VERTEX_MESH   *pvVertexBuffer;
	int              FaceCount;  //ポリゴン数カウンター

	struct MY_MATERIAL {
		CHAR MaterialName[110];//マテリアルファイル内のマテリアル名が入る
		VECTOR4 Kd;           //ディフューズ
		CHAR TextureName[110];//マテリアルファイル内のテクスチャ�ｂﾌ文字列が入る
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
	bool disp = FALSE;//テセレータフラグ
	D3D12_PRIMITIVE_TOPOLOGY_TYPE  primType_create;
	D3D_PRIMITIVE_TOPOLOGY         primType_draw;

	void LoadMaterialFromFile(LPSTR FileName, MY_MATERIAL** ppMaterial);
	void GetShaderByteCode(bool disp);
	void DrawParts();

public:
	static HANDLE *MeshObj_H;
	static MeshData *MeshObj;
	static char **MeshPass;

	static void GetVBarrayThreadArray(MeshData *meshObj, char **Mpass, int pcs);

	MeshData();
	~MeshData();
	void SetCommandList(int no);
	void SetState(bool alpha, bool blend, bool disp);
	void GetVBarray(LPSTR FileName);
	void GetTexture();
	ID3D12PipelineState *GetPipelineState();
	//木./dat/mesh/tree.obj
	void InstancedMap(float x, float y, float z, float thetaZ, float thetaY, float thetaX, float size);
	void InstanceDraw(float r, float g, float b, float disp);
	void Draw(float x, float y, float z, float r, float g, float b, float thetaZ, float thetaY, float thetaX, float size, float disp);
};
//GetVBarrayThreadArray内で使用
unsigned __stdcall GetVB0(void *);
unsigned __stdcall GetVB1(void *);
unsigned __stdcall GetVB2(void *);
unsigned __stdcall GetVB3(void *);
unsigned __stdcall GetVB4(void *);
unsigned __stdcall GetVB5(void *);
unsigned __stdcall GetVB6(void *);
unsigned __stdcall GetVB7(void *);
unsigned __stdcall GetVB8(void *);
unsigned __stdcall GetVB9(void *);
unsigned __stdcall GetVB10(void *);
unsigned __stdcall GetVB11(void *);
unsigned __stdcall GetVB12(void *);
unsigned __stdcall GetVB13(void *);
unsigned __stdcall GetVB14(void *);
unsigned __stdcall GetVB15(void *);
unsigned __stdcall GetVB16(void *);
unsigned __stdcall GetVB17(void *);
unsigned __stdcall GetVB18(void *);
unsigned __stdcall GetVB19(void *);
unsigned __stdcall GetVB20(void *);
unsigned __stdcall GetVB21(void *);
unsigned __stdcall GetVB22(void *);
unsigned __stdcall GetVB23(void *);
unsigned __stdcall GetVB24(void *);
unsigned __stdcall GetVB25(void *);
unsigned __stdcall GetVB26(void *);
unsigned __stdcall GetVB27(void *);
unsigned __stdcall GetVB28(void *);
unsigned __stdcall GetVB29(void *);
unsigned __stdcall GetVB30(void *);
unsigned __stdcall GetVB31(void *);
unsigned __stdcall GetVB32(void *);
unsigned __stdcall GetVB33(void *);
unsigned __stdcall GetVB34(void *);
unsigned __stdcall GetVB35(void *);

//*********************************PolygonDataクラス*************************************//

class PolygonData {

private:
	//ポインタで受け取る
	Dx12Process *dx;
	ID3D12CommandAllocator     *mDirectCmdListAlloc;
	ID3D12GraphicsCommandList  *mCommandList;   
	int                        com_no = 0;
	ID3DBlob                   *vs = nullptr;
	ID3DBlob                   *ps = nullptr;
	ID3DBlob                   *hs = nullptr;
	ID3DBlob                   *ds = nullptr;

	Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mSrvHeap = nullptr;

	//コンスタントバッファOBJ
	UploadBuffer<CONSTANT_BUFFER> *mObjectCB = nullptr;

	//頂点バッファOBJ
	std::unique_ptr<VertexView> Vview = nullptr;
	std::unique_ptr<IndexView> Iview = nullptr;

	//テクスチャ保持(directshow用)
	ID3D12Resource *texture = NULL;
	ID3D12Resource *textureUp = NULL;
	//movie_on
	bool           m_on = FALSE;
	//テクスチャ番号(通常テクスチャ用)
	int            t_no = -1;
	
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
	D3D12_TEXTURE_COPY_LOCATION dest, src;

	//パイプラインOBJ
	Microsoft::WRL::ComPtr<ID3D12PipelineState> mPSO = nullptr;

	Vertex         *d3varray;  //頂点配列
	std::uint16_t  *d3varrayI;//頂点インデックス
	int            ver;      //頂点個数
	int            verI;    //頂点インデックス

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

//*********************************PolygonData2Dクラス*************************************//

class PolygonData2D {

private:
	friend DxText;

	Dx12Process                *dx;
	ID3D12CommandAllocator     *mDirectCmdListAlloc;
	ID3D12GraphicsCommandList  *mCommandList;
	int                        com_no = 0;
	ID3DBlob                   *vs = nullptr;
	ID3DBlob                   *ps = nullptr;

	int                        ver;//頂点数

	Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mSrvHeap = nullptr;

	//コンスタントバッファOBJ
	UploadBuffer<CONSTANT_BUFFER2D> *mObjectCB = nullptr;

	//頂点バッファOBJ
	std::unique_ptr<VertexView> Vview = nullptr;
	std::unique_ptr<IndexView> Iview = nullptr;

	Microsoft::WRL::ComPtr<ID3D12PipelineState> mPSO = nullptr;

	//テクスチャ保持,DxText classでしか使わない
	ID3D12Resource *texture = NULL;
	ID3D12Resource *textureUp = NULL;
	bool tex_on = FALSE;

	CONSTANT_BUFFER2D cb;
	int               ins_no = 0;

	void GetShaderByteCode();
	void SetConstBf(UploadBuffer<CONSTANT_BUFFER2D> *mObjectCB, float x, float y, float z, float r, float g, float b, float a, float sizeX, float sizeY);
	void SetText(int width, int height, int textCount, TEXTMETRIC **TM, GLYPHMETRICS **GM, BYTE **ptr, DWORD **allsize);//DxText classでしか使わない
	void DrawParts();

public:
	MY_VERTEX2         *d2varray;  //頂点配列
	std::uint16_t      *d2varrayI;//頂点インデックス

	static void Pos2DCompute(VECTOR3 *p);//3D座標→2D座標変換

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
	static DWORD time_fps;//FPS計測用
	static int frame;    //FPS計測使用
	static char str[50];//ウインドウ枠文字表示使用

public:
	static void GetTime(HWND hWnd);//常に実行
	float Add(float f);
};

//*********************************ParticleDataクラス*************************************//

class ParticleData {

private:
	Dx12Process                *dx;
	ID3D12CommandAllocator     *mDirectCmdListAlloc;
	ID3D12GraphicsCommandList  *mCommandList;
	int                        com_no = 0;
	ID3DBlob                   *gsSO;
	ID3DBlob                   *vsSO;
	ID3DBlob                   *gs;
	ID3DBlob                   *vs;
	ID3DBlob                   *ps;
	int                        ver;//頂点数

	PartPos                    *P_pos;//パーティクルデータ配列

	Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature_com = nullptr;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature_draw = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mSrvHeap = nullptr;

	//コンスタントバッファOBJ
	UploadBuffer<CONSTANT_BUFFER_P> *mObjectCB = nullptr;

	D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
	D3D12_TEXTURE_COPY_LOCATION dest, src;

	//テクスチャ保持(directshow用)
	ID3D12Resource *texture = NULL;
	ID3D12Resource *textureUp = NULL;
	//movie_on
	bool           m_on = FALSE;
	bool           texpar_on = FALSE;

	//頂点バッファOBJ
	std::unique_ptr<VertexView> Vview = nullptr;
	//ストリームバッファOBJ
	std::unique_ptr<StreamView> Sview1 = nullptr;
	std::unique_ptr<StreamView> Sview2 = nullptr;//BufferFilledSizeLocationの送り先

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
	void GetVBarray(int v);
	void SetVertex(int i,
		float vx, float vy, float vz,
		float r, float g, float b, float a);
	void CreateParticle(int texture_no, int texpar, float size, float density);//テクスチャを元にパーティクルデータ生成, 全体サイズ倍率, 密度
	void CreateBillboard();//ver個の四角形を生成
	void Draw(float x, float y, float z, float theta, float size, bool init, float speed);//sizeパーティクル1個のサイズ
	void SetTextureMPixel(int **m_pix, BYTE r, BYTE g, BYTE b, int a);
	void DrawBillboard(float size);
};

//エラーメッセージ
void ErrorMessage(char *E_mes);

#endif
