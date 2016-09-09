//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Heroクラス                                      **//
//**                                   Statedraw関数                                     **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Hero.h"

void Hero::OBJWalkDraw(float x, float y, float z, float r, float g, float b, float theta, int walkI) {

	if (walkI == -1) {
		//静止
		map_walk[0].Draw(x, y, z, r, g, b, theta, 0, 0, 2.0f, 0.1f);
	}
	else {
		float ax, ay, wi;
		ax = ay = wi = 0.0f;
		wi = (float)walkI / 18.0f * 30.0f;
		switch ((int)theta) {
		case 360:
		case 0:
			ay = wi;
			ax = 0.0f;
			break;
		case 90:
			ax = -wi;
			ay = 0.0f;
			break;
		case 180:
			ay = -wi;
			ax = 0.0f;
			break;
		case 270:
			ax = wi;
			ay = 0.0f;
			break;
		}
		//移動
		map_walk[walkI + 1].Draw(x + ax, y + ay, z, r, g, b, theta, 0, 0, 2.0f, 0.1f);
	}
}

Act_fin_flg Hero::Statedraw(Battle *battle, int *select_obj, Position::H_Pos *h_pos, Position::E_Pos *e_pos, float me, bool command_run, Action action, MagicSelect H_Magrun) {

	static bool clr_f = TRUE;
	static float r = 1.0f;
	float x;
	if (o_no == 0)x = 20.0f;
	if (o_no == 1)x = 190.0f;
	if (o_no == 2)x = 370.0f;
	if (o_no == 3)x = 540.0f;

	Position::Bt_H_Pos *b_pos = battle->GetBtPos(h_pos);

	//NORMAL,LOST以外のアクション中にNORMAL,LOST以外のアクション発生時の初期化
	if (action != NORMAL&& action != LOST) {
		mov_y = 0.0f;
		mov_x = 0.0f;
		mov_z = 0.0f;
		up = TRUE;
		count = 0;
	}

	float cntMax = 0;
	switch (action) {
	case ATTACK:
		act_f = ATTACK;
		break;
	case MAGIC:
		act_f = MAGIC;
		break;
	case DAMAGE:
		if (act_f == ATTACK || act_f == MAGIC)break;
		act_f = DAMAGE;
		break;
	case LOST:
		act_f = LOST;
		break;
	case RECOVER:
		act_f = RECOVER;
		break;
	}

	float m;
	TCHAR *str = L"フレイムＬＶ";
	TCHAR *str1 = text->CreateTextValue(GetFlameATT_LV());
	TCHAR *str2 = L"ヒーリングＬＶ";
	TCHAR *str3 = text->CreateTextValue(GetHealing_LV());
	TCHAR *str4 = L"リカバリＬＶ";
	TCHAR *str5 = text->CreateTextValue(GetRecover_LV());
	TCHAR *str6 = L"MPが足りない";
	switch (act_f) {
	case ATTACK:
		m = tfloat.Add(0.15f);
		cntMax = (float)(ObjCntMax * 3 - 1);
		if (effect_f == FALSE) {
			if ((p_att_cnt += m) < cntMax) {//カウントcnt o_no=0:Max27, o_no=1:Max32, o_no=2:Max26, o_no=3:Max15
				int cnt = (int)(p_att_cnt / 3.0f);
				p_att_Ind = cnt;
			}
			else {
				p_att_cnt = 0.0f;
				p_att_Ind = 0;
				effect_f = TRUE;
			}
		}
		if (effect_f == TRUE) {
			effect_no = 0;
			if (Effectdraw(battle, select_obj, h_pos, e_pos) == FALSE) {
				effect_f = FALSE;
				act_f = NORMAL;
				return AT_FIN;
			}
		}
		break;
	case MAGIC:
		m = tfloat.Add(0.15f);
		float mx, my;
		MovieSoundManager::Magic_sound(TRUE);
		mag.Draw(b_pos[o_no].BtPos_x1, b_pos[o_no].BtPos_y1, (float)h_pos->pz * 100.0f, 0, 0, 0, count += m, 0);
		VECTOR3 p3;
		p3.as(b_pos[o_no].BtPos_x1, b_pos[o_no].BtPos_y1, (float)h_pos->pz * 100.0f + 20.0f);
		PolygonData2D::Pos2DCompute(&p3);
		mx = p3.x - 130.0f;//位置補正
		my = p3.y - 200.0f;//位置補正
		switch (H_Magrun) {
		case FLAME:
			text->UpDateText(&str, mx, my, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
			text->UpDateText(&str1, mx + 180.0f, my, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
			effect_no = 1;
			break;
		case HEAL:
			text->UpDateText(&str2, mx, my, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
			text->UpDateText(&str3, mx + 210.0f, my, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
			effect_no = 2;
			break;
		case RECOV:
			text->UpDateText(&str4, mx, my, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
			text->UpDateText(&str5, mx + 180.0f, my, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
			effect_no = 3;
			break;
		case NOSEL:
			text->UpDateText(&str6, mx, my, 30.0f, { 1.0f, 0.5f, 0.5f, 1.0f });
			break;
		}
		if (effect_f == FALSE && (count += m) > 200.0f) {
			count = 0;
			if (H_Magrun == NOSEL) {
				act_f = NORMAL;
				return AT_FIN;
			}
			effect_f = TRUE;
		}
		if (effect_f == TRUE) {
			if (Effectdraw(battle, select_obj, h_pos, e_pos) == FALSE) {
				effect_f = FALSE;
				act_f = NORMAL;
				return AT_FIN;
			}
		}
		break;
	case DAMAGE:
		m = tfloat.Add(0.05f);
		if ((count += m) < 10) {
			int rnd = rand() % 4;
			rnd -= 2;
			mov_x = (float)rnd;
			rnd = rand() % 4;
			rnd -= 2;
			mov_y = (float)rnd;
			rnd = rand() % 4;
			rnd -= 2;
			mov_z = (float)rnd;
		}
		else {
			count = 0.0f;
			act_f = NORMAL;
			mov_x = mov_y = mov_z = 0.0f;
		}
		break;
	case LOST:
		m = tfloat.Add(1.0f);
		if ((LA += m) >= 90.0f) {
			LA = 90.0f;//ひたすら加算を防止
		}
		break;
	case RECOVER:
		m = tfloat.Add(1.0f);
		if ((LA -= m) <= 0.0f) {
			LA = 0.0f;
			act_f = NORMAL;//終わり次第ノーマルに戻す
		}
		break;
	}

	switch ((int)h_pos->theta) {
	case 360:
	case 0:
		LA_x = LA;
		LA_y = 0.0f;
		break;
	case 90:
		LA_y = LA;
		LA_x = 0.0f;
		break;
	case 180:
		LA_x = -LA;
		LA_y = 0.0f;
		break;
	case 270:
		LA_y = -LA;
		LA_x = 0.0f;
		break;
	}
	p_att[p_att_Ind].Draw(b_pos[o_no].BtPos_x1 + mov_x, b_pos[o_no].BtPos_y1 + mov_y, (float)h_pos->pz * 100.0f + mov_z + LA / 9.0f, 0, 0, 0, h_pos->theta, LA_y, LA_x, 2.0f, 0.1f);

	Statecreate(command_run);
	TCHAR *str7 = L"囚人Ｎｏ";
	text->UpDateText(&str7, x, 470.0f, 15.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateValue(o_no, x + 60.0f, 470.0f, 15.0f, 1, { 1.0f, 1.0f, 1.0f, 1.0f });
	TCHAR *str8 = L"ＨＰ";
	text->UpDateText(&str8, x - 5.0f, 490.0f, 15.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateValue(p_data.HP, x + 25.0f, 490.0f, 15.0f, 5, { 1.0f, 1.0f, 1.0f, 1.0f });
	TCHAR *str9 = L"ＭＰ";
	text->UpDateText(&str9, x - 5.0f, 510.0f, 15.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateValue(p_data.MP, x + 25.0f, 510.0f, 15.0f, 5, { 1.0f, 1.0f, 1.0f, 1.0f });
	if (p_data.HP <= 0) {
		TCHAR *str10 = L"戦闘不能";
		text->UpDateText(&str10, x, 525.0f, 25.0f, { 1.0f, 0.0f, 0.0f, 1.0f });
		return NOT_FIN;
	}
	if (me >= 1.0f) {
		m = tfloat.Add(0.001f);
		if (clr_f) {
			if ((r -= m) <= 0.5f)clr_f = FALSE;
		}
		else {
			if ((r += m) >= 1.0f)clr_f = TRUE;
		}
		TCHAR *str11 = L"戦闘不能";
		text->UpDateText(&str11, x, 525.0f, 25.0f, { 1.0f, 1.0f, 1.0f, r });
		return NOT_FIN;
	}
	Metercreate(me);

	//LOST状態
	if (act_f == LOST && LA == 90)return LOST_FIN;
	//LOST以外
	return NOT_FIN;
}

void Hero::Act_f_init(){
	act_f = NORMAL;
}

Action Hero::Normal_act_get(){
	return NORMAL;
}