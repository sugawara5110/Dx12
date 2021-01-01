//*****************************************************************************************//
//**                                                                                     **//
//**                   Å@Å@Å@	         HeroÉNÉâÉX                                      **//
//**                                   Statedrawä÷êî                                     **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Hero.h"
#define DISP_SIZE 0.3f

void Hero::OBJWalkUpdate(float x, float y, float z, float r, float g, float b, float theta) {
	p_att->Update(3, -1.0f, { x, y, z }, { r, g, b, 0.0f }, { 0,0,theta }, { 2.0f,2.0f,2.0f });
}

void Hero::OBJWalkUpdate(float x, float y, float z, float r, float g, float b, float theta, bool walkOn) {

	VECTOR3 v3;
	int i;
	float m = tfloat.Add(2.0f);
	if (!walkOn) {
		//ê√é~
		i = 4;
	}
	else {
		//à⁄ìÆ
		i = 3;
	}
	p_att->Update(i, m, { x, y, z }, { r, g, b, 0.0f }, { 0,0,theta }, { 2.0f,2.0f,2.0f }, DISP_SIZE);
	if (true/*torchOn*/) {
		torchWood->Update(i, m, { x, y, z }, { r, g, b, 0.0f }, { 0,0,theta }, { 2.0f,2.0f,2.0f });
		v3 = torchWood->GetVertexPosition(0, 4, 3.0f, -1.0f, 0.0f, theta, 0, 0, 2.0f);
		dx->PointLightPosSet(torchFlame->emissiveNo, { v3.x + x, v3.y + y, v3.z + z },
			{ 1.0f, 0.4f, 0.4f, 1.0f },
			true, 1000.0f, { 0.001f,0.001f,0.01f });
		torchFlame->Update({ v3.x + x, v3.y + y, v3.z + z }, { r, g, b, -0.2f }, { 0,0,theta }, { 1,1,1 }, 0.0f);
	}
	else {
		torchWood->DrawOff();
		torchFlame->DrawOff();
		dx->PointLightPosSet(torchFlame->emissiveNo, { 0, 0, 0 },
			{ 1.0f, 0.4f, 0.4f, 1.0f },
			false, 1000.0f, { 0.001f,0.001f,0.01f });
	}
}

Act_fin_flg Hero::HeroUpdate(Battle* battle, int* select_obj, Position::H_Pos* h_pos, Position::E_Pos* e_pos, float me, bool command_run, Action action, MagicSelect H_Magrun) {

	static bool clr_f = TRUE;
	static float r = 1.0f;
	float x;
	if (o_no == 0)x = 20.0f;
	if (o_no == 1)x = 190.0f;
	if (o_no == 2)x = 370.0f;
	if (o_no == 3)x = 540.0f;

	Position::Bt_H_Pos* b_pos = battle->GetBtPos(h_pos);

	//NORMAL,LOSTà»äOÇÃÉAÉNÉVÉáÉìíÜÇ…NORMAL,LOSTà»äOÇÃÉAÉNÉVÉáÉìî≠ê∂éûÇÃèâä˙âª
	if (action != NORMAL && action != LOST) {
		mov_y = 0.0f;
		mov_x = 0.0f;
		mov_z = 0.0f;
		up = TRUE;
		count = 0;
	}

	switch (action) {
	case ATTACK:
		act_f = ATTACK;
		break;
	case MAGIC:
		act_f = MAGIC;
		magicAttOn = TRUE;
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
	switch (act_f) {
	case ATTACK:
		if (effect_f == FALSE) {
			attOn = TRUE;
			if (attFin) {
				attOn = FALSE;
				attFin = FALSE;
				effect_f = TRUE;
			}
		}
		if (effect_f == TRUE) {
			effect_no = 0;
			if (EffectUpdate(battle, select_obj, h_pos, e_pos) == FALSE) {
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
		mag.Update({ b_pos[o_no].BtPos_x1, b_pos[o_no].BtPos_y1, (float)h_pos->pz * 100.0f + 5.0f },
			{ 0, 0, 0, 0 },
			{ 0,0,count += m },
			{ 1,1,1 }, 0);
		VECTOR3 p3;
		p3.as(b_pos[o_no].BtPos_x1, b_pos[o_no].BtPos_y1, (float)h_pos->pz * 100.0f + 20.0f);
		PolygonData2D::Pos2DCompute(&p3);
		mx = p3.x - 130.0f;//à íuï‚ê≥
		my = p3.y - 200.0f;//à íuï‚ê≥
		switch (H_Magrun) {
		case FLAME:
			text->UpDateText(L"ÉtÉåÉCÉÄÇkÇu", mx, my, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
			text->UpDateValue(GetFlameATT_LV(), mx + 180.0f, my, 30.0f, 3, { 1.0f, 1.0f, 1.0f, 1.0f });
			effect_no = 1;
			break;
		case HEAL:
			text->UpDateText(L"ÉqÅ[ÉäÉìÉOÇkÇu", mx, my, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
			text->UpDateValue(GetHealing_LV(), mx + 210.0f, my, 30.0f, 3, { 1.0f, 1.0f, 1.0f, 1.0f });
			effect_no = 2;
			break;
		case RECOV:
			text->UpDateText(L"ÉäÉJÉoÉäÇkÇu", mx, my, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
			text->UpDateValue(GetRecover_LV(), mx + 180.0f, my, 30.0f, 3, { 1.0f, 1.0f, 1.0f, 1.0f });
			effect_no = 3;
			break;
		case NOSEL:
			text->UpDateText(L"MPÇ™ë´ÇËÇ»Ç¢", mx, my, 30.0f, { 1.0f, 0.5f, 0.5f, 1.0f });
			break;
		}
		if (effect_f == FALSE && (count += m) > 200.0f) {
			count = 0;
			magicAttOn = FALSE;
			if (H_Magrun == NOSEL) {
				act_f = NORMAL;
				mag.DrawOff();
				return AT_FIN;
			}
			effect_f = TRUE;
		}
		if (effect_f == TRUE) {
			if (EffectUpdate(battle, select_obj, h_pos, e_pos) == FALSE) {
				effect_f = FALSE;
				act_f = NORMAL;
				mag.DrawOff();
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
			LA = 90.0f;//Ç–ÇΩÇ∑ÇÁâ¡éZÇñhé~
		}
		break;
	case RECOVER:
		m = tfloat.Add(1.0f);
		if ((LA -= m) <= 0.0f) {
			LA = 0.0f;
			act_f = NORMAL;//èIÇÌÇËéüëÊÉmÅ[É}ÉãÇ…ñﬂÇ∑
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

	m = tfloat.Add(2.0f);
	if (Dieflg() == TRUE)m = 0.0f;
	if (attOn) {
		attFin = p_att->Update(0, m,
			{ b_pos[o_no].BtPos_x1 + mov_x, b_pos[o_no].BtPos_y1 + mov_y, (float)h_pos->pz * 100.0f + mov_z + LA / 9.0f },
			{ 0, 0, 0, 0 },
			{ LA_x, LA_y,h_pos->theta },
			{ 2.0f,2.0f,2.0f },
			DISP_SIZE);
	}
	if (magicAttOn) {
		p_att->Update(2, m * 0.3f,
			{ b_pos[o_no].BtPos_x1 + mov_x, b_pos[o_no].BtPos_y1 + mov_y, (float)h_pos->pz * 100.0f + mov_z + LA / 9.0f },
			{ 0, 0, 0, 0 },
			{ LA_x, LA_y,h_pos->theta },
			{ 2.0f,2.0f,2.0f },
			DISP_SIZE);
	}
	if (!attOn && !magicAttOn) {
		p_att->Update(1, m * 0.5f,
			{ b_pos[o_no].BtPos_x1 + mov_x, b_pos[o_no].BtPos_y1 + mov_y, (float)h_pos->pz * 100.0f + mov_z + LA / 9.0f },
			{ 0, 0, 0, 0 },
			{ LA_x, LA_y,h_pos->theta },
			{ 2.0f,2.0f,2.0f },
			DISP_SIZE);
	}

	Statecreate(command_run);
	text->UpDateText(L"é˙êlÇmÇè", x, 470.0f, 15.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateValue(o_no, x + 60.0f, 470.0f, 15.0f, 1, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateText(L"ÇgÇo", x - 5.0f, 490.0f, 15.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateValue(p_data.HP, x + 25.0f, 490.0f, 15.0f, 5, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateText(L"ÇlÇo", x - 5.0f, 510.0f, 15.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateValue(p_data.MP, x + 25.0f, 510.0f, 15.0f, 5, { 1.0f, 1.0f, 1.0f, 1.0f });
	if (p_data.HP <= 0) {
		meter.DrawOff();
		text->UpDateText(L"êÌì¨ïsî\", x, 525.0f, 25.0f, { 1.0f, 0.0f, 0.0f, 1.0f });
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
		meter.DrawOff();
		text->UpDateText(L"ó’êÌë‘ê®", x, 525.0f, 25.0f, { 1.0f, 1.0f, 1.0f, r });
		return NOT_FIN;
	}
	Metercreate(me);

	for (int i = 0; i < 4; i++)for (int j = 0; j < 4; j++) {
		if (!effectOn[i][j]) {
			effect[i][j].DrawOff();
			int emissiveNo = effect[i][j].emissiveNo;
			dx->PointLightPosSet(emissiveNo, { 0, 0, 0 },
				{ 0, 0, 0, 0 },
				false, 0);
		}
	}

	//LOSTèÛë‘
	if (act_f == LOST && LA == 90)return LOST_FIN;
	//LOSTà»äO
	return NOT_FIN;
}

void Hero::SetMovie(Encount enc) {
	if (enc == NOENCOUNT && torchWood) {
		torchFlame->SetTextureMPixel(MovieSoundManager::Torch_GetFrame(128, 128));
	}
}

void Hero::Draw(Encount enc, bool ending) {
	if (ending) {
		p_att->DrawOff();
		return;
	}
	if (enc == NOENCOUNT) {
		if (o_no == 0) {
			p_att->Draw();
		}
		else {
			p_att->DrawOff();
		}
		if (torchWood) {//o_no==0à»äOÇÕNULLÇ…Ç»Ç¡ÇƒÇÈ
			torchWood->Draw();
			torchFlame->Draw();
		}
	}
	else {
		p_att->Draw();
		mag.Draw();
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				effect[i][j].Draw();
		if (torchWood) {
			torchWood->DrawOff();
			torchFlame->DrawOff();
		}
	}
}

void Hero::StreamOutput(Encount enc, bool ending) {
	if (ending) {
		p_att->DrawOff();
		return;
	}
	if (enc == NOENCOUNT) {
		if (o_no == 0) {
			p_att->StreamOutput();
		}
		else {
			p_att->DrawOff();
		}
		if (torchWood) {//o_no==0à»äOÇÕNULLÇ…Ç»Ç¡ÇƒÇÈ
			torchWood->StreamOutput();
			torchFlame->StreamOutput();
		}
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				effect[i][j].DrawOff();
	}
	else {
		p_att->StreamOutput();
		mag.StreamOutput();
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++) {
				effect[i][j].StreamOutput();
			}
		if (torchWood) {
			torchWood->DrawOff();
			torchFlame->DrawOff();
		}
	}
}

void Hero::Draw2D(Encount enc, bool ending) {

	if (ending)return;

	if (enc == NOENCOUNT) {
		state.DrawOff();
		meter.DrawOff();
	}
	else {
		state.Draw();
		meter.Draw();
	}
}

void Hero::Act_f_init() {
	act_f = NORMAL;
}

Action Hero::Normal_act_get() {
	return NORMAL;
}

ParameterDXR** Hero::getParameterDXRMap(int* numPara) {
	int num1 = p_att->getNumMesh();
	int n = 0;
	pdx = std::make_unique<ParameterDXR* []>(num1 + 2);

	for (int i = 0; i < num1; i++)pdx[n++] = p_att->getParameter(i);
	pdx[n++] = torchWood->getParameter(0);
	pdx[n++] = torchFlame->getParameter();
	*numPara = n;
	return pdx.get();
}

void Hero::setPointLightNoMap() {
	torchFlame->emissiveNo = EmissiveCount::getNo();
}

MaterialType* Hero::getMaterialTypeMap() {
	int num1 = p_att->getNumMesh();
	int n = 0;
	materialType = std::make_unique<MaterialType[]>(num1 + 2);
	for (int i = 0; i < num1; i++)materialType[n++] = NONREFLECTION;
	materialType[n++] = NONREFLECTION;
	materialType[n++] = EMISSIVE;
	return materialType.get();
}

ParameterDXR** Hero::getParameterDXRBat(int* numPara) {
	int num1 = p_att->getNumMesh();
	int n = 0;
	pdx = std::make_unique<ParameterDXR* []>(num1 + 17);

	for (int i = 0; i < num1; i++)pdx[n++] = p_att->getParameter(i);
	pdx[n++] = mag.getParameter();
	for (int i = 0; i < 4; i++) {
		pdx[n++] = effect[0][i].getParameter();
		pdx[n++] = effect[1][i].getParameter();
		pdx[n++] = effect[2][i].getParameter();
		pdx[n++] = effect[3][i].getParameter();
	}
	*numPara = n;
	return pdx.get();
}

void Hero::setPointLightNoBat() {
	mag.emissiveNo = EmissiveCount::getNo();
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			effect[i][j].emissiveNo = EmissiveCount::getNo();
}

MaterialType* Hero::getMaterialTypeBat() {
	int num1 = p_att->getNumMesh();
	int n = 0;
	materialType = std::make_unique<MaterialType[]>(num1 + 17);
	for (int i = 0; i < num1; i++)materialType[n++] = NONREFLECTION;
	materialType[n++] = EMISSIVE;
	for (int i = 0; i < 4; i++) {
		materialType[n++] = EMISSIVE;
		materialType[n++] = EMISSIVE;
		materialType[n++] = EMISSIVE;
		materialType[n++] = EMISSIVE;
	}
	return materialType.get();
}