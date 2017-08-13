//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Battleクラス                                    **//
//**                                    Menu_main関数                                    **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "./Direct3DWrapper/DxText.h"
#include <new>     //placement new
#include <stdlib.h>
#include <time.h>
#include "Enemy.h"
#include "EnemySide.h"
#include "EnemyBoss.h"
#include "Battle.h"
#include "Hero.h"

CommandSelect Battle::Menu_main(Hero *hero, int i, Directionkey direction) {

	float r[5];
	float g[5];
	r[0] = r[1] = r[2] = r[3] = r[4] = 1.0f;
	g[0] = g[1] = g[2] = g[3] = g[4] = 1.0f;
	command.Update(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

	switch (h_draw[i].M_select) {
	case 0:
		r[0] = 0.0f;
		g[0] = 0.7f;
		break;
	case 1:
		r[1] = 0.0f;
		g[1] = 0.7f;
		h_draw[i].Magrun = FLAME;
		break;
	case 2:
		r[2] = 0.0f;
		g[2] = 0.7f;
		h_draw[i].Magrun = HEAL;
		break;
	case 3:
		r[3] = 0.0f;
		g[3] = 0.7f;
		h_draw[i].Magrun = RECOV;
		break;
	case 4:
		r[4] = 0.0f;
		g[4] = 0.7f;
		break;
	}

	switch (direction) {
	case LEFT:

		break;
	case RIGHT:
		MovieSoundManager::Select_sound(FALSE);
		SelectPermissionMove(hero);
		return OTHER;//選択権移動後即ループ抜けしないとそのまま選択権移動先の処理になるので切り替わらない
		break;
	case UP:
		MovieSoundManager::Select_sound(FALSE);
		if (h_draw[i].M_select == 0)break;
		h_draw[i].M_select--;
		if (hero[i].GetRecover_LV() <= 0 && h_draw[i].M_select == 3)h_draw[i].M_select = 2;
		if (hero[i].GetHealing_LV() <= 0 && h_draw[i].M_select == 2)h_draw[i].M_select = 1;
		if (hero[i].GetFlameATT_LV() <= 0 && h_draw[i].M_select == 1)h_draw[i].M_select = 0;
		break;
	case DOWN:
		MovieSoundManager::Select_sound(FALSE);
		if (h_draw[i].M_select == 4)break;
		h_draw[i].M_select++;
		if (hero[i].GetFlameATT_LV() <= 0 && h_draw[i].M_select == 1)h_draw[i].M_select = 2;
		if (hero[i].GetHealing_LV() <= 0 && h_draw[i].M_select == 2)h_draw[i].M_select = 3;
		if (hero[i].GetRecover_LV() <= 0 && h_draw[i].M_select == 3)h_draw[i].M_select = 4;
		break;
	case ENTER:
		MovieSoundManager::Enter_sound(FALSE);
		command.DrawOff();
		switch (h_draw[i].M_select) {
		case 0:
			return ATT;
			break;
		case 1:
			MAG_select = FLAME;
			return MAG;
			break;
		case 2:
			MAG_select = HEAL;
			return MAG;
			break;
		case 3:
			MAG_select = RECOV;
			return MAG;
			break;
		case 4:
			return ESCAPE;
			break;
		}
		break;
	}
	text->UpDateText(L"攻撃", 25.0f, 10.0f, 15.0f, { r[0], g[0], 1.0f, 1.0f });
	if (hero[i].GetFlameATT_LV() >= 1) {
		text->UpDateText(L"フレイム", 10.0f, 40.0f, 15.0f, { r[1], g[1], 1.0f, 1.0f });
	}
	if (hero[i].GetHealing_LV() >= 1) {
		text->UpDateText(L"ヒーリング", 10.0f, 70.0f, 15.0f, { r[2], g[2], 1.0f, 1.0f });
	}
	if (hero[i].GetRecover_LV() >= 1) {
		text->UpDateText(L"リカバリ", 10.0f, 100.0f, 15.0f, { r[3], g[3], 1.0f, 1.0f });
	}
	text->UpDateText(L"逃げる", 10.0f, 130.0f, 15.0f, { r[4], g[4], 1.0f, 1.0f });
	text->UpDateText(L"→スキップ", 10.0f, 180.0f, 15.0f, { 1.0f, 1.0f, 1.0f, 1.0f });

	return NOSELECT;
}

CommandSelect Battle::Menu_AT(int i, Directionkey direction) {

	//元選択中の敵がLOSTした場合の選択対象切り替え
	if (enemy[h_draw[i].A_select].Dieflg() == TRUE) {
		for (int i1 = 0; i1 < e_num; i1++)if (enemy[i1].Dieflg() == FALSE) {
			h_draw[i].A_select = i1; break;
		}
	}

	float r, b;
	Cursor_e(h_draw[i].A_select, &r, &b);
	E_select.InstanceUpdate(r, b, 0.0f, 0.0f, 0.0f);

	switch (direction) {
	case LEFT:
		MovieSoundManager::Select_sound(FALSE);
		if (e_pos[1].element == TRUE && enemy[1].Dieflg() == FALSE)h_draw[i].A_select = 1;
		break;
	case RIGHT:
		MovieSoundManager::Select_sound(FALSE);
		if (e_pos[3].element == TRUE && enemy[3].Dieflg() == FALSE)h_draw[i].A_select = 3;
		break;
	case UP:
		MovieSoundManager::Select_sound(FALSE);
		if (e_pos[2].element == TRUE && enemy[2].Dieflg() == FALSE)h_draw[i].A_select = 2;
		break;
	case DOWN:
		MovieSoundManager::Select_sound(FALSE);
		if (enemy[0].Dieflg() == FALSE)h_draw[i].A_select = 0;
		break;
	case ENTER:
		MovieSoundManager::Enter_sound(FALSE);
		select_obj = h_draw[i].A_select;
		E_select.DrawOff();
		return ATT;
		break;
	case CANCEL:
		E_select.DrawOff();
		return CANCEL_M;
		break;
	}
	return NOSELECT;
}

CommandSelect Battle::Menu_MAG_AT(int i, Directionkey direction) {

	//元選択中の敵がLOSTした場合の選択対象切り替え(全体攻撃の場合は切り替えしない)
	if (h_draw[i].MA_select != 4 && enemy[h_draw[i].MA_select].Dieflg() == TRUE) {
		for (int i1 = 0; i1 < e_num; i1++)if (enemy[i1].Dieflg() == FALSE) {
			h_draw[i].MA_select = i1; break;
		}
	}
	float r, b;
	if (h_draw[i].MA_select != 4)Cursor_e(h_draw[i].MA_select, &r, &b);//単体選択
	else {
		for (int i1 = 0; i1 < e_num; i1++) {
			if (enemy[i1].Dieflg() == TRUE)continue;
			Cursor_e(i1, &r, &b);//全体選択(h_draw[i].A_select == 4)
		}
	}
	E_select.InstanceUpdate(r, b, 0.0f, 0.0f, 0.0f);

	switch (direction) {
	case LEFT:
		MovieSoundManager::Select_sound(FALSE);
		if (e_pos[1].element == TRUE && enemy[1].Dieflg() == FALSE)h_draw[i].MA_select = 1;
		break;
	case RIGHT:
		MovieSoundManager::Select_sound(FALSE);
		if (e_pos[3].element == TRUE && enemy[3].Dieflg() == FALSE)h_draw[i].MA_select = 3;
		break;
	case UP:
		MovieSoundManager::Select_sound(FALSE);
		if (e_pos[2].element == TRUE && enemy[2].Dieflg() == FALSE)h_draw[i].MA_select = 2;
		break;
	case DOWN:
		MovieSoundManager::Select_sound(FALSE);
		if (h_draw[i].MA_select == 4) {
			h_draw[i].MA_select = 0;
			break;
		}
		if (h_draw[i].MA_select == 0) {
			h_draw[i].MA_select = 4;
			break;
		}
		if (enemy[0].Dieflg() == TRUE && h_draw[i].MA_select != 4) {
			h_draw[i].MA_select = 4;
			break;
		}
		if (enemy[0].Dieflg() == FALSE)h_draw[i].MA_select = 0;
		break;
	case ENTER:
		MovieSoundManager::Enter_sound(FALSE);
		select_obj = h_draw[i].MA_select;
		E_select.DrawOff();
		return MAG;
		break;
	case CANCEL:
		MovieSoundManager::Select_sound(FALSE);
		E_select.DrawOff();
		return CANCEL_M;
		break;
	}
	return NOSELECT;
}

CommandSelect Battle::Menu_RCV(Hero *hero, int i, Directionkey direction) {

	//RECOV選択時の全体選択解除
	if (MAG_select == RECOV && h_draw[i].MA_select == 4)h_draw[i].MA_select = 0;

	//プレイヤ回復対象カーソル
	if (h_draw[i].MA_select == 4) {
		for (int i1 = 0; i1 < 4; i1++) {
			Cursor_h(i1);
		}
	}
	else Cursor_h(h_draw[i].MA_select);
	h_select.InstanceUpdate();

	switch (direction) {
	case LEFT:
		MovieSoundManager::Select_sound(FALSE);
		if (h_draw[i].MA_select == 0)break;
		h_draw[i].MA_select--;
		break;
	case RIGHT:
		MovieSoundManager::Select_sound(FALSE);
		if (h_draw[i].MA_select == 3)break;
		if (h_draw[i].MA_select == 4) {
			h_draw[i].MA_select = 0;
			break;
		}
		h_draw[i].MA_select++;
		break;
	case UP:
		break;
	case DOWN:
		MovieSoundManager::Select_sound(FALSE);
		if (MAG_select == HEAL) {
			if (h_draw[i].MA_select == 4)h_draw[i].MA_select = 0;
			else h_draw[i].MA_select = 4;
		}
		break;
	case ENTER:
		MovieSoundManager::Enter_sound(FALSE);
		if (MAG_select == HEAL && h_draw[i].MA_select != 4 && hero[h_draw[i].MA_select].Dieflg() == TRUE)break;
		select_obj = h_draw[i].MA_select;
		h_select.DrawOff();
		return MAG;
		break;
	case CANCEL:
		MovieSoundManager::Select_sound(FALSE);
		h_select.DrawOff();
		return CANCEL_M;
		break;
	}
	return NOSELECT;
}
