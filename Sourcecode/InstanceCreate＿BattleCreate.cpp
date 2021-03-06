//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	 InstanceCreateクラス                                    **//
//**                               BattleCreate関数                                      **//
//*****************************************************************************************//

#include "InstanceCreate.h"
#include "TextureBinaryLoader.h"
#include "../../Common/TextureLoader/TextureLoader.h"

int InstanceCreate::progress = 0;
HANDLE *InstanceCreate::resource_loading_h = NULL;
HANDLE *InstanceCreate::hero_loading_h = NULL;

HANDLE *InstanceCreate::battle_loading_h = NULL;
Battle *InstanceCreate::battle = NULL;

HANDLE *InstanceCreate::map_loading_h = NULL;
Position::H_Pos *InstanceCreate::h_p = NULL;
Map *InstanceCreate::map[2] = { 0 };
int InstanceCreate::mapInd = 0;

Hero *InstanceCreate::he = NULL;

int InstanceCreate::GetProgress() {
	return progress;
}

void InstanceCreate::CreateThread_R() {
	resource_loading_h = (HANDLE*)_beginthreadex(NULL, 0, ResourceLoading, NULL, 0, NULL);
}

void InstanceCreate::CreateThread_H() {
	hero_loading_h = (HANDLE*)_beginthreadex(NULL, 0, InstanceLoadingHero, NULL, 0, NULL);
}

void InstanceCreate::CreateThread_B() {
	battle_loading_h = (HANDLE*)_beginthreadex(NULL, 0, InstanceLoadingBattle, NULL, 0, NULL);
}

void InstanceCreate::CreateThread_M() {
	map_loading_h = (HANDLE*)_beginthreadex(NULL, 0, InstanceLoadingMap, NULL, 0, NULL);
}

void InstanceCreate::DeleteThread_R() {
	WaitForSingleObject(resource_loading_h, INFINITE);//スレッドが終了するまで待つ
	CloseHandle(resource_loading_h);                 //ハンドルを閉じる
	resource_loading_h = NULL;
	MovieSoundManager::ObjInit();
	TextureLoader::GetTexture2(TextureBinaryLoader::getTexNum(), TextureBinaryLoader::getTexture(),
		Dx12Process::GetInstance());
	progress = 30;
}

void InstanceCreate::DeleteThread_H() {
	WaitForSingleObject(hero_loading_h, INFINITE);//スレッドが終了するまで待つ
	CloseHandle(hero_loading_h);                 //ハンドルを閉じる
	hero_loading_h = NULL;
}

void InstanceCreate::DeleteThread_B() {
	WaitForSingleObject(battle_loading_h, INFINITE);//スレッドが終了するまで待つ
	CloseHandle(battle_loading_h);                 //ハンドルを閉じる
	battle_loading_h = NULL;
	Dx12Process::GetInstance()->RunGpu();
	Dx12Process::GetInstance()->WaitFence();
	battle->SetCommandList(0);
}

void InstanceCreate::DeleteThread_M() {
	WaitForSingleObject(map_loading_h, INFINITE);//スレッドが終了するまで待つ
	CloseHandle(map_loading_h);                 //ハンドルを閉じる
	map_loading_h = NULL;
	Dx12Process::GetInstance()->RunGpu();
	Dx12Process::GetInstance()->WaitFence();
	map[1 - mapInd]->SetCommandList(0);
	mapInd = 1 - mapInd;
	MapObjSet();//マップのセット
}

void InstanceCreate::HeroGetBuffer() {
	he = new Hero[4];
	for (int i = 0; i < 4; i++) {
		new(he + i) Hero(i);//配列をplacement newを使って初期化する
		progress += 10;
	}
}

void InstanceCreate::HeroSetVertex() {
	for (int i = 0; i < 4; i++) {
		he[i].SetVertex();
		progress += 2;
	}
}

void InstanceCreate::HeroCreate() {
	Dx12Process::GetInstance()->Bigin(1);
	for (int i = 0; i < 4; i++) {
		he[i].SetCommandList(1);
		he[i].CreateHero();
	}
	Dx12Process::GetInstance()->End(1);
}

Hero *InstanceCreate::HeroCreate_f() {
	DWORD th_end;
	GetExitCodeThread(hero_loading_h, &th_end);
	if (th_end == STILL_ACTIVE)return NULL;
	return he;
}

void InstanceCreate::ResourceLoad() {
	TextureBinaryLoader::TextureGetBufferAll();
	progress = 10;
	TextureBinaryLoader::TextureBinaryDecodeAll();
}

bool InstanceCreate::Resource_load_f() {
	DWORD th_end;
	GetExitCodeThread(resource_loading_h, &th_end);
	if (th_end == STILL_ACTIVE)return FALSE;
	return TRUE;
}

HANDLE *InstanceCreate::GetHANDLE_B() {
	return battle_loading_h;
}

void InstanceCreate::SetInstanceParameter_B(Hero *h, Position::E_Pos *e_pos, Position::H_Pos *h_pos, Encount encount, int no, int e_nu) {
	battle = new Battle();
	battle->SetParameter(h, e_pos, h_pos, encount, no, e_nu);
}

void InstanceCreate::BattleGetBuffer() {
	battle->Init();
}

void InstanceCreate::BattleSetVertex() {
	battle->SetVertex();
}

void InstanceCreate::BattleCreate() {
	Dx12Process::GetInstance()->Bigin(1);
	battle->SetCommandList(1);
	battle->CreateBattle();
	Dx12Process::GetInstance()->End(1);
}

bool InstanceCreate::BattleCreate_f() {
	DWORD th_end;
	GetExitCodeThread(battle_loading_h, &th_end);
	if (th_end == STILL_ACTIVE)return FALSE;
	return TRUE;
}

void InstanceCreate::BattleDelete() {
	S_DELETE(battle)
}

Battle *InstanceCreate::GetInstance_B() {
	return battle;
}

HANDLE *InstanceCreate::GetHANDLE_M() {
	return map_loading_h;
}

void InstanceCreate::SetInstanceParameter_M(Position::H_Pos *h_pos, Hero *h) {
	h_p = h_pos;
	he = h;
}

void InstanceCreate::MapGetBuffer() {
	S_DELETE(map[1 - mapInd]);
	map[1 - mapInd] = new Map();
	new(map[1 - mapInd]) Map(h_p, he);
	progress = 85;
}

void InstanceCreate::MapSetVertex() {
	map[1 - mapInd]->SetVertex();
	progress = 90;
}

void InstanceCreate::MapCreate() {
	Dx12Process::GetInstance()->Bigin(1);
	map[1 - mapInd]->SetCommandList(1);
	map[1 - mapInd]->CreateMap();
	Dx12Process::GetInstance()->End(1);
	progress = 95;
}

bool InstanceCreate::MapCreate_f() {
	DWORD th_end;
	GetExitCodeThread(map_loading_h, &th_end);
	if (th_end == STILL_ACTIVE)return FALSE;
	return TRUE;
}

void InstanceCreate::MapObjSet() {
	MovieSoundManager::ObjCreate_map(Map::GetMapNo());
	MovieSoundManager::ObjChange_map();
	progress = 100;
}

void InstanceCreate::MapDelete() {
	S_DELETE(map[0]);
	S_DELETE(map[1]);
}

void InstanceCreate::InsDelete() {
	S_DELETE(map[1 - mapInd]);//使用されなくなったobj
}

Map* InstanceCreate::GetInstance_M() {
	return map[mapInd];//使用するobj
}

bool InstanceCreate::CreateBattleIns(Hero *h, Encount encount, int no, int e_nu) {
	//ロード中は常に入ってくる
	static int th = 0;

	switch (th) {
	case 0:
		SetInstanceParameter_B(h, GetInstance_M()->Getposition(e_nu),
			GetInstance_M()->Getposition(), encount, no, e_nu + 1);
		CreateThread_B();
		th = 1;
		break;

	case 1:
		if (GetHANDLE_B() != NULL && BattleCreate_f()) {
			DeleteThread_B();
			th = 0;
			return TRUE;
		}
		break;
	}
	return FALSE;
}

bool InstanceCreate::CreateMapIns(Position::H_Pos *h_pos, Hero *h, int *map_no) {
	if (GetHANDLE_M() == NULL) {
		SetInstanceParameter_M(h_pos, h);
		CreateThread_M();
		*map_no = Map::GetMapNo();
	}
	if (GetHANDLE_M() != NULL && MapCreate_f()) {
		DeleteThread_M();
		return FALSE;
	}
	return TRUE;
}

unsigned __stdcall ResourceLoading(void *) {
	InstanceCreate::ResourceLoad();
	return 0;
}

unsigned __stdcall InstanceLoadingHero(void*) {
	InstanceCreate::HeroGetBuffer();
	InstanceCreate::HeroSetVertex();
	InstanceCreate::HeroCreate();
	return 0;
}

unsigned __stdcall InstanceLoadingBattle(void *) {
	InstanceCreate::BattleGetBuffer();
	InstanceCreate::BattleSetVertex();
	InstanceCreate::BattleCreate();
	return 0;
}

unsigned __stdcall InstanceLoadingMap(void *) {
	InstanceCreate::MapGetBuffer();
	InstanceCreate::MapSetVertex();
	InstanceCreate::MapCreate();
	return 0;
}