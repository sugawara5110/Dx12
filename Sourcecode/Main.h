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
#include "./Direct3DWrapper/Dx12Process.h"
#include "./Direct3DWrapper//DxText.h"
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
	//�E�B���h�E�n���h��
	HWND hWnd;
	//DirectX12���b�p�[
	Dx12Process *dx;
	//��������
	DxText *text;
	//�v���C���[
	Hero *hero = NULL;
	//����
	Control *control;
	//���j���[
	StateMenu *statemenu;
	//�G���f�B���O
	Ending *ending = NULL;
	bool endingflg = FALSE;
	//����
	T_float tfloat;

	//���̑�Loop���Ŏg�p����ϐ�
	int map_no = 0;
	MapState mapstate = NORMAL_MAP;
	Encount encount = NOENCOUNT;
	Result result = IN_BATTLE;
	bool menu = FALSE;
	
	void DrawNowLoading(int com_no);

public:
	//���b�Z�[�W
	MSG msg;  

	bool Init(HINSTANCE hInstance, int nCmdShow);
	void Loop();
	void UpDate();
	~Main();
};

#endif
