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
#include "DxEnum.h"
#include <dxgi1_4.h>
#include <d3d12.h>
#include "./MicroSoftLibrary/d3dx12.h"
#include <d3d10_1.h>
#include <D3Dcompiler.h>
#include <DirectXColors.h>
//#include <DirectXCollision.h>
#include "./MicroSoftLibrary/WICTextureLoader12.h"
//#include <memory>
#include <stdlib.h>
#include <string>
#include <vector>
#include <array>
//#include <unordered_map>
#include <assert.h>
#include <Process.h>
#include <fbxsdk.h>
#include <mutex>
#include <new>

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

#define COM_NO        7

//前方宣言
template<typename T>
class UploadBuffer;
class Dx12Process;
class MeshData;
class PolygonData;
class PolygonData2D;
class ParticleData;
class SkinMesh;
class DxText;
class Wave;
//前方宣言

class Dx12Process_sub {

private:
	friend Dx12Process;
	friend MeshData;
	friend PolygonData;
	friend PolygonData2D;
	friend ParticleData;
	friend SkinMesh;
	friend Wave;

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
	friend Wave;

	Microsoft::WRL::ComPtr<IDXGIFactory4> mdxgiFactory;
	Microsoft::WRL::ComPtr<ID3D12Device> md3dDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
	Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
	UINT64 mCurrentFence = 0;

	//MultiSampleレベルチェック
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

	//シェーダーバイトコード
	Microsoft::WRL::ComPtr<ID3DBlob> pGeometryShader_PSO = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pGeometryShader_P = nullptr;

	Microsoft::WRL::ComPtr<ID3DBlob> pHullShader_Wave = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pHullShader_MESH_D = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pHullShader_DISPL = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pHullShader_DISP = nullptr;

	Microsoft::WRL::ComPtr<ID3DBlob> pDomainShader_Wave = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pDomainShader_MESH_D = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pDomainShader_DISPL = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pDomainShader_DISP = nullptr;

	std::vector<D3D12_INPUT_ELEMENT_DESC> pVertexLayout_SKIN;
	std::vector<D3D12_SO_DECLARATION_ENTRY> pDeclaration_PSO;
	std::vector<D3D12_INPUT_ELEMENT_DESC> pVertexLayout_P;
	std::vector<D3D12_INPUT_ELEMENT_DESC> pVertexLayout_MESH;
	std::vector<D3D12_INPUT_ELEMENT_DESC> pVertexLayout_3D;
	std::vector<D3D12_INPUT_ELEMENT_DESC> pVertexLayout_3DBC;
	std::vector<D3D12_INPUT_ELEMENT_DESC> pVertexLayout_2D;

	Microsoft::WRL::ComPtr<ID3DBlob> pVertexShader_Wave = nullptr;
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

	Microsoft::WRL::ComPtr<ID3DBlob> pPixelShader_Wave = nullptr;
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

	Microsoft::WRL::ComPtr<ID3DBlob> pComputeShader_Wave = nullptr;

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
	Texture *tex = NULL;//外部からアドレスが渡される
	int texNum = 0;    //配列数
	ID3D12Resource **texture = nullptr;
	ID3D12Resource **textureUp = nullptr;

	static Dx12Process *dx;//クラス内でオブジェクト生成し使いまわす
	static std::mutex mtx;

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

	int  ins_no = 0;

	Dx12Process() {}//外部からのオブジェクト生成禁止
	Dx12Process(const Dx12Process &obj) {}     // コピーコンストラクタ禁止
	void operator=(const Dx12Process& obj) {};// 代入演算子禁止
	~Dx12Process();

	void CreateShaderByteCode();

	Microsoft::WRL::ComPtr<ID3D12Resource> CreateDefaultBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList,
		const void* initData, UINT64 byteSize, Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer);

	Microsoft::WRL::ComPtr<ID3D12Resource> CreateStreamBuffer(ID3D12Device* device, UINT64 byteSize);

	Microsoft::WRL::ComPtr<ID3DBlob> CompileShader(LPSTR szFileName, size_t size, LPSTR szFuncName, LPSTR szProfileName);

	void InstancedMap(CONSTANT_BUFFER *cb, float x, float y, float z, float thetaZ, float thetaY, float thetaX, float size);

	void InstancedMapSize3(CONSTANT_BUFFER *cb, float x, float y, float z, float thetaZ, float thetaY, float thetaX, float sizeX, float sizeY, float sizeZ);

	void MatrixMap2(CONSTANT_BUFFER *cb, float r, float g, float b, float a, float disp, float px, float py, float mx, float my);

	void MatrixMap(CONSTANT_BUFFER *cb, float x, float y, float z,
		float r, float g, float b, float a, float thetaZ, float thetaY, float thetaX, float size, float disp, float px, float py, float mx, float my);

	void MatrixMapSize3(CONSTANT_BUFFER *cb, float x, float y, float z,
		float r, float g, float b, float a, float thetaZ, float thetaY, float thetaX, float sizeX, float sizeY, float sizeZ, float disp, float px, float py, float mx, float my);
	void WaitFence(int fence);

public:
	static void InstanceCreate();
	static Dx12Process *GetInstance();
	static void DeleteInstance();

	static void Lock() { mtx.lock(); }
	static void Unlock() { mtx.unlock(); }

	bool Initialize(HWND hWnd);
	char *GetNameFromPass(char *pass);//パスからファイル名を抽出
	void SetTextureBinary(Texture *tex, int size);//外部で生成したデコード済みバイナリ配列のポインタと配列数をセットする,解放は外部で
	int GetTexNumber(CHAR *fileName);//リソースとして登録済みのテクスチャ配列番号をファイル名から取得
	void GetTexture(int com_no);//デコード済みのバイナリからリソースの生成
	void UpTextureRelease();
	void Sclear(int com_no);
	void Bigin(int com_no);
	void End(int com_no);
	void WaitFenceCurrent();//GPU処理そのまま待つ
	void WaitFencePast();//前回GPU処理未完の場合待つ
	void RequestSync();//同期要求
	void ReplySync();//同期返答
	void SyncFin(bool on);//TRUE:同期終了
	bool SyncFin();
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
		sbv.BufferFilledSizeLocation = BufferFilledSizeLocation;//以前のサイズ?
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

		//コンスタントバッファサイズは256バイト単位にしておく(アライメント)
		// at m*256 byte offsets and of n*256 byte lengths. 
		// typedef struct D3D12_CONSTANT_BUFFER_VIEW_DESC {
		// UINT64 OffsetInBytes; // multiple of 256
		// UINT   SizeInBytes;   // multiple of 256
		// } D3D12_CONSTANT_BUFFER_VIEW_DESC;
		if (isConstantBuffer)//コンスタントバッファの場合
			mElementByteSize = (sizeof(T) + 255) & ~255;//255を足して255の補数の論理積を取る。(255単位に変換)

		if (FAILED(device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(mElementByteSize * elementCount),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mUploadBuffer)))) {
			char *str = "UploadBufferエラー";
			throw str;
		}

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

//*********************************MeshDataクラス*************************************//

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

	//コンスタントバッファOBJ
	UploadBuffer<CONSTANT_BUFFER> *mObjectCB = nullptr;
	UploadBuffer<CONSTANT_BUFFER_MESH> *mObject_MESHCB = nullptr;//マテリアル渡し用(1回しか更新しない)
	//UpLoad用
	CONSTANT_BUFFER cb[2];
	int sw = 0;//↑切り替え
	//UpLoadカウント
	int upCount = 0;
	//初回Up終了
	bool UpOn = FALSE;
	//DrawOn
	bool DrawOn = FALSE;

	//頂点バッファOBJ
	std::unique_ptr<VertexView> Vview = nullptr;
	std::unique_ptr<IndexView[]> Iview = nullptr;

	Microsoft::WRL::ComPtr<ID3D12PipelineState> mPSO = nullptr;//パイプラインOBJ

	int              MaterialCount = 0;//マテリアル数
	int              *piFaceBuffer;
	MY_VERTEX_MESH   *pvVertexBuffer;
	int              FaceCount;  //ポリゴン数カウンター
	char             mFileName[255];
	//一時保管
	VECTOR3 *pvCoord;
	VECTOR3 *pvNormal;
	VECTOR2 *pvTexture;
	int insNum = 0;

	struct MY_MATERIAL {
		CHAR MaterialName[110];//マテリアルファイル内のマテリアル名が入る
		VECTOR4 Kd;           //ディフューズ
		CHAR TextureName[110];//マテリアルファイル内のテクスチャ№の文字列が入る
		int tex_no;

		MY_MATERIAL()
		{
			ZeroMemory(this, sizeof(MY_MATERIAL));
			tex_no = -1;
		}
		~MY_MATERIAL() {}
	};
	MY_MATERIAL* pMaterial;

	bool alpha = FALSE;
	bool blend = FALSE;
	bool disp = FALSE;//テセレータフラグ
	D3D12_PRIMITIVE_TOPOLOGY_TYPE  primType_create;
	D3D_PRIMITIVE_TOPOLOGY         primType_draw;

	static std::mutex mtx;
	static void Lock() { mtx.lock(); }
	static void Unlock() { mtx.unlock(); }

	void LoadMaterialFromFile(char *FileName, MY_MATERIAL **ppMaterial);
	void GetShaderByteCode(bool disp);
	void CbSwap();

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
	//木./dat/mesh/tree.obj
	//複数Update
	void InstancedMap(float x, float y, float z, float thetaZ, float thetaY, float thetaX, float size);
	void InstanceUpdate(float r, float g, float b, float a, float disp);
	//単体Update
	void Update(float x, float y, float z, float r, float g, float b, float a, float thetaZ, float thetaY, float thetaX, float size, float disp);
	//描画
	void DrawOff();
	void Draw();
};

//*********************************PolygonDataクラス*************************************//

class PolygonData {

private:
	//ポインタで受け取る
	Dx12Process *dx;
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
	CONSTANT_BUFFER cb[2];
	int sw = 0;
	//UpLoadカウント
	int upCount = 0;
	//初回Up終了
	bool UpOn = FALSE;
	//DrawOn
	bool DrawOn = FALSE;

	//頂点バッファOBJ
	std::unique_ptr<VertexView> Vview = nullptr;
	std::unique_ptr<IndexView> Iview = nullptr;

	//テクスチャ保持(SetTextureMPixel用)
	ID3D12Resource *texture = NULL;
	ID3D12Resource *textureUp = NULL;
	//movie_on
	bool   m_on = FALSE;
	//テクスチャ番号(通常テクスチャ用)
	int    t_no = -1;
	int    insNum = 0;

	D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
	D3D12_TEXTURE_COPY_LOCATION dest, src;

	//パイプラインOBJ
	Microsoft::WRL::ComPtr<ID3D12PipelineState> mPSO = nullptr;

	Vertex         *d3varray;  //頂点配列
	VertexBC       *d3varrayBC;//頂点配列基本色
	std::uint16_t  *d3varrayI;//頂点インデックス
	int            ver;      //頂点個数
	int            verI;    //頂点インデックス

	D3D12_PRIMITIVE_TOPOLOGY_TYPE  primType_create;
	D3D_PRIMITIVE_TOPOLOGY         primType_draw;

	static std::mutex mtx;
	static void Lock() { mtx.lock(); }
	static void Unlock() { mtx.unlock(); }

	void GetShaderByteCode(bool light, int tNo);
	void CbSwap();

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
		float u, float v);
	void SetVertex(int I1, int i,
		float vx, float vy, float vz,
		float nx, float ny, float nz,
		float u, float v);
	void SetVertexBC(int I1, int I2, int i,
		float vx, float vy, float vz,
		float r, float g, float b, float a);
	void SetVertexBC(int I1, int i,
		float vx, float vy, float vz,
		float r, float g, float b, float a);
	void InstancedMap(float x, float y, float z, float theta);
	void InstancedMap(float x, float y, float z, float theta, float size);
	void InstancedMapSize3(float x, float y, float z, float theta, float sizeX, float sizeY, float sizeZ);
	void InstanceUpdate(float r, float g, float b, float a, float disp);
	void InstanceUpdate(float r, float g, float b, float a, float disp, float px, float py, float mx, float my);
	void Update(float x, float y, float z, float r, float g, float b, float a, float theta, float disp);
	void Update(float x, float y, float z, float r, float g, float b, float a, float theta, float disp, float size);
	void Update(float x, float y, float z, float r, float g, float b, float a, float theta, float disp, float size, float px, float py, float mx, float my);
	void DrawOff();
	void Draw();
};

//*********************************PolygonData2Dクラス*************************************//

class PolygonData2D {

private:
	friend DxText;

	Dx12Process                *dx;
	ID3D12GraphicsCommandList  *mCommandList;
	int                        com_no = 0;
	ID3DBlob                   *vs = nullptr;
	ID3DBlob                   *ps = nullptr;

	int                        ver;//頂点数

	Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mSrvHeap = nullptr;

	//コンスタントバッファOBJ
	UploadBuffer<CONSTANT_BUFFER2D> *mObjectCB = nullptr;
	CONSTANT_BUFFER2D cb2[2];
	int sw = 0;
	//UpLoadカウント
	int upCount = 0;
	//初回Up終了
	bool UpOn = FALSE;
	//DrawOn
	bool DrawOn = FALSE;

	//頂点バッファOBJ
	std::unique_ptr<VertexView> Vview = nullptr;
	std::unique_ptr<IndexView> Iview = nullptr;

	Microsoft::WRL::ComPtr<ID3D12PipelineState> mPSO = nullptr;

	//テクスチャ保持,DxText classでしか使わない
	ID3D12Resource *texture = NULL;
	ID3D12Resource *textureUp = NULL;
	bool tex_on = FALSE;
	//SetTextParameter
	int Twidth;
	int Theight;
	int Tcount;
	TEXTMETRIC *Tm;
	GLYPHMETRICS *Gm;
	BYTE *Ptr;
	DWORD *Allsize;
	bool CreateTextOn = FALSE;

	int  ins_no = 0;
	int  insNum = 0;//Drawで読み込み用

	static std::mutex mtx;
	static void Lock() { mtx.lock(); }
	static void Unlock() { mtx.unlock(); }

	void GetShaderByteCode();
	void SetConstBf(CONSTANT_BUFFER2D *cb2, float x, float y, float z, float r, float g, float b, float a, float sizeX, float sizeY);
	void SetTextParameter(int width, int height, int textCount, TEXTMETRIC **TM, GLYPHMETRICS **GM, BYTE **ptr, DWORD **allsize);
	void SetText();//DxText classでしか使わない
	void CbSwap();

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
	void InstanceUpdate();
	void Update(float x, float y, float r, float g, float b, float a, float sizeX, float sizeY);
	void Update(float x, float y, float z, float r, float g, float b, float a, float sizeX, float sizeY);
	void DrawOff();
	void Draw();
};

class T_float {

private:
	static DWORD f[2], time[2];
	static DWORD time_fps[2];//FPS計測用
	static int frame[2];    //FPS計測使用
	static int up;
	static char str[50];//ウインドウ枠文字表示使用
	static float adj;

public:
	static void GetTime(HWND hWnd);//常に実行
	static void GetTimeUp(HWND hWnd);//常に実行
	static void AddAdjust(float ad);//1.0fが標準
	static int GetUps();
	float Add(float f);
};

//*********************************ParticleDataクラス*************************************//

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
	int                        ver;//頂点数

	PartPos                    *P_pos;//パーティクルデータ配列

	Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature_com = nullptr;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature_draw = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mSrvHeap = nullptr;

	//コンスタントバッファOBJ
	UploadBuffer<CONSTANT_BUFFER_P> *mObjectCB = nullptr;
	CONSTANT_BUFFER_P cbP[2];
	int sw = 0;
	//UpLoadカウント
	int upCount = 0;
	//初回Up終了
	bool UpOn = FALSE;
	//DrawOn
	bool DrawOn = FALSE;

	D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
	D3D12_TEXTURE_COPY_LOCATION dest, src;

	//テクスチャ保持(directshow用)
	ID3D12Resource *texture = NULL;
	ID3D12Resource *textureUp = NULL;
	//movie_on
	bool m_on = FALSE;
	bool texpar_on = FALSE;

	//頂点バッファOBJ
	std::unique_ptr<VertexView> Vview = nullptr;
	//ストリームバッファOBJ
	std::unique_ptr<StreamView[]> Sview1 = nullptr;
	std::unique_ptr<StreamView[]> Sview2 = nullptr;//BufferFilledSizeLocationの送り先
	int svInd = 0;
	bool firstDraw = FALSE;
	int  streamInitcount = 0;
	bool parInit = FALSE;

	Microsoft::WRL::ComPtr<ID3D12PipelineState> mPSO_com = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> mPSO_draw = nullptr;

	static std::mutex mtx;
	static void Lock() { mtx.lock(); }
	static void Unlock() { mtx.unlock(); }

	void GetShaderByteCode();
	void MatrixMap(CONSTANT_BUFFER_P *cb_p, float x, float y, float z, float theta, float size, float speed, bool tex);
	void MatrixMap2(CONSTANT_BUFFER_P *cb_p, bool init);
	void GetVbColarray(int texture_no, float size, float density);
	void CreateVbObj();
	void CreatePartsCom();
	void CreatePartsDraw(int texpar);
	void DrawParts0();
	void DrawParts1();
	void DrawParts2();
	void CbSwap(bool init);

public:
	ParticleData();
	~ParticleData();
	void SetCommandList(int no);
	void TextureInit(int width, int height);
	void GetBufferParticle(int texture_no, float size, float density);//テクスチャを元にパーティクルデータ生成, 全体サイズ倍率, 密度
	void GetBufferBill(int v);
	void SetVertex(int i,
		float vx, float vy, float vz,
		float r, float g, float b, float a);
	void CreateParticle(int texpar);//パーティクル1個のテクスチャナンバー
	void CreateBillboard();//ver個の四角形を生成
	void Update(float x, float y, float z, float theta, float size, bool init, float speed);//sizeパーティクル1個のサイズ
	void DrawOff();
	void Draw();
	void SetTextureMPixel(int **m_pix, BYTE r, BYTE g, BYTE b, int a);
	void Update(float size);
	void DrawBillboard();
};

//*********************************SkinMeshクラス*************************************//

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
	//InitFBX排他処理用
	static volatile bool stInitFBX_ON, stSetNewPose_ON;

	Dx12Process                *dx;
	ID3D12GraphicsCommandList  *mCommandList;
	int                        com_no = 0;
	ID3DBlob                   *vs = nullptr;
	ID3DBlob                   *ps = nullptr;
	bool alpha = FALSE;
	bool blend = FALSE;

	Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mSrvHeap = nullptr;

	//コンスタントバッファOBJ
	UploadBuffer<CONSTANT_BUFFER> *mObjectCB0 = nullptr;
	UploadBuffer<SHADER_GLOBAL1> *mObjectCB1 = nullptr;
	UploadBuffer<SHADER_GLOBAL_BONES> *mObject_BONES = nullptr;
	CONSTANT_BUFFER cb[2];
	SHADER_GLOBAL_BONES sgb[2];
	int sw = 0;
	//UpLoadカウント
	int upCount = 0;
	//初回Up終了
	bool UpOn = FALSE;
	//DrawOn
	bool DrawOn = FALSE;

	std::unique_ptr<VertexView> Vview = nullptr;
	std::unique_ptr<IndexView[]> Iview = nullptr;

	Microsoft::WRL::ComPtr<ID3D12PipelineState> mPSO = nullptr;//パイプラインOBJ

	//メッシュ関連	
	DWORD *m_pdwNumVert;//メッシュ毎の頂点数
	DWORD VerAllpcs;    //全頂点数
	MY_VERTEX_S *pvVB;//使用後保持するか破棄するかフラグで決める,通常は破棄
	bool pvVB_delete_f;

	int MateAllpcs;  //全マテリアル数
	MY_MATERIAL_S *m_pMaterial;

	//一時格納用
	DWORD *m_pMaterialCount;//メッシュ毎のマテリアルカウント
	DWORD *pdwNumFace; //メッシュ毎のポリゴン数
	int *IndexCount34Me;  //4頂点ポリゴン分割前のメッシュ毎のインデックス数
	int *IndexCount3M;  //4頂点ポリゴン分割後のマテリアル毎のインデックス数
	int **piFaceBuffer;//メッシュ毎の頂点インデックス配列
	int **pIndex;      //メッシュ毎の4頂点分割後インデックス配列

	//ボーン
	int m_iNumBone;
	BONE *m_BoneArray;

	//FBX
	SkinMesh_sub *fbx;
	FbxCluster **m_ppCluster;//ボーン情報
	char *m_pClusterName;
	FbxNode **m_ppNodeArray;//各Nodeへのポインタ配列
	int NodeArraypcs;
	FbxNode **m_ppSubAnimationBone;//その他アニメーションボーンポインタ配列
	MATRIX *m_pLastBoneMatrix;
	int AnimLastInd;
	float BoneConnect;

	//共通で使うマネージャー生成(Dx12Processで生成解放を行う)
	static void CreateManager();
	static void DeleteManager();

	static std::mutex mtx;
	static void Lock() { mtx.lock(); }
	static void Unlock() { mtx.unlock(); }

	void DestroyFBX();
	FbxScene* GetScene(int p);
	int SearchNodeCount(FbxNode *pnode, FbxNodeAttribute::EType SearchType);
	FbxNode *SearchNode(FbxNode *pnode, FbxNodeAttribute::EType SearchType, int Ind);
	HRESULT InitFBX(CHAR* szFileName, int p);
	void CreateIndexBuffer(int cnt, int *pIndex, int IviewInd);
	void CreateIndexBuffer2(int *pIndex, int IviewInd);
	HRESULT ReadSkinInfo(MY_VERTEX_S *pvVB);
	MATRIX GetCurrentPoseMatrix(int index);
	void MatrixMap_Bone(SHADER_GLOBAL_BONES *sbB);
	void GetTexture();
	bool SetNewPoseMatrices(float time, int ind);
	void CreateRotMatrix(float thetaZ, float thetaY, float thetaX, int ind);
	void CbSwap();

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
	HRESULT GetFbx(CHAR* szFileName);
	void GetBuffer(float end_frame);
	void SetVertex();
	void CreateFromFBX();
	HRESULT GetFbxSub(CHAR* szFileName, int ind);
	HRESULT GetBuffer_Sub(int ind, float end_frame);
	void CreateFromFBX_SubAnimation(int ind);
	bool Update(float time, float x, float y, float z, float r, float g, float b, float a, float thetaZ, float thetaY, float thetaX, float size);
	bool Update(int ind, float time, float x, float y, float z, float r, float g, float b, float a, float thetaZ, float thetaY, float thetaX, float size);
	void DrawOff();
	void Draw();
	VECTOR3 GetVertexPosition(int verNum, float adjustZ, float adjustY, float adjustX, float thetaZ, float thetaY, float thetaX, float scale);
};

//エラーメッセージ
void ErrorMessage(char *E_mes);

class Wave {

private:
	Dx12Process *dx;
	ID3D12GraphicsCommandList  *mCommandList;
	int                        com_no = 0;
	ID3DBlob                   *cs = nullptr;
	ID3DBlob                   *vs = nullptr;
	ID3DBlob                   *ps = nullptr;
	ID3DBlob                   *hs = nullptr;
	ID3DBlob                   *ds = nullptr;

	Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignatureCom = nullptr;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignatureDraw = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mSrvHeap = nullptr;

	UploadBuffer<CONSTANT_BUFFER> *mObjectCB = nullptr;
	UploadBuffer<CONSTANT_BUFFER_WAVE> *mObjectCB_WAVE = nullptr;
	CONSTANT_BUFFER cb[2];
	CONSTANT_BUFFER_WAVE cbw;
	int sw = 0;
	//UpLoadカウント
	int upCount = 0;
	//初回Up終了
	bool UpOn = FALSE;
	//DrawOn
	bool DrawOn = FALSE;

	std::unique_ptr<VertexView> Vview = nullptr;
	std::unique_ptr<IndexView> Iview = nullptr;

	//テクスチャ番号(通常テクスチャ用)
	int    t_no = -1;
	int    insNum = 0;

	int div;//分割数

	Vertex         *d3varray;  //頂点配列
	std::uint16_t  *d3varrayI;//頂点インデックス
	int            ver;      //頂点個数
	int            verI;    //頂点インデックス

	Microsoft::WRL::ComPtr<ID3D12PipelineState> mPSOCom = nullptr;//パイプラインOBJ
	Microsoft::WRL::ComPtr<ID3D12PipelineState> mPSODraw = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> mInputBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> mInputUploadBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> mOutputBuffer = nullptr;

	static std::mutex mtx;
	static void Lock() { mtx.lock(); }
	static void Unlock() { mtx.unlock(); }

	void GetShaderByteCode();
	void ComCreate();
	void DrawCreate(int texNo, bool blend, bool alpha);
	void CbSwap();
	void Compute();
	void DrawSub();

public:
	Wave();
	void SetCommandList(int no);
	~Wave();
	void GetVBarray(int v);
	void Create(int texNo, bool blend, bool alpha, float waveHeight, float divide);
	//順番:左上左下右下右上
	void SetVertex(int i,
		float vx, float vy, float vz,
		float nx, float ny, float nz,
		float u, float v);
	void InstancedMap(float x, float y, float z, float theta);
	void InstancedMap(float x, float y, float z, float theta, float size);
	void InstancedMapSize3(float x, float y, float z, float theta, float sizeX, float sizeY, float sizeZ);
	void InstanceUpdate(float r, float g, float b, float a, float disp);
	void InstanceUpdate(float r, float g, float b, float a, float disp, float px, float py, float mx, float my);
	void Update(float x, float y, float z, float r, float g, float b, float a, float theta, float disp);
	void Update(float x, float y, float z, float r, float g, float b, float a, float theta, float disp, float size);
	void Update(float x, float y, float z, float r, float g, float b, float a, float theta, float disp, float size, float px, float py, float mx, float my);
	void DrawOff();
	void Draw();
};

#endif
