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
#include "../../MultiThread/MultiThread.h"

class InstanceCreate {

private:
	//進行状況
	static int progress;

	//Battle生成用パラメータ
	static Battle* battle;

	//Map生成用パラメータ
	static Position::H_Pos* h_p;
	static Map* map[2];
	static int mapInd;//セットするindex

	//Heroオブジェクト保持用
	static Hero* he;

	static MultiThread_NotSync th;

	InstanceCreate() {}//インスタンス生成無

public:
	static int GetProgress();
	static void CreateThread_R();
	static void CreateThread_H();
	static void CreateThread_B();
	static void CreateThread_M();

	static void DeleteThread_R();
	static void DeleteThread_H();
	static void DeleteThread_B();
	static void DeleteThread_M();

	static void HeroGetBuffer();//シングルスレッド使用
	static void HeroSetVertex();//マルチスレッド使用
	static void HeroCreate();//シングルスレッド使用
	static Hero* HeroCreate_f();

	static void ResourceLoad();
	static bool Resource_load_f();

	static void SetInstanceParameter_B(Hero* h, Position::E_Pos* e_pos, Position::H_Pos* h_pos, Encount encount, int no, int e_nu);
	static void BattleGetBuffer();
	static void BattleSetVertex();
	static void BattleCreate();
	static bool BattleCreate_f();
	static void BattleDelete();
	static Battle* GetInstance_B();//マルチスレッドで生成する為NULLチェックでは無く終了フラグBattleCreate_f()で生成確認する

	static void SetInstanceParameter_M(Position::H_Pos* h_pos, Hero* h);
	static void MapGetBuffer();
	static void MapSetVertex();
	static void MapCreate();
	static bool MapCreate_f();
	static void MapObjSet();
	static void InsDelete();
	static void MapDelete();
	static Map* GetInstance_M();//マルチスレッドで生成する為NULLチェックでは無く終了フラグMapCreate_f()で生成確認する

	static bool CreateBattleIns(Hero* h, Encount encount, int no, int e_nu);
	static bool CreateMapIns(Position::H_Pos* h_pos, Hero* h, int* map_no);
};

#endif