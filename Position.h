//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	      Positionクラス                                     **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Position_Header
#define Class_Position_Header

#include "Dx12Process.h"

class Position{

public:
	typedef struct{
		float theta;
		bool element;
		float x;
		float y;
		float z;
	}E_Pos;//enemy位置

	typedef struct{
		float cx1;//現在位置
		float cx2;//注視点
		float cy1;//現在位置
		float cy2;//注視点
		float cx;//視点
		float cy;//視点
		float cz;
		int px;
		int py;
		int pz;
		float theta;
	}H_Pos;//walk中hero位置, 視点注視点

	typedef struct{
		float BtPos_x1;//位置
		float BtPos_x2;//注視点
		float BtPos_y1;//位置
		float BtPos_y2;//注視点
	}Bt_H_Pos;//battle用hero位置

private:
	Position(){};//外部からのオブジェクト生成禁止
	Position(const Position &obj){};     // コピーコンストラクタ禁止
	void operator=(const Position& obj){};// 代入演算子禁止
	static void P_Copy(H_Pos *h_posIn, H_Pos *h_posOut, float view_rev);

public:
	static bool CamAdvance(H_Pos *h_posIn, H_Pos *h_posOut, float speed);
	static bool CamRecession(H_Pos *h_posIn, H_Pos *h_posOut, float speed);
	static void CamActionBackView(Bt_H_Pos *b_posIn, H_Pos *h_posOut, float speed, bool CamActOn, Action act);
	static void CamActionRoundView(Bt_H_Pos *b_posIn, H_Pos *h_posOut, float speed, bool CamActOn, Action act, char LR);
};

#endif