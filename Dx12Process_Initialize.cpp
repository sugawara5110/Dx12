//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　          Dx12Processクラス                                 **//
//**                                   Initialize関数                                    **//
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
		//コマンドアロケータ生成(コマンドリストに積むバッファを確保するObj)
		if (FAILED(Dx12Process::dx->md3dDevice->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(mCmdListAlloc[i].GetAddressOf()))))FALSE;
	}

	//コマンドリスト生成
	if (FAILED(Dx12Process::dx->md3dDevice->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		mCmdListAlloc[0].Get(),
		nullptr,
		IID_PPV_ARGS(mCommandList.GetAddressOf()))))FALSE;

	//最初は閉じた方が良い
	mCommandList->Close();
	mCommandList->Reset(mCmdListAlloc[0].Get(), nullptr);
}

void Dx12Process_sub::Bigin() {
	mAloc_Num = 1 - mAloc_Num;
	mCmdListAlloc[mAloc_Num]->Reset();
	mCommandList->Reset(mCmdListAlloc[mAloc_Num].Get(), nullptr);
}

void Dx12Process_sub::End() {

	//コマンドクローズ
	mCommandList->Close();

	while (Dx12Process::dx->CommandQueueAcc);
	Dx12Process::dx->CommandQueueAcc = true;
	//クローズ後リストに加える
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	Dx12Process::dx->mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
	Dx12Process::dx->CommandQueueAcc = false;
}

Dx12Process *Dx12Process::dx = NULL;

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

	for (int i = 0; i < TEX_PCS; i++) {
		if (binary_ch[i] == NULL)continue;
		free(binary_ch[i]);
		binary_ch[i] = NULL;
		RELEASE(texture[i]);
		RELEASE(textureUp[i]);
	}
	free(binary_ch);
	free(binary_size);
}

void Dx12Process::WaitFence(int fence) {

	while (Dx12Process::dx->CommandQueueAcc);
	Dx12Process::dx->CommandQueueAcc = true;
	//インクリメントされたことで描画完了と判断
	mCurrentFence++;
	//GPU上で実行されているコマンド完了後,自動的にフェンスにmCurrentFenceを書き込む
	//(mFence->GetCompletedValue()で得られる値がmCurrentFenceと同じになる)
	mCommandQueue->Signal(mFence.Get(), mCurrentFence);

	//ここまででコマンドキューが終了してしまうと
	//↓のイベントが正しく処理されない可能性有る為↓ifでチェックしている
	//GetCompletedValue():Fence内部UINT64のカウンタ取得(初期値0)
	//コマンドアロケータをダブルバッファにしているのでFence値1つ手前までのGPU処理が未完の場合のみ待ち処理を行う
	if (mFence->GetCompletedValue() < mCurrentFence - fence) {

		HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
		//このFenceにおいて,mCurrentFence の値になったらイベントを発火させる
		//↑上記のif文が無いと既にmCurrentFenceと同値になってしまう事もあるのでその際は↓WaitForSingleObjectで停止したままになる
		mFence->SetEventOnCompletion(mCurrentFence, eventHandle);
		//イベントが発火するまで待つ(GPUの処理待ち)これによりGPU上の全コマンド実行終了まで待たせる事が出来る
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}
	Dx12Process::dx->CommandQueueAcc = false;
}

void Dx12Process::WaitFenceCurrent() {
	WaitFence(0);
}

void Dx12Process::WaitFencePast() {
	WaitFence(1);
}

void Dx12Process::CreateShaderByteCode() {

	//スキンメッシュ
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

	//ストリーム出力データ定義(パーティクル用)
	pDeclaration_PSO =
	{
		{ 0, "POSITION", 0, 0, 3, 0 }, //「x,y,z」をスロット「0」の「POSITION」に出力
		{ 0, "POSITION", 1, 0, 3, 0 },
		{ 0, "POSITION", 2, 0, 3, 0 },
		{ 0, "COLOR", 0, 0, 4, 0 }
	};
	//ストリーム出力
	pVertexShader_PSO = dx->CompileShader(ShaderParticle, strlen(ShaderParticle), "VS_SO", "vs_5_0");
	pGeometryShader_PSO = dx->CompileShader(ShaderParticle, strlen(ShaderParticle), "GS_Point_SO", "gs_5_0");

	//パーティクル頂点インプットレイアウトを定義
	pVertexLayout_P = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "POSITION", 1, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "POSITION", 2, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3 * 2, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 3 * 3, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
	//パーティクル
	pVertexShader_P = dx->CompileShader(ShaderParticle, strlen(ShaderParticle), "VS", "vs_5_0");
	pGeometryShader_P = dx->CompileShader(ShaderParticle, strlen(ShaderParticle), "GS_Point", "gs_5_0");
	pPixelShader_P = dx->CompileShader(ShaderParticle, strlen(ShaderParticle), "PS", "ps_5_0");

	//メッシュレイアウト
	pVertexLayout_MESH =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3 * 2, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
	//メッシュ
	pVertexShader_MESH = dx->CompileShader(ShaderMesh, strlen(ShaderMesh), "VSMesh", "vs_5_0");
	pPixelShader_MESH = dx->CompileShader(ShaderMesh, strlen(ShaderMesh), "PSMesh", "ps_5_0");
	//テセレーター有メッシュ
	pVertexShader_MESH_D = dx->CompileShader(ShaderMesh_D, strlen(ShaderMesh_D), "VSMesh", "vs_5_0");
	pPixelShader_MESH_D = dx->CompileShader(ShaderMesh_D, strlen(ShaderMesh_D), "PSMesh", "ps_5_0");
	pHullShader_MESH_D = dx->CompileShader(ShaderMesh_D, strlen(ShaderMesh_D), "HSMesh", "hs_5_0");
	pDomainShader_MESH_D = dx->CompileShader(ShaderMesh_D, strlen(ShaderMesh_D), "DSMesh", "ds_5_0");

	//3Dレイアウト
	pVertexLayout_3D =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 3 * 2, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3 * 2 + 4 * 4, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
	//テクスチャ3Dライト無
	pVertexShader_TC = dx->CompileShader(Shader3D, strlen(Shader3D), "VSTextureColor", "vs_5_0");
	pPixelShader_TC = dx->CompileShader(Shader3D, strlen(Shader3D), "PSTextureColor", "ps_5_0");
	//テクスチャ3Dライト有
	pVertexShader_TCL = dx->CompileShader(Shader3D, strlen(Shader3D), "VSTextureColorL", "vs_5_0");
	pPixelShader_TCL = dx->CompileShader(Shader3D, strlen(Shader3D), "PSTextureColorL", "ps_5_0");
	//基本色3D
	pVertexShader_BC = dx->CompileShader(Shader3D, strlen(Shader3D), "VSBaseColor", "vs_5_0");
	pPixelShader_BC = dx->CompileShader(Shader3D, strlen(Shader3D), "PSBaseColor", "ps_5_0");
	//テセレータライト無
	pVertexShader_DISP = dx->CompileShader(ShaderDisp, strlen(ShaderDisp), "VSDisp", "vs_5_0");
	pPixelShader_DISP = dx->CompileShader(ShaderDisp, strlen(ShaderDisp), "PSDisp", "ps_5_0");
	pHullShader_DISP = dx->CompileShader(ShaderDisp, strlen(ShaderDisp), "HSDisp", "hs_5_0");
	pDomainShader_DISP = dx->CompileShader(ShaderDisp, strlen(ShaderDisp), "DSDisp", "ds_5_0");
	//テセレータライト有
	pVertexShader_DISPL = dx->CompileShader(ShaderDisp, strlen(ShaderDisp), "VSDispL", "vs_5_0");
	pPixelShader_DISPL = dx->CompileShader(ShaderDisp, strlen(ShaderDisp), "PSDispL", "ps_5_0");
	pHullShader_DISPL = dx->CompileShader(ShaderDisp, strlen(ShaderDisp), "HSDispL", "hs_5_0");
	pDomainShader_DISPL = dx->CompileShader(ShaderDisp, strlen(ShaderDisp), "DSDispL", "ds_5_0");

	//2Dレイアウト
	pVertexLayout_2D =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 3, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3 + 4 * 4, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
	//テクスチャ2D
	pVertexShader_2DTC = dx->CompileShader(Shader2D, strlen(Shader2D), "VSTextureColor", "vs_5_0");
	pPixelShader_2DTC = dx->CompileShader(Shader2D, strlen(Shader2D), "PSTextureColor", "ps_5_0");
	//2D
	pVertexShader_2D = dx->CompileShader(Shader2D, strlen(Shader2D), "VSBaseColor", "vs_5_0");
	pPixelShader_2D = dx->CompileShader(Shader2D, strlen(Shader2D), "PSBaseColor", "ps_5_0");
}

void Dx12Process::TextureBinaryDecode(char *Bpass, int i) {
	Bdecode(Bpass, &binary_ch[i], &binary_size[i]);
}

void Dx12Process::TextureBinaryDecodeAll() {

	//マップ0
	TextureBinaryDecode("./dat/texture/map/wall1.da", 0);
	TextureBinaryDecode("./dat/texture/map/ground1.da", 1);
	TextureBinaryDecode("./dat/texture/map/ceiling1.da", 2);
	//マップ1
	TextureBinaryDecode("./dat/texture/map/wall2.da", 4);
	TextureBinaryDecode("./dat/texture/map/ground2.da", 5);
	TextureBinaryDecode("./dat/texture/map/ceiling2.da", 6);
	TextureBinaryDecode("./dat/texture/map/wall2-1.da", 8);
	TextureBinaryDecode("./dat/texture/map/leaf.da", 9);
	TextureBinaryDecode("./dat/texture/map/wood.da", 10);
	//マップ2
	TextureBinaryDecode("./dat/texture/map/ceiling3_wall3.da", 11);
	TextureBinaryDecode("./dat/texture/map/ground3.da", 12);
	//マップ3
	TextureBinaryDecode("./dat/texture/map/ceiling4_ground4.da", 15);
	//マップ4
	TextureBinaryDecode("./dat/texture/map/wall5.da", 26);
	TextureBinaryDecode("./dat/texture/map/ground5.da", 27);
	TextureBinaryDecode("./dat/texture/map/ceiling5.da", 28);
	//通常敵
	TextureBinaryDecode("./dat/texture/enemy/enemy1.da", 30);
	TextureBinaryDecode("./dat/texture/enemy/enemy2.da", 31);
	TextureBinaryDecode("./dat/texture/enemy/enemy3.da", 32);
	TextureBinaryDecode("./dat/texture/enemy/enemy4.da", 33);
	TextureBinaryDecode("./dat/texture/enemy/enemy5.da", 34);
	TextureBinaryDecode("./dat/texture/enemy/enemy6.da", 35);
	TextureBinaryDecode("./dat/texture/enemy/enemy7.da", 36);
	TextureBinaryDecode("./dat/texture/enemy/enemy8.da", 37);
	TextureBinaryDecode("./dat/texture/enemy/enemy9.da", 38);
	TextureBinaryDecode("./dat/texture/enemy/enemy10.da", 39);
	TextureBinaryDecode("./dat/texture/enemy/enemy11.da", 40);
	TextureBinaryDecode("./dat/texture/enemy/enemy12.da", 41);
	TextureBinaryDecode("./dat/texture/enemy/enemy13.da", 42);
	TextureBinaryDecode("./dat/texture/enemy/enemy14.da", 43);
	TextureBinaryDecode("./dat/texture/enemy/enemy15.da", 44);
	TextureBinaryDecode("./dat/texture/enemy/enemy16.da", 45);
	TextureBinaryDecode("./dat/texture/enemy/enemy17.da", 46);
	TextureBinaryDecode("./dat/texture/enemy/enemy18.da", 47);
	TextureBinaryDecode("./dat/texture/enemy/enemy19.da", 48);
	TextureBinaryDecode("./dat/texture/enemy/enemy20.da", 49);
	//ボス
	TextureBinaryDecode("./dat/texture/enemy/boss1.da", 50);
	TextureBinaryDecode("./dat/texture/enemy/boss2.da", 51);
	TextureBinaryDecode("./dat/texture/enemy/boss3.da", 52);
	TextureBinaryDecode("./dat/texture/enemy/boss4.da", 53);
	//ラストボス
	TextureBinaryDecode("./dat/texture/enemy/lastboss.da", 59);
	//魔方陣通常
	TextureBinaryDecode("./dat/texture/magic/side_magic.da", 60);
	//魔方陣ボス
	TextureBinaryDecode("./dat/texture/magic/boss_magic.da", 61);
	TextureBinaryDecode("./dat/texture/magic/particle.da", 62);
	//回復ポイント
	TextureBinaryDecode("./dat/texture/magic/recover.da", 70);
	//エフェクト
	TextureBinaryDecode("./dat/texture/effect/e_att.da", 80);
	TextureBinaryDecode("./dat/texture/effect/h_att.da", 81);
	TextureBinaryDecode("./dat/texture/effect/flame.da", 82);
	TextureBinaryDecode("./dat/texture/effect/healing.da", 83);
	TextureBinaryDecode("./dat/texture/effect/recov.da", 84);
	//プレイヤー1テクスチャ
	TextureBinaryDecode("./dat/texture/player/p1/brown_eye.da", 90);
	TextureBinaryDecode("./dat/texture/player/p1/classicshoes_texture_diffuse.da", 91);
	TextureBinaryDecode("./dat/texture/player/p1/eyebrow001.da", 92);
	TextureBinaryDecode("./dat/texture/player/p1/jacket01_diffuse.da", 93);
	TextureBinaryDecode("./dat/texture/player/p1/jeans01_black_diffuse.da", 94);
	TextureBinaryDecode("./dat/texture/player/p1/male01_diffuse_black.da", 95);
	TextureBinaryDecode("./dat/texture/player/p1/young_lightskinned_male_diffuse.da", 96);
	//プレイヤー2テクスチャ
	TextureBinaryDecode("./dat/texture/player/p2/brown_eye.da", 100);
	TextureBinaryDecode("./dat/texture/player/p2/diffuse_black.da", 101);
	TextureBinaryDecode("./dat/texture/player/p2/eyebrow006.da", 102);
	TextureBinaryDecode("./dat/texture/player/p2/eyelashes03.da", 103);
	TextureBinaryDecode("./dat/texture/player/p2/shoes02_default.da", 104);
	TextureBinaryDecode("./dat/texture/player/p2/short01_black_diffuse.da", 105);
	TextureBinaryDecode("./dat/texture/player/p2/tshirt02_texture.da", 106);
	TextureBinaryDecode("./dat/texture/player/p2/young_lightskinned_female_diffuse.da", 107);
	//プレイヤー3テクスチャ
	TextureBinaryDecode("./dat/texture/player/p3/brown_eye.da", 110);
	TextureBinaryDecode("./dat/texture/player/p3/classicshoes_texture_diffuse.da", 111);
	TextureBinaryDecode("./dat/texture/player/p3/jeans_basic_diffuse.da", 112);
	TextureBinaryDecode("./dat/texture/player/p3/young_darkskinned_male_diffuse.da", 113);
	//プレイヤー4テクスチャ
	TextureBinaryDecode("./dat/texture/player/p4/afro.da", 120);
	TextureBinaryDecode("./dat/texture/player/p4/brown_eye.da", 121);
	TextureBinaryDecode("./dat/texture/player/p4/eyebrow007.da", 122);
	TextureBinaryDecode("./dat/texture/player/p4/eyelashes03.da", 123);
	TextureBinaryDecode("./dat/texture/player/p4/shoes02_default.da", 124);
	TextureBinaryDecode("./dat/texture/player/p4/short01_black_diffuse.da", 125);
	TextureBinaryDecode("./dat/texture/player/p4/tshirt_texture_blue.da", 126);
	TextureBinaryDecode("./dat/texture/player/p4/young_darkskinned_female_diffuse.da", 127);
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
			sprintf(str, "テクスチャ№%d読み込みエラー", (i));
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
	//デバッグ中はデバッグレイヤーを有効化する
	{
		ComPtr<ID3D12Debug> debugController;
		if (FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))FALSE;
		debugController->EnableDebugLayer();
	}
#endif

	//ファクトリ生成
	//アダプターの列挙、スワップ チェーンの作成、
	//および全画面表示モードとの間の切り替えに使用される Alt + 
	//Enter キー シーケンスとのウィンドウの関連付けを行うオブジェクトを生成するために使用
	if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&mdxgiFactory))))FALSE;

	//ハードウエア処理可能か,ハードウエア処理デバイス生成
	HRESULT hardwareResult = D3D12CreateDevice(
		nullptr,             // default adapter
		D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS(&md3dDevice));

	//↑ハードウエア処理不可の場合ソフトウエア処理にする,ソフトウエア処理デバイス生成
	if (FAILED(hardwareResult))
	{
		ComPtr<IDXGIAdapter> pWarpAdapter;
		if (FAILED(mdxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter))))FALSE;

		if (FAILED(D3D12CreateDevice(
			pWarpAdapter.Get(),
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&md3dDevice))))FALSE;
	}

	//フェンス生成
	//Command Queueに送信したCommand Listの完了を検知するために使用
	if (FAILED(md3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS(&mFence))))FALSE;

	//Descriptor のアドレス計算で使用
	mRtvDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	mDsvDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	mCbvSrvUavDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//MultiSampleレベルチェック
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
	msQualityLevels.Format = mBackBufferFormat;
	msQualityLevels.SampleCount = 4;
	msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQualityLevels.NumQualityLevels = 0;
	if (FAILED(md3dDevice->CheckFeatureSupport(
		D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
		&msQualityLevels,     //機能のサポートを示すデータが格納
		sizeof(msQualityLevels))))FALSE;

	m4xMsaaQuality = msQualityLevels.NumQualityLevels;
	assert(m4xMsaaQuality > 0 && "Unexpected MSAA quality level.");

	//コマンドキューデスク
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT; //直接コマンドキュー
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE; //GPUタイムアウトが有効
	//コマンド待ち行列生成
	if (FAILED(md3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mCommandQueue))))FALSE;

	for (int i = 0; i < COM_NO; i++) {
		//コマンドアロケータ,コマンドリスト生成
		dx_sub[i].ListCreate();
	}
	//初期化
	mSwapChain.Reset();

	//スワップチェイン生成
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

	//Descriptor:何のバッファかを記述される
	//スワップチェインをRenderTargetとして使用するためのDescriptorHeapを作成(Descriptorの記録場所)
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
	rtvHeapDesc.NumDescriptors = SwapChainBufferCount;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;   //RenderTargetView
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; //シェーダからアクセスしないのでNONEでOK
	rtvHeapDesc.NodeMask = 0;
	if (FAILED(md3dDevice->CreateDescriptorHeap(
		&rtvHeapDesc, IID_PPV_ARGS(mRtvHeap.GetAddressOf()))))FALSE;

	//深度ステンシルビュ-DescriptorHeapを作成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dsvHeapDesc.NodeMask = 0;
	if (FAILED(md3dDevice->CreateDescriptorHeap(
		&dsvHeapDesc, IID_PPV_ARGS(mDsvHeap.GetAddressOf()))))FALSE;

	//レンダーターゲットビューデスクリプターヒープの開始ハンドル取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(mRtvHeap->GetCPUDescriptorHandleForHeapStart());
	for (UINT i = 0; i < SwapChainBufferCount; i++)
	{
		//スワップチェインバッファ取得
		if (FAILED(mSwapChain->GetBuffer(i, IID_PPV_ARGS(&mSwapChainBuffer[i]))))FALSE;
		//レンダーターゲットビュー(Descriptor)生成,DescriptorHeapにDescriptorが記録される
		md3dDevice->CreateRenderTargetView(mSwapChainBuffer[i].Get(), nullptr, rtvHeapHandle);
		//ヒープ位置オフセット(2個あるので2個目記録位置にDescriptorSize分オフセット)
		rtvHeapHandle.Offset(1, mRtvDescriptorSize);
	}

	//デプスステンシルビューDESC
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
	//深度ステンシルバッファ領域確保
	if (FAILED(md3dDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthStencilDesc,
		D3D12_RESOURCE_STATE_COMMON,
		&optClear,
		IID_PPV_ARGS(mDepthStencilBuffer.GetAddressOf()))))FALSE;

	//深度ステンシルビューデスクリプターヒープの開始ハンドル取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE mDsvHeapHeapHandle(mDsvHeap->GetCPUDescriptorHandleForHeapStart());
	//深度ステンシルビュー生成
	md3dDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), nullptr, mDsvHeapHeapHandle);

	//深度ステンシルバッファ,リソースバリア共有→深度書き込み
	dx_sub[0].mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mDepthStencilBuffer.Get(),
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));

	//ビューポート
	mScreenViewport.TopLeftX = 0;
	mScreenViewport.TopLeftY = 0;
	mScreenViewport.Width = static_cast<float>(mClientWidth);
	mScreenViewport.Height = static_cast<float>(mClientHeight);
	mScreenViewport.MinDepth = 0.0f;
	mScreenViewport.MaxDepth = 1.0f;

	mScissorRect = { 0, 0, mClientWidth, mClientHeight };

	MatrixIdentity(&mProj);
	MatrixIdentity(&mView);
	//カメラ画角
	ViewY_theta = 55.0f;
	// アスペクト比の計算
	aspect = (float)mScreenViewport.Width / (float)mScreenViewport.Height;
	//nearプレーン
	NearPlane = 1.0f;
	//farプレーン
	FarPlane = 10000.0f;
	MatrixPerspectiveFovLH(&mProj, ViewY_theta, aspect, NearPlane, FarPlane);

	//ビューポート行列作成(3D座標→2D座標変換に使用)
	MatrixViewPort(&Vp);

	//クローズしてからキューに積む(コマンドリストは全部積む、そうしないとエラーが出る)
	for (int i = 0; i < COM_NO; i++) {
		dx_sub[i].mCommandList->Close();
		ID3D12CommandList* cmdsLists[] = { dx_sub[i].mCommandList.Get() };
		mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
	}
	WaitFenceCurrent();
	//ポイントライト構造体初期化
	ResetPointLight();

	//平行光源初期化
	dlight.Direction.as(0.0f, 0.0f, 0.0f, 0.0f);
	dlight.LightColor.as(1.0f, 1.0f, 1.0f, 1.0f);
	dlight.Lightst.as(1.0f, 0.0f, 0.3f, 0.0f);

	//フォグ初期化
	fog.FogColor.as(1.0f, 1.0f, 1.0f, 1.0f);
	fog.Amount = 0.0f;
	fog.Density = 0.0f;
	fog.on_off = 0.0f;

	//テクスチャバイナリ
	binary_ch = (char**)malloc(sizeof(char*) * TEX_PCS);
	binary_size = (int*)malloc(sizeof(int) * TEX_PCS);
	for (int i = 0; i < TEX_PCS; i++) {
		binary_ch[i] = NULL;
		binary_size[i] = 0;
	}

	CreateShaderByteCode();

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
	WaitFencePast();
}

void Dx12Process::Cameraset(float cx1, float cx2, float cy1, float cy2, float cz1, float cz2) {

	//カメラの位置と方向を設定
	MatrixLookAtLH(&mView,
		cx1, cy1, cz1,   //カメラの位置
		cx2, cy2, cz2,   //カメラの方向を向ける点
		0.0f, 0.0f, 1.0f); //カメラの上の方向(通常視点での上方向を1.0fにする)
	//シェーダー計算用座標登録(視点からの距離で使う)
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

	static int pcs = LIGHT_PCS_init;//個数初期値(予約済み)

	if (Idx > LIGHT_PCS - 1 || Idx < 0)return;//エラー防止

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

	//デフォルトバッファ生成
	device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(byteSize),
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(defaultBuffer.GetAddressOf()));

	//デフォルトバッファにCPUメモリデータをコピーするための中間バッファ生成
	device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(byteSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(uploadBuffer.GetAddressOf()));

	//デフォルトバッファにコピーしたいデータの内容
	D3D12_SUBRESOURCE_DATA subResourceData = {};
	subResourceData.pData = initData;
	subResourceData.RowPitch = (LONG_PTR)byteSize;
	subResourceData.SlicePitch = subResourceData.RowPitch;

	//デフォルトバッファ,リソースバリア共有→コピーされる側
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(),
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));
	//UpdateSubresources:中間バッファにCPUメモリをコピーする
	//その後CommandListを使うと中間バッファの内容をアップロードされる
	UpdateSubresources<1>(cmdList, defaultBuffer.Get(), uploadBuffer.Get(), 0, 0, 1, &subResourceData);
	//デフォルトバッファ,リソースバリアコピーされる側→アップロード
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ));

	//コマンドリストが作成されない場合でもコピーは実行される
	//コピーが実行された後uploadBufferはリリースされる

	return defaultBuffer;
}

Microsoft::WRL::ComPtr<ID3D12Resource> Dx12Process::CreateStreamBuffer(ID3D12Device* device, UINT64 byteSize)
{
	ComPtr<ID3D12Resource> defaultBuffer;

	//ストリームバッファ生成
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

	//エラー処理をどうするか

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

	//拡大縮小
	MatrixScaling(&scale, sizeX, sizeY, sizeZ);
	//表示位置
	MatrixRotationZ(&rotZ, thetaZ);
	MatrixRotationY(&rotY, thetaY);
	MatrixRotationX(&rotX, thetaX);
	MatrixMultiply(&rotZY, &rotZ, &rotY);
	MatrixMultiply(&rotZYX, &rotZY, &rotX);
	MatrixTranslation(&mov, x, y, z);
	MatrixMultiply(&scro, &rotZYX, &scale);
	MatrixMultiply(&world, &scro, &mov);

	//ワールド、カメラ、射影行列、等
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

	//拡大縮小
	MatrixScaling(&scale, sizeX, sizeY, sizeZ);
	//表示位置
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

//移動量一定化
DWORD T_float::f = timeGetTime();
DWORD T_float::time = 0;
DWORD T_float::time_fps = 0;//FPS計測用
int T_float::frame = 0;     //FPS計測使用
char T_float::str[50];     //ウインドウ枠文字表示使用

void T_float::GetTime(HWND hWnd) {
	time = timeGetTime() - f;
	f = timeGetTime();

	//FPS計測
	frame++;
	sprintf(str, "     Ctrl:決定  Delete:キャンセル  fps=%d", frame);
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

//エラーメッセージ
void ErrorMessage(char *E_mes) {
	MessageBoxA(0, E_mes, 0, MB_OK);
}