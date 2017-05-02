//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	 InstanceCreateクラス                                    **//
//**                               BattleCreate関数                                      **//
//*****************************************************************************************//

#include "InstanceCreate.h"
#include "TextureBinaryLoader.h"

int InstanceCreate::progress = 0;
HANDLE *InstanceCreate::resource_loading_h = NULL;
HANDLE *InstanceCreate::hero_loading_h = NULL;

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
	TextureBinaryLoader::TextureGetBufferAll();
	progress = 10;
	resource_loading_h = (HANDLE*)_beginthreadex(NULL, 0, ResourceLoading, NULL, 0, NULL);
}

void InstanceCreate::CreateThread_H() {
	InstanceCreate::HeroGetBuffer();
	hero_loading_h = (HANDLE*)_beginthreadex(NULL, 0, InstanceLoadingHero, NULL, 0, NULL);
}

void InstanceCreate::CreateThread_B(){
	InstanceCreate::BattleGetBuffer();
	battle_loading_h = (HANDLE*)_beginthreadex(NULL, 0, InstanceLoadingBattle, NULL, 0, NULL);
}

void InstanceCreate::CreateThread_M(){
	InstanceCreate::MapGetBuffer();
	map_loading_h = (HANDLE*)_beginthreadex(NULL, 0, InstanceLoadingMap, NULL, 0, NULL);
}

void InstanceCreate::DeleteThread_R() {
	WaitForSingleObject(resource_loading_h, INFINITE);//スレッドが終了するまで待つ
	CloseHandle(resource_loading_h);                 //ハンドルを閉じる
	resource_loading_h = NULL;
	Dx12Process::GetInstance()->GetTexture();
	MovieSoundManager::ObjInit();
	progress = 30;
}

void InstanceCreate::DeleteThread_H() {
	WaitForSingleObject(hero_loading_h, INFINITE);//スレッドが終了するまで待つ
	CloseHandle(hero_loading_h);                 //ハンドルを閉じる
	hero_loading_h = NULL;
}

void InstanceCreate::DeleteThread_B(){
	WaitForSingleObject(battle_loading_h, INFINITE);//スレッドが終了するまで待つ
	CloseHandle(battle_loading_h);                 //ハンドルを閉じる
	battle_loading_h = NULL;
}

void InstanceCreate::DeleteThread_M() {
	WaitForSingleObject(map_loading_h, INFINITE);//スレッドが終了するまで待つ
	CloseHandle(map_loading_h);                 //ハンドルを閉じる
	map_loading_h = NULL;
	MapObjSet();//マップのセット
}

void InstanceCreate::HeroGetBuffer() {
	he = new Hero[4];
	Dx12Process::GetInstance()->Bigin(HERO_COM);
	for (int i = 0; i < 4; i++) {
		new(he + i) Hero(i);//配列をplacement newを使って初期化する
	}
	progress += 10;
}

void InstanceCreate::HeroSetVertex() {
	for (int i = 0; i < 4; i++) {
		he[i].SetVertex();
		progress += 10;
	}
}

void InstanceCreate::HeroCreate() {
	for (int i = 0; i < 4; i++) {
		he[i].CreateHero();
	}
	Dx12Process::GetInstance()->End(HERO_COM);
}

Hero *InstanceCreate::HeroCreate_f() {
	DWORD th_end;
	GetExitCodeThread(hero_loading_h, &th_end);
	if (th_end == STILL_ACTIVE)return NULL;
	return he;
}

void InstanceCreate::ResourceLoad() {
	TextureBinaryLoader::TextureBinaryDecodeAll();
}

bool InstanceCreate::Resource_load_f() {
	DWORD th_end;
	GetExitCodeThread(resource_loading_h, &th_end);
	if (th_end == STILL_ACTIVE)return FALSE;
	return TRUE;
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

void InstanceCreate::BattleGetBuffer() {
	if (battle == NULL) {
		battle = new Battle();
		new(battle)Battle(he, e_po, h_po, encount_d, no_d, e_nu_d);
	}
}

void InstanceCreate::BattleSetVertex() {
	battle->SetVertex();
}

void InstanceCreate::BattleCreate() {
	battle->CreateBattle();
}

bool InstanceCreate::BattleCreate_f() {
	return battle->CreateB_Fin();
}

void InstanceCreate::BattleDelete(){
	S_DELETE(battle)
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

void InstanceCreate::MapGetBuffer() {
	S_DELETE(map_t);
	map_t = new Map();
	new(map_t) Map(h_p, he);
	progress = 85;
}

void InstanceCreate::MapSetVertex() {
	map_t->SetVertex();
	progress = 90;
}

void InstanceCreate::MapCreate() {
	map_t->CreateMap();
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
	S_DELETE(map);
	MovieSoundManager::ObjChange_map();
	map = map_t;//生成したマップのアドレスを描画用ポインタ変数にコピー
	map_t = NULL;//コピーしたのでNULL設定にする
	progress = 100;
}

void InstanceCreate::MapDelete(){
	S_DELETE(map);
	S_DELETE(map_t);
}

Map *InstanceCreate::GetInstance_M(){
	return map;
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
		if (BattleCreate_f()) {
			DeleteThread_B();
			Dx12Process::GetInstance()->WaitFenceCurrent();
			th = 0;
			return TRUE;
		}
		break;
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
		DeleteThread_M();
		return FALSE;
	}
	return TRUE;
}

unsigned __stdcall ResourceLoading(void *) {
	InstanceCreate::ResourceLoad();
	return 0;
}

unsigned __stdcall InstanceLoadingHero(void *) {
	InstanceCreate::HeroSetVertex();
	InstanceCreate::HeroCreate();
	return 0;
}

unsigned __stdcall InstanceLoadingBattle(void *){
	InstanceCreate::BattleSetVertex();
	InstanceCreate::BattleCreate();
	return 0;
}

unsigned __stdcall InstanceLoadingMap(void *){
	InstanceCreate::MapSetVertex();
	InstanceCreate::MapCreate();
	return 0;
}