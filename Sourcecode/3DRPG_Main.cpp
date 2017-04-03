//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@    3DRPG_Main                                          **//
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
// �A�v���P�[�V�����̃G���g���|�C���g
// ����
//		hInstance     : ���݂̃C���X�^���X�̃n���h��
//		hPrevInstance : �ȑO�̃C���X�^���X�̃n���h��(win16�ł̕ʃC���X�^���X�m�F�pwin32�ł͏��NULL)
//		lpCmdLine	  : �R�}���h���C���p�����[�^
//		nCmdShow	  : �E�B���h�E�̕\�����
// �߂�l
//		����������0�ȊO�̒l
//-------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	HWND hWnd = NULL;//�E�B���h�E�n���h��
	MSG msg;        //���b�Z�[�W

	srand((unsigned)time(NULL));

	if (Createwindow(&hWnd, hInstance, nCmdShow) == -1)return -1;

	//Dx12Process�I�u�W�F�N�g����
	Dx12Process::InstanceCreate();
	//Dx11Process�I�u�W�F�N�g�擾
	Dx12Process *dx = Dx12Process::GetInstance();
	try {
		//DirectX������
		dx->Initialize(hWnd);
	}
	catch (char *E_mes) {
		ErrorMessage(E_mes);
		Dx12Process::DeleteInstance();
		return -1;
	}
	//DxText�I�u�W�F�N�g����
	DxText::InstanceCreate();
	//�v���C���[
	Hero *hero = NULL;
	//�X���b�h����
	InstanceCreate::CreateThread_R();
	double i = 300.0;
	bool down = TRUE;
	
	try {
		while (1) {
			if (!DispatchMSG(&msg)) {
				Dx12Process::DeleteInstance();
				return -1;	//�A�v���I��
			}

			if (hero == NULL)hero = InstanceCreate::Resource_load_f();
			else {
				InstanceCreate::DeleteThread_R();
				break;
			}

			dx->Sclear();
			DxText::GetInstance()->BiginDraw();
			DxText::GetInstance()->UpDateText(L"�m�����k������������", 215.0f, (float)i, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
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

	while (1) {//�A�v�����s�����[�v
		if (!DispatchMSG(&msg))break;

		dx->Sclear();
		DxText::GetInstance()->BiginDraw();

		if (title) {
			if (title_in) {
				title_in = statemenu.TitleMenu(control->Direction());
			}
			if (!title_in) {
				if (heroInput == NULL) {
					for (int i = 0; i < 4; i++)hero[i].P_DataInput(statemenu.SetP_Data(i));//�p�����[�^�[�C���v�b�g
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
					//���x���ɂ���ēG�o��������
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
					Position::CamAdvance(h, NULL, NULL, 0, NULL, encount);//������
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
					if (encount == BOSS)Map::SetBossKilled(map_no, 1);//�{�X���j�����X�V
					encount = NOENCOUNT;
				}
				break;
			case DIE:
				DxText::GetInstance()->UpDateText(L"�f�`�l�d�n�u�d�q", 280.0f, 300.0f, 35.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
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