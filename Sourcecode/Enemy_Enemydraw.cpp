//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Enemyクラス                                     **//
//**                                    Enemydraw関数                                    **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Enemy.h"
#include "Battle.h"

Act_fin_flg Enemy::EnemyUpdate(Battle *battle, int *E_select_obj, Action action, MagicSelect E_Magrun) {

	//MOVE,LOST以外のアクション中にMOVE,LOST以外のアクション発生時の初期化
	if (action != NORMAL && action != MOVE && action != LOST) {
		mov_z = 0.0f;
		mov_y = 0.0f;
		mov_x = 0.0f;
		up = TRUE;
		zoom = TRUE;
		count = 0;
	}

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
		RecoverActionInit();
		break;
	}

	float m;
	switch (act_f) {
	case MOVE:
		m = tfloat.Add(0.005f);
		if (up == TRUE && (mov_z -= m) < -4.0f)up = FALSE;
		if (up == FALSE && (mov_z += m) > 0.0f) {
			up = TRUE; mov_z = 0.0f;
		}
		break;

	case ATTACK:
		AttackAction();
		if (effect_f == TRUE) {
			effect_no = 0;
			if (EffectUpdate(battle, E_select_obj) == FALSE) {
				effect_f = FALSE;
				act_f = normal_action;
				return AT_FIN;
			}
		}
		break;

	case MAGIC:
		float mx, my;
		VECTOR3 p3;
		p3.as(e_pos[o_no].x, e_pos[o_no].y, e_pos[o_no].z);
		PolygonData2D::Pos2DCompute(&p3);
		mx = p3.x - 50.0f;//位置補正
		my = p3.y - 160.0f;//位置補正
		switch (E_Magrun) {
		case FLAME:
			text->UpDateText(L"フレイムＬＶ", mx, my, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
			text->UpDateValue(GetFlameATT_LV(), mx + 180.0f, my, 30.0f, 3, { 1.0f, 1.0f, 1.0f, 1.0f });
			effect_no = 1;
			magicSel = FLAME;
			break;
		case HEAL:
			text->UpDateText(L"ヒーリングＬＶ", mx, my, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
			text->UpDateValue(GetHealing_LV(), mx + 210.0f, my, 30.0f, 3, { 1.0f, 1.0f, 1.0f, 1.0f });
			effect_no = 2;
			break;
		case RECOV:
			text->UpDateText(L"リカバリＬＶ", mx, my, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
			text->UpDateValue(GetRecover_LV(), mx + 180.0f, my, 30.0f, 3, { 1.0f, 1.0f, 1.0f, 1.0f });
			effect_no = 3;
			break;
		case NOSEL:
			text->UpDateText(L"MPが足りない", mx, my, 30.0f, { 1.0f, 0.5f, 0.5f, 1.0f });
			break;
		}

		if (effect_f == FALSE && Magiccreate(e_pos[o_no].x, e_pos[o_no].y, e_pos[o_no].z) == FALSE) {
			if (E_Magrun == NOSEL) {//MP足りない場合抜ける
				act_f = normal_action;
				return AT_FIN;
			}
			effect_f = TRUE;
		}
		if (effect_f == TRUE && EffectUpdate(battle, E_select_obj) == FALSE) {
			effect_f = FALSE;
			act_f = normal_action;
			return AT_FIN;
		}
		break;

	case DAMAGE:
		DamageAction();
		break;

	case LOST:
		if (LostAction(e_pos[o_no].x, e_pos[o_no].y, e_pos[o_no].z) == TRUE) {
			return LOST_FIN;
		}
		break;

	case RECOVER:
		RecoverAction();
		break;
	}
	dx->P_ShadowBright(0.3f);
	ObjUpdate(e_pos[o_no].x + mov_x, e_pos[o_no].y + mov_y, e_pos[o_no].z + mov_z, cr, cg, cb, e_pos[o_no].theta);

	return NOT_FIN;
}

bool Enemy::GetBossEffectState(float *blur) {
	static float blurRet = 0.0f;
	static bool eff = FALSE;

	if ((en_boss_att || en_boss_att0) && magicSel == FLAME && effect_f)eff = TRUE;

	if (eff && (blurRet += 0.001f) > 0.1f) {
		blurRet = 0.0f;
		eff = FALSE;
		magicSel = NOSEL;
	}
	*blur = blurRet;
	return eff;
}

void Enemy::Draw(Encount enc) {
	if (enc == NOENCOUNT) {
		//SideEnemy
		if (en) {
			en->DrawOff();
			mag->DrawOff();
		}
		//Boss2
		if (en_boss_att0)en_boss_att0->DrawOff();
		//Boss 0,1,3,4
		if (en_boss_att)en_boss_att->DrawOff();
		//BossMag
		if (mag_boss)mag_boss->DrawOff();
		//エフェクト
		for (int i = 0; i < 4; i++)effect[i].DrawOff();
	}
	else {
		//SideEnemy
		if (en) {
			en->Draw();
			mag->Draw();
		}
		//Boss2
		if (en_boss_att0)en_boss_att0->Draw();
		//Boss 0,1,3,4
		if (en_boss_att)en_boss_att->Draw();
		//BossMag
		if (mag_boss)mag_boss->Draw();
		//エフェクト
		for (int i = 0; i < 4; i++)effect[i].Draw();
	}
}

Action Enemy::Normal_act_get(){
	return normal_action;
}

bool Enemy::M_run_flg(){
	return FALSE;
}

void Enemy::M_select(int *r, int *r1){}

bool Enemy::s_esc(){
	return esc;
}

void Enemy::PosOffset(int o_no){

	if (e_pos[0].theta == 0 || e_pos[0].theta == 360)e_pos[o_no].y -= pos_offset;
	if (e_pos[0].theta == 90)e_pos[o_no].x += pos_offset;
	if (e_pos[0].theta == 180)e_pos[o_no].y += pos_offset;
	if (e_pos[0].theta == 270)e_pos[o_no].x -= pos_offset;
}