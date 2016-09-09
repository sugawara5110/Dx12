//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@	      EnemyBoss�N���X                                    **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_EnemyBoss_Header
#define Class_EnemyBoss_Header

#include "Enemy.h"

class EnemyBoss :public Enemy{

protected:
	//@Override
	virtual void AttackAction();
	virtual void DamageAction();
	virtual void RecoverActionInit();
	virtual void RecoverAction();
	virtual bool LostAction(float x, float y, float z);
	virtual bool Magiccreate(float x, float y, float z);
	virtual void ObjDraw(float x, float y, float z, float r, float g, float b, float theta);

public:
	EnemyBoss();
	EnemyBoss(int t_no, int no, Position::H_Pos *h_pos, Position::E_Pos *e_pos);
	//@Override
	virtual bool M_run_flg();//�}�W�b�N���s�t���O
	//@Override
	virtual void M_select(int *r, int *r1);//�}�W�b�N�I��
	virtual ~EnemyBoss();
};

#endif