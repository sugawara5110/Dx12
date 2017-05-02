//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@	 InstanceCreate�N���X                                    **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_InstanceCreate_Header
#define Class_InstanceCreate_Header

#include "Battle.h"
#include "Map.h"
#include <Process.h>

class InstanceCreate{

private:
	//�i�s��
	static int progress;
	//�������\�[�X�ǂݍ���
	static HANDLE *resource_loading_h;
	static HANDLE *hero_loading_h;

	//Battle�����p�p�����[�^
	static HANDLE *battle_loading_h;
	static Position::E_Pos *e_po;
	static Position::H_Pos *h_po;
	static Encount encount_d;
	static int no_d;
	static int e_nu_d;
	static Battle *battle;

	//Map�����p�p�����[�^
	static HANDLE *map_loading_h;
	static Position::H_Pos *h_p;
	static Map *map, *map_t;

	//Hero�I�u�W�F�N�g�ێ��p
	static Hero *he;

	InstanceCreate(){}//�C���X�^���X������

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

	static void HeroGetBuffer();//�V���O���X���b�h�g�p
	static void HeroSetVertex();//�}���`�X���b�h�g�p
	static void HeroCreate();//�V���O���X���b�h�g�p
	static Hero *HeroCreate_f();

	static void ResourceLoad();
	static bool Resource_load_f();

	static HANDLE *GetHANDLE_B();
	static void SetInstanceParameter_B(Hero *h, Position::E_Pos *e_pos, Position::H_Pos *h_pos, Encount encount, int no, int e_nu);
	static void BattleGetBuffer();
	static void BattleSetVertex();
	static void BattleCreate();
	static bool BattleCreate_f();
	static void BattleDelete();
	static Battle *GetInstance_B();//�}���`�X���b�h�Ő��������NULL�`�F�b�N�ł͖����I���t���OBattleCreate_f()�Ő����m�F����

	static HANDLE *GetHANDLE_M();
	static void SetInstanceParameter_M(Position::H_Pos *h_pos, Hero *h);
	static void MapGetBuffer();
	static void MapSetVertex();
	static void MapCreate();
	static bool MapCreate_f();
	static void MapObjSet();
	static void MapDelete();
	static Map *GetInstance_M();//�}���`�X���b�h�Ő��������NULL�`�F�b�N�ł͖����I���t���OMapCreate_f()�Ő����m�F����

	static bool CreateBattleIns(Hero *h, Encount encount, int no, int e_nu);
	static bool CreateMapIns(Position::H_Pos *h_pos, Hero *h, int *map_no);
};

#endif

unsigned __stdcall ResourceLoading(void *);
unsigned __stdcall InstanceLoadingHero(void *);
unsigned __stdcall InstanceLoadingBattle(void *);
unsigned __stdcall InstanceLoadingMap(void *);