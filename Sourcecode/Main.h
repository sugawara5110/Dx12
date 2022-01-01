//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@    Main�N���X                                          **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Main_Header
#define Class_Main_Header

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../Common/Window/Win.h"
#include "../../Common/Direct3DWrapper/Dx12Process.h"
#include "../../Common/Direct3DWrapperOption/DxText.h"
#include "../../Common/Direct3DWrapperOption/Dx_PostEffect.h"
#include "../../CreateGeometry/CreateGeometry.h"
#include "Map.h"
#include "Control.h"
#include "InstanceCreate.h"
#include "Hero.h"
#include "StateMenu.h"
#include <Process.h>
#include "Ending.h"
#include "TextureBinaryLoader.h"
#include "Position.h"
#define REFWIDTH 800
#define REFHEIGHT 600
#define CURRWIDTH 1024
#define CURRHEIGHT 768

class Main {

private:
	//�E�B���h�E�n���h��
	HWND hWnd;
	//DirectX12���b�p�[
	Dx12Process* dx;
	//��������
	DxText* text;
	DXR_Basic* dxr = nullptr;
	ParameterDXR* pdx[200];
	//�|�X�g�G�t�F�N�g
	PostEffect* mosaic = NULL;
	PostEffect* blur = NULL;
	PostEffect* blur2 = NULL;
	//�v���C���[
	Hero* hero = NULL;
	//����
	Control* control;
	//���j���[
	StateMenu* statemenu;
	//�G���f�B���O
	Ending* ending = NULL;
	bool endingflg = FALSE;
	//����
	T_float tfloat;

	static Main* main;

	//���̑�Loop���Ŏg�p����ϐ�
	int map_no = 0;
	MapState mapstate = NORMAL_MAP;
	Encount encount = NOENCOUNT;
	Result result = IN_BATTLE;
	bool menu = FALSE;
	int battleSwitch = 0;
	volatile bool mpDel_f = FALSE;
	volatile bool btDel_f = FALSE;

	PolygonData* pd = nullptr;
	SkinMesh* sk[5] = {};

	MultiThread th;

	void DrawNowLoading(int com_no);
	void CreateThreadUpdate();
	void DeleteThreadUpdate();
	void changeMap();
	bool changeBattleF = false;
	void changeBattle();

public:
	//���b�Z�[�W
	MSG msg;

	static Main* GetInstance();
	static void DeleteInstance();

	bool Init(HINSTANCE hInstance, int nCmdShow);
	void Loop();
	void UpDate();
	void SetMovie(int com_no);
	void Draw();
	void StreamOutput();
	void AS();
	void Raytrace();
	void StreamOutputAfterDraw();
	void ObjDel();
	~Main();
};

#endif
