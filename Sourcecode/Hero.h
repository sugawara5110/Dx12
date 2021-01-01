//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@	         Hero�N���X                                      **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Hero_Header
#define Class_Hero_Header

#include "Parameter.h"
#include "Position.h"
#include "Battle.h"

//�O���錾
class Battle;
//�O���錾

class Hero :public Parameter {

protected:
	int o_no;//�I�u�W�F�N�g�i���o�[
	SkinMesh* p_att = NULL;
	bool attOn, attFin;    //att�A�j���[�V������
	bool magicAttOn;      //�}�W�b�N�A�j���[�V������
	float LA, LA_x, LA_y; //LostAction�pthetaZ�ɂ���ĕς��
	SkinMesh* torchWood = NULL;    //o_no==0�p
	EmissiveObj_Po* torchFlame = NULL;//o_no==0�p
	bool torchOn;
	int comNo;
	std::unique_ptr<MaterialType[]> materialType;
	std::unique_ptr<ParameterDXR* []> pdx;

	PolygonData2D state, meter;
	EmissiveObj_Po mag, effect[4][4];
	bool effectOn[4][4] = {};
	int effect_no = 0;
	float mov_y;
	float mov_x;
	float mov_z;
	Action act_f;
	bool up;
	float count;
	bool effect_f;//�G�t�F�N�gON,OFF
	float tx, ty;//�G�t�F�N�g�p�e�N�X�`�����W
	float tt;     //�G�t�F�N�g���x�����p

	bool Statecreate_clr_f;
	float Statecreate_r;

	void CreateTorchFlame();
	void Statecreate(bool command_run);
	void Metercreate(float me);
	void Magiccreate();
	bool EffectUpdate(Battle* battle, int* select_obj, Position::H_Pos* h_pos, Position::E_Pos* e_pos);

public:
	Hero() {}
	Hero(int no);
	void SetVertex();
	void SetCommandList(int com_no);
	void CreateHero();
	void P_DataInput(P_Data* p_dat);
	void TorchSwitch(bool f);
	void OBJWalkUpdate(float x, float y, float z, float r, float g, float b, float theta);
	void OBJWalkUpdate(float x, float y, float z, float r, float g, float b, float theta, bool walkOn);//walkOn��Map�N���X���瑀��
	Act_fin_flg HeroUpdate(Battle* battle, int* select_obj, Position::H_Pos* h_pos, Position::E_Pos* e_pos, float me, bool command_run, Action action, MagicSelect H_Magrun);
	void SetMovie(Encount enc);
	void Draw(Encount enc, bool ending);
	void StreamOutput(Encount enc, bool ending);
	Action Normal_act_get();
	void Act_f_init();
	void Draw2D(Encount enc, bool ending);
	virtual ~Hero();
	ParameterDXR** getParameterDXRMap(int* numPara);
	void setPointLightNoMap();
	ParameterDXR** getParameterDXRBat(int* numPara);
	void setPointLightNoBat();
	MaterialType* getMaterialTypeMap();
	MaterialType* getMaterialTypeBat();
};

#endif