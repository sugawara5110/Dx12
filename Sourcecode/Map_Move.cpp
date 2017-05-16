//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@	         Map�N���X                                       **//
//**                                     Move�֐�                                        **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include "Map.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

bool Map::MoveUpCond(int Ind){
	if (mxy.m[Ind] == 49 ||
		mxy.m[Ind] == 74 ||
		mxy.m[Ind] == 75 ||
		mxy.m[Ind] == 76 ||
		mxy.m[Ind] == 77 ||
		mxy.m[Ind] == 78 ||
		mxy.m[Ind] == 79 ||
		mxy.m[Ind] == 52 ||
		mxy.m[Ind] == 53 ||
		mxy.m[Ind] == 45 ||
		mxy.m[Ind] == 87 ||
		mxy.m[Ind] == 73)return TRUE;
	return FALSE;
}

bool Map::MoveDownCond(int Ind){
	if (MoveUpCond(Ind) ||
		mxy.m[Ind] == 56 ||
		mxy.m[Ind] == 51 ||
		mxy.m[Ind] == 54 ||
		mxy.m[Ind] == 55) return TRUE;
	return FALSE;
}

bool Map::CollisionDetection(float in_y, float in_x, Directionkey dir) {
	int indY = (int)(in_y * 0.01f);
	int indX = (int)(in_x * 0.01f);

	for (int y = -1; y < 2; y++) {
		for (int x = -1; x < 2; x++) {
			int ind = posz * mxy.y * mxy.x + (indY + y) * mxy.x + (indX + x);
			switch (dir) {
			case UP:
				if (MoveUpCond(ind)) {
					float xc = (indX + x) * 100.0f;//�����蔻�蒆�u���b�Nx0�_
					float yc = (indY + y) * 100.0f;//�����蔻�蒆�u���b�Ny0�_
					float xs = xc - 10.0f;//x���������蔻��
					float xe = xc + 110.0f;//x�E��
					float ys = yc - 10.0f;//y��
					float ye = yc + 110.0f;//y��

					if (in_x > xs && in_x < xe && in_y > ys && in_y < ye)return TRUE;
				}
				break;

			case DOWN:
				if (MoveDownCond(ind)) {
					float xc = (indX + x) * 100.0f;//�����蔻�蒆�u���b�Nx0�_
					float yc = (indY + y) * 100.0f;//�����蔻�蒆�u���b�Ny0�_
					float xs = xc - 10.0f;//x���������蔻��
					float xe = xc + 110.0f;//x�E��
					float ys = yc - 10.0f;//y��
					float ye = yc + 110.0f;//y��

					if (in_x > xs && in_x < xe && in_y > ys && in_y < ye)return TRUE;
				}
				break;
			}
		}
	}
	return FALSE;
}

Encount Map::Move(MapState *mapstate, Directionkey direction) {

	float cax1Tmp = cax1;
	float cax2Tmp = cax2;
	float cay1Tmp = cay1;
	float cay2Tmp = cay2;
	float src_thetaTmp = src_theta;
	float m = tfloat.Add(0.4f);

	//�ړ�����
	float yt, xt, xyt;
	if (!elevator_UP && !elevator_DOWN) {

		switch (direction) {
		case LEFT:
			src_thetaTmp = src_thetaTmp - m;
			if (src_thetaTmp <= 0)src_thetaTmp = 360;
			break;

		case RIGHT:
			src_thetaTmp = src_thetaTmp + m;
			if (src_thetaTmp >= 360)src_thetaTmp = 0;
			break;

		case UP:
			yt = -cos(src_thetaTmp * (float)M_PI / 180.0f) * 70.0f;
			xt = sin(src_thetaTmp * (float)M_PI / 180.0f) * 70.0f;
			xyt = fabs(yt) + fabs(xt);
			cay1Tmp += yt / xyt * m;
			cax1Tmp += xt / xyt * m;
			break;

		case DOWN:
			yt = -cos(src_thetaTmp * (float)M_PI / 180.0f) * 70.0f;
			xt = sin(src_thetaTmp * (float)M_PI / 180.0f) * 70.0f;
			xyt = fabs(yt) + fabs(xt);
			cay1Tmp -= yt / xyt * m;
			cax1Tmp -= xt / xyt * m;
			break;
		}
	}

	//�����_�v�Z
	cay2Tmp = cay1Tmp - cos(src_thetaTmp * (float)M_PI / 180.0f) * 70.0f;
	cax2Tmp = cax1Tmp + sin(src_thetaTmp * (float)M_PI / 180.0f) * 70.0f;

	//�����蔻��
	switch (direction) {
	case UP:
		//x����,y�����ʁX�ɔ������, �^�̕����̂ݍX�V����
		if (CollisionDetection(cay1Tmp, cax1Tmp, UP)) {
			if (CollisionDetection(cay1, cax1Tmp, UP)) {
				if (CollisionDetection(cay1Tmp, cax1, UP))
					return NOENCOUNT;
				//���W�X�V
				cay1 = cay1Tmp;
				cay2 = cay2Tmp;
			}
			else {
				//���W�X�V
				cax1 = cax1Tmp;
				cax2 = cax2Tmp;
			}
		}
		else {
			//���W�X�V
			cax1 = cax1Tmp;
			cax2 = cax2Tmp;
			cay1 = cay1Tmp;
			cay2 = cay2Tmp;
		}
		break;

	case DOWN:
		if (CollisionDetection(cay1Tmp, cax1Tmp, DOWN))return NOENCOUNT;
		//���W�X�V
		cax1 = cax1Tmp;
		cax2 = cax2Tmp;
		cay1 = cay1Tmp;
		cay2 = cay2Tmp;
		break;

	default:
		//�����]���̏ꍇ���_�����ς���
		cax2 = cax2Tmp;
		cay2 = cay2Tmp;
		break;
	}

	//���W�X�V
	src_theta = src_thetaTmp;
	posx = (int)(cax1 * 0.01f);
	posy = (int)(cay1 * 0.01f);

	//�G���x�[�^A�㓞�B
	if (!elevator_UP && mxy.m[POS_CE] == 65) {
		if (((int)(cax1) % 100) > 30 && ((int)(cax1) % 100) < 70 &&
			((int)(cay1) % 100) > 30 && ((int)(cay1) % 100) < 70)
			elevator_UP = TRUE;
	}
	if (elevator_UP) {
		if ((elevator_step += tfloat.Add(1.0f)) > 300.0f) {
			posz += 3;
			elevator_step = 0.0f;
			elevator_UP = FALSE;
		}
		return NOENCOUNT;
	}
	//�G���x�[�^B�����B
	if (!elevator_DOWN && mxy.m[POS_CE] == 66) {
		if (((int)(cax1) % 100) > 30 && ((int)(cax1) % 100) < 70 &&
			((int)(cay1) % 100) > 30 && ((int)(cay1) % 100) < 70)
			elevator_DOWN = TRUE;
	}
	if (elevator_DOWN) {
		if ((elevator_step -= tfloat.Add(1.0f)) < -300.0f) {
			posz -= 3;
			elevator_step = 0.0f;
			elevator_DOWN = FALSE;
		}
		return NOENCOUNT;
	}

	//�o��1�|�C���g���B
	if (mxy.m[POS_CE] == 54) {
		*mapstate = CHANGE_MAP;
		switch (map_no) {
		case 0:
			map_no_s = 1;
			MPos = POS_ST;
			break;
		case 1:
			map_no_s = 2;
			MPos = POS_ST;
			break;
		case 2:
			map_no_s = 3;
			MPos = POS_ST;
			break;
		}
		return NOENCOUNT;
	}

	//�o��2�|�C���g���B
	if (mxy.m[POS_CE] == 56) {
		*mapstate = CHANGE_MAP;
		switch (map_no) {
		case 0:
			break;
		case 1:
			map_no_s = 4;
			MPos = POS_ST;
			break;
		}
		return NOENCOUNT;
	}

	//�����|�C���g���B
	if (mxy.m[POS_CE] == 55) {
		*mapstate = CHANGE_MAP;
		switch (map_no) {
		case 0:
			break;
		case 1:
			map_no_s = 0;
			MPos = POS_EN1;
			break;
		case 2:
			map_no_s = 1;
			MPos = POS_EN1;
			break;
		case 3:
			map_no_s = 2;
			MPos = POS_EN1;
			break;
		case 4:
			map_no_s = 1;
			MPos = POS_EN2;
			break;
		}
		return NOENCOUNT;
	}

	//�񕜃|�C���g����
	if (mxy.m[POS_CE] == 50 && recover_p_f == FALSE) {
		recover_p_f = TRUE;
		map_text_f = 300;
		_tcscpy_s(m_tx, L"�g�o�l�o�S�񕜁I�I");
		*mapstate = RECOV_MAP;
	}
	else if (mxy.m[POS_CE] != 50)recover_p_f = FALSE;

	//�{�X�|�C���g���B
	if (mxy.m[POS_CE] == 51 && boss_p_f == FALSE) {
		boss_p_f = TRUE;//���x��return BOSS�o���Ȃ�
		poBoss.DrawOff();
		return BOSS;
	}
	else if (mxy.m[POS_CE] != 51)boss_p_f = FALSE;//�{�X�|�C���g���ꂽ��FALSE�ɂ���
	//�ʏ�G���J�E���g
	int rnd = 0;
	if (direction == UP)rnd = rand() % 1000;
	if (direction == DOWN)rnd = rand() % 150;
	if (rnd == 3) {
		if (mxy.m[POS_CE] != 50 && mxy.m[POS_CE] != 51 && mxy.m[POS_CE] != 65 && mxy.m[POS_CE] != 66) {
			return SIDE;
		}
	}

	return NOENCOUNT;
}

void Map::MapText(TCHAR str[30]) {

	if (map_text_f != 0) {
		if ((map_text_f += tfloat.Add(0.1f)) < 340) {
			text->UpDateText(str, 300.0f, 300.0f, 25, { 0.0f, 1.0f, 1.0f, 1.0f });
		}
		else {
			map_text_f = 0;
		}
	}
}