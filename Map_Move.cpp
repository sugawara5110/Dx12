//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Mapクラス                                       **//
//**                                     Move関数                                        **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
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

Encount Map::Move(MapState *mapstate, Directionkey direction){

	int rnd;

	//一マス分の移動先決定
	if (elevator_UP == FALSE && elevator_DOWN == FALSE && moving == FALSE && direction != NOTPRESS &&
		direction != ENTER && direction != TWOPRESS && direction != CANCEL){
		direction_move = direction;
		if (direction_move == LEFT){
			if (src_theta == 0)src_theta = 360;
			m_theta = src_theta - 90;
		}
		if (direction_move == RIGHT){
			if (src_theta == 360)src_theta = 0;
			m_theta = src_theta + 90;
		}
		if (direction_move == UP){
			if (src_theta == 0 || src_theta == 360){
				stepx = cax1; stepy = cay1 - 100;
			}
			if (src_theta == 90){
				stepx = cax1 + 100; stepy = cay1;
			}
			if (src_theta == 180){
				stepx = cax1; stepy = cay1 + 100;
			}
			if (src_theta == 270){
				stepx = cax1 - 100; stepy = cay1;
			}
		}
		if (direction_move == DOWN){
			if (src_theta == 0 || src_theta == 360){
				stepx = cax1; stepy = cay1 + 100;
			}
			if (src_theta == 90){
				stepx = cax1 - 100; stepy = cay1;
			}
			if (src_theta == 180){
				stepx = cax1; stepy = cay1 - 100;
			}
			if (src_theta == 270){
				stepx = cax1 + 100; stepy = cay1;
			}
		}
		moving = TRUE;
	}

	//エレベータA上到達
	if (elevator_UP == FALSE && mxy.m[POS_CE] == 65){
		elevator_UP = TRUE;
	}
	if (elevator_UP == TRUE){
		if ((elevator_step += tfloat.Add(1.0f)) > 300.0f){
			posz += 3;
			elevator_step = 0.0f;
			elevator_UP = FALSE;
		}
		return NOENCOUNT;
	}
	//エレベータB下到達
	if (elevator_DOWN == FALSE && mxy.m[POS_CE] == 66){
		elevator_DOWN = TRUE;
	}
	if (elevator_DOWN == TRUE){
		if ((elevator_step -= tfloat.Add(1.0f)) < -300.0f){
			posz -= 3;
			elevator_step = 0.0f;
			elevator_DOWN = FALSE;
		}
		return NOENCOUNT;
	}

	//出口1ポイント到達
	if (mxy.m[POS_CE] == 54){
		*mapstate = CHANGE_MAP;
		switch (map_no){
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
	if (mxy.m[POS_CE] == 56){
		*mapstate = CHANGE_MAP;
		switch (map_no){
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
	if (mxy.m[POS_CE] == 55){
		*mapstate = CHANGE_MAP;
		switch (map_no){
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
	if (mxy.m[POS_CE] == 50 && recover_p_f == FALSE){
		recover_p_f = TRUE;
		map_text_f = 300;
		_tcscpy_s(m_tx, L"ＨＰＭＰ全回復！！");
		*mapstate = RECOV_MAP;
	}
	else if (mxy.m[POS_CE] != 50)recover_p_f = FALSE;

	//当たり判定
	if (direction_move == UP){
		if (((src_theta == 0 || src_theta == 360) &&
			(posy == 0 || MoveUpCond(POSY_D1))) ||
			(src_theta == 90 &&
			(posx == mxy.x - 1 || MoveUpCond(POSX_U1))) ||
			(src_theta == 180 &&
			(posy == mxy.y - 1 || MoveUpCond(POSY_U1))) ||
			(src_theta == 270 &&
			(posx == 0 || MoveUpCond(POSX_D1)))){
			moving = FALSE; return NOENCOUNT;
		}
	}
	if (direction_move == DOWN){
		if (((src_theta == 0 || src_theta == 360) &&
			(posy == mxy.y - 1 || MoveDownCond(POSY_U1))) ||
			(src_theta == 90 &&
			(posx == 0 || MoveDownCond(POSX_D1))) ||
			(src_theta == 180 &&
			(posy == 0 || MoveDownCond(POSY_D1))) ||
			(src_theta == 270 &&
			(posx == mxy.x - 1 || MoveDownCond(POSX_U1)))){
			moving = FALSE; return NOENCOUNT;
		}
	}

	//移動処理
	bool movf;
	float m = tfloat.Add(0.3f);
	switch (direction_move){

	case LEFT:
		src_theta = src_theta - m;
		cay2 = cay1 - (int)(cos(src_theta * 3.14f / 180.0f) * 70.0f);
		cax2 = cax1 + (int)(sin(src_theta * 3.14f / 180.0f) * 70.0f);
		if (src_theta <= m_theta){
			src_theta = m_theta;
			moving = FALSE;
			direction_move = NOTPRESS;
			cay2 = (float)rounding((int)cay2, 1);
			cax2 = (float)rounding((int)cax2, 1);
		}
		break;

	case RIGHT:
		src_theta = src_theta + m;
		cay2 = cay1 - (int)(cos(src_theta * 3.14f / 180.0f) * 70.0f);
		cax2 = cax1 + (int)(sin(src_theta * 3.14f / 180.0f) * 70.0f);
		if (src_theta >= m_theta){
			src_theta = m_theta;
			moving = FALSE;
			direction_move = NOTPRESS;
			cay2 = (float)rounding((int)cay2, 1);
			cax2 = (float)rounding((int)cax2, 1);
		}
		break;

	case UP:
		movf = FALSE;
		if (src_theta == 0 || src_theta == 360){
			cay1 -= m; cay2 -= m;
			if (stepy >= cay1){
				cay1 = stepy;
				cay2 = cay1 - 70.0f;
				movf = TRUE;
			}
		}
		if (src_theta == 90){
			cax1 += m; cax2 += m;
			if (stepx <= cax1){
				cax1 = stepx;
				cax2 = cax1 + 70.0f;
				movf = TRUE;
			}
		}
		if (src_theta == 180){
			cay1 += m; cay2 += m;
			if (stepy <= cay1){
				cay1 = stepy;
				cay2 = cay1 + 70.0f;
				movf = TRUE;
			}
		}
		if (src_theta == 270){
			cax1 -= m; cax2 -= m;
			if (stepx >= cax1){
				cax1 = stepx;
				cax2 = cax1 - 70.0f;
				movf = TRUE;
			}
		}
		if (movf == TRUE){
			if (src_theta == 0 || src_theta == 360)posy -= 1;
			if (src_theta == 90)posx += 1;
			if (src_theta == 180)posy += 1;
			if (src_theta == 270)posx -= 1;
			moving = FALSE;
			direction_move = NOTPRESS;
			//ボスエンカウント
			if (mxy.m[POS_CE] == 51 && boss_p_f == FALSE){
				if (((src_theta == 0 || src_theta == 360) && mxy.m[POSY_D1] == 48) ||//アスキーコード48 = 0
					(src_theta == 90 && mxy.m[POSX_U1] == 48) ||
					(src_theta == 180 && mxy.m[POSY_U1] == 48) ||
					(src_theta == 270 && mxy.m[POSX_D1] == 48)){
					boss_p_f = TRUE;
					return BOSS;
				}
			}
			else if (mxy.m[POS_CE] != 51)boss_p_f = FALSE;
			//通常エンカウント
			rnd = rand() % 10;
			if (rnd == 1){
				if (mxy.m[POS_CE] != 50 && mxy.m[POS_CE] != 51 && mxy.m[POS_CE] != 65 && mxy.m[POS_CE] != 66){
					if (((src_theta == 0 || src_theta == 360) && mxy.m[POSY_D1] == 48) ||//アスキーコード48 = 0
						(src_theta == 90 && mxy.m[POSX_U1] == 48) ||
						(src_theta == 180 && mxy.m[POSY_U1] == 48) ||
						(src_theta == 270 && mxy.m[POSX_D1] == 48))return SIDE;
				}
			}
		}
		break;

	case DOWN:
		movf = FALSE;
		if (src_theta == 0 || src_theta == 360){
			cay1 += m; cay2 += m;
			if (stepy <= cay1){
				cay1 = stepy;
				cay2 = cay1 - 70.0f;
				movf = TRUE;
			}
		}
		if (src_theta == 90){
			cax1 -= m; cax2 -= m;
			if (stepx >= cax1){
				cax1 = stepx;
				cax2 = cax1 + 70.0f;
				movf = TRUE;
			}
		}
		if (src_theta == 180){
			cay1 -= m; cay2 -= m;
			if (stepy >= cay1){
				cay1 = stepy;
				cay2 = cay1 + 70.0f;
				movf = TRUE;
			}
		}
		if (src_theta == 270){
			cax1 += m; cax2 += m;
			if (stepx <= cax1){
				cax1 = stepx;
				cax2 = cax1 - 70.0f;
				movf = TRUE;
			}
		}
		if (movf == TRUE){
			if (src_theta == 0 || src_theta == 360)posy += 1;
			if (src_theta == 90)posx -= 1;
			if (src_theta == 180)posy -= 1;
			if (src_theta == 270)posx += 1;
			moving = FALSE;
			direction_move = NOTPRESS;
			//通常エンカウント
			rnd = rand() % 3;
			if (rnd == 1){
				if (mxy.m[POS_CE] != 50 && mxy.m[POS_CE] != 51 && mxy.m[POS_CE] != 65 && mxy.m[POS_CE] != 66){
					if (((src_theta == 0 || src_theta == 360) && mxy.m[POSY_D1] == 48) ||
						(src_theta == 90 && mxy.m[POSX_U1] == 48) ||
						(src_theta == 180 && mxy.m[POSY_U1] == 48) ||
						(src_theta == 270 && mxy.m[POSX_D1] == 48))return SIDE;
				}
			}
		}
		break;
	}
	return NOENCOUNT;
}

void Map::MapText(TCHAR str[30]) {

	if (map_text_f != 0) {
		if ((map_text_f += tfloat.Add(0.1f)) < 340) {
			text->UpDateText(&str, 300.0f, 300.0f, 25, { 0.0f, 1.0f, 1.0f, 1.0f });
		}
		else {
			map_text_f = 0;
		}
	}
}