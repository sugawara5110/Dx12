//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@	      EnemySide�N���X                                    **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_EnemySide_Header
#define Class_EnemySide_Header

#include "Enemy.h"

class EnemySide :public Enemy{

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
	EnemySide() {}
	EnemySide(int t_no, int no, Position::H_Pos *h_pos, Position::E_Pos *e_pos);
	//@Override
	virtual void SetVertex();
	virtual void CreateEnemy();
	virtual bool M_run_flg();//�}�W�b�N���s�t���O
	virtual void M_select(int *r, int *r1);//�}�W�b�N�I��
	virtual ~EnemySide();
};

#endif