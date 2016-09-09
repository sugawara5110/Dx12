//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　    3DRPG_Main                                          **//
//**                                                                                     **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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

//-------------------------------------------------------------
// メッセージ処理用コールバック関数
// 引数
//		hWnd	: ウィンドウハンドル
//		msg		: メッセージ
//		wParam	: メッセージの最初のパラメータ(押されたキーの特定等に使用)
//		lParam	: メッセージの2番目のパラメータ
// 戻り値
//		メッセージ処理結果
//-------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){

	switch (msg) {
	case WM_CLOSE:			//×ボタン
		PostQuitMessage(0);//アプリケーション終了処理,メッセージキューにWM_QUITをポスト
		break;
	case WM_KEYDOWN:
		switch ((CHAR)wParam){
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		case VK_LEFT:
			Control::directionkey = LEFT;
			break;
		case VK_RIGHT:
			Control::directionkey = RIGHT;
			break;
		case VK_UP:
			Control::directionkey = UP;
			break;
		case VK_DOWN:
			Control::directionkey = DOWN;
			break;
		case VK_CONTROL:
			Control::directionkey = ENTER;
			break;
		case VK_DELETE:
			Control::directionkey = CANCEL;
			break;
		default:
			Control::directionkey = NOTPRESS;
			break;
		}
		break;
	default:
		Control::directionkey = NOTPRESS;
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);//アプリケーションが処理しないウィンドウメッセージに対しての既定の処理実行
}

//-------------------------------------------------------------
// アプリケーションのエントリポイント
// 引数
//		hInstance     : 現在のインスタンスのハンドル
//		hPrevInstance : 以前のインスタンスのハンドル(win16での別インスタンス確認用win32では常にNULL)
//		lpCmdLine	  : コマンドラインパラメータ
//		nCmdShow	  : ウィンドウの表示状態
// 戻り値
//		成功したら0以外の値
//-------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){

	srand((unsigned)time(NULL));
	TCHAR clsName[] = L"3DRPG";// ウィンドウクラス名

	HWND hWnd;//ウィンドウハンドル
	MSG msg; //メッセージ
	//ウインドウクラスの初期化
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX); //この構造体のサイズ
	wcex.style = NULL;               //ウインドウスタイル(default)
	wcex.lpfnWndProc = WindowProc;  //メッセージ処理関数の登録
	wcex.cbClsExtra = 0;       //通常は0	                
	wcex.cbWndExtra = 0;      //通常は0					
	wcex.hInstance = hInstance; //インスタンスへのハンドル				
	wcex.hIcon = NULL;         //アイコン (無し)				
	wcex.hCursor = NULL;      //カーソルの形				
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); //背景				
	wcex.lpszMenuName = NULL;                       //メニュー無し				
	wcex.lpszClassName = (TCHAR*)clsName;          //クラス名               
	wcex.hIconSm = NULL;                          //小アイコン			   

	//ウインドウクラスの登録(RegisterClassEx関数)
	if (!RegisterClassEx(&wcex))return -1;

	//ウインドウ生成ウインドウモード
	if (!(hWnd = CreateWindow(clsName, //登録クラス名
		clsName,                      //ウインドウ名
		WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE,//ウインドウスタイル
		CW_USEDEFAULT, //ウインドウ横位置
		0,            //ウインドウ縦位置
		WINDOW_WIDTH,    //ウインドウ幅
		WINDOW_HEIGHT,  //ウインドウ高さ
		NULL,          //親ウインドウハンドル
		NULL,         //メニュー,子ウインドウハンドル
		hInstance,   //アプリケーションインスタンスハンドル
		NULL)))     //ウインドウ作成データ
		return -1;

	// ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);
	// WM_PAINTが呼ばれないようにする
	ValidateRect(hWnd, 0);

	//Dx12Processオブジェクト生成
	Dx12Process::InstanceCreate();
	//Dx11Processオブジェクト取得
	Dx12Process *dx = Dx12Process::GetInstance();
	try{
		//DirectX初期化
		dx->Initialize(hWnd);
	}
	catch (char *E_mes){
		ErrorMessage(E_mes);
		Dx12Process::DeleteInstance();
		return -1;
	}
	//DxTextオブジェクト生成
	DxText::InstanceCreate();
	//プレイヤー
	Hero *hero = NULL;
	//スレッド生成
	InstanceCreate::CreateThread_R();
	double i = 300.0;
	bool down = TRUE;
	bool res_f = FALSE;
	bool Thread_H_st_f = FALSE;
	try {
		while (1) {
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				if (msg.message == WM_QUIT) {	// PostQuitMessage()が呼ばれた(×押された)
					Dx12Process::DeleteInstance();
					return -1;	//アプリ終了
				}
				else {
					// メッセージの翻訳とディスパッチWindowProc呼び出し
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			if (hero == NULL)hero = InstanceCreate::HeroCreate_f();
			if (res_f == FALSE)res_f = InstanceCreate::Resource_load_f();
			if (res_f == TRUE && Thread_H_st_f == FALSE) {
				InstanceCreate::DeleteThread_R();
				InstanceCreate::CreateThread_H();
				Thread_H_st_f = TRUE;
			}
			if (res_f == TRUE && hero != NULL) {
				InstanceCreate::DeleteThread_H();
				break;
			}
			dx->Sclear();
			DxText::GetInstance()->BiginDraw();
			TCHAR *str = L"ＮｏｗＬｏａｄｉｎｇ";
			DxText::GetInstance()->UpDateText(&str, 215.0f, (float)i, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
			DxText::GetInstance()->EndDraw();
			dx->DrawScreen();
			if (down == TRUE)i += 0.01;
			if (down == FALSE)i -= 0.01;
			if (i > 320.0)down = FALSE;
			if (i < 280.0)down = TRUE;
		}
	}
	catch (char *E_mes){
		ErrorMessage(E_mes);
		DxText::DeleteInstance();
		Dx12Process::DeleteInstance();
		return -1;
	}

	bool battle_flg[3] = { FALSE, FALSE, FALSE };
	Control control;
	int map_no = 0;
	InstanceCreate::MapCreate();//タイトルに出力するマップ
	InstanceCreate::MapObjSet();
	StateMenu statemenu;
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

	while (1){//アプリ実行中ループ
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {	// PostQuitMessage()が呼ばれた(×押された)
				break;	//ループの終了
			}
			else {
				// メッセージの翻訳とディスパッチWindowProc呼び出し
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		dx->Sclear();
		DxText::GetInstance()->BiginDraw();
		
		if (title == TRUE){
			if (title_in == TRUE){
				title_in = statemenu.TitleMenu(control.Direction());
			}
			if (title_in == FALSE){
				if (heroInput == NULL){
					for (int i = 0; i < 4; i++)hero[i].P_DataInput(statemenu.SetP_Data(i));//パラメーターインプット
					heroInput = TRUE;
					Map::SetMapNo(statemenu.SetMap_No());
					for (int i = 0; i < 5; i++)Map::SetBossKilled(i, statemenu.Set_boss_kil(i));
				}
				title = InstanceCreate::CreateMapIns(statemenu.SetH_Pos(), &hero[0], &map_no);
			}
		}

		dx->Bigin(HERO_COM, nullptr);

		encount = InstanceCreate::GetInstance_M()->Mapdraw(&mapstate, control.Direction(TRUE), encount, menu, title, endingflg);

		if (mapstate == CHANGE_MAP){
			bool m_flg = InstanceCreate::CreateMapIns(NULL, &hero[0], &map_no);
			if (!m_flg)mapstate = NORMAL_MAP;
		}

		if (mapstate == RECOV_MAP){
			for (int i = 0; i < 4; i++){
				hero[i].all_recover();
			}
			mapstate = NORMAL_MAP;
		}

		if (endingflg == FALSE && title == FALSE && encount == NOENCOUNT && menu == FALSE && control.Direction() == ENTER)menu = TRUE;

		if (endingflg == FALSE && title == FALSE && encount != NOENCOUNT && menu == FALSE){
			if (battle_flg[2] == FALSE){
				if (encount == SIDE){
					int LV = (hero[0].s_LV() + hero[1].s_LV() + hero[2].s_LV() + hero[3].s_LV()) / 4;
					//レベルによって敵出現数制限
					rnd = 0;
					if ((map_no + 1) * 3 < LV)rnd = rand() % 2;
					if ((map_no + 1) * 5 < LV)rnd = rand() % 3;
					if ((map_no + 1) * 7 < LV)rnd = rand() % 4;
				}
				else rnd = 0;
				Position::H_Pos *h_posIn = InstanceCreate::GetInstance_M()->Getposition();
				Position::H_Pos h_posOut;

				bool f = Position::CamAdvance(h_posIn, &h_posOut, tfloat.Add(0.2f));

				dx->Cameraset(h_posOut.cx, h_posOut.cx2, h_posOut.cy, h_posOut.cy2, h_posOut.cz, h_posOut.cz);
				hero[0].OBJWalkDraw(h_posIn->cx1, h_posIn->cy1, h_posIn->cz - 35.0f, 0, 0, 0, h_posIn->theta, -1);
				battle_flg[0] = InstanceCreate::CreateBattleIns(hero, encount, map_no, rnd);
				if (battle_flg[0] == TRUE)battle_flg[1] = TRUE;
				if (f == TRUE && battle_flg[1] == TRUE){
					battle_flg[1] = FALSE;
					battle_flg[2] = TRUE;
					Position::CamAdvance(NULL, NULL, 0);//初期化
				}
				battle_flg[0] = FALSE;
			}
			if (battle_flg[2] != FALSE)result = InstanceCreate::GetInstance_B()->Fight(hero, control.Direction(), result);
			if (result == WIN && battle_flg[2] != FALSE){
				InstanceCreate::BattleDelete();
				battle_flg[2] = FALSE;
				if (encount == BOSS)Map::SetBossKilled(map_no, 1);//ボス撃破履歴更新
				encount = NOENCOUNT;
			}
			if (result == DIE){
				TCHAR *str1 = L"ＧＡＭＥＯＶＥＲ";
				DxText::GetInstance()->UpDateText(&str1, 280.0f, 300.0f, 35.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
			}
		}

		if (endingflg == FALSE && Map::GetBossKilled(4) == 1){
			endingflg = TRUE;
			ending = new Ending();
		}
		if (endingflg == TRUE)ending->StaffRoll();

		if (mapstate == NORMAL_MAP && endingflg == FALSE && title == FALSE && encount == NOENCOUNT && menu == TRUE){
			menu = statemenu.Menudraw(InstanceCreate::GetInstance_M()->Getposition(),
				map_no, Map::GetBossKilled(), hero, control.Direction());
		}

		dx->End(HERO_COM);

		T_float::GetTime(hWnd);
		DxText::GetInstance()->EndDraw();
		dx->DrawScreen();
	}

	MovieSoundManager::ObjDelete();
	InstanceCreate::BattleDelete();
	InstanceCreate::MapDelete();
	ARR_DELETE(hero);
	S_DELETE(ending);
	DxText::DeleteInstance();
	Dx12Process::DeleteInstance();
	return (int)msg.wParam;
}