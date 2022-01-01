//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	      Positionクラス                                     **//
//**                                  CamAdvance関数                                     **//
//*****************************************************************************************//

#include "Position.h"
#define _USE_MATH_DEFINES
#include <math.h>

int EmissiveCount::count = 0;

void Position::P_Copy(H_Pos *h_posIn, H_Pos *h_posOut, float view_rev){
	float view_x = 0.0f;
	float view_y = 0.0f;
	switch ((int)h_posIn->theta){
	case 360:
	case 0:
		view_y = -view_rev;
		break;
	case 90:
		view_x = view_rev;
		break;
	case 180:
		view_y = view_rev;
		break;
	case 270:
		view_x = -view_rev;
		break;
	}
	h_posOut->cx1 = h_posIn->cx1;
	h_posOut->cx2 = h_posIn->cx2 + view_x;
	h_posOut->cy1 = h_posIn->cy1;
	h_posOut->cy2 = h_posIn->cy2 + view_y;
	h_posOut->cx = h_posIn->cx + view_x;
	h_posOut->cy = h_posIn->cy + view_y;
	h_posOut->cz = h_posIn->cz;
	h_posOut->px = h_posIn->px;
	h_posOut->py = h_posIn->py;
	h_posOut->pz = h_posIn->pz;
	h_posOut->theta = h_posIn->theta;
}

bool Position::CamAdvance(H_Pos *h_posIn, H_Pos *h_posOut, H_Pos *h_posOut2, float speed, mapxy *mxy, Encount en) {

	static int sel = -1;
	static float targetX = 0.0f;
	static float targetY = 0.0f;
	static float targetTheta = 0.0f;
	static float currentX = 0.0f;
	static float currentY = 0.0f;
	static float currentTheta = 0.0f;
	static float signX = 1.0f;
	static float signY = 1.0f;
	static float signTheta = 1.0f;
	static bool fin = FALSE;

	//使用後リセット
	if (h_posIn == NULL) {
		sel = -1;
		signX = 1.0f;
		signY = 1.0f;
		signTheta = 1.0f;
		fin = FALSE;
		return FALSE;
	}

	//初期化
	while (sel == -1) {
		if (en == BOSS || mxy->m[POSY_D] == 48 && (rand() % 2) == 0) {
			targetX = h_posIn->px * 100.0f + 50.0f;
			currentX = h_posIn->cx;
			if (targetX - h_posIn->cx < 0)signX = -1.0f;
			targetY = (h_posIn->py - 1) * 100.0f + 50.0f;
			currentY = h_posIn->cy;
			if (targetY - h_posIn->cy < 0)signY = -1.0f;
			if (h_posIn->theta <= 180.0f)targetTheta = 0.0f; else targetTheta = 360.0f;
			currentTheta = h_posIn->theta;
			if (targetTheta - h_posIn->theta < 0)signTheta = -1.0f;
			sel = 0; break;
		}
		if (mxy->m[POSX_U] == 48 && (rand() % 2) == 0) {
			targetX = (h_posIn->px + 1) * 100.0f + 50.0f;
			currentX = h_posIn->cx;
			if (targetX - h_posIn->cx < 0)signX = -1.0f;
			targetY = h_posIn->py * 100.0f + 50.0f;
			currentY = h_posIn->cy;
			if (targetY - h_posIn->cy < 0)signY = -1.0f;
			targetTheta = 90.0f;
			currentTheta = h_posIn->theta;
			if (targetTheta - h_posIn->theta < 0)signTheta = -1.0f;
			sel = 1; break;
		}
		if (mxy->m[POSY_U] == 48 && (rand() % 2) == 0) {
			targetX = h_posIn->px * 100.0f + 50.0f;
			currentX = h_posIn->cx;
			if (targetX - h_posIn->cx < 0)signX = -1.0f;
			targetY = (h_posIn->py + 1) * 100.0f + 50.0f;
			currentY = h_posIn->cy;
			if (targetY - h_posIn->cy < 0)signY = -1.0f;
			targetTheta = 180.0f;
			currentTheta = h_posIn->theta;
			if (targetTheta - h_posIn->theta < 0)signTheta = -1.0f;
			sel = 2; break;
		}
		if (mxy->m[POSX_D] == 48 && (rand() % 2) == 0) {
			targetX = (h_posIn->px - 1) * 100.0f + 50.0f;
			currentX = h_posIn->cx;
			if (targetX - h_posIn->cx < 0)signX = -1.0f;
			targetY = h_posIn->py * 100.0f + 50.0f;
			currentY = h_posIn->cy;
			if (targetY - h_posIn->cy < 0)signY = -1.0f;
			targetTheta = 270.0f;
			currentTheta = h_posIn->theta;
			if (targetTheta - h_posIn->theta < 0)signTheta = -1.0f;
			sel = 3; break;
		}
	}

	h_posOut2->cx1 = h_posIn->px * 100.0f + 50.0f;
	h_posOut2->cx2 = h_posOut2->cx1 + sin(targetTheta * (float)M_PI / 180.0f) * 70.0f;
	h_posOut2->cy1 = h_posIn->py * 100.0f + 50.0f;
	h_posOut2->cy2 = h_posOut2->cy1 - cos(targetTheta * (float)M_PI / 180.0f) * 70.0f;
	h_posOut2->cx = h_posOut2->cx1 + h_posOut2->cx1 - h_posOut2->cx2;
	h_posOut2->cy = h_posOut2->cy1 + h_posOut2->cy1 - h_posOut2->cy2;
	h_posOut2->cz = h_posIn->cz;
	h_posOut2->px = h_posIn->px;
	h_posOut2->py = h_posIn->py;
	h_posOut2->pz = h_posIn->pz;
	h_posOut2->theta = targetTheta;

	currentX += (signX * speed);
	if (signX == 1.0f && targetX < currentX ||
		signX == -1.0f && targetX > currentX)currentX = targetX;
	currentY += (signY * speed);
	if (signY == 1.0f && targetY < currentY ||
		signY == -1.0f && targetY > currentY)currentY = targetY;
	currentTheta += (signTheta * speed);
	if (signTheta == 1.0f && targetTheta < currentTheta ||
		signTheta == -1.0f && targetTheta > currentTheta)currentTheta = targetTheta;

	h_posOut->cx1 = h_posIn->cx1;
	h_posOut->cx2 = currentX + sin(currentTheta * (float)M_PI / 180.0f) * 70.0f;
	h_posOut->cy1 = h_posIn->cy1;
	h_posOut->cy2 = currentY - cos(currentTheta * (float)M_PI / 180.0f) * 70.0f;
	h_posOut->cx = currentX;
	h_posOut->cy = currentY;
	h_posOut->cz = h_posIn->cz;
	h_posOut->px = h_posIn->px;
	h_posOut->py = h_posIn->py;
	h_posOut->pz = h_posIn->pz;
	h_posOut->theta = currentTheta;

	if (targetX == currentX && targetY == currentY && targetTheta == currentTheta) {
		fin = TRUE;
	}

	return fin;
}

bool Position::CamRecession(H_Pos *h_posIn, H_Pos *h_posOut, float speed){

	static float view_rev = 80.0f;

	if (h_posIn == NULL){
		view_rev = 80.0f;//初期化
		return FALSE;
	}

	P_Copy(h_posIn, h_posOut, view_rev);

	if ((view_rev -= speed) <= 0.0f){
		view_rev = 0.0f;//TRUE出し続ける為
		return TRUE;
	}
	return FALSE;
}

void Position::CamActionBackView(Bt_H_Pos *b_posIn, H_Pos *h_posOut, float speed, bool CamActOn, Action act){

	static bool view_on = FALSE;
	static H_Pos h_posEnd, h_posCrt;
	static bool sign_x = TRUE;
	static bool sign_y = TRUE;

	if (CamActOn == FALSE){
		view_on = FALSE;
		sign_x = sign_y = TRUE;
		return;
	}
	if (CamActOn == TRUE && (act == ATTACK)){
		view_on = TRUE;
		h_posEnd.cx = b_posIn->BtPos_x1 + b_posIn->BtPos_x1 - b_posIn->BtPos_x2;
		h_posEnd.cy = b_posIn->BtPos_y1 + b_posIn->BtPos_y1 - b_posIn->BtPos_y2;
		h_posEnd.cx2 = b_posIn->BtPos_x2;
		h_posEnd.cy2 = b_posIn->BtPos_y2;
		h_posCrt.cx = h_posOut->cx;
		h_posCrt.cy = h_posOut->cy;
		h_posCrt.cx2 = h_posOut->cx2;
		h_posCrt.cy2 = h_posOut->cy2;
		if (h_posEnd.cx < h_posCrt.cx)sign_x = FALSE;
		if (h_posEnd.cy < h_posCrt.cy)sign_y = FALSE;
	}

	if (view_on){
		if (sign_x){
			if (h_posCrt.cx < h_posEnd.cx){
				h_posCrt.cx += speed;
				h_posCrt.cx2 += speed;
			}
		}
		else{
			if (h_posCrt.cx > h_posEnd.cx){
				h_posCrt.cx -= speed;
				h_posCrt.cx2 -= speed;
			}
		}
		if (sign_y){
			if (h_posCrt.cy < h_posEnd.cy){
				h_posCrt.cy += speed;
				h_posCrt.cy2 += speed;
			}
		}
		else{
			if (h_posCrt.cy > h_posEnd.cy){
				h_posCrt.cy -= speed;
				h_posCrt.cy2 -= speed;
			}
		}
		h_posOut->cx = h_posCrt.cx;
		h_posOut->cy = h_posCrt.cy;
		h_posOut->cx2 = h_posCrt.cx2;
		h_posOut->cy2 = h_posCrt.cy2;
	}
}

void Position::CamActionRoundView(Bt_H_Pos *b_posIn, H_Pos *h_posOut, float speed, bool CamActOn, Action act, char LR){

	static bool view_on = FALSE;
	static float addtheta, s_theta;

	if (CamActOn == FALSE){ view_on = FALSE; return; }
	if (CamActOn == TRUE && (act == MAGIC)){
		view_on = TRUE;
		s_theta = h_posOut->theta;
		addtheta = 180.0f;
	}

	if (view_on){
		if (LR == 'L'){
			if ((addtheta -= speed) < 0.0f)addtheta = 0.0f;
		}
		else{
			if ((addtheta += speed) > 360.0f)addtheta = 360.0f;
		}
		float the = (float)((int)(s_theta + addtheta) % 360);
		h_posOut->cy = b_posIn->BtPos_y1 - (int)(cos(the * (float)M_PI / 180.0f) * 50.0f);
		h_posOut->cx = b_posIn->BtPos_x1 + (int)(sin(the * (float)M_PI / 180.0f) * 50.0f);
		h_posOut->cy2 = b_posIn->BtPos_y1;
		h_posOut->cx2 = b_posIn->BtPos_x1;
	}
}