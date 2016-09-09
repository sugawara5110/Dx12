//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@	         Battle�N���X                                    **//
//**                                      Fight�֐�                                      **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <new>     //placement new
#include <stdlib.h>
#include <time.h>
#include "Enemy.h"
#include "EnemySide.h"
#include "EnemyBoss.h"
#include "Battle.h"
#include "Hero.h"

Result Battle::Fight(Hero *hero, Directionkey direction, Result result) {

	Position::H_Pos h_posOut;//���_�ϊ���p
	//���_�����l
	h_posOut.cx = h_pos->cx;//���_
	h_posOut.cy = h_pos->cy;//���_
	h_posOut.cx2 = h_pos->cx2;//�����_
	h_posOut.cy2 = h_pos->cy2;//�����_
	h_posOut.cz = h_pos->cz;
	h_posOut.theta = h_pos->theta;

	dx->Bigin(ENEMY_COM, nullptr);

	Debug(enemy);

	//�퓬�J�n���㎋�_
	if (!battlefirst) {
		battlefirst = Position::CamRecession(h_pos, &h_posOut, tfloat.Add(0.1f));
		if (battlefirst)Position::CamRecession(NULL, NULL, 0);//������(���������Ă����Ȃ��Ǝ���퓬���@�\���Ȃ�)
	}

	if (CamActInd != -1) {
		Position::CamActionBackView(&b_pos[CamActInd], &h_posOut, tfloat.Add(0.1f), CamActOn, h_draw[CamActInd].action);
		char lr;
		if (CamActInd == 0 || CamActInd == 1)lr = 'L'; else lr = 'R';
		Position::CamActionRoundView(&b_pos[CamActInd], &h_posOut, tfloat.Add(0.3f), CamActOn, h_draw[CamActInd].action, lr);
	}

	float cx = h_posOut.cx;//���_
	float cy = h_posOut.cy;//���_
	float cx2 = h_posOut.cx2;//�����_
	float cy2 = h_posOut.cy2;//�����_
	float cz = h_posOut.cz;

	dx->Cameraset(cx, cx2, cy, cy2, cz, cz);
	Act_fin_flg act;
	//�G�퓬�s�\�A�N�V�����t���O
	//LOST�͔��������炸���ƂȂ̂Œ���
	for (int i = 0; i < e_num; i++)if (enemy[i].Dieflg() == TRUE)e_draw[i].action = LOST;
	//�G�\��
	for (int i = 0; i < e_num; i++) {
		if (e_draw[i].LOST_fin == FALSE) {//�G����LOST_fin==TRUE��Ԃ��ƕ\������Ȃ��悤�ɂ���
			act = enemy[i].Enemydraw(this, &E_select_obj, e_draw[i].action, e_draw[i].Magrun);
			switch (act) {
			case AT_FIN://�G���U��,�񕜏I��
				ValueDraw<Enemy>(enemy, h_draw, e_draw, 4, e_num);//�t���O, action = DAMAGE or RECOVER
				break;
			case LOST_FIN://�G���퓬�s�\�A�N�V�����I��
				e_draw[i].LOST_fin = TRUE;
				break;
			}
		}
		e_draw[i].action = enemy[i].Normal_act_get();//��action�͈�x������OK�ȍ\���Ȃ̂œ��ꂽ�瑦����������B������Ȃ��Ƃ�����action�����ςȂ��B
	}

	//�v���C���[�퓬�s�\�A�N�V�����t���O
	//LOST�͔��������炸���ƂȂ̂Œ���
	for (int i = 0; i < 4; i++)if (hero[i].Dieflg() == TRUE)h_draw[i].action = LOST;
	//�v���C���[�X�e�[�^�X�\��
	for (int i = 0; i < 4; i++) {
		act = hero[i].Statedraw(this, &select_obj, h_pos, e_pos, h_draw[i].AGmeter / METER_MAX, h_draw[i].command_run, h_draw[i].action, h_draw[i].Magrun);
		switch (act) {
		case AT_FIN://�v���C���[���U��,�񕜏I��
			ValueDraw<Hero>(hero, e_draw, h_draw, e_num, 4);//�t���O, action = DAMAGE or RECOVER
			CamActOn = FALSE;
			break;
		case LOST_FIN:
			h_draw[i].LOST_fin = TRUE;
			break;
		}
		h_draw[i].action = NORMAL;
	}

	//Debug(enemy);//�f�o�b�N�p

	//GAMEOVER�����[�v�X�L�b�v
	if (result == DIE) {
		MovieSoundManager::Die_sound(TRUE);
		dx->End(ENEMY_COM);
		return DIE;
	}

	MovieSoundManager::Enemy_sound(TRUE);

	//�������_�I���܂ŌJ��Ԃ�
	if (!battlefirst) {
		dx->End(ENEMY_COM); return IN_BATTLE;
	}

	//�G�U���p�^�[������1�̂���
	E_com_select = E_AT_select(hero);//���[�^�[MAX���s���͑����肷�邪�e���v���[�g�֐��g�p�̊֌W�Ńv���C���[���ƍ��킹�Ă���B

	//�G�U��
	if (time_stop_flg == FALSE && E_com_select != NOSELECT && E_com_select != OTHER) {//��ɓ���
		//���l�ۊǏ�����
		for (int i = 0; i < 4; i++)h_draw[i].DMdata = -1;
		for (int i = 0; i < e_num; i++)e_draw[i].RCVdata = -1;

		if (E_com_select == ATT) {
			for (int i = 0; i < e_num; i++) {
				if (enemy[i].Dieflg() == FALSE && e_draw[i].command_run == TRUE) {//command_run == TRUE��1�̂����Ȃ�Ȃ�
					//��time_stop�n�܂�, action = ATTACK
					ATprocess<Hero, Enemy>(&hero[E_select_obj], &enemy[i], &h_draw[E_select_obj], &e_draw[i]);
					break;//1�̌��o�������͖����̂Ŕ�����
				}
			}
		}
		if (E_com_select == MAG) {
			for (int i = 0; i < e_num; i++) {
				if (enemy[i].Dieflg() == FALSE && e_draw[i].command_run == TRUE) {
					//��time_stop�n�܂�, action = MAGIC
					MAGprocess<Hero, Enemy>(hero, enemy, &enemy[i], &e_draw[i], e_draw, h_draw, &E_select_obj, &E_MAG_select, E_ATT);
					break;
				}
			}
		}
		E_com_select = NOSELECT;
	}
	//�G��
	for (int i = 0; i < e_num; i++) {
		E_drawPos(i);//���̕����\���̈ʒu�v�Z
		RCVdraw<Enemy>(&enemy[i], &e_draw[i], -50.0f, 0.0f);//��time_stop�I���, �t���O�������S�Ď��s
	}
	//�v���C���[�_���[�W
	for (int i = 0; i < 4; i++) {
		H_drawPos(i);
		DMdraw<Hero>(&hero[i], &h_draw[i], -100.0f, 0.0f);//��time_stop�I���, �t���O�������S�Ď��s
	}

	//�v���C���[�S��
	if (hero[0].Dieflg() == TRUE && hero[1].Dieflg() == TRUE && hero[2].Dieflg() == TRUE && hero[3].Dieflg() == TRUE) {
		MovieSoundManager::Enemy_soundoff();
		MovieSoundManager::Die_sound(FALSE);
		SelectPermissionMove(hero);
		dx->End(ENEMY_COM);
		return DIE;
	}

	//�퓬�s�\���R�}���h�I�����j��,�A�N�V����������������Ȃ��Ɛ퓬�s�\�҂̏��ŃR�}���h���͎~�܂����܂�
	for (int i = 0; i < 4; i++) {
		if (hero[i].Dieflg() == TRUE) {
			if (h_draw[i].command_run == TRUE) {
				SelectPermissionMove(hero);
				com_select = NOSELECT;
			}
			hero[i].Act_f_init();
		}
	}

	//�R�}���h����
	//�s����(time_stop_flg == TRUE)�ł��I���\��Ԃɓ����Ă�ꍇ�͑I���̂ݎ��s�ł���
	//�R�}���h���͌�,�s���I(time_stop_flg == FALSE)�ɂȂ�܂ł́��ɓ���Ȃ�(com_select != NOSELECT�̈�)
	if (com_select == NOSELECT || com_select == OTHER) {
		for (int i = 0; i < 4; i++) {
			com_select = H_AT_select(hero, i, direction);
			if (com_select != NOSELECT)break;
		}
	}

	//�v���C���[�U��
	if (time_stop_flg == FALSE && com_select != NOSELECT && com_select != OTHER) {//�I���ς݂̏ꍇ���s�����
		//���l�ۊǏ�����
		for (int i = 0; i < e_num; i++)e_draw[i].DMdata = -1;
		for (int i = 0; i < 4; i++)h_draw[i].RCVdata = -1;

		if (com_select == ATT) {
			for (int i = 0; i < 4; i++) {
				if (hero[i].Dieflg() == FALSE && h_draw[i].command_run == TRUE) {
					//��time_stop�n�܂�, action = ATTACK
					ATprocess<Enemy, Hero>(&enemy[select_obj], &hero[i], &e_draw[select_obj], &h_draw[i]);
					bool f = FALSE;
					if (rand() % 2 == 0)f = TRUE;
					CamActOn = f;
					CamActInd = i;
					//�I�����ړ�������Ȃ��Ƒ��̃����o���R�}���h�I���ł��Ȃ�
					SelectPermissionMove(hero);
					break;
				}
			}
		}
		if (com_select == MAG) {
			for (int i = 0; i < 4; i++) {
				if (hero[i].Dieflg() == FALSE && h_draw[i].command_run == TRUE) {
					//��time_stop�n�܂�, action = MAGIC
					MAGprocess<Enemy, Hero>(enemy, hero, &hero[i], &h_draw[i], h_draw, e_draw, &select_obj, &MAG_select, H_ATT);
					bool f = FALSE;
					if (rand() % 2 == 0)f = TRUE;
					CamActOn = f;
					CamActInd = i;
					//�I�����ړ�������Ȃ��Ƒ��̃����o���R�}���h�I���ł��Ȃ�
					SelectPermissionMove(hero);
					break;
				}
			}
		}
		if (com_select == ESCAPE) {
			int h_esc = 0;
			int e_esc = 0;
			for (int i = 0; i < 4; i++) {
				if (hero[i].Dieflg() == TRUE)continue;
				h_esc += hero[i].s_AG();
			}
			for (int i = 0; i < e_num; i++) {
				if (enemy[i].Dieflg() == TRUE)continue;
				e_esc += enemy[i].s_AG();
			}
			//�G�X�P�[�v�ۊm�F
			bool es = TRUE;
			for (int i = 0; i < e_num; i++) {
				if (enemy[i].Dieflg() == TRUE)continue;
				if (enemy[i].s_esc() == FALSE) {
					es = FALSE;
					break;
				}
			}
			if (es == TRUE && h_esc > e_esc)Escape_s = TRUE;
			else Escape_s = FALSE;
			Escape_f = 300;
			SelectPermissionMove(hero);
			for (int i = 0; i < 4; i++)h_draw[i].AGmeter = 0;
		}
		com_select = NOSELECT;//��������s���Ȃ��ƃR�}���h���͓���Ȃ�
	}
	//�v���C���[��
	for (int i = 0; i < 4; i++) {
		H_drawPos(i);
		RCVdraw<Hero>(&hero[i], &h_draw[i], -100.0f, 0.0f);//��time_stop�I���, �t���O�������S�Ď��s
	}
	//�G�_���[�W
	for (int i = 0; i < e_num; i++) {
		E_drawPos(i);
		DMdraw<Enemy>(&enemy[i], &e_draw[i], -50.0f, 0.0f);//��time_stop�I���, �t���O�������S�Ď��s
	}

	//�G�X�P�[�v�\��
	if (Escapedraw() == FALSE) {
		if (Escape_s == TRUE) {
			dx->End(ENEMY_COM); return WIN;
		}
	}
	//�v���C���[�S�����[�^�[���܂��ĂȂ���Ԃł̑I�����j��
	if (h_draw[0].AGmeter <= METER_MAX && h_draw[1].AGmeter <= METER_MAX && h_draw[2].AGmeter <= METER_MAX && h_draw[3].AGmeter <= METER_MAX) {
		SelectPermissionMove(hero);
	}

	//�G�S�Ł�Exp����
	if (e_draw[0].LOST_fin == TRUE &&
		(e_pos[1].element == FALSE || e_draw[1].LOST_fin == TRUE) &&
		(e_pos[2].element == FALSE || e_draw[2].LOST_fin == TRUE) &&
		(e_pos[3].element == FALSE || e_draw[3].LOST_fin == TRUE)) {
		int GetExp = 0;
		for (int i = 0; i < e_num; i++) {
			if (e_pos[i].element == TRUE)GetExp += enemy[i].s_Exp();
		}
		for (int i = 0; i < 4; i++) {
			if (hero[i].Dieflg() == TRUE)continue;
			hero[i].s_Exp(GetExp);
		}
		for (int i = 0; i < 4; i++) {
			hero[i].Act_f_init();
		}
		dx->End(ENEMY_COM);
		return WIN;
	}

	dx->End(ENEMY_COM);
	return IN_BATTLE;
}

Position::Bt_H_Pos *Battle::GetBtPos(Position::H_Pos *h_p){
	//�퓬���e�I�u�W�F�N�g�z�u�ʒu
	static Position::Bt_H_Pos b_p[4];
	for (int i = 0; i < 4; i++){
		float ajst = ((float)i - 1.5f) * 20.0f;
		switch ((int)h_p->theta){
		case 360:
		case 0:
			b_p[i].BtPos_x1 = h_p->cx1 + ajst;
			b_p[i].BtPos_x2 = h_p->cx2 + ajst;
			b_p[i].BtPos_y1 = h_p->cy1;
			b_p[i].BtPos_y2 = h_p->cy2;
			break;
		case 90:
			b_p[i].BtPos_x1 = h_p->cx1;
			b_p[i].BtPos_x2 = h_p->cx2;
			b_p[i].BtPos_y1 = h_p->cy1 + ajst;
			b_p[i].BtPos_y2 = h_p->cy2 + ajst;
			break;
		case 180:
			b_p[i].BtPos_x1 = h_p->cx1 - ajst;
			b_p[i].BtPos_x2 = h_p->cx2 - ajst;
			b_p[i].BtPos_y1 = h_p->cy1;
			b_p[i].BtPos_y2 = h_p->cy2;
			break;
		case 270:
			b_p[i].BtPos_x1 = h_p->cx1;
			b_p[i].BtPos_x2 = h_p->cx2;
			b_p[i].BtPos_y1 = h_p->cy1 - ajst;
			b_p[i].BtPos_y2 = h_p->cy2 - ajst;
			break;
		}
	}
	return b_p;
}

void Battle::E_drawPos(int i){
	VECTOR3 p3;
	p3.as(e_pos[i].x, e_pos[i].y, e_pos[i].z);
	PolygonData2D::Pos2DCompute(&p3);
	e_draw[i].draw_x = p3.x;
	e_draw[i].draw_y = p3.y;
}

void Battle::H_drawPos(int i){
	VECTOR3 p3;
	p3.as(b_pos[i].BtPos_x1, b_pos[i].BtPos_y1, h_pos->pz * 100.0f);
	PolygonData2D::Pos2DCompute(&p3);
	h_draw[i].draw_x = p3.x;
	h_draw[i].draw_y = p3.y;
}

bool Battle::Escapedraw() {

	float m = tfloat.Add(0.1f);
	if (Escape_f != 0) {
		if ((Escape_f += m) < 330) {
			TCHAR *str = L"�G�X�P�[�v����";
			TCHAR *str1 = L"�G�X�P�[�v���s";
			if (Escape_s == TRUE)text->UpDateText(&str, 300.0f, 300.0f, 35.0f, { 0.0f, 0.7f, 1.0f, 1.0f });
			if (Escape_s == FALSE)text->UpDateText(&str1, 300.0f, 300.0f, 35.0f, { 0.0f, 0.7f, 1.0f, 1.0f });
			return TRUE;
		}
		else {
			Escape_f = 0;
			return FALSE;
		}
	}
	return TRUE;
}

bool Battle::GetH_DM(int element){

	if (h_draw[element].DMdata >= 0)return TRUE;

	return FALSE;
}

bool Battle::GetH_RCV(int element){

	if (h_draw[element].RCVdata >= 0)return TRUE;

	return FALSE;
}

bool Battle::GetE_DM(int element){

	if (e_pos[element].element == FALSE)return FALSE;
	if (e_draw[element].DMdata >= 0)return TRUE;

	return FALSE;
}

bool Battle::GetE_RCV(int element){

	if (e_pos[element].element == FALSE)return FALSE;
	if (e_draw[element].RCVdata >= 0)return TRUE;

	return FALSE;
}

void Battle::Debug(Enemy *enemy) {//�f�o�b�N�p

	for (int i = 0; i < 4; i++) {
		if (e_pos[i].element == FALSE)continue;
		text->UpDateValue(enemy[i].s_HP(), 500.0f, 10.0f + 90.0f * i, 15.0f, 5, { 1.0f, 1.0f, 1.0f, 1.0f });
		text->UpDateValue(enemy[i].s_MHP(), 580.0f, 10.0f + 90.0f * i, 15.0f, 5, { 1.0f, 1.0f, 1.0f, 1.0f });
		text->UpDateValue(enemy[i].s_MP(), 500.0f, 25.0f + 90.0f * i, 15.0f, 5, { 1.0f, 1.0f, 1.0f, 1.0f });
		text->UpDateValue(enemy[i].s_MMP(), 580.0f, 25.0f + 90.0f * i, 15.0f, 5, { 1.0f, 1.0f, 1.0f, 1.0f });
		text->UpDateValue((int)e_draw[i].AGmeter, 500.0f, 40.0f + 90.0f * i, 15.0f, 5, { 1.0f, 1.0f, 1.0f, 1.0f });
	}
}