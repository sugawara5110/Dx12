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
#include "../../Common/Direct3DWrapper//DxText.h"
#include "Map.h"
#include "Control.h"
#include "InstanceCreate.h"
#include "Hero.h"
#include "StateMenu.h"
#include <Process.h>
#include "Ending.h"
#include "TextureBinaryLoader.h"
#pragma comment(lib,"winmm.lib")
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
	//�|�X�g�G�t�F�N�g
	PostEffect* mosaic = NULL;
	PostEffect* blur = NULL;
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
	HANDLE* update_h;
	int sync = 0;
	SkinMesh* dragon = nullptr;

	void DrawNowLoading(int com_no);
	void CreateThreadUpdate();
	void DeleteThreadUpdate();

public:
	//���b�Z�[�W
	MSG msg;
	volatile bool UpDateThreadLoop = TRUE;

	static Main* GetInstance();
	static void DeleteInstance();

	bool Init(HINSTANCE hInstance, int nCmdShow);
	void Loop();
	void UpDate();
	void Draw();
	void ObjDel();
	~Main();
};

unsigned __stdcall UpDateThread(void *);

#endif
