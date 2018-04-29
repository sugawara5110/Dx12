//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	       EnemySideクラス                                   **//
//**                                   Magiccreate関数                                   **//
//*****************************************************************************************//

#include "EnemySide.h"

EnemySide::EnemySide(int t_no, int no, Position::H_Pos *h_po, Position::E_Pos *e_po) :Enemy(t_no, no) {

	h_pos = h_po;
	e_pos = e_po;

	pos_offset = 0.0f;
	size_x = 50.0f;
	size_y = 50.0f;
	//t_no=敵№
	switch (t_no) {
		//マップ0
	case 0:
		normal_action = NORMAL;
		M_run_flg_rnd = 1;
		p_data.Attack = 1;
		p_data.Magic = 1;
		p_data.Agility = 1;
		p_data.Vitality = 1;
		p_data.Exp = 1;
		p_data.LV = 1;
		p_data.FlameATT_LV = 0;
		p_data.Fpoint = 10;
		p_data.Healing_LV = 0;
		p_data.Hpoint = 10;
		p_data.Recover_LV = 0;
		p_data.Rpoint = 10;
		size_x = 30.0f;
		size_y = 20.0f;
		break;
	case 1:
		normal_action = NORMAL;
		M_run_flg_rnd = 1;
		p_data.Attack = 2;
		p_data.Magic = 2;
		p_data.Agility = 2;
		p_data.Vitality = 2;
		p_data.Exp = 2;
		p_data.LV = 2;
		p_data.FlameATT_LV = 0;
		p_data.Fpoint = 10;
		p_data.Healing_LV = 0;
		p_data.Hpoint = 10;
		p_data.Recover_LV = 0;
		p_data.Rpoint = 10;
		size_x = 25.0f;
		size_y = 35.0f;
		break;
	case 2:
		normal_action = MOVE;
		M_run_flg_rnd = 3;
		p_data.Attack = 3;
		p_data.Magic = 6;
		p_data.Agility = 3;
		p_data.Vitality = 4;
		p_data.Exp = 3;
		p_data.LV = 4;
		p_data.FlameATT_LV = 0;
		p_data.Fpoint = 10;
		p_data.Healing_LV = 2;
		p_data.Hpoint = 20;
		p_data.Recover_LV = 0;
		p_data.Rpoint = 10;
		size_x = 45.0f;
		size_y = 35.0f;
		break;
	case 3:
		normal_action = NORMAL;
		M_run_flg_rnd = 3;
		p_data.Attack = 7;
		p_data.Magic = 6;
		p_data.Agility = 5;
		p_data.Vitality = 6;
		p_data.Exp = 6;
		p_data.LV = 6;
		p_data.FlameATT_LV = 3;
		p_data.Fpoint = 30;
		p_data.Healing_LV = 0;
		p_data.Hpoint = 10;
		p_data.Recover_LV = 0;
		p_data.Rpoint = 10;
		size_x = 40.0f;
		size_y = 50.0f;
		break;
		//マップ1
	case 4:
		normal_action = NORMAL;
		M_run_flg_rnd = 3;
		p_data.Attack = 7;
		p_data.Magic = 9;
		p_data.Agility = 8;
		p_data.Vitality = 8;
		p_data.Exp = 8;
		p_data.LV = 8;
		p_data.FlameATT_LV = 4;
		p_data.Fpoint = 40;
		p_data.Healing_LV = 2;
		p_data.Hpoint = 20;
		p_data.Recover_LV = 0;
		p_data.Rpoint = 10;
		break;
	case 5:
		normal_action = MOVE;
		M_run_flg_rnd = 3;
		p_data.Attack = 9;
		p_data.Magic = 12;
		p_data.Agility = 9;
		p_data.Vitality = 10;
		p_data.Exp = 7;
		p_data.LV = 10;
		p_data.FlameATT_LV = 4;
		p_data.Fpoint = 40;
		p_data.Healing_LV = 3;
		p_data.Hpoint = 30;
		p_data.Recover_LV = 1;
		p_data.Rpoint = 10;
		break;
	case 6:
		normal_action = NORMAL;
		M_run_flg_rnd = 3;
		p_data.Attack = 10;
		p_data.Magic = 16;
		p_data.Agility = 12;
		p_data.Vitality = 10;
		p_data.Exp = 10;
		p_data.LV = 12;
		p_data.FlameATT_LV = 0;
		p_data.Fpoint = 10;
		p_data.Healing_LV = 7;
		p_data.Hpoint = 70;
		p_data.Recover_LV = 0;
		p_data.Rpoint = 10;
		break;
	case 7:
		normal_action = NORMAL;
		M_run_flg_rnd = 2;
		p_data.Attack = 16;
		p_data.Magic = 12;
		p_data.Agility = 14;
		p_data.Vitality = 14;
		p_data.Exp = 10;
		p_data.LV = 14;
		p_data.FlameATT_LV = 8;
		p_data.Fpoint = 80;
		p_data.Healing_LV = 0;
		p_data.Hpoint = 10;
		p_data.Recover_LV = 0;
		p_data.Rpoint = 10;
		break;
		//マップ2
	case 8:
		normal_action = NORMAL;
		M_run_flg_rnd = 2;
		p_data.Attack = 17;
		p_data.Magic = 18;
		p_data.Agility = 13;
		p_data.Vitality = 16;
		p_data.Exp = 15;
		p_data.LV = 16;
		p_data.FlameATT_LV = 8;
		p_data.Fpoint = 80;
		p_data.Healing_LV = 5;
		p_data.Hpoint = 50;
		p_data.Recover_LV = 0;
		p_data.Rpoint = 10;
		break;
	case 9:
		normal_action = NORMAL;
		M_run_flg_rnd = 1;
		p_data.Attack = 27;
		p_data.Magic = 5;
		p_data.Agility = 18;
		p_data.Vitality = 22;
		p_data.Exp = 15;
		p_data.LV = 18;
		p_data.FlameATT_LV = 0;
		p_data.Fpoint = 10;
		p_data.Healing_LV = 0;
		p_data.Hpoint = 10;
		p_data.Recover_LV = 0;
		p_data.Rpoint = 10;
		break;
	case 10:
		normal_action = MOVE;
		M_run_flg_rnd = 2;
		p_data.Attack = 20;
		p_data.Magic = 20;
		p_data.Agility = 20;
		p_data.Vitality = 20;
		p_data.Exp = 15;
		p_data.LV = 20;
		p_data.FlameATT_LV = 14;
		p_data.Fpoint = 140;
		p_data.Healing_LV = 10;
		p_data.Hpoint = 100;
		p_data.Recover_LV = 0;
		p_data.Rpoint = 10;
		break;
	case 11:
		normal_action = NORMAL;
		M_run_flg_rnd = 1;
		p_data.Attack = 28;
		p_data.Magic = 5;
		p_data.Agility = 22;
		p_data.Vitality = 33;
		p_data.Exp = 17;
		p_data.LV = 22;
		p_data.FlameATT_LV = 0;
		p_data.Fpoint = 10;
		p_data.Healing_LV = 0;
		p_data.Hpoint = 10;
		p_data.Recover_LV = 0;
		p_data.Rpoint = 10;
		size_x = 40.0f;
		size_y = 50.0f;
		break;
		//マップ3
	case 12:
		normal_action = NORMAL;
		M_run_flg_rnd = 2;
		p_data.Attack = 24;
		p_data.Magic = 24;
		p_data.Agility = 24;
		p_data.Vitality = 24;
		p_data.Exp = 28;
		p_data.LV = 24;
		p_data.FlameATT_LV = 15;
		p_data.Fpoint = 150;
		p_data.Healing_LV = 15;
		p_data.Hpoint = 150;
		p_data.Recover_LV = 15;
		p_data.Rpoint = 150;
		size_x = 45.0f;
		size_y = 45.0f;
		break;
	case 13:
		normal_action = NORMAL;
		M_run_flg_rnd = 1;
		p_data.Attack = 39;
		p_data.Magic = 3;
		p_data.Agility = 26;
		p_data.Vitality = 36;
		p_data.Exp = 30;
		p_data.LV = 26;
		p_data.FlameATT_LV = 0;
		p_data.Fpoint = 10;
		p_data.Healing_LV = 0;
		p_data.Hpoint = 10;
		p_data.Recover_LV = 0;
		p_data.Rpoint = 10;
		size_x = 40.0f;
		size_y = 50.0f;
		break;
	case 14:
		normal_action = NORMAL;
		M_run_flg_rnd = 2;
		p_data.Attack = 28;
		p_data.Magic = 28;
		p_data.Agility = 28;
		p_data.Vitality = 28;
		p_data.Exp = 34;
		p_data.LV = 28;
		p_data.FlameATT_LV = 20;
		p_data.Fpoint = 200;
		p_data.Healing_LV = 20;
		p_data.Hpoint = 200;
		p_data.Recover_LV = 10;
		p_data.Rpoint = 100;
		break;
	case 15:
		normal_action = MOVE;
		M_run_flg_rnd = 2;
		p_data.Attack = 30;
		p_data.Magic = 30;
		p_data.Agility = 30;
		p_data.Vitality = 30;
		p_data.Exp = 30;
		p_data.LV = 30;
		p_data.FlameATT_LV = 30;
		p_data.Fpoint = 300;
		p_data.Healing_LV = 0;
		p_data.Hpoint = 10;
		p_data.Recover_LV = 0;
		p_data.Rpoint = 10;
		break;
		//マップ4(ラスト)
	case 16:
		normal_action = MOVE;
		M_run_flg_rnd = 2;
		p_data.Attack = 32;
		p_data.Magic = 32;
		p_data.Agility = 32;
		p_data.Vitality = 32;
		p_data.Exp = 50;
		p_data.LV = 32;
		p_data.FlameATT_LV = 32;
		p_data.Fpoint = 320;
		p_data.Healing_LV = 30;
		p_data.Hpoint = 300;
		p_data.Recover_LV = 20;
		p_data.Rpoint = 200;
		break;
	case 17:
		normal_action = NORMAL;
		M_run_flg_rnd = 2;
		p_data.Attack = 30;
		p_data.Magic = 38;
		p_data.Agility = 34;
		p_data.Vitality = 34;
		p_data.Exp = 55;
		p_data.LV = 34;
		p_data.FlameATT_LV = 35;
		p_data.Fpoint = 350;
		p_data.Healing_LV = 30;
		p_data.Hpoint = 300;
		p_data.Recover_LV = 0;
		p_data.Rpoint = 10;
		break;
	case 18:
		normal_action = MOVE;
		M_run_flg_rnd = 2;
		p_data.Attack = 33;
		p_data.Magic = 39;
		p_data.Agility = 36;
		p_data.Vitality = 36;
		p_data.Exp = 50;
		p_data.LV = 36;
		p_data.FlameATT_LV = 35;
		p_data.Fpoint = 350;
		p_data.Healing_LV = 0;
		p_data.Hpoint = 10;
		p_data.Recover_LV = 40;
		p_data.Rpoint = 400;
		break;
	case 19:
		normal_action = NORMAL;
		M_run_flg_rnd = 2;
		p_data.Attack = 50;
		p_data.Magic = 50;
		p_data.Agility = 50;
		p_data.Vitality = 50;
		p_data.Exp = 150;
		p_data.LV = 50;
		p_data.FlameATT_LV = 50;
		p_data.Fpoint = 500;
		p_data.Healing_LV = 50;
		p_data.Hpoint = 500;
		p_data.Recover_LV = 50;
		p_data.Rpoint = 500;
		size_x = 70.0f;
		size_y = 70.0f;
		break;
	}
	p_data.HP = s_MHP();
	p_data.MP = s_MMP();
	esc = TRUE;

	PosOffset(o_no);

	EffectGetBuffer();

	en = new PolygonData();
	en->GetVBarray(CONTROL_POINT, 1);

	mag = new PolygonData();
	mag->GetVBarray(SQUARE, 1);
}

//@Override
void EnemySide::SetVertex() {
	Enemycreate(size_x, size_y);
	//マジック左上
	mag->SetVertex(0, 0,
		(float)-35.0f, (float)-35.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f);

	//マジック右上
	mag->SetVertex(1, 4, 1,
		(float)35.0f, (float)-35.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f);

	//マジック左下
	mag->SetVertex(2, 3, 2,
		(float)-35.0f, (float)35.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f);

	//マジック右下
	mag->SetVertex(5, 3,
		(float)35.0f, (float)35.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f);
}

//@Override
void EnemySide::SetCommandList(int com_no) {
	comNo = com_no;
	for (int i = 0; i < 4; i++)effect[i].SetCommandList(comNo);
	en->SetCommandList(comNo);
	mag->SetCommandList(comNo);
}

//@Override
void EnemySide::CreateEnemy() {
	EffectCreate();
	char str[20];
	sprintf_s(str, sizeof(char) * 20, "enemy%d.jpg", e_no + 1);
	en->Create(TRUE, dx->GetTexNumber(str), TRUE, TRUE);
	mag->Create(FALSE, dx->GetTexNumber("side_magic.jpg"), TRUE, TRUE);
}

//@Override
void EnemySide::AttackAction() {
	float m = tfloat.Add(0.15f);
	if (effect_f == FALSE) {
		if (e_pos[o_no].theta >= 338.0f || e_pos[o_no].theta <= 22.0f) {
			if (zoom == TRUE && (mov_y += m) > 30.0f)zoom = FALSE;
			if (zoom == FALSE && (mov_y -= m) < 0.0f) {
				zoom = TRUE;
				mov_y = 0.0f;
				effect_f = TRUE;
			}
		}
		if (e_pos[o_no].theta >= 68.0f && e_pos[o_no].theta <= 112.0f) {
			if (zoom == TRUE && (mov_x -= m) < -30.0f)zoom = FALSE;
			if (zoom == FALSE && (mov_x += m) > 0.0f) {
				zoom = TRUE;
				mov_y = 0.0f;
				effect_f = TRUE;
			}
		}
		if (e_pos[o_no].theta >= 158.0f && e_pos[o_no].theta <= 202.0f) {
			if (zoom == TRUE && (mov_y -= m) < -30.0f)zoom = FALSE;
			if (zoom == FALSE && (mov_y += m) > 0.0f) {
				zoom = TRUE;
				mov_y = 0.0f;
				effect_f = TRUE;
			}
		}
		if (e_pos[o_no].theta >= 248.0f && e_pos[o_no].theta <= 292.0f) {
			if (zoom == TRUE && (mov_x += m) > 30.0f)zoom = FALSE;
			if (zoom == FALSE && (mov_x -= m) < 0.0f) {
				zoom = TRUE;
				mov_y = 0.0f;
				effect_f = TRUE;
			}
		}
	}
}

void EnemySide::DamageAction() {

	float m = tfloat.Add(0.05f);
	if ((count += m) < 10) {
		int rnd = rand() % 20;
		rnd -= 10;
		mov_x = (float)rnd;
		rnd = rand() % 20;
		rnd -= 10;
		mov_y = (float)rnd;
		rnd = rand() % 20;
		rnd -= 10;
		mov_z = (float)rnd;
	}
	else {
		count = 0.0f;
		act_f = normal_action;
		mov_x = mov_y = mov_z = 0.0f;
	}
}

//@Override
void EnemySide::RecoverActionInit() {
	mov_z = -50.0f;
	act_f = RECOVER;
}

//@Override
void EnemySide::RecoverAction() {

	float m = tfloat.Add(0.03f);
	if ((mov_z += m) > 0.0f) {
		mov_z = 0.0f;
		act_f = normal_action;
	}
}

//@Override
bool EnemySide::LostAction(float x, float y, float z) {

	float m = tfloat.Add(0.2f);
	if ((mov_z -= m) < -100.0f) {
		//↓LOST中はひたすらLOSTフラグが入ってくるので0より下がらないようにする。
		//act_fもそのまま
		mov_z = 0.0f; return TRUE;
	}
	return FALSE;
}

//@Override
bool EnemySide::Magiccreate(float x, float y, float z) {

	MovieSoundManager::Magic_sound(TRUE);
	mag->Update(x + mov_x, y + mov_y, z + 5.0f + mov_z, 0, 0, 0, 0, count, 0);
	float m = tfloat.Add(0.15f);
	if ((count += m) > 100) {
		count = 0.0f;
		mag->DrawOff();
		return FALSE;
	}
	return TRUE;
}

//@Override
void EnemySide::ObjUpdate(float x, float y, float z, float r, float g, float b, float theta) {
	en->Update(x, y, z, r, g, b, 0.0f, theta, 0);
}

//@Override
bool EnemySide::M_run_flg() {

	int rnd = rand() % M_run_flg_rnd;
	if (rnd == 1)return TRUE;
	else return FALSE;
}

//@Override
void EnemySide::M_select(int *r, int *r1) {

	//マジック選択 ランダム数 0:Flame, 1:Healing, 2:Recover 
	if (GetFlameATT_LV() != 0) {
		if (GetHealing_LV() != 0 && GetRecover_LV() != 0)*r = rand() % 3;
		if (GetHealing_LV() != 0 && GetRecover_LV() == 0)*r = rand() % 2;
		if (GetHealing_LV() == 0 && GetRecover_LV() != 0) {
			*r = rand() % 2;
			if (*r == 1)*r = 2;
		}
		if (GetHealing_LV() == 0 && GetRecover_LV() == 0)*r = 0;
	}
	if (GetHealing_LV() != 0) {
		if (GetFlameATT_LV() == 0 && GetRecover_LV() != 0) {
			*r = rand() % 2;
			if (*r == 0)*r = 1;
			else *r = 2;
		}
		if (GetFlameATT_LV() == 0 && GetRecover_LV() == 0)*r = 1;
	}
	if (GetRecover_LV() != 0 && GetFlameATT_LV() == 0 && GetHealing_LV() == 0)*r = 2;

	//選択マジック毎の処理,全体,個体選択
	*r1 = rand() % 2;
}

EnemySide::~EnemySide() {
	S_DELETE(mag);
	S_DELETE(en);
}