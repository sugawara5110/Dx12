//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Mapクラス                                       **//
//**                                     Move関数                                        **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include "Map.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

//四捨五入
int rounding(int val, int digit_number){

	int v = 5 * (int)pow(10.0, (double)digit_number - 1.0);
	val += v;
	return val -= (val % (int)pow(10.0, (double)digit_number));
}

//downNum切り下げ
//upNum切り上げ
int rounding2(int val, int digit_num, int downNum, int upNum) {
	int d = (int)pow(10.0, (double)digit_num - 1.0);
	int valT = val / d;
	int valT2 = valT / 10;
	if ((valT % 10) >= upNum)return (valT2 + 1) * d * 10;
	if ((valT % 10) <= downNum)return (valT2 - 1) * d * 10;
	return val;
}

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

Encount Map::Move(MapState *mapstate, Directionkey direction) {

	float cax1Tmp = cax1;
	float cax2Tmp = cax2;
	float cay1Tmp = cay1;
	float cay2Tmp = cay2;
	float src_thetaTmp = src_theta;
	float m = tfloat.Add(0.4f);

	//移動処理
	float yt, xt, xyt;
	if (!elevator_UP && !elevator_DOWN) {

		switch (direction) {
		case LEFT:
			src_thetaTmp = src_thetaTmp - m;
			if (src_thetaTmp <= 0)src_thetaTmp = 360;
			cay2Tmp = cay1Tmp - cos(src_thetaTmp * (float)M_PI / 180.0f) * 70.0f;
			cax2Tmp = cax1Tmp + sin(src_thetaTmp * (float)M_PI / 180.0f) * 70.0f;
			break;

		case RIGHT:
			src_thetaTmp = src_thetaTmp + m;
			if (src_thetaTmp >= 360)src_thetaTmp = 0;
			cay2Tmp = cay1Tmp - cos(src_thetaTmp * (float)M_PI / 180.0f) * 70.0f;
			cax2Tmp = cax1Tmp + sin(src_thetaTmp * (float)M_PI / 180.0f) * 70.0f;
			break;

		case UP:
			yt = -cos(src_thetaTmp * (float)M_PI / 180.0f) * 70.0f;
			xt = sin(src_thetaTmp * (float)M_PI / 180.0f) * 70.0f;
			xyt = fabs(yt) + fabs(xt);
			cay1Tmp += yt / xyt * m;
			cay2Tmp += yt / xyt * m;
			cax1Tmp += xt / xyt * m;
			cax2Tmp += xt / xyt * m;
			break;

		case DOWN:
			yt = -cos(src_thetaTmp * (float)M_PI / 180.0f) * 70.0f;
			xt = sin(src_thetaTmp * (float)M_PI / 180.0f) * 70.0f;
			xyt = fabs(yt) + fabs(xt);
			cay1Tmp -= yt / xyt * m;
			cay2Tmp -= yt / xyt * m;
			cax1Tmp -= xt / xyt * m;
			cax2Tmp -= xt / xyt * m;
			break;
		}
	}

	//当たり判定
	int indy = posz * mxy.y * mxy.x + (int)(rounding2((int)cay1Tmp, 2, 1, 9) * 0.01f) * mxy.x + (int)(cax1Tmp * 0.01f);
	int indx = posz * mxy.y * mxy.x + (int)(cay1Tmp * 0.01f) * mxy.x + (int)(rounding2((int)cax1Tmp, 2, 1, 9) * 0.01f);

	if (direction == UP) {
		if (MoveUpCond(indy) || MoveUpCond(indx))return NOENCOUNT;
	}
	if (direction == DOWN) {
		if (MoveDownCond(indy) || MoveDownCond(indx))return NOENCOUNT;
	}
	//座標更新
	cax1 = cax1Tmp;
	cax2 = cax2Tmp;
	cay1 = cay1Tmp;
	cay2 = cay2Tmp;
	src_theta = src_thetaTmp;
	posx = (int)(cax1 * 0.01f);
	posy = (int)(cay1 * 0.01f);

	//エレベータA上到達
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
	//エレベータB下到達
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

	//出口1ポイント到達
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

	//出口2ポイント到達
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

	//入口ポイント到達
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

	//回復ポイント処理
	if (mxy.m[POS_CE] == 50 && recover_p_f == FALSE) {
		recover_p_f = TRUE;
		map_text_f = 300;
		_tcscpy_s(m_tx, L"ＨＰＭＰ全回復！！");
		*mapstate = RECOV_MAP;
	}
	else if (mxy.m[POS_CE] != 50)recover_p_f = FALSE;

	//ボスポイント到達
	if (mxy.m[POS_CE] == 51 && boss_p_f == FALSE) {
		boss_p_f = TRUE;//何度もreturn BOSS出さない
		return BOSS;
	}
	else if (mxy.m[POS_CE] != 51)boss_p_f = FALSE;//ボスポイント離れたらFALSEにする
	//通常エンカウント
	int rnd = 0;
	if (direction == UP)rnd = rand() % 500;
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