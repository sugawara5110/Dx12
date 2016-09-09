//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	 InstanceCreateクラス                                    **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_InstanceCreate_Header
#define Class_InstanceCreate_Header

#include "Battle.h"
#include "Map.h"
#include <Process.h>
#include <new>

class InstanceCreate{

private:
	//プレイヤーオブジェクト生成
	static HANDLE *hero_loading_h;

	//初期リソース読み込み
	static HANDLE *resource_loading_h;

	//Battle生成用パラメータ
	static HANDLE *battle_loading_h;
	static Position::E_Pos *e_po;
	static Position::H_Pos *h_po;
	static Encount encount_d;
	static int no_d;
	static int e_nu_d;
	static Battle *battle;

	//Map生成用パラメータ
	static HANDLE *map_loading_h;
	static Position::H_Pos *h_p;
	static Map *map, *map_t;

	//Heroオブジェクト保持用
	static Hero *he;

	InstanceCreate(){};//インスタンス生成無

public:
	static void CreateThread_H();
	static void CreateThread_R();
	static void CreateThread_B();
	static void CreateThread_M();

	static void DeleteThread_H();
	static void DeleteThread_R();
	static void DeleteThread_B();
	static void DeleteThread_M();

	static void HeroCreate();
	static Hero *HeroCreate_f();

	static void ResourceLoad();
	static bool Resource_load_f();

	static HANDLE *GetHANDLE_B();
	static void SetInstanceParameter_B(Hero *h, Position::E_Pos *e_pos, Position::H_Pos *h_pos, Encount encount, int no, int e_nu);
	static void BattleCreate();//マルチスレッド以外で使用禁止
	static void BattleDelete();
	static bool BattleCreate_f();
	static Battle *GetInstance_B();//マルチスレッドで生成する為NULLチェックでは無く終了フラグBattleCreate_f()で生成確認する

	static HANDLE *GetHANDLE_M();
	static void SetInstanceParameter_M(Position::H_Pos *h_pos, Hero *h);
	static void MapCreate();//マルチスレッド以外で使用禁止(メインループ外は可)
	static void MapObjSet();
	static void MapDelete();
	static bool MapCreate_f();
	static Map *GetInstance_M();//マルチスレッドで生成する為NULLチェックでは無く終了フラグMapCreate_f()で生成確認する

	static bool CreateBattleIns(Hero *h, Encount encount, int no, int e_nu);
	static bool CreateMapIns(Position::H_Pos *h_pos, Hero *h, int *map_no);
};

#endif

unsigned __stdcall HeroLoading(void *);
unsigned __stdcall ResourceLoading(void *);
unsigned __stdcall InstanceLoadingBattle(void *);
unsigned __stdcall InstanceLoadingMap(void *);