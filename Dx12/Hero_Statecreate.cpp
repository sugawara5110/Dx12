//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Heroクラス                                      **//
//**                                   Statecreate関数                                   **//
//*****************************************************************************************//

#include "Hero.h"

Hero::Hero(){}

void Hero::P_DataInput(P_Data *p_dat){
	p_data.Attack = p_dat->Attack;
	p_data.Magic = p_dat->Magic;
	p_data.Agility = p_dat->Agility;
	p_data.Vitality = p_dat->Vitality;
	p_data.Exp = p_dat->Exp;
	p_data.LV = p_dat->LV;
	p_data.ParameterPoint = p_dat->ParameterPoint;
	p_data.HP = p_dat->HP;
	p_data.MP = p_dat->MP;
	p_data.FlameATT_LV = p_dat->FlameATT_LV;
	p_data.Healing_LV = p_dat->Healing_LV;
	p_data.Recover_LV = p_dat->Recover_LV;
	p_data.Fpoint = p_dat->Fpoint;
	p_data.Hpoint = p_dat->Hpoint;
	p_data.Rpoint = p_dat->Rpoint;
}

Hero::Hero(int no) {

	o_no = no;
	effect_f = FALSE;
	tx = ty = 0.0f;
	tt = 0;

	map_walk = NULL;
	map_walk_pass = NULL;
	p_att = NULL;
	p_att_pass = NULL;
	p_att_cnt = 0;
	p_att_Ind = 0;

	switch (o_no) {
	case 0:
		map_walk = new MeshData[19];
		map_walk_pass = (char**)malloc(sizeof(char*) * 19);
		for (int i = 0; i < 19; i++) {
			map_walk[i].SetCommandList(HERO_COM);
			map_walk[i].SetState(TRUE, TRUE, FALSE);
			map_walk_pass[i] = (char*)malloc(sizeof(char) * 50);
			sprintf_s(map_walk_pass[i], sizeof(char) * 50, "./dat/mesh/player_walk/player_walk_0000%02d.obj", i);
		}
		MeshData::GetVBarrayThreadArray(map_walk, map_walk_pass, 19);
		//テクスチャ設定
		for (int i = 0; i < 19; i++)map_walk[i].GetTexture();
		if (map_walk_pass != NULL) {
			for (int i = 0; i < 19; i++) {
				free(map_walk_pass[i]);
				map_walk_pass[i] = NULL;
			}
			free(map_walk_pass);
			map_walk_pass = NULL;
		}
		ObjCntMax = 28;
		break;
	case 1:
		ObjCntMax = 33;
		break;
	case 2:
		ObjCntMax = 27;
		break;
	case 3:
		ObjCntMax = 16;
		break;
	}
	p_att = new MeshData[ObjCntMax];
	p_att_pass = (char**)malloc(sizeof(char*) * ObjCntMax);
	for (int i = 0; i < ObjCntMax; i++) {
		p_att[i].SetCommandList(HERO_COM);
		p_att[i].SetState(TRUE, TRUE, FALSE);
		p_att_pass[i] = (char*)malloc(sizeof(char) * 50);
		sprintf_s(p_att_pass[i], sizeof(char) * 50, "./dat/mesh/player%datt/player%datt_0000%02d.obj", o_no + 1, o_no + 1, i + 1);
	}
	MeshData::GetVBarrayThreadArray(p_att, p_att_pass, ObjCntMax);
	//テクスチャ設定
	for (int i = 0; i < ObjCntMax; i++)p_att[i].GetTexture();
	//パスはもう使わないのでここで解放
	if (p_att_pass != NULL) {
		for (int i = 0; i < ObjCntMax; i++) {
			free(p_att_pass[i]);
			p_att_pass[i] = NULL;
		}
		free(p_att_pass);
		p_att_pass = NULL;
	}

	state.SetCommandList(HERO_COM);
	state.GetVBarray2D(1);
	state.CreateBox(0.0f, 0.0f, 0.0f, 0.1f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, TRUE, TRUE);
	meter.SetCommandList(HERO_COM);
	meter.GetVBarray2D(1);
	meter.CreateBox(0.0f, 0.0f, 0.0f, 0.1f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, TRUE, TRUE);

	mag.SetCommandList(HERO_COM);
	mag.GetVBarray(SQUARE, 1);
	Magiccreate();
	mag.Create(FALSE, 60, TRUE, TRUE);

	for (int i = 0; i < 4; i++) {
		effect[i].SetCommandList(HERO_COM);
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
		effect[i].Create(FALSE, 81 + i, TRUE, TRUE);
	}

	mov_y = 0.0f;
	mov_x = 0.0f;
	mov_z = 0.0f;
	act_f = NORMAL;
	up = TRUE;
	count = 0.0f;
	LA = LA_x = LA_y = 0.0f;
}

void Hero::Statecreate(bool command_run) {

	static bool clr_f = TRUE;
	static float r = 1.0f;
	float m = tfloat.Add(0.002f);
	if (command_run == FALSE) { r = 0.5; }
	if (command_run == TRUE) {
		if (clr_f) {
			if ((r -= m) <= 0.0f)clr_f = FALSE;
		}
		else {
			if ((r += m) >= 1.0f)clr_f = TRUE;
		}
	}

	float x;
	if (o_no == 0)x = 10.0f;
	if (o_no == 1)x = 180.0f;
	if (o_no == 2)x = 360.0f;
	if (o_no == 3)x = 530.0f;

	//ステータスウインドウ
	state.Draw(x, 450.0f, 0.2f, r, r, r, 0.7f, 120.0f, 100.0f);
}

void Hero::Metercreate(float me) {

	float x;
	if (o_no == 0)x = 10.0f;
	if (o_no == 1)x = 180.0f;
	if (o_no == 2)x = 360.0f;
	if (o_no == 3)x = 530.0f;

	//メーター1
	meter.InstancedSetConstBf(x + 1.0f, 539.0f, 0.1f, 1.0f, 1.0f, 1.0f, 1.0f, 118.0f, 10.0f);

	//メーター2
	meter.Draw(x + 3.0f, 541.0f, 0.0f, 1.0f, 0.5f, 0.0f, 1.0f, 114.0f * me, 6.0f);
}

void Hero::Magiccreate(){

	//マジック左上
	mag.SetVertex(0, 0,
		(float)-25.0f, (float)-25.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f);

	//マジック右上
	mag.SetVertex(1, 4, 1,
		(float)25.0f, (float)-25.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f);

	//マジック左下
	mag.SetVertex(2, 3, 2,
		(float)-25.0f, (float)25.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f);

	//マジック右下
	mag.SetVertex(5, 3,
		(float)25.0f, (float)25.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f);
}

bool Hero::Effectdraw(Battle *battle, int *select_obj, Position::H_Pos *h_pos, Position::E_Pos *e_pos) {

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

	if (effect_no == 0 || effect_no == 1) {
		if (effect_no == 0)MovieSoundManager::Att_sound(TRUE);
		if (effect_no == 1)MovieSoundManager::Flame_sound(TRUE);
		float r, g, b;
		if (effect_no == 0) { r = 1.0f, g = 1.0f, b = 1.0f; }
		if (effect_no == 1) { r = 0.7f, g = 0.3f, b = 0.2f; }
		if (*select_obj != 4) {
			effect[effect_no].Draw(e_pos[*select_obj].x + ex, e_pos[*select_obj].y + ey, e_pos[*select_obj].z, 0, 0, 0, e_pos[*select_obj].theta, 0.0f, 1.0f, px, py, u_cnt, v_cnt);
			dx->PointLightPosSet(3, e_pos[*select_obj].x + ex, e_pos[*select_obj].y + ey, e_pos[*select_obj].z, r, g, b, 1.0f, 50.0f, 20.0f, 2.0f, TRUE);
		}
		else {
			for (int i = 0; i < 4; i++) {
				if (battle->GetE_DM(i) == FALSE)continue;
				effect[effect_no].InstancedMap(e_pos[i].x + ex, e_pos[i].y + ey, e_pos[i].z, e_pos[i].theta);
				dx->PointLightPosSet(i + 3, e_pos[i].x + ex, e_pos[i].y + ey, e_pos[i].z, r, g, b, 1.0f, 50.0f, 20.0f, 2.0f, TRUE);
			}
			effect[effect_no].InstanceDraw(0.0f, 0.0f, 0.0f, 0.0f, px, py, u_cnt, v_cnt);
		}
	}

	Position::Bt_H_Pos *b_pos = battle->GetBtPos(h_pos);
	if (effect_no == 2 || effect_no == 3) {
		MovieSoundManager::Heal_sound(TRUE);
		float r, g, b;
		if (effect_no == 2) { r = 0.2f, g = 0.7f, b = 0.3f; }
		if (effect_no == 3) { r = 0.2f, g = 0.3f, b = 0.7f; }
		if (*select_obj != 4) {
			effect[effect_no].Draw(b_pos[*select_obj].BtPos_x1, b_pos[*select_obj].BtPos_y1, (float)h_pos->pz * 100.0f, 0, 0, 0, h_pos->theta, 0.0f, 1.0f, px, py, u_cnt, v_cnt);
			dx->PointLightPosSet(3, b_pos[*select_obj].BtPos_x1, b_pos[*select_obj].BtPos_y1, (float)h_pos->pz * 100.0f, r, g, b, 1.0f, 50.0f, 20.0f, 2.0f, TRUE);
		}
		else {
			for (int i = 0; i < 4; i++) {
				if (battle->GetH_RCV(i) == FALSE)continue;
				effect[effect_no].InstancedMap(b_pos[i].BtPos_x1, b_pos[i].BtPos_y1, (float)h_pos->pz * 100.0f, h_pos->theta);
				dx->PointLightPosSet(i + 3, b_pos[i].BtPos_x1, b_pos[i].BtPos_y1, (float)h_pos->pz * 100.0f, r, g, b, 1.0f, 50.0f, 20.0f, 2.0f, TRUE);
			}
			effect[effect_no].InstanceDraw(0.0f, 0.0f, 0.0f, 0.0f, px, py, u_cnt, v_cnt);
		}
	}
	return TRUE;
}

Hero::~Hero(){
	ARR_DELETE(map_walk);
	ARR_DELETE(p_att);
}

