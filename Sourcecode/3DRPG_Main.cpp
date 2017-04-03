//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　    3DRPG_Main                                          **//
//**                                                                                     **//
//*****************************************************************************************//

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

	HWND hWnd = NULL;//ウィンドウハンドル
	MSG msg;        //メッセージ

	srand((unsigned)time(NULL));

	if (Createwindow(&hWnd, hInstance, nCmdShow) == -1)return -1;

	//Dx12Processオブジェクト生成
	Dx12Process::InstanceCreate();
	//Dx11Processオブジェクト取得
	Dx12Process *dx = Dx12Process::GetInstance();
	try {
		//DirectX初期化
		dx->Initialize(hWnd);
	}
	catch (char *E_mes) {
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
	
	try {
		while (1) {
			if (!DispatchMSG(&msg)) {
				Dx12Process::DeleteInstance();
				return -1;	//アプリ終了
			}

			if (hero == NULL)hero = InstanceCreate::Resource_load_f();
			else {
				InstanceCreate::DeleteThread_R();
				break;
			}

			dx->Sclear();
			DxText::GetInstance()->BiginDraw();
			DxText::GetInstance()->UpDateText(L"ＮｏｗＬｏａｄｉｎｇ", 215.0f, (float)i, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
			DxText::GetInstance()->EndDraw();
			dx->DrawScreen();
			if (down)i += 0.01;
			if (!down)i -= 0.01;
			if (i > 320.0)down = FALSE;
			if (i < 280.0)down = TRUE;
		}
	}
	catch (char *E_mes) {
		ErrorMessage(E_mes);
		DxText::DeleteInstance();
		Dx12Process::DeleteInstance();
		return -1;
	}

	bool battle_flg[3] = { FALSE, FALSE, FALSE };
	Position::H_Pos *h_posIn = NULL;
	Position::H_Pos h_posOut, h_posOut2;
	Position::mapxy *mxy = NULL;
	bool posget[2] = { FALSE, FALSE };

	Control *control = Control::GetInstance();
	int map_no = 0;
	
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

	while (1) {//アプリ実行中ループ
		if (!DispatchMSG(&msg))break;

		dx->Sclear();
		DxText::GetInstance()->BiginDraw();

		if (title) {
			if (title_in) {
				title_in = statemenu.TitleMenu(control->Direction());
			}
			if (!title_in) {
				if (heroInput == NULL) {
					for (int i = 0; i < 4; i++)hero[i].P_DataInput(statemenu.SetP_Data(i));//パラメーターインプット
					heroInput = TRUE;
					Map::SetMapNo(statemenu.SetMap_No());
					for (int i = 0; i < 5; i++)Map::SetBossKilled(i, statemenu.Set_boss_kil(i));
				}
				title = InstanceCreate::CreateMapIns(statemenu.SetH_Pos(), &hero[0], &map_no);
			}
		}

		dx->Bigin(HERO_COM);

		encount = InstanceCreate::GetInstance_M()->Mapdraw(&mapstate, control->Direction(TRUE), encount, menu, title, endingflg);

		if (!endingflg && !title && encount == NOENCOUNT && !menu && control->Direction() == ENTER)menu = TRUE;

		switch (mapstate) {
		case CHANGE_MAP:
			if (!InstanceCreate::CreateMapIns(NULL, &hero[0], &map_no))mapstate = NORMAL_MAP;
			break;
		case RECOV_MAP:
			for (int i = 0; i < 4; i++) {
				hero[i].all_recover();
			}
			mapstate = NORMAL_MAP;
			break;
		case NORMAL_MAP:
			if (menu) {
				menu = statemenu.Menudraw(InstanceCreate::GetInstance_M()->Getposition(),
					map_no, Map::GetBossKilled(), hero, control->Direction());
			}
			break;
		}

		if (!endingflg && !title && encount != NOENCOUNT && !menu) {
			if (!battle_flg[2]) {
				if (encount == SIDE) {
					int LV = (hero[0].s_LV() + hero[1].s_LV() + hero[2].s_LV() + hero[3].s_LV()) / 4;
					//レベルによって敵出現数制限
					rnd = 0;
					if ((map_no + 1) * 3 < LV)rnd = rand() % 2;
					if ((map_no + 1) * 5 < LV)rnd = rand() % 3;
					if ((map_no + 1) * 7 < LV)rnd = rand() % 4;
				}
				else rnd = 0;

				if (!posget[0]) {
					h_posIn = InstanceCreate::GetInstance_M()->Getposition();
					mxy = InstanceCreate::GetInstance_M()->Getmap();
					posget[0] = TRUE;
				}

				bool f = Position::CamAdvance(h_posIn, &h_posOut, &h_posOut2, tfloat.Add(0.2f), mxy, encount);
				if (!posget[1]) {
					InstanceCreate::GetInstance_M()->Setposition(&h_posOut2);
					posget[1] = TRUE;
				}

				dx->Cameraset(h_posOut.cx, h_posOut.cx2, h_posOut.cy, h_posOut.cy2, h_posOut.cz, h_posOut.cz);
				hero[0].OBJWalkDraw(h_posIn->cx1, h_posIn->cy1, h_posIn->cz - 35.0f, 0, 0, 0, h_posIn->theta, FALSE);
				battle_flg[0] = InstanceCreate::CreateBattleIns(hero, encount, map_no, rnd);
				if (battle_flg[0])battle_flg[1] = TRUE;
				if (f && battle_flg[1]) {
					battle_flg[1] = FALSE;
					battle_flg[2] = TRUE;
					Position::H_Pos *h = NULL;
					Position::CamAdvance(h, NULL, NULL, 0, NULL, encount);//初期化
					posget[0] = FALSE;
					posget[1] = FALSE;
				}
				battle_flg[0] = FALSE;
			}

			if (battle_flg[2])result = InstanceCreate::GetInstance_B()->Fight(hero, control->Direction(), result);
			switch (result) {
			case WIN:
				if (battle_flg[2]) {
					InstanceCreate::BattleDelete();
					battle_flg[2] = FALSE;
					if (encount == BOSS)Map::SetBossKilled(map_no, 1);//ボス撃破履歴更新
					encount = NOENCOUNT;
				}
				break;
			case DIE:
				DxText::GetInstance()->UpDateText(L"ＧＡＭＥＯＶＥＲ", 280.0f, 300.0f, 35.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
				break;
			}

		}

		switch (endingflg) {
		case FALSE:
			if (Map::GetBossKilled(4) == 1) {
				endingflg = TRUE;
				ending = new Ending();
			}
			break;
		case TRUE:
			ending->StaffRoll();
			break;
		}

		dx->End(HERO_COM);

		T_float::GetTime(hWnd);
		DxText::GetInstance()->EndDraw();
		dx->DrawScreen();
	}

	Control::DeleteInstance();
	MovieSoundManager::ObjDelete();
	InstanceCreate::BattleDelete();
	InstanceCreate::MapDelete();
	ARR_DELETE(hero);
	S_DELETE(ending);
	DxText::DeleteInstance();
	Dx12Process::DeleteInstance();
	return (int)msg.wParam;
}