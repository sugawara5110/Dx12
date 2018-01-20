//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@	      Position�N���X                                     **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Position_Header
#define Class_Position_Header

#include "./Direct3DWrapper/Dx12Process.h"
#include "Enum.h"

#define POSY_U (h_posIn->pz * mxy->y * mxy->x + (h_posIn->py + 1) * mxy->x + h_posIn->px)
#define POSY_D (h_posIn->pz * mxy->y * mxy->x + (h_posIn->py - 1) * mxy->x + h_posIn->px)
#define POSX_U (h_posIn->pz * mxy->y * mxy->x + h_posIn->py * mxy->x + (h_posIn->px + 1))
#define POSX_D (h_posIn->pz * mxy->y * mxy->x + h_posIn->py * mxy->x + (h_posIn->px - 1))

struct MapHistoryData {
	UINT maphis0[1 * 35 * 30] = { 0 };
	UINT maphis1[1 * 40 * 40] = { 0 };
	UINT maphis2[1 * 40 * 30] = { 0 };
	UINT maphis3[1 * 35 * 30] = { 0 };
	UINT maphis4[3 * 30 * 30] = { 0 };
};

class Position{

public:
	typedef struct {
		int *m;
		int x;
		int y;
		int z;
	}mapxy;

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
	Position(){}//�O������̃I�u�W�F�N�g�����֎~
	Position(const Position &obj){};     // �R�s�[�R���X�g���N�^�֎~
	void operator=(const Position& obj){};// ������Z�q�֎~
	static void P_Copy(H_Pos *h_posIn, H_Pos *h_posOut, float view_rev);

public:
	static bool CamAdvance(H_Pos *h_posIn, H_Pos *h_posOut, H_Pos *h_posOut2, float speed, mapxy *mxy, Encount en);
	static bool CamRecession(H_Pos *h_posIn, H_Pos *h_posOut, float speed);
	static void CamActionBackView(Bt_H_Pos *b_posIn, H_Pos *h_posOut, float speed, bool CamActOn, Action act);
	static void CamActionRoundView(Bt_H_Pos *b_posIn, H_Pos *h_posOut, float speed, bool CamActOn, Action act, char LR);
};

#endif