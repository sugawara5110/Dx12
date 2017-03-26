//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@	         Enemy�N���X                                     **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Enemy_Header
#define Class_Enemy_Header

#include "Parameter.h"
#include "Position.h"

#define ENEMY_COM 4

//�O���錾
class Battle;
//�O���錾

class Enemy :public Parameter{

protected:
	int o_no;//�I�u�W�F�N�g�i���o�[
	int e_no;//�G�i���o�[
	float pos_offset;//�{�X�ʒu�I�t�Z�b�g
	Position::H_Pos *h_pos;
	Position::E_Pos *e_pos;
	PolygonData *en, *mag, effect[4];
	int effect_no = 0;
	MeshData *en_boss_att0;       //boss2�̂ݎg�p
	SkinMesh *en_boss_att;       //�{�X�A�j���[�V����
	bool attOn, attFin;         //att�A�j���[�V������
	bool magicAttOn;            //magic�A�j���[�V�����t���O
	ParticleData *mag_boss;    //�{�X�p
	float mag_size;           //�{�X�p

	bool esc;//�G�X�P�[�v�� TRUE==�G�X�P�[�v��
	float mov_z;
	float mov_y;
	float mov_x;
	float size_x;
	float size_y;
	Action normal_action;//�ʏ펞�̃A�N�V����
	Action act_f;
	int M_run_flg_rnd;  //�}�W�b�N���s�t���O�����p�����_��
	bool up;
	bool zoom;
	float count;
	bool effect_f;//�G�t�F�N�gON,OFF
	float tx, ty;//�G�t�F�N�g�p�e�N�X�`�����W
	float tt;     //�G�t�F�N�g���x�����p
	float cr, cg, cb;//�G�摜�F

	void Enemycreate(float x, float y);
	virtual void AttackAction();
	virtual void DamageAction();
	virtual void RecoverActionInit();
	virtual void RecoverAction();
	virtual bool LostAction(float x, float y, float z);
	virtual bool Magiccreate(float x, float y, float z);
	virtual void ObjDraw(float x, float y, float z, float r, float g, float b, float theta);
	bool Effectdraw(Battle *battle, int *E_select_obj);
	void PosOffset(int o_no);

public:
	Enemy();
	Enemy(int t_no, int no);
	Act_fin_flg Enemydraw(Battle *battle, int *E_select_obj, Action action, MagicSelect E_Magrun);
	Action Normal_act_get();
	virtual bool M_run_flg();//�}�W�b�N���s�t���O
	virtual void M_select(int *r, int *r1);//�}�W�b�N�I��
	bool s_esc();
	virtual ~Enemy();
};

#endif
