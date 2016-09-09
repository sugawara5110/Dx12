//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@	      Position�N���X                                     **//
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
	}E_Pos;//enemy�ʒu

	typedef struct{
		float cx1;//���݈ʒu
		float cx2;//�����_
		float cy1;//���݈ʒu
		float cy2;//�����_
		float cx;//���_
		float cy;//���_
		float cz;
		int px;
		int py;
		int pz;
		float theta;
	}H_Pos;//walk��hero�ʒu, ���_�����_

	typedef struct{
		float BtPos_x1;//�ʒu
		float BtPos_x2;//�����_
		float BtPos_y1;//�ʒu
		float BtPos_y2;//�����_
	}Bt_H_Pos;//battle�phero�ʒu

private:
	Position(){};//�O������̃I�u�W�F�N�g�����֎~
	Position(const Position &obj){};     // �R�s�[�R���X�g���N�^�֎~
	void operator=(const Position& obj){};// ������Z�q�֎~
	static void P_Copy(H_Pos *h_posIn, H_Pos *h_posOut, float view_rev);

public:
	static bool CamAdvance(H_Pos *h_posIn, H_Pos *h_posOut, float speed);
	static bool CamRecession(H_Pos *h_posIn, H_Pos *h_posOut, float speed);
	static void CamActionBackView(Bt_H_Pos *b_posIn, H_Pos *h_posOut, float speed, bool CamActOn, Action act);
	static void CamActionRoundView(Bt_H_Pos *b_posIn, H_Pos *h_posOut, float speed, bool CamActOn, Action act, char LR);
};

#endif