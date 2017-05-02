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
#include "Win.h"
#include "Dx12Process.h"
#include "DxText.h"
#include "Map.h"
#include "Control.h"
#include "InstanceCreate.h"
#include "Hero.h"
#include "StateMenu.h"
#include <Process.h>
#include "Ending.h"
#pragma comment(lib,"winmm.lib")

class Main {

private:
	HWND hWnd;//�E�B���h�E�n���h��
	
	//DirectX12���b�p�[
	Dx12Process *dx;
	//��������
	DxText *text;
	//�v���C���[
	Hero *hero = NULL;

	//Loop���Ŏg�p����ϐ�
	bool battle_flg[3] = { FALSE, FALSE, FALSE };
	Position::H_Pos *h_posIn = NULL;
	Position::H_Pos h_posOut, h_posOut2;
	Position::mapxy *mxy = NULL;
	bool posget[2] = { FALSE, FALSE };

	Control *control;
	int map_no = 0;

	StateMenu *statemenu;
	Encount encount = NOENCOUNT;
	bool menu = FALSE;
	bool title = TRUE;
	bool title_in = TRUE;
	bool heroInput = FALSE;
	Result result = WIN;
	MapState mapstate = NORMAL_MAP;
	Ending *ending = NULL;
	bool endingflg = FALSE;
	int  rnd;
	T_float tfloat;

	void DrawNowLoading();

public:
	MSG msg;  //���b�Z�[�W

	bool Init(HINSTANCE hInstance, int nCmdShow);
	void Loop();
	void UpDate();
	~Main();
};

#endif
