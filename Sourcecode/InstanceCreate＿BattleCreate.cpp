//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	 InstanceCreateクラス                                    **//
//**                               BattleCreate関数                                      **//
//*****************************************************************************************//

#include "InstanceCreate.h"
#include "TextureBinaryLoader.h"

int InstanceCreate::progress = 0;
HANDLE *InstanceCreate::resource_loading_h = NULL;

HANDLE *InstanceCreate::battle_loading_h = NULL;
Position::E_Pos *InstanceCreate::e_po = NULL;
Position::H_Pos *InstanceCreate::h_po = NULL;
Encount InstanceCreate::encount_d = NOENCOUNT;
int InstanceCreate::no_d = -1;
int InstanceCreate::e_nu_d = -1;
Battle *InstanceCreate::battle = NULL;

HANDLE *InstanceCreate::map_loading_h = NULL;
Position::H_Pos *InstanceCreate::h_p = NULL;
Map *InstanceCreate::map = NULL;
Map *InstanceCreate::map_t = NULL;

Hero *InstanceCreate::he = NULL;

int InstanceCreate::GetProgress() {
	return progress;
}

void InstanceCreate::CreateThread_R(){
	resource_loading_h = (HANDLE*)_beginthreadex(NULL, 0, ResourceLoading, NULL, 0, NULL);
}

void InstanceCreate::CreateThread_B(){
	battle_loading_h = (HANDLE*)_beginthreadex(NULL, 0, InstanceLoadingBattle, NULL, 0, NULL);
}

void InstanceCreate::CreateThread_M(){
	map_loading_h = (HANDLE*)_beginthreadex(NULL, 0, InstanceLoadingMap, NULL, 0, NULL);
}

void InstanceCreate::DeleteThread_R() {
	WaitForSingleObject(resource_loading_h, INFINITE);//スレッドが終了するまで待つ
	CloseHandle(resource_loading_h);                 //ハンドルを閉じる
	resource_loading_h = NULL;
}

void InstanceCreate::DeleteThread_B(){
	WaitForSingleObject(battle_loading_h, INFINITE);//スレッドが終了するまで待つ
	CloseHandle(battle_loading_h);                 //ハンドルを閉じる
	battle_loading_h = NULL;
}

void InstanceCreate::DeleteThread_M(){
	WaitForSingleObject(map_loading_h, INFINITE);//スレッドが終了するまで待つ
	CloseHandle(map_loading_h);                 //ハンドルを閉じる
	map_loading_h = NULL;
}

void InstanceCreate::HeroCreate() {
	Dx12Process::Lock();
	he = new Hero[4];
	Dx12Process::Unlock();
	for (int i = 0; i < 4; i++) {
		Dx12Process::GetInstance()->Bigin(HERO_COM);
		new(he + i) Hero(i); progress += 10;//配列をplacement newを使って初期化する
		Dx12Process::GetInstance()->End(HERO_COM);
		Dx12Process::GetInstance()->RequestSync();
	}
}

void InstanceCreate::ResourceLoad() {
	Dx12Process::GetInstance()->RequestSync();
	TextureBinaryLoader::TextureGetBufferAll();
	progress = 10;
	Dx12Process::GetInstance()->RequestSync();
	TextureBinaryLoader::TextureBinaryDecodeAll();
	progress = 30;
	Dx12Process::GetInstance()->RequestSync();
	MovieSoundManager::ObjInit();
	progress = 40;
	Dx12Process::GetInstance()->RequestSync();
	Dx12Process::GetInstance()->GetTexture();
	progress = 50;
	Dx12Process::GetInstance()->RequestSync();
}

Hero *InstanceCreate::Resource_load_f() {
	DWORD th_end;
	GetExitCodeThread(resource_loading_h, &th_end);
	if (th_end == STILL_ACTIVE)return NULL;
	return he;
}

HANDLE *InstanceCreate::GetHANDLE_B(){
	return battle_loading_h;
}

void InstanceCreate::SetInstanceParameter_B(Hero *h, Position::E_Pos *e_pos, Position::H_Pos *h_pos, Encount encount, int no, int e_nu){
	he = h;
	e_po = e_pos;
	h_po = h_pos;
	encount_d = encount;
	no_d = no;
	e_nu_d = e_nu;
}

void InstanceCreate::BattleCreate() {
	if (battle == NULL) {
		Dx12Process::Lock();
		battle = new Battle();
		Dx12Process::Unlock();
		new(battle)Battle(he, e_po, h_po, encount_d, no_d, e_nu_d);
	}
	//Dx12Process::GetInstance()->RequestSync();
}

void InstanceCreate::BattleDelete(){
	S_DELETE(battle)
}

bool InstanceCreate::BattleCreate_f() {
	if (Battle::InitFin() == FALSE)return FALSE;
	return TRUE;
}

Battle *InstanceCreate::GetInstance_B(){
	return battle;
}

HANDLE *InstanceCreate::GetHANDLE_M(){
	return map_loading_h;
}

void InstanceCreate::SetInstanceParameter_M(Position::H_Pos *h_pos, Hero *h){
	h_p = h_pos;
	he = h;
}

void InstanceCreate::MapCreate(){
	S_DELETE(map_t);
	Dx12Process::Lock();
	map_t = new Map();
	Dx12Process::Unlock();
	new(map_t) Map(h_p, he);
	MovieSoundManager::ObjCreate_map(Map::GetMapNo());
	progress = 95;
	Dx12Process::GetInstance()->RequestSync();
}

void InstanceCreate::MapObjSet() {
	S_DELETE(map);
	MovieSoundManager::ObjChange_map();
	map = map_t;
	map_t = NULL;
	progress = 100;
}

void InstanceCreate::MapDelete(){
	S_DELETE(map);
	S_DELETE(map_t);
}

bool InstanceCreate::MapCreate_f(){
	DWORD th_end;
	GetExitCodeThread(map_loading_h, &th_end);
	if (th_end == STILL_ACTIVE)return FALSE;
	return TRUE;
}

Map *InstanceCreate::GetInstance_M(){
	return map;
}

bool InstanceCreate::CreateBattleIns(Hero *h, Encount encount, int no, int e_nu) {

	static bool th = FALSE;

	if (!th && GetHANDLE_B() == NULL) {
		SetInstanceParameter_B(h, GetInstance_M()->Getposition(e_nu),
			GetInstance_M()->Getposition(), encount, no, e_nu + 1);
		CreateThread_B();
	}

	if (!th && GetHANDLE_B() != NULL) {
		bool bf = BattleCreate_f();
		if (bf == TRUE) {
			DeleteThread_B();
			th = TRUE;
		}
	}

	if (th) {
		if (battle != NULL && GetInstance_B()->InitFin()) {
			//GPU処理を終わらせる,ここは本スレで実行されるのでWaitFenceCurrent()を使用
			Dx12Process::GetInstance()->WaitFenceCurrent();
			th = FALSE; return TRUE;
		}
	}
	return FALSE;
}

bool InstanceCreate::CreateMapIns(Position::H_Pos *h_pos, Hero *h, int *map_no){
	if (GetHANDLE_M() == NULL){
		SetInstanceParameter_M(h_pos, h);
		CreateThread_M();
		*map_no = Map::GetMapNo();
	}
	if (GetHANDLE_M() != NULL && MapCreate_f() == TRUE){
		MapObjSet();
		DeleteThread_M();
		return FALSE;
	}
	return TRUE;
}

unsigned __stdcall ResourceLoading(void *) {
	Dx12Process::GetInstance()->SyncFin(FALSE);
	InstanceCreate::ResourceLoad();
	InstanceCreate::HeroCreate();
	InstanceCreate::MapCreate();//タイトルに出力するマップ
	InstanceCreate::MapObjSet();
	Dx12Process::GetInstance()->RequestSync();
	Dx12Process::GetInstance()->SyncFin(TRUE);
	return 0;
}

unsigned __stdcall InstanceLoadingBattle(void *){
	//Dx12Process::GetInstance()->SyncFin(FALSE);
	InstanceCreate::BattleCreate();//後で他のスレッド同様のCreate時の処理に変更する
	//Dx12Process::GetInstance()->SyncFin(TRUE);
	return 0;
}

unsigned __stdcall InstanceLoadingMap(void *){
	Dx12Process::GetInstance()->SyncFin(FALSE);
	InstanceCreate::MapCreate();
	Dx12Process::GetInstance()->SyncFin(TRUE);
	return 0;
}