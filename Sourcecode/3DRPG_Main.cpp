//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　    3DRPG_Main                                          **//
//**                                                                                     **//
//*****************************************************************************************//

#include "Main.h"

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

	Main main;
	main.Init(hInstance, nCmdShow);
	main.Loop();

	return (int)main.msg.wParam;
}