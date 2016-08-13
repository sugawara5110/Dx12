//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　    Dx12TestMain                                        **//
//**                                                                                     **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Dx12Process.h"

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
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

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
		800,             //ウインドウ幅
		600,            //ウインドウ高さ
		NULL,          //親ウインドウハンドル
		NULL,         //メニュー,子ウインドウハンドル
		hInstance,   //アプリケーションインスタンスハンドル
		NULL)))     //ウインドウ作成データ
		return -1;

	// ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);
	// WM_PAINTが呼ばれないようにする
	ValidateRect(hWnd, 0);

	//Directx初期化
	Dx12Process::InstanceCreate();
	Dx12Process *dx = Dx12Process::GetInstance();

	if (!dx->Initialize(hWnd)) return -1;

	PolygonData a[1];
	for (int i = 0; i < 1; i++) {
		a[i].GetVBarray(CONTROL_POINT, true, 1);
		//左上
		a[i].SetVertex(0, 0,
			-10.0f, -10.0f, -10.0f,
			0.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			0.0f, 0.0f);

		//左下
		a[i].SetVertex(1, 1,
			-10.0f, 10.0f, -10.0f,
			0.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			0.0f, 1.0f);

		//右下
		a[i].SetVertex(2, 2,
			10.0f, 10.0f, -10.0f,
			0.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f);

		//右上
		a[i].SetVertex(3, 3,
			10.0f, -10.0f, -10.0f,
			0.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f);
		a[i].Create();
	}
	float theta = 0.0f;
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

		//MainLoop
		dx->Sclear();
		dx->Cameraset(0.0f, 0.0f, 0.5f, 0.0f, 40.0f, 0.0f);
		dx->P_ShadowBright(0.3f);
		dx->PointLightPosSet(0, 0, 40, 30, 1, 1, 1, 1, 100,
			8.0f, 2, true);//0:視点, 1:ラスボス, 2:出入り口, 3456:戦闘
		for (int i = 0; i < 1; i++)
			a[i].Draw(i * 10, i * 10, i * 10, 0, 0, 0, (int)(theta+=0.1f)%360, 1.0f);
		dx->Drawscreen();
		T_float::GetTime(hWnd);
	}

	Dx12Process::DeleteInstance();
	return (int)msg.wParam;
}
