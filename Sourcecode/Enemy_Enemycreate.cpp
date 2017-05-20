//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Enemyクラス                                     **//
//**                                   Enemycreate関数                                   **//
//*****************************************************************************************//

#include "Enemy.h"
#include "Battle.h"

Enemy::Enemy(int t_no, int no) {

	o_no = no;
	e_no = t_no;
	comNo = 0;
	mov_z = 0.0f;
	mov_y = 0.0f;
	mov_x = 0.0f;
	act_f = normal_action;
	up = TRUE;
	zoom = TRUE;
	count = 0.0f;
	effect_f = FALSE;
	tx = ty = 0.0f;
	tt = 0;
	cr = cg = cb = 0.0f;
	en = NULL;
	en_boss_att = NULL;
	attFin = attOn = magicAttOn = FALSE;
}

void Enemy::EffectGetBuffer() {
	for (int i = 0; i < 4; i++) {
		effect[i].GetVBarray(SQUARE, 1);
		float ver = 25;
		//左前
		effect[i].SetVertex(0, 0,
			-ver, 0.0f, ver * 2,
			0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			0.0f, 0.0f);

		//右前
		effect[i].SetVertex(1, 4, 1,
			ver, 0.0f, ver * 2,
			0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f);

		//左奥
		effect[i].SetVertex(2, 3, 2,
			-ver, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			0.0f, 1.0f);

		//右奥
		effect[i].SetVertex(5, 3,
			ver, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f);
	}
}

void Enemy::EffectCreate() {
	int ef = dx->GetTexNumber("h_att.jpg");
	for (int i = 0; i < 4; i++)
		effect[i].Create(FALSE, ef + i, TRUE, TRUE);
}

void Enemy::Enemycreate(float x, float y){

	//敵左前
	en->SetVertex(0, 0,
		(float)-(x / 2), (float)0.0f, y,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f);

	//敵左奥
	en->SetVertex(1, 1,
		(float)-(x / 2), (float)0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f);

	//敵右奥
	en->SetVertex(2, 2,
		(float)(x / 2), (float)0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f);

	//敵右前
	en->SetVertex(3, 3,
		(float)(x / 2), (float)0.0f, y,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f);
}

void Enemy::SetVertex(){}

void Enemy::SetCommandList(int com_no){}

void Enemy::CreateEnemy(){}

void Enemy::AttackAction(){}

void Enemy::DamageAction(){}

void Enemy::RecoverActionInit(){}

void Enemy::RecoverAction(){}

bool Enemy::LostAction(float x, float y, float z){ return TRUE; }

bool Enemy::Magiccreate(float x, float y, float z){ return TRUE; }

void Enemy::ObjUpdate(float x, float y, float z, float r, float g, float b, float theta){}

bool Enemy::EffectUpdate(Battle *battle, int *E_select_obj) {

	float px, py;
	float u_cnt;
	float v_cnt;
	switch (effect_no) {
		//通常攻撃
	case 0:
		px = 0.1111f;
		py = 1.0f;
		break;
		//フレイム
	case 1:
		px = 0.1428f;
		py = 1.0f;
		break;
		//ヒール
	case 2:
		px = 0.1f;
		py = 1.0f;
		break;
		//リカバリ
	case 3:
		px = 0.1f;
		py = 1.0f;
		break;
	}

	if ((tt += tfloat.Add(0.8f)) > 10.0f) {//速度調整用
		tt = 0;
		if ((tx += px) + px > 1.0f) {
			for (int i = 3; i < 7; i++)dx->PointLightPosSet(i, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, FALSE);
			for (int i = 0; i < 4; i++)effect[i].DrawOff();
			tx = 0; return FALSE;
		}
	}
	u_cnt = tx / px;
	v_cnt = ty / py;

	float ex = 0.0f;
	float ey = 0.0f;
	switch ((int)h_pos->theta) {
	case 360:
	case 0:
		ey = 15.0f;
		break;
	case 90:
		ex = -15.0f;
		break;
	case 180:
		ey = -15.0f;
		break;
	case 270:
		ex = 15.0f;
		break;
	}

	if (effect_no == 2 || effect_no == 3) {
		MovieSoundManager::Heal_sound(TRUE);
		float r, g, b;
		if (effect_no == 2) { r = 0.2f, g = 0.7f, b = 0.3f; }
		if (effect_no == 3) { r = 0.2f, g = 0.3f, b = 0.7f; }
		if (*E_select_obj != 4) {
			effect[effect_no].Update(e_pos[*E_select_obj].x + ex, e_pos[*E_select_obj].y + ey, e_pos[*E_select_obj].z, 0, 0, 0, e_pos[*E_select_obj].theta, 0.0f, 1.0f, px, py, u_cnt, v_cnt);
			dx->PointLightPosSet(3, e_pos[*E_select_obj].x + ex, e_pos[*E_select_obj].y + ey, e_pos[*E_select_obj].z, r, g, b, 1.0f, 50.0f, 20.0f, 2.0f, TRUE);
		}
		else {
			for (int i = 0; i < 4; i++) {
				if (battle->GetE_RCV(i) == FALSE)continue;
				effect[effect_no].InstancedMap(e_pos[i].x + ex, e_pos[i].y + ey, e_pos[i].z, e_pos[i].theta);
				dx->PointLightPosSet(i + 3, e_pos[i].x + ex, e_pos[i].y + ey, e_pos[i].z, r, g, b, 1.0f, 50.0f, 20.0f, 2.0f, TRUE);
			}
			effect[effect_no].InstanceUpdate(0.0f, 0.0f, 0.0f, 0.0f, px, py, u_cnt, v_cnt);
		}
	}

	Position::Bt_H_Pos *b_pos = battle->GetBtPos(h_pos);
	if (effect_no == 0 || effect_no == 1) {
		if (effect_no == 0)MovieSoundManager::Att_sound(TRUE);
		if (effect_no == 1)MovieSoundManager::Flame_sound(TRUE);
		float r, g, b;
		if (effect_no == 0) { r = 1.0f, g = 1.0f, b = 1.0f; }
		if (effect_no == 1) { r = 0.7f, g = 0.3f, b = 0.2f; }
		if (*E_select_obj != 4) {
			effect[effect_no].Update(b_pos[*E_select_obj].BtPos_x1, b_pos[*E_select_obj].BtPos_y1, (float)h_pos->pz * 100.0f, 0, 0, 0, h_pos->theta, 0.0f, 1.0f, px, py, u_cnt, v_cnt);
			dx->PointLightPosSet(3, b_pos[*E_select_obj].BtPos_x1, b_pos[*E_select_obj].BtPos_y1, (float)h_pos->pz * 100.0f, r, g, b, 1.0f, 50.0f, 20.0f, 2.0f, TRUE);
		}
		else {
			for (int i = 0; i < 4; i++) {
				if (battle->GetH_DM(i) == FALSE)continue;
				effect[effect_no].InstancedMap(b_pos[i].BtPos_x1, b_pos[i].BtPos_y1, (float)h_pos->pz * 100.0f, h_pos->theta);
				dx->PointLightPosSet(i + 3, b_pos[i].BtPos_x1, b_pos[i].BtPos_y1, (float)h_pos->pz * 100.0f, r, g, b, 1.0f, 50.0f, 20.0f, 2.0f, TRUE);
			}
			effect[effect_no].InstanceUpdate(0.0f, 0.0f, 0.0f, 0.0f, px, py, u_cnt, v_cnt);
		}
	}
	return TRUE;
}

Enemy::~Enemy(){

}
