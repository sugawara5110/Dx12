//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　    Mainクラス                                          **//
//**                                 Init()                                              **//
//*****************************************************************************************//

#include "Main.h"

void Main::DrawNowLoading() {
	static double i = 300.0;
	static bool down = TRUE;

	dx->Sclear();
	text->BiginDraw();
	text->UpDateText(L"ＮｏｗＬｏａｄｉｎｇ・・   ％完了", 185.0f, (float)i, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateValue(InstanceCreate::GetProgress(), 540.0f, (float)i, 30.0f, 3, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->EndDraw();

	dx->WaitFenceCurrent();
	dx->DrawScreen();

	if (down)i += 0.3;
	if (!down)i -= 0.3;
	if (i > 320.0)down = FALSE;
	if (i < 280.0)down = TRUE;
}

bool Main::Init(HINSTANCE hInstance, int nCmdShow) {

	srand((unsigned)time(NULL));

	if (Createwindow(&hWnd, hInstance, nCmdShow) == -1)return FALSE;

	//Dx12Processオブジェクト生成
	Dx12Process::InstanceCreate();
	//Dx11Processオブジェクト取得
	dx = Dx12Process::GetInstance();
	try {
		//DirectX初期化
		dx->Initialize(hWnd);
	}
	catch (char *E_mes) {
		ErrorMessage(E_mes);
		Dx12Process::DeleteInstance();
		return FALSE;
	}

	//DxTextオブジェクト生成
	DxText::InstanceCreate();
	text = DxText::GetInstance();

	//実行中スレッドナンバー
	int thNo = 0;

	try {
		while (1) {
			if (!DispatchMSG(&msg)) {
				DxText::DeleteInstance();
				Dx12Process::DeleteInstance();
				return TRUE;	//アプリ終了
			}

			bool fin = FALSE;
			bool drawF = TRUE;
			switch (thNo) {
			case 5:
				if (InstanceCreate::MapCreate_f()) {
					InstanceCreate::DeleteThread_M();
					fin = TRUE;
				}
				break;
			case 4:
				InstanceCreate::CreateThread_M();
				thNo = 5;
				break;
			case 3:
				if (hero = InstanceCreate::HeroCreate_f()) {
					InstanceCreate::DeleteThread_H();
					thNo = 4;
				}
				break;
			case 2:
				InstanceCreate::CreateThread_H();
				thNo = 3;
				break;
			case 1:
				if (InstanceCreate::Resource_load_f()) {
					InstanceCreate::DeleteThread_R();
					thNo = 2;
				}
				else drawF = FALSE;//このタイミングのみDrawNowLoading()実行しない(エラー出る為, 原因わからず)
				break;
			case 0:
				InstanceCreate::CreateThread_R();
				thNo = 1;
				break;
			}

			if (drawF)DrawNowLoading();

			if (fin) break;
		}
	}
	catch (char *E_mes) {
		ErrorMessage(E_mes);
		DxText::DeleteInstance();
		Dx12Process::DeleteInstance();
		return FALSE;
	}

	control = Control::GetInstance();
	statemenu = new StateMenu();

	return TRUE;
}

void Main::Loop() {

	while (1) {//アプリ実行中ループ
		if (!DispatchMSG(&msg))break;
		UpDate();
	}
}

void Main::UpDate() {

	dx->Sclear();
	text->BiginDraw();

	if (title) {
		if (title_in) {
			title_in = statemenu->TitleMenu(control->Direction());
		}
		if (!title_in) {
			if (heroInput == NULL) {
				for (int i = 0; i < 4; i++)hero[i].P_DataInput(statemenu->SetP_Data(i));//パラメーターインプット
				heroInput = TRUE;
				Map::SetMapNo(statemenu->SetMap_No());
				for (int i = 0; i < 5; i++)Map::SetBossKilled(i, statemenu->Set_boss_kil(i));
			}
			title = InstanceCreate::CreateMapIns(statemenu->SetH_Pos(), &hero[0], &map_no);
		}
	}

	dx->Bigin(HERO_COM);

	encount = InstanceCreate::GetInstance_M()->Mapdraw(&mapstate, control->Direction(TRUE), encount, menu, title, endingflg);

	if (!endingflg && !title && encount == NOENCOUNT && !menu && control->Direction() == ENTER)menu = TRUE;

	switch (mapstate) {
	case CHANGE_MAP:
		if (!InstanceCreate::CreateMapIns(NULL, &hero[0], &map_no)) { mapstate = NORMAL_MAP; }
		break;
	case RECOV_MAP:
		for (int i = 0; i < 4; i++) {
			hero[i].all_recover();
		}
		mapstate = NORMAL_MAP;
		break;
	case NORMAL_MAP:
		if (menu) {
			menu = statemenu->Menudraw(InstanceCreate::GetInstance_M()->Getposition(),
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
			if (!battle_flg[1])battle_flg[0] = InstanceCreate::CreateBattleIns(hero, encount, map_no, rnd);
			if (battle_flg[0]) {
				battle_flg[1] = TRUE;
			}
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

	dx->WaitFencePast();
	dx->DrawScreen();
}

Main::~Main() {
	Control::DeleteInstance();
	S_DELETE(statemenu);
	MovieSoundManager::ObjDelete();
	InstanceCreate::BattleDelete();
	InstanceCreate::MapDelete();
	ARR_DELETE(hero);
	S_DELETE(ending);
	DxText::DeleteInstance();
	Dx12Process::DeleteInstance();
}