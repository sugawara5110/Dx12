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
#include "Position.h"
#pragma comment(lib,"winmm.lib")
#define REFWIDTH 800
#define REFHEIGHT 600
#define CURRWIDTH 1024
#define CURRHEIGHT 768

class Main {

private:
	//ウィンドウハンドル
	HWND hWnd;
	//DirectX12ラッパー
	Dx12Process* dx;
	//文字入力
	DxText* text;
	DXR_Basic* dxr = nullptr;
	MaterialType materialType[200];
	ParameterDXR* pdx[200];
	//ポストエフェクト
	PostEffect* mosaic = NULL;
	PostEffect* blur = NULL;
	PostEffect* blur2 = NULL;
	//プレイヤー
	Hero* hero = NULL;
	//入力
	Control* control;
	//メニュー
	StateMenu* statemenu;
	//エンディング
	Ending* ending = NULL;
	bool endingflg = FALSE;
	//時間
	T_float tfloat;

	static Main* main;

	//その他Loop内で使用する変数
	int map_no = 0;
	MapState mapstate = NORMAL_MAP;
	Encount encount = NOENCOUNT;
	Result result = IN_BATTLE;
	bool menu = FALSE;
	int battleSwitch = 0;
	volatile bool mpDel_f = FALSE;
	volatile bool btDel_f = FALSE;
	HANDLE* update_h;

	void DrawNowLoading(int com_no);
	void CreateThreadUpdate();
	void DeleteThreadUpdate();
	void changeMap();
	bool changeBattleF = false;
	void changeBattle();

public:
	//メッセージ
	MSG msg;
	volatile bool UpDateThreadLoop = TRUE;

	static Main* GetInstance();
	static void DeleteInstance();

	bool Init(HINSTANCE hInstance, int nCmdShow);
	void Loop();
	void UpDate();
	void SetMovie();
	void Draw();
	void StreamOutput();
	void StreamOutputAfterDraw();
	void ObjDel();
	~Main();
};

unsigned __stdcall UpDateThread(void *);

#endif
