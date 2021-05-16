//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@	         Enemy�N���X                                     **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Enemy_Header
#define Class_Enemy_Header

#include "Parameter.h"
#include "Position.h"

//�O���錾
class Battle;
//�O���錾

class Enemy :public Parameter {

protected:
	int o_no;//�I�u�W�F�N�g�i���o�[
	int e_no;//�G�i���o�[
	float pos_offset;//�{�X�ʒu�I�t�Z�b�g
	Position::H_Pos* h_pos;
	Position::E_Pos* e_pos;
	PolygonData* en = NULL;//SideEnemy�g�p
	PolygonData* mag = NULL;//SideEnemy�g�p
	EmissiveObj_Po effect[4][4];//�G�t�F�N�g
	bool effectOn[4][4] = {};
	MagicSelect magicSel;

	MaterialType materialType[19];
	ParameterDXR* pdx[19];

	int effect_no = 0;
	MeshData* en_boss_att0 = NULL;//boss2�̂ݎg�p
	SkinMesh* en_boss_att = NULL;//�{�X�A�j���[�V����
	bool attOn, attFin;         //att�A�j���[�V������
	bool magicAttOn;            //magic�A�j���[�V�����t���O
	ParticleData* mag_boss = NULL;//�{�X�p
	float mag_size;           //�{�X�p
	float disp_size;         //�{�X�p
	bool dispOn;            //�{�X�p
	int comNo;

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
	virtual void ObjUpdate(float x, float y, float z, float r, float g, float b, float theta);
	bool EffectUpdate(Battle* battle, int* E_select_obj);
	void PosOffset(int o_no);

public:
	Enemy() {}
	Enemy(int t_no, int no);
	Act_fin_flg EnemyUpdate(Battle* battle, int* E_select_obj, Action action, MagicSelect E_Magrun);
	void Draw(Encount enc);
	void StreamOutput(Encount enc);
	void StreamOutputAfterDraw(Encount enc);
	Action Normal_act_get();
	void EffectGetBuffer();
	void EffectCreate();
	virtual void SetVertex();
	virtual void SetCommandList(int com_no);
	virtual void CreateEnemy();
	virtual bool M_run_flg();//�}�W�b�N���s�t���O
	virtual void M_select(int* r, int* r1);//�}�W�b�N�I��
	bool s_esc();
	bool GetBossEffectState(float* blur);
	virtual ~Enemy();

	ParameterDXR** getParameterDXR(int* numPara);
	void setPointLightNo();
};

#endif
