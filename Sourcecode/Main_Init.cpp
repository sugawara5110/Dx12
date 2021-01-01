//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　    Mainクラス                                          **//
//**                                 Init()                                              **//
//*****************************************************************************************//

#include "Main.h"

Main *Main::main = NULL;
HANDLE event[2] = {};

Main *Main::GetInstance() {

	if (main != NULL)return main;
	main = new Main();
	return main;
}

void Main::DeleteInstance() {

	if (main != NULL) {
		delete main;
		main = NULL;
	}
}

void Main::DrawNowLoading(int com_no) {
	static double i = 300.0;
	static bool down = TRUE;

	text->UpDateText(L"ＮｏｗＬｏａｄｉｎｇ・・   ％完了", 185.0f, (float)i, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateValue(InstanceCreate::GetProgress(), 540.0f, (float)i, 30.0f, 3, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDate();
	text->Draw(com_no);

	if (down)i += 0.1;
	if (!down)i -= 0.1;
	if (i > 320.0)down = FALSE;
	if (i < 280.0)down = TRUE;
}

void Main::CreateThreadUpdate() {
	update_h = (HANDLE*)_beginthreadex(NULL, 0, UpDateThread, NULL, 0, NULL);
}

void Main::DeleteThreadUpdate() {
	UpDateThreadLoop = FALSE;
	SetEvent(event[0]);
	WaitForSingleObject(event[1], INFINITE);
	CloseHandle(update_h);                 
	update_h = NULL;
}

bool Main::Init(HINSTANCE hInstance, int nCmdShow) {

	srand((unsigned)time(NULL));

	if (Createwindow(&hWnd, hInstance, nCmdShow, CURRWIDTH, CURRHEIGHT, L"3DRPG", 10) == -1)return FALSE;

	//Dx12Processオブジェクト生成
	Dx12Process::InstanceCreate();
	//Dx11Processオブジェクト取得
	dx = Dx12Process::GetInstance();
	try {
		//DirectX初期化
		dx->dxrCreateResource();
		dx->Initialize(hWnd, CURRWIDTH, CURRHEIGHT);
		float cuw = CURRWIDTH;
		float cuh = CURRHEIGHT;
		float rew = REFWIDTH;
		float reh = REFHEIGHT;
		PolygonData2D::SetMagnification(cuw / rew, cuh / reh);
	}
	catch (char* E_mes) {
		ErrorMessage(E_mes);
		DxText::DeleteInstance();
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
			dx->Bigin(0);
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
				TextureBinaryLoader::DeleteTextureBinary();
				InstanceCreate::CreateThread_H();
				thNo = 3;
				break;
			case 1:
				if (InstanceCreate::Resource_load_f()) {
					InstanceCreate::DeleteThread_R();
					thNo = 2;
				}
				break;
			case 0:
				InstanceCreate::CreateThread_R();
				thNo = 1;
				break;
			}

			dx->BiginDraw(0);
			DrawNowLoading(0);
			dx->EndDraw(0);
			dx->End(0);
			dx->RunGpu();
			dx->WaitFence();
			dx->DrawScreen();

			if (fin) break;
		}
	}
	catch (char* E_mes) {
		ErrorMessage(E_mes);
		DxText::DeleteInstance();
		Dx12Process::DeleteInstance();
		return FALSE;
	}

	control = Control::GetInstance();
	DxInput* di = DxInput::GetInstance();
	di->create(hWnd);
	di->SetWindowMode(true);

	DivideArr divArr[1];
	divArr[0].distance = 7000.0f;
	divArr[0].divide = 12;
	UINT numDiv = 1;

	dx->Bigin(0);
	statemenu = new StateMenu();
	mosaic = new PostEffect();
	mosaic->ComCreateMosaic();
	blur = new PostEffect();
	blur->ComCreateBlur();

	dx->End(0);
	dx->RunGpu();
	dx->WaitFence();
	changeMap();
	CreateThreadUpdate();

	return TRUE;
}

void Main::changeMap() {
	if (dxr)S_DELETE(dxr);
	EmissiveCount::reset();
	dxr = new DXR_Basic();
	int mapN = 0;
	int mapTN = 0;
	MaterialType* mtype = InstanceCreate::GetInstance_M()->getMaterialType(&mapTN);
	ParameterDXR** mDXR = InstanceCreate::GetInstance_M()->getParameterDXR(&mapN);
	InstanceCreate::GetInstance_M()->setPointLightNo();
	int heN = 0;
	ParameterDXR** hDXR = hero[0].getParameterDXRMap(&heN);
	MaterialType* htype = hero[0].getMaterialTypeMap();
	hero[0].setPointLightNoMap();
	memcpy(&pdx[0], mDXR, sizeof(ParameterDXR*) * mapN);
	memcpy(&pdx[mapN], hDXR, sizeof(ParameterDXR*) * heN);
	memcpy(&materialType[0], mtype, sizeof(MaterialType) * mapTN);
	memcpy(&materialType[mapTN], htype, sizeof(MaterialType) * heN);

	for (int i = 0; i < mapN + heN; i++) {
		pdx[i]->resetCreateAS();
	}

	dxr->initDXR(mapN + heN, pdx, materialType, 4);
}

void Main::changeBattle() {
	if (dxr)S_DELETE(dxr);
	EmissiveCount::reset();
	dxr = new DXR_Basic();
	int mapN = 0;
	int mapTN = 0;
	MaterialType* typeM = InstanceCreate::GetInstance_M()->getMaterialType(&mapTN);
	ParameterDXR** DxrM = InstanceCreate::GetInstance_M()->getParameterDXR(&mapN);
	InstanceCreate::GetInstance_M()->setPointLightNo();

	MaterialType* typeH[4];
	ParameterDXR** DxrH[4];
	int heroN[4] = {};

	for (int i = 0; i < 4; i++) {
		DxrH[i] = hero[i].getParameterDXRBat(&heroN[i]);
		typeH[i] = hero[i].getMaterialTypeBat();
		hero[i].setPointLightNoBat();
	}

	int batN = 0;
	MaterialType* typeB = InstanceCreate::GetInstance_B()->getMaterialType();
	ParameterDXR** DxrB = InstanceCreate::GetInstance_B()->getParameterDXR(&batN);
	InstanceCreate::GetInstance_B()->setPointLightNo();

	memcpy(&pdx[0], DxrM, sizeof(ParameterDXR*) * mapN);
	memcpy(&materialType[0], typeM, sizeof(MaterialType) * mapTN);
	int size = mapN;
	int size2 = mapTN;
	for (int i = 0; i < 4; i++) {
		memcpy(&pdx[size], DxrH[i], sizeof(ParameterDXR*) * heroN[i]);
		memcpy(&materialType[size2], typeH[i], sizeof(MaterialType) * heroN[i]);
		size += heroN[i];
		size2 += heroN[i];
	}

	memcpy(&pdx[size], DxrB, sizeof(ParameterDXR*) * batN);
	memcpy(&materialType[size2], typeB, sizeof(MaterialType) * batN);
	size += batN;

	for (int i = 0; i < size; i++) {
		pdx[i]->updateDXR[0].createAS = false;
		pdx[i]->updateDXR[1].createAS = false;
	}

	dxr->initDXR(size, pdx, materialType, 4);
}

void Main::Loop() {

	for (int i = 0; i < 2; i++)
		event[i] = CreateEvent(NULL, FALSE, FALSE, NULL);

	statemenu->SetCommandList(0);
	for (int i = 0; i < 4; i++)hero[i].SetCommandList(0);
	while (1) {//アプリ実行中ループ
		if (!DispatchMSG(&msg)) {
			break;
		}

		T_float::GetTime(hWnd);

		Sync::sync[0] = 1 - Sync::sync[0];
		Sync::sync[1] = 1 - Sync::sync[1];
		Sync::sync[2] = 1 - Sync::sync[2];
		dx->setUpSwapIndex(Sync::sync[0]);
		dx->setDrawSwapIndex(1 - Sync::sync[0]);
		dx->setStreamOutputSwapIndex(Sync::sync[1]);
		dx->setRaytraceSwapIndex(1 - Sync::sync[1]);
		dxr->setASswapIndex(Sync::sync[2]);
		dxr->setRaytraceSwapIndex(1 - Sync::sync[2]);

		SetEvent(event[0]);
		dx->Bigin(0);
		SetMovie();
		dx->End(0);
		dx->RunGpu();
		dx->WaitFence();
		//Draw();
		StreamOutput();
		StreamOutputAfterDraw();
		WaitForSingleObject(event[1], INFINITE);
		if (changeBattleF) {
			changeBattle();
			changeBattleF = false;
		}
		ObjDel();
	}

	DeleteThreadUpdate();
	for (int i = 0; i < 2; i++)
		CloseHandle(event[i]);
}

void Main::UpDate() {

	T_float::GetTimeUp(hWnd);
	T_float::AddAdjust(0.8f);

	static bool Drawtitle = TRUE;
	static bool titleOn = TRUE;
	static int titleSwitch = 0;
	switch (titleSwitch) {
	case 0:
		if (!Drawtitle)titleSwitch = 1;
		break;
	case 1:
		for (int i = 0; i < 4; i++)hero[i].P_DataInput(statemenu->SetP_Data(i));//パラメーターインプット
		Map::SetMapNo(statemenu->SetMap_No());
		for (int i = 0; i < 5; i++)Map::SetBossKilled(i, statemenu->Set_boss_kil(i));
		Map::SetMapHistory(statemenu->GetMapHistory());
		titleSwitch = 2;
		break;
	case 2:
		titleOn = InstanceCreate::CreateMapIns(statemenu->SetH_Pos(), &hero[0], &map_no);
		if (!titleOn) {
			mapstate = NORMAL_MAP; titleSwitch = 3; mpDel_f = TRUE;
			SetEvent(event[1]);
			WaitForSingleObject(event[0], INFINITE);
			while (mpDel_f);
		}
		break;
	}

	//タイトル表示
	if (Drawtitle)Drawtitle = statemenu->TitleMenu(control->Direction());

	if (InstanceCreate::GetInstance_M())
		encount = InstanceCreate::GetInstance_M()->MapUpdate(&mapstate, control->Direction(TRUE), encount, menu, titleOn, endingflg);

	if (!endingflg && !titleOn && encount == NOENCOUNT && !menu && control->Direction() == ENTER)menu = TRUE;
	T_float tfloat;

	switch (mapstate) {
	case CHANGE_MAP:
		if (!InstanceCreate::CreateMapIns(NULL, &hero[0], &map_no)) {
			mapstate = NORMAL_MAP; mpDel_f = TRUE;
			SetEvent(event[1]);
			WaitForSingleObject(event[0], INFINITE);
			while (mpDel_f);
		}
		break;
	case RECOV_MAP:
		for (int i = 0; i < 4; i++) {
			hero[i].all_recover();
		}
		mapstate = NORMAL_MAP;
		break;
	case NORMAL_MAP:
		if (menu) {
			menu = statemenu->MenuUpdate(InstanceCreate::GetInstance_M()->Getposition(),
				map_no, Map::GetBossKilled(), Map::GetMapHistory(), hero, control->Direction());
		}
		break;
	}

	static bool posget = FALSE;
	static bool btLoad[2] = { FALSE };
	static Position::H_Pos* h_posIn = NULL;
	static Position::H_Pos h_posOut, h_posOut2;
	static Position::mapxy* mxy = NULL;
	static int rnd = 0;
	if (encount != NOENCOUNT) {
		switch (battleSwitch) {
		case 0:
			if (encount == SIDE) {
				int LV = (hero[0].s_LV() + hero[1].s_LV() + hero[2].s_LV() + hero[3].s_LV()) / 4;
				//レベルによって敵出現数制限
				rnd = 0;
				if ((map_no + 1) * 3 < LV)rnd = rand() % 2;
				if ((map_no + 1) * 5 < LV)rnd = rand() % 3;
				if ((map_no + 1) * 7 < LV)rnd = rand() % 4;
			}
			else rnd = 0;
			//現ポジション取得
			h_posIn = InstanceCreate::GetInstance_M()->Getposition();
			mxy = InstanceCreate::GetInstance_M()->Getmap();
			battleSwitch = 1;
			break;
		case 1:
			//視点切り替え, 完了時TRUE
			btLoad[0] = Position::CamAdvance(h_posIn, &h_posOut, &h_posOut2, tfloat.Add(0.2f), mxy, encount);
			if (!posget) {
				//戦闘後ポジション取得, セット
				InstanceCreate::GetInstance_M()->Setposition(&h_posOut2);
				posget = TRUE;
			}
			//視点切り替え
			dx->Cameraset({ h_posOut.cx, h_posOut.cy, h_posOut.cz },
				{ h_posOut.cx2, h_posOut.cy2, h_posOut.cz });
			//視点切り替え中obj
			hero[0].OBJWalkUpdate(h_posIn->cx1, h_posIn->cy1, h_posIn->cz - 35.0f, 0, 0, 0, h_posIn->theta, FALSE);
			//battleInstance生成, 完了時TRUE
			if (!btLoad[1])btLoad[1] = InstanceCreate::CreateBattleIns(hero, encount, map_no, rnd);
			//視点切り替え,battleInstance生成終了
			if (btLoad[0] && btLoad[1]) {
				btLoad[0] = FALSE;
				btLoad[1] = FALSE;
				Position::H_Pos* h = NULL;//↓の初期化用
				Position::CamAdvance(h, NULL, NULL, 0, NULL, encount);//初期化
				posget = FALSE;
				battleSwitch = 2;
				changeBattleF = true;
			}
			break;
		case 2:
			//battle表示
			result = InstanceCreate::GetInstance_B()->FightUpdate(hero, control->Direction(), result);
			switch (result) {
			case WIN:
				if (encount == BOSS)Map::SetBossKilled(map_no, 1);//ボス撃破履歴更新
				encount = NOENCOUNT;
				result = IN_BATTLE;
				battleSwitch = 0;
				btDel_f = TRUE;
				SetEvent(event[1]);
				WaitForSingleObject(event[0], INFINITE);
				while (btDel_f);
				break;
			case DIE:
				DxText::GetInstance()->UpDateText(L"ＧＡＭＥＯＶＥＲ", 280.0f, 300.0f, 35.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
				break;
			}
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

	DxText::GetInstance()->UpDate();
}

void Main::SetMovie() {
	if(InstanceCreate::GetInstance_M())InstanceCreate::GetInstance_M()->SetMovie();
	hero[0].SetMovie(encount);
}

void Main::Draw() {
	dx->Bigin(0);
	dx->BiginDraw(0);
	float blu = 0.0f;
	bool bluRet = FALSE;
	if (battleSwitch == 2) {
		InstanceCreate::GetInstance_B()->FightDraw(encount);
		bluRet = InstanceCreate::GetInstance_B()->GetBossEffectState(&blu);
	}
	for (int i = 0; i < 4; i++) {
		hero[i].Draw(encount, ending);
	}
	
	InstanceCreate::GetInstance_M()->MapDraw();
	int cnt = 0;
	mosaic->ComputeMosaic(InstanceCreate::GetInstance_M()->GetMenuState(&cnt), cnt);
	blur->ComputeBlur(bluRet, 400.0f, 300.0f, blu);
	bluRet = FALSE;
	blu = 0.0f;
	for (int i = 0; i < 4; i++) {
		hero[i].Draw2D(encount, ending);
	}
	if (battleSwitch == 2)InstanceCreate::GetInstance_B()->Draw2D(encount);
	statemenu->Draw();
	DxText::GetInstance()->Draw(0);
	dx->EndDraw(0);
	dx->End(0);
	dx->RunGpu();
	dx->WaitFence();
	dx->DrawScreen();
}

void Main::StreamOutput() {
	dx->Bigin(0);
	if (battleSwitch == 2) {
		InstanceCreate::GetInstance_B()->StreamOutput(encount);
	}
	for (int i = 0; i < 4; i++) {
		hero[i].StreamOutput(encount, ending);
	}
	if (InstanceCreate::GetInstance_M()) {
		InstanceCreate::GetInstance_M()->StreamOutput();
	}
	
	dx->End(0);
	dx->RunGpu();
	dx->WaitFence();

	dx->BiginCom(0);
	dxr->update_c(0, 3);
	dx->EndCom(0);
	dx->Bigin(0);
	dxr->updateVertexBuffer(0);
	dxr->raytrace_g(0);
	dxr->copyBackBuffer(0);
	dxr->copyDepthBuffer(0);
	dx->End(0);
	dx->RunGpuCom();
	dx->RunGpu();
	dx->WaitFenceCom();
	dx->WaitFence();
}

void Main::StreamOutputAfterDraw() {
	dx->Bigin(0);
	dx->BiginDraw(0, false);

	float blu = 0.0f;
	bool bluRet = FALSE;
	if (battleSwitch == 2) {
		bluRet = InstanceCreate::GetInstance_B()->GetBossEffectState(&blu);
		InstanceCreate::GetInstance_B()->StreamOutputAfterDraw(encount);
	}
	int cnt = 0;
	mosaic->ComputeMosaic(InstanceCreate::GetInstance_M()->GetMenuState(&cnt), cnt);
	blur->ComputeBlur(bluRet, 400.0f, 300.0f, blu);
	bluRet = FALSE;
	blu = 0.0f;
	for (int i = 0; i < 4; i++) {
		hero[i].Draw2D(encount, ending);
	}
	if (InstanceCreate::GetInstance_M())
		InstanceCreate::GetInstance_M()->StreamOutputAfterDraw();
	if (battleSwitch == 2)InstanceCreate::GetInstance_B()->Draw2D(encount);
	statemenu->Draw();
	DxText::GetInstance()->Draw(0);
	dx->EndDraw(0);
	dx->End(0);
	dx->RunGpu();
	dx->WaitFence();
	dx->DrawScreen();
}

void Main::ObjDel() {
	//battle削除(コマンドリストClose後に削除)
	if (btDel_f) {
		dx->RunGpu();
		dx->WaitFence();
		InstanceCreate::BattleDelete();
		changeMap();
		btDel_f = FALSE;
	}
	//map削除
	if (mpDel_f) {
		dx->RunGpu();
		dx->WaitFence();
		InstanceCreate::InsDelete();
		changeMap();
		mpDel_f = FALSE;
	}
}

Main::~Main() {
	dx->WaitFenceCom();
	dx->WaitFence();
	Control::DeleteInstance();
	S_DELETE(statemenu);
	MovieSoundManager::ObjDelete();
	InstanceCreate::BattleDelete();
	InstanceCreate::MapDelete();
	ARR_DELETE(hero);
	S_DELETE(ending);
	TextureBinaryLoader::DeleteTextureStruct();
	S_DELETE(mosaic);
	S_DELETE(blur);
	S_DELETE(dxr);
	DxText::DeleteInstance();
	Dx12Process::DeleteInstance();
}

unsigned __stdcall UpDateThread(void*) {
	while (Main::GetInstance()->UpDateThreadLoop) {
		WaitForSingleObject(event[0], INFINITE);
		Main::GetInstance()->UpDate();
		SetEvent(event[1]);
	}
	return 0;
}