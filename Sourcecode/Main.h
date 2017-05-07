//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　    Mainクラス                                          **//
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
	//ウィンドウハンドル
	HWND hWnd;
	//DirectX12ラッパー
	Dx12Process *dx;
	//文字入力
	DxText *text;
	//プレイヤー
	Hero *hero = NULL;
	//入力
	Control *control;
	//メニュー
	StateMenu *statemenu;
	//エンディング
	Ending *ending = NULL;
	bool endingflg = FALSE;
	//時間
	T_float tfloat;

	//その他Loop内で使用する変数
	int map_no = 0;
	MapState mapstate = NORMAL_MAP;
	Encount encount = NOENCOUNT;
	Result result = IN_BATTLE;
	bool menu = FALSE;
	
	void DrawNowLoading(int com_no);

public:
	//メッセージ
	MSG msg;  

	bool Init(HINSTANCE hInstance, int nCmdShow);
	void Loop();
	void UpDate();
	~Main();
};

#endif
