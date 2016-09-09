//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	      Positionクラス                                     **//
//**                                  CamAdvance関数                                     **//
//*****************************************************************************************//

#include "Position.h"

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

bool Position::CamAdvance(H_Pos *h_posIn, H_Pos *h_posOut, float speed){

	static float view_rev = 0.0f;

	if (h_posIn == NULL){
		view_rev = 0.0f;//初期化
		return FALSE;
	}

	P_Copy(h_posIn, h_posOut, view_rev);

	if ((view_rev += speed) >= 80.0f){
		view_rev = 80.0f;//TRUE出し続ける為
		return TRUE;
	}
	return FALSE;
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
		h_posOut->cy = b_posIn->BtPos_y1 - (int)(cos(the * 3.14f / 180.0f) * 50.0f);
		h_posOut->cx = b_posIn->BtPos_x1 + (int)(sin(the * 3.14f / 180.0f) * 50.0f);
		h_posOut->cy2 = b_posIn->BtPos_y1;
		h_posOut->cx2 = b_posIn->BtPos_x1;
	}
}