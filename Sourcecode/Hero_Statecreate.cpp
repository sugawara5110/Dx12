//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Heroクラス                                      **//
//**                                   Statecreate関数                                   **//
//*****************************************************************************************//

#include "Hero.h"

void Hero::P_DataInput(P_Data *p_dat) {
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

void Hero::CreateTorchFlame() {
	Vertex to[4] = {
		//左前
		-15.0f, 0.0f, 15.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f,
		//右前
		15.0f, 0.0f, 15.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f,
		//左奥
		-15.0f, 0.0f, -15.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f,
		//右奥
		15.0f, 0.0f, -15.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f
	};
	UINT ind[12] = { 0, 1, 2, 2, 1, 3,
	                 1, 0, 3, 3, 0, 2};
	torchFlame->setVertex(to, 4, ind, 12);
}

void Hero::TorchSwitch(bool f) {
	torchOn = f;
	if (!torchOn)dx->PointLightPosSet(torchFlame->emissiveNo, { 0, 0, 0 },
		{ 1.0f, 0.4f, 0.4f, 1.0f },
		false, 80.0f);
}

Hero::Hero(int no) {
	o_no = no;
	effect_f = FALSE;
	tx = ty = 0.0f;
	tt = 0;
	comNo = 0;
	torchWood = NULL;
	torchFlame = NULL;
	torchOn = TRUE;
	p_att = NULL;
	attOn = attFin = magicAttOn = FALSE;
	float ofsetthetaZ = 0.0f;
	float frameMaxWalk, frameMaxWait, frameMaxAtt0, frameMaxAtt;

	switch (o_no) {
	case 0:
		frameMaxWalk = 800.0f;
		frameMaxWait = 6500.0f;
		frameMaxAtt0 = 500.0f;
		frameMaxAtt = 2900.0f;
		break;
	case 1:
		frameMaxAtt0 = 1300.0f;
		frameMaxAtt = 2500.0f;
		break;
	case 2:
		frameMaxAtt0 = 1000.0f;
		frameMaxAtt = 1500.0f;
		break;
	case 3:
		ofsetthetaZ = 90.0f;
		frameMaxAtt0 = 1000.0f;
		frameMaxAtt = 2000.0f;
		break;
	}
	p_att = new SkinMesh();
	p_att->SetState(false, false);
	char p_att_pass[42];
	char p_att_pass2[50];
	char p_att_pass3[51];
	sprintf_s(p_att_pass, sizeof(char) * 42, "./dat/mesh/player%datt/player%d_FBX_att.fbx", o_no + 1, o_no + 1);
	sprintf_s(p_att_pass2, sizeof(char) * 50, "./dat/mesh/player%datt/player%d_FBX_att0_deform.fbx", o_no + 1, o_no + 1);
	sprintf_s(p_att_pass3, sizeof(char) * 51, "./dat/mesh/player%datt/player%d_FBX_magic_deform.fbx", o_no + 1, o_no + 1);
	p_att->ObjOffset(0.0f, 0.0f, 10.0f, ofsetthetaZ, 0.0f, 0.0f, 0);
	p_att->GetFbx(p_att_pass);
	p_att->GetBuffer(frameMaxAtt);
	p_att->ObjOffset(0.0f, 0.0f, 10.0f, ofsetthetaZ, 0.0f, 0.0f, 1);
	p_att->GetFbxSub(p_att_pass2, 1);
	p_att->GetBuffer_Sub(1, frameMaxAtt0);
	p_att->ObjOffset(0.0f, 0.0f, 10.0f, ofsetthetaZ, 0.0f, 0.0f, 2);
	p_att->GetFbxSub(p_att_pass3, 2);
	p_att->GetBuffer_Sub(2, frameMaxAtt0);

	if (o_no == 0) {
		p_att->ObjCentering(0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 3);
		p_att->GetFbxSub("./dat/mesh/player_walk/player1_FBX_walk_deform.fbx", 3);
		p_att->GetBuffer_Sub(3, frameMaxWalk);
		p_att->ObjOffset(0.0f, 0.0f, 10.0f, 90.0f, 0.0f, 0.0f, 4);
		p_att->GetFbxSub("./dat/mesh/player_walk/player1_FBX_wait_deform.fbx", 4);
		p_att->GetBuffer_Sub(4, frameMaxWait);
		torchWood = new SkinMesh();
		torchWood->SetState(false, TRUE);
		torchWood->Vertex_hold();
		torchWood->GetFbx("./dat/mesh/player_walk/player1_FBX_torch.fbx");
		torchWood->GetBuffer(frameMaxAtt);//0番にはアニメーション入っていない
		torchWood->ObjCentering(0.0f, 0.0f, 8.0f, 0.0f, 0.0f, 0.0f, 3);
		torchWood->GetFbxSub("./dat/mesh/player_walk/player1_FBX_walk_deform.fbx", 3);
		torchWood->GetBuffer_Sub(3, frameMaxWalk);
		torchWood->ObjOffset(0.0f, 0.0f, 8.0f, 90.0f, 0.0f, 0.0f, 4);
		torchWood->GetFbxSub("./dat/mesh/player_walk/player1_FBX_wait_deform.fbx", 4);
		torchWood->GetBuffer_Sub(4, frameMaxWait);
		torchFlame = new EmissiveObj_Po();
		torchFlame->GetVBarray(SQUARE, 1);
	}

	state.GetVBarray2D(1);
	meter.GetVBarray2D(1);
	mag.GetVBarray(SQUARE, 1);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			effect[i][j].GetVBarray(SQUARE, 1);
	mov_y = 0.0f;
	mov_x = 0.0f;
	mov_z = 0.0f;
	act_f = NORMAL;
	up = TRUE;
	count = 0.0f;
	LA = LA_x = LA_y = 0.0f;

	Statecreate_clr_f = TRUE;
	Statecreate_r = 1.0f;
	for (int i = 0; i < 4; i++)for (int j = 0; j < 4; j++)effectOn[i][j] = false;
}

void Hero::SetVertex() {
	p_att->SetVertex();
	p_att->CreateFromFBX_SubAnimation(1);
	p_att->CreateFromFBX_SubAnimation(2);

	if (o_no == 0) {
		p_att->CreateFromFBX_SubAnimation(3);
		p_att->CreateFromFBX_SubAnimation(4);
		torchWood->SetVertex();
		torchWood->CreateFromFBX_SubAnimation(3);
		torchWood->CreateFromFBX_SubAnimation(4);
		CreateTorchFlame();
	}
	Magiccreate();
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			float ver = 25;
			Vertex ef[4] = {
				//左前
				-ver, 0.0f, ver * 2,
				0.0f, 0.0f, 0.0f,
				0.0f, 0.0f,
				//右前
				ver, 0.0f, ver * 2,
				0.0f, 0.0f, 0.0f,
				1.0f, 0.0f,
				//左奥
				-ver, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f,
				0.0f, 1.0f,
				//右奥
				ver, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f,
				1.0f, 1.0f
			};
			UINT ind[6] = { 0, 1, 2, 2, 1, 3 };
			effect[i][j].setVertex(ef, 4, ind, 6);
		}
	}
}

void Hero::SetCommandList(int com_no) {
	comNo = com_no;
	p_att->SetCommandList(comNo);
	if (o_no == 0) {
		torchWood->SetCommandList(comNo);
		torchFlame->SetCommandList(comNo);
	}
	state.SetCommandList(comNo);
	meter.SetCommandList(comNo);
	mag.SetCommandList(comNo);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			effect[i][j].SetCommandList(comNo);
}

void Hero::CreateHero() {
	p_att->CreateFromFBX();
	if (o_no == 0) {
		torchWood->CreateFromFBX();
		torchFlame->TextureInit(256, 256);
		torchFlame->setMaterialType(EMISSIVE);
		torchFlame->Create(FALSE, -1, TRUE, true);
	}
	state.CreateBox(0.0f, 0.0f, 0.0f, 0.1f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, TRUE, TRUE);
	meter.CreateBox(0.0f, 0.0f, 0.0f, 0.1f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, TRUE, TRUE);
	mag.Create(FALSE, dx->GetTexNumber("side_magic.jpg"), TRUE, TRUE);
	for (int i = 0; i < 4; i++) {
		effect[0][i].setMaterialType(EMISSIVE);
		effect[1][i].setMaterialType(EMISSIVE);
		effect[2][i].setMaterialType(EMISSIVE);
		effect[3][i].setMaterialType(EMISSIVE);

		effect[0][i].Create(FALSE, dx->GetTexNumber("h_att.jpg"), TRUE, TRUE);
		effect[1][i].Create(FALSE, dx->GetTexNumber("flame.jpg"), TRUE, TRUE);
		effect[2][i].Create(FALSE, dx->GetTexNumber("healing.jpg"), TRUE, TRUE);
		effect[3][i].Create(FALSE, dx->GetTexNumber("recov.jpg"), TRUE, TRUE);
	}
}

void Hero::Statecreate(bool command_run) {

	float m = tfloat.Add(0.002f);
	if (command_run == FALSE) { Statecreate_r = 0.5f; }
	if (command_run == TRUE) {

		if (Statecreate_clr_f) {
			if ((Statecreate_r -= m) <= 0.0f)Statecreate_clr_f = FALSE;
		}
		else {
			if ((Statecreate_r += m) >= 1.0f)Statecreate_clr_f = TRUE;
		}
	}

	float x;
	if (o_no == 0)x = 10.0f;
	if (o_no == 1)x = 180.0f;
	if (o_no == 2)x = 360.0f;
	if (o_no == 3)x = 530.0f;

	//ステータスウインドウ
	state.Update(x, 450.0f, 0.2f, Statecreate_r, Statecreate_r, Statecreate_r, 0.7f, 120.0f, 100.0f);
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
	meter.Update(x + 3.0f, 541.0f, 0.0f, 1.0f, 0.5f, 0.0f, 1.0f, 114.0f * me, 6.0f);
}

void Hero::Magiccreate() {
	Vertex m[4] = {
		//マジック左上
		(float)-25.0f, (float)-25.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f,
		//マジック右上
		(float)25.0f, (float)-25.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f,
		//マジック左下
		(float)-25.0f, (float)25.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		//マジック右下
		(float)25.0f, (float)25.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f
	};
	UINT ind[6] = { 0, 1, 2, 2, 1, 3 };
	mag.setVertex(m, 4, ind, 6);
}

bool Hero::EffectUpdate(Battle* battle, int* select_obj, Position::H_Pos* h_pos, Position::E_Pos* e_pos) {

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
			for (int k = 0; k < 4; k++) {
				for (int j = 0; j < 4; j++) {
					int emissiveNo = effect[k][j].emissiveNo;
					dx->PointLightPosSet(emissiveNo, { 0, 0, 0 },
						{ 0, 0, 0, 0 },
						false, 0);
					effect[k][j].DrawOff();
					effectOn[k][j] = false;
				}
			}
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
			effect[effect_no][0].Update({ e_pos[*select_obj].x + ex, e_pos[*select_obj].y + ey, e_pos[*select_obj].z },
				{ 0, 0, 0, 0 },
				{ 0,0,e_pos[*select_obj].theta },
				{ 1,1,1 },
				0.0f, 4.0f, px, py, u_cnt, v_cnt);
			dx->PointLightPosSet(effect[effect_no][0].emissiveNo, { e_pos[*select_obj].x + ex, e_pos[*select_obj].y + ey, e_pos[*select_obj].z },
				{ r, g, b, 1.0f },
				true, 500.0f);
			effectOn[effect_no][0] = true;
		}
		else {
			for (int i = 0; i < 4; i++) {
				if (battle->GetE_DM(i) == FALSE)continue;
				effect[effect_no][i].Instancing({ e_pos[i].x + ex, e_pos[i].y + ey, e_pos[i].z },
					{ e_pos[i].theta, 0, 0 },
					{ 1,1,1 });
				dx->PointLightPosSet(i + effect[effect_no][i].emissiveNo, { e_pos[i].x + ex, e_pos[i].y + ey, e_pos[i].z },
					{ r, g, b, 1.0f },
					true, 500.0f);
				effect[effect_no][i].InstancingUpdate({ 0.0f, 0.0f, 0.0f, 0.0f }, 0.0f, 4.0f, px, py, u_cnt, v_cnt);
				effectOn[effect_no][i] = true;
			}
		}
	}

	Position::Bt_H_Pos* b_pos = battle->GetBtPos(h_pos);
	if (effect_no == 2 || effect_no == 3) {
		MovieSoundManager::Heal_sound(TRUE);
		float r, g, b;
		if (effect_no == 2) { r = 0.2f, g = 0.7f, b = 0.3f; }
		if (effect_no == 3) { r = 0.2f, g = 0.3f, b = 0.7f; }
		if (*select_obj != 4) {
			effect[effect_no][0].Update({ b_pos[*select_obj].BtPos_x1, b_pos[*select_obj].BtPos_y1, (float)h_pos->pz * 100.0f },
				{ 0, 0, 0, 0 },
				{ 0,0,h_pos->theta },
				{ 1,1,1 },
				0.0f, 4.0f, px, py, u_cnt, v_cnt);
			dx->PointLightPosSet(effect[effect_no][0].emissiveNo, { b_pos[*select_obj].BtPos_x1, b_pos[*select_obj].BtPos_y1, (float)h_pos->pz * 100.0f },
				{ r, g, b, 1.0f },
				true, 500.0f);
			effectOn[effect_no][0] = true;
		}
		else {
			for (int i = 0; i < 4; i++) {
				if (battle->GetH_RCV(i) == FALSE)continue;
				effect[effect_no][i].Instancing({ b_pos[i].BtPos_x1, b_pos[i].BtPos_y1, (float)h_pos->pz * 100.0f },
					{ h_pos->theta,0,0 },
					{ 1,1,1 });
				dx->PointLightPosSet(i + effect[effect_no][i].emissiveNo, { b_pos[i].BtPos_x1, b_pos[i].BtPos_y1, (float)h_pos->pz * 100.0f },
					{ r, g, b, 1.0f },
					true, 500.0f);
				effect[effect_no][i].InstancingUpdate({ 0.0f, 0.0f, 0.0f, 0.0f }, 0.0f, 4.0f, px, py, u_cnt, v_cnt);
				effectOn[effect_no][i] = true;
			}
		}
	}
	return TRUE;
}

Hero::~Hero() {
	S_DELETE(torchWood);
	S_DELETE(torchFlame);
	S_DELETE(p_att);
}

