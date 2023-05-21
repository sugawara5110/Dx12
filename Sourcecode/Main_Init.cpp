//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　    Mainクラス                                          **//
//**                                 Init()                                              **//
//*****************************************************************************************//

#include "Main.h"

Main *Main::main = NULL;

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

void UpDateThread() {
	Main::GetInstance()->UpDate();
}

void streamOutput() {
	Main::GetInstance()->StreamOutput();
}

void as() {
	Main::GetInstance()->AS();
}

void raytrace() {
	Main::GetInstance()->Raytrace();
}

void Main::CreateThreadUpdate() {
	th.setFunc(UpDateThread);
	th.setFunc(streamOutput);
	th.setFunc(as);
	th.setFunc(raytrace);
	th.start();
}

void Main::DeleteThreadUpdate() {
	th.end();
}

bool Main::Init(HINSTANCE hInstance, int nCmdShow) {

	srand((unsigned)time(NULL));

	if (Createwindow(&hWnd, hInstance, nCmdShow, CURRWIDTH, CURRHEIGHT, L"3DRPG") == -1)return FALSE;

	try {
		//DirectX初期化
		Dx_Device::InstanceCreate();
		Dx_Device::GetInstance()->createDevice();
		Dx_Device::GetInstance()->reportLiveDeviceObjectsOn();
		Dx_CommandManager::InstanceCreate();
		Dx_SwapChain::InstanceCreate();

		Dx_TextureHolder::InstanceCreate();
		Dx_TextureHolder* dx = Dx_TextureHolder::GetInstance();

		Dx_Device* dev = Dx_Device::GetInstance();
		dev->dxrCreateResource();
		Dx_SwapChain* sw = Dx_SwapChain::GetInstance();

		Dx_CommandManager::setNumResourceBarrier(1024);

		sw->Initialize(hWnd, CURRWIDTH, CURRHEIGHT);

		sw->setPerspectiveFov(55, 1, 10000);
		Dx_Light::Initialize();
		Dx_ShaderHolder::CreateShaderByteCode();
		Dx_Light::setGlobalAmbientLight(0.2f, 0.2f, 0.2f);

		float cuw = CURRWIDTH;
		float cuh = CURRHEIGHT;
		float rew = REFWIDTH;
		float reh = REFHEIGHT;
		PolygonData2D::SetMagnification(cuw / rew, cuh / reh);
	}
	catch (char* E_mes) {
		Dx_Util::ErrorMessage(E_mes);
		Dx_SwapChain::DeleteInstance();
		Dx_TextureHolder::DeleteInstance();
		Dx_CommandManager::DeleteInstance();
		Dx_Device::DeleteInstance();
		return FALSE;
	}

	//DxTextオブジェクト生成
	DxText::InstanceCreate();
	text = DxText::GetInstance();

	Dx_SwapChain* sw = Dx_SwapChain::GetInstance();
	Dx_CommandManager* cMa = Dx_CommandManager::GetInstance();
	Dx_CommandListObj* cObj = cMa->getGraphicsComListObj(0);

	//実行中スレッドナンバー
	int thNo = 0;

	try {
		while (1) {
			if (!DispatchMSG(&msg)) {
				DxText::DeleteInstance();
				Dx_SwapChain::DeleteInstance();
				Dx_TextureHolder::DeleteInstance();
				Dx_CommandManager::DeleteInstance();
				Dx_Device::DeleteInstance();
				return TRUE;	//アプリ終了
			}

			bool fin = FALSE;
			cObj->Bigin();
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

			sw->BiginDraw(0);
			DrawNowLoading(0);
			sw->EndDraw(0);
			cObj->End();
			cMa->RunGpu();
			cMa->WaitFence();
			sw->DrawScreen();

			if (fin) break;
		}
	}
	catch (char* E_mes) {
		Dx_Util::ErrorMessage(E_mes);
		DxText::DeleteInstance();
		Dx_SwapChain::DeleteInstance();
		Dx_TextureHolder::DeleteInstance();
		Dx_CommandManager::DeleteInstance();
		Dx_Device::DeleteInstance();
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

	cObj->Bigin();
	statemenu = new StateMenu();
	mosaic = new PostEffect();
	mosaic->ComCreateMosaic(0);
	blur = new PostEffect();
	blur->ComCreateBlur(0);
	blur2 = new PostEffect();
	blur2->ComCreateDepthOfField(0);
	cObj->End();
	cMa->RunGpu();
	cMa->WaitFence();
	changeMap();
	CreateThreadUpdate();

	return TRUE;
}

void Main::changeMap() {
	if (dxr)S_DELETE(dxr);
	dxr = new DxrRenderer();
	std::vector<ParameterDXR*> p;
	int mapN = 0;
	ParameterDXR** mDXR = InstanceCreate::GetInstance_M()->getParameterDXR(&mapN);
	int heN = 0;
	ParameterDXR** hDXR = hero[0].getParameterDXRMap(&heN);
	for (int i = 0; i < mapN; i++)p.push_back(mDXR[i]);
	for (int i = 0; i < heN; i++)p.push_back(hDXR[i]);

	for (int i = 0; i < mapN + heN; i++) {
		p[i]->resetCreateAS();
	}

	dxr->initDXR(p, 4);
}

void Main::changeBattle() {
	if (dxr)S_DELETE(dxr);
	dxr = new DxrRenderer();
	std::vector<ParameterDXR*> p;
	int mapN = 0;
	ParameterDXR** DxrM = InstanceCreate::GetInstance_M()->getParameterDXR(&mapN);

	ParameterDXR** DxrH[4];
	int heroN[4] = {};

	for (int i = 0; i < 4; i++) {
		DxrH[i] = hero[i].getParameterDXRBat(&heroN[i]);
	}

	int batN = 0;
	ParameterDXR** DxrB = InstanceCreate::GetInstance_B()->getParameterDXR(&batN);

	for (int i = 0; i < mapN; i++)p.push_back(DxrM[i]);


	int size = mapN;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < heroN[i]; j++)p.push_back(DxrH[i][j]);
		size += heroN[i];
	}

	for (int i = 0; i < batN; i++)p.push_back(DxrB[i]);
	size += batN;

	for (int i = 0; i < size; i++) {
		p[i]->updateDXR[0].createAS = false;
		p[i]->updateDXR[1].createAS = false;
	}

	dxr->initDXR(p, 4);
}

void Main::Loop() {

	statemenu->SetCommandList(0);
	for (int i = 0; i < 4; i++)hero[i].SetCommandList(0);

	Dx_CommandManager* cMa = Dx_CommandManager::GetInstance();
	Dx_CommandListObj* cObj = cMa->getGraphicsComListObj(0);

	while (1) {//アプリ実行中ループ
		if (!DispatchMSG(&msg)) {
			break;
		}

		T_float::GetTime(hWnd);

		cObj->Bigin();
		SetMovie(0);
		cObj->End();
		cMa->RunGpu();
		cMa->WaitFence();

		dxr->allSwapIndex();

		th.wait();

		cMa->RunGpuCom();
		cMa->RunGpu();
		cMa->WaitFenceCom();
		cMa->WaitFence();

		cObj->Bigin();
		dxr->copyBackBuffer(0);
		dxr->copyDepthBuffer(0);
		cObj->End();
		cMa->RunGpu();
		cMa->WaitFence();

		StreamOutputAfterDraw();
		if (changeBattleF) {
			changeBattle();
			changeBattleF = false;
		}
		ObjDel();
	}

	DeleteThreadUpdate();
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
			return;
		}
		break;
	}

	//タイトル表示
	if (Drawtitle)Drawtitle = statemenu->TitleMenu(control->Direction());

	if (InstanceCreate::GetInstance_M())
		encount = InstanceCreate::GetInstance_M()->MapUpdate(&mapstate, control->Direction(TRUE), encount, battleSwitch, menu, titleOn, endingflg);

	if (!endingflg && !titleOn && encount == NOENCOUNT && !menu && control->Direction() == ENTER) {
		menu = TRUE;
		return;
	}
	T_float tfloat;

	switch (mapstate) {
	case CHANGE_MAP:
		if (!InstanceCreate::CreateMapIns(NULL, &hero[0], &map_no)) {
			mapstate = NORMAL_MAP; mpDel_f = TRUE;
			return;
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

	Dx_SwapChain* sw = Dx_SwapChain::GetInstance();

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
			sw->Cameraset({ h_posOut.cx, h_posOut.cy, h_posOut.cz },
				{ h_posOut.cx2, h_posOut.cy2, h_posOut.cz });
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
				return;
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

void Main::SetMovie(int com_no) {
	if (InstanceCreate::GetInstance_M())InstanceCreate::GetInstance_M()->SetMovie(com_no);
	hero[0].SetMovie(com_no, encount);
}

void Main::Draw() {

	Dx_SwapChain* sw = Dx_SwapChain::GetInstance();
	Dx_CommandManager* cMa = Dx_CommandManager::GetInstance();
	Dx_CommandListObj* cObj = cMa->getGraphicsComListObj(3);

	cObj->Bigin();
	sw->BiginDraw(3);
	if (battleSwitch == 2) {
		InstanceCreate::GetInstance_B()->FightDraw(3, encount);
	}
	for (int i = 0; i < 4; i++) {
		hero[i].Draw(3, battleSwitch, ending);
	}

	InstanceCreate::GetInstance_M()->MapDraw(3);

	sw->EndDraw(3);
	cObj->End();
}

void Main::StreamOutput() {

	Dx_CommandManager* cMa = Dx_CommandManager::GetInstance();
	Dx_CommandListObj* cObj = cMa->getGraphicsComListObj(3);

	cObj->Bigin();
	if (battleSwitch == 2) {
		InstanceCreate::GetInstance_B()->StreamOutput(3, encount);
	}
	for (int i = 0; i < 4; i++) {
		hero[i].StreamOutput(3, battleSwitch, ending);
	}
	if (InstanceCreate::GetInstance_M()) {
		InstanceCreate::GetInstance_M()->StreamOutput(3);
	}
	cObj->End();
}

void Main::AS() {

	Dx_CommandManager* cMa = Dx_CommandManager::GetInstance();
	Dx_CommandListObj* cObj = cMa->getGraphicsComListObj(4);

	cObj->Bigin();
	dxr->update_g(4, 3);
	cObj->End();
}

void Main::Raytrace() {

	Dx_CommandManager* cMa = Dx_CommandManager::GetInstance();
	Dx_CommandListObj* cObj = cMa->getComputeComListObj(2);

	cObj->Bigin();
	dxr->raytrace_c(2);
	cObj->End();
}

void Main::StreamOutputAfterDraw() {

	Dx_SwapChain* sw = Dx_SwapChain::GetInstance();
	Dx_CommandManager* cMa = Dx_CommandManager::GetInstance();
	Dx_CommandListObj* cObj = cMa->getGraphicsComListObj(0);

	cObj->Bigin();
	sw->BiginDraw(0, false);

	float blu = 0.0f;
	bool bluRet = FALSE;
	if (battleSwitch == 2) {
		bluRet = InstanceCreate::GetInstance_B()->GetBossEffectState(&blu);
		InstanceCreate::GetInstance_B()->StreamOutputAfterDraw(encount);
	}
	if (InstanceCreate::GetInstance_M())
		InstanceCreate::GetInstance_M()->StreamOutputAfterDraw();
	int cnt = 0;
	mosaic->ComputeMosaic(0, InstanceCreate::GetInstance_M()->GetMenuState(&cnt), cnt);
	blur->ComputeBlur(0, bluRet, 400.0f, 300.0f, blu);
	bluRet = FALSE;
	if (battleSwitch == 2)
		blur2->ComputeDepthOfField(0, true, 600, 0.98f, 0.6f);
	else
		blur2->ComputeDepthOfField(0, true, 600, 0.98f, 0.05f);
	blu = 0.0f;
	for (int i = 0; i < 4; i++) {
		hero[i].Draw2D(encount, ending);
	}

	if (battleSwitch == 2)InstanceCreate::GetInstance_B()->Draw2D(encount);
	statemenu->Draw();
	DxText::GetInstance()->Draw(0);
	sw->EndDraw(0);
	cObj->End();
	cMa->RunGpu();
	cMa->WaitFence();
	sw->DrawScreen();
}

void Main::ObjDel() {
	//battle削除(コマンドリストClose後に削除)
	if (btDel_f) {
		InstanceCreate::BattleDelete();
		changeMap();
		btDel_f = FALSE;
	}
	//map削除
	if (mpDel_f) {
		InstanceCreate::InsDelete();
		changeMap();
		mpDel_f = FALSE;
	}
}

Main::~Main() {

	Dx_CommandManager* cMa = Dx_CommandManager::GetInstance();

	cMa->WaitFenceCom();
	cMa->WaitFence();
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
	S_DELETE(blur2);
	S_DELETE(dxr);
	DxText::DeleteInstance();
	Dx_SwapChain::DeleteInstance();
	Dx_TextureHolder::DeleteInstance();
	Dx_CommandManager::DeleteInstance();
	Dx_Device::DeleteInstance();
}

