//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@           DxStruct.h                                       **//
//**                                   Directx�p�\����                                   **//
//*****************************************************************************************//

#ifndef Class_DxStruct_Header
#define Class_DxStruct_Header

#include "Function.h"

#define LIGHT_PCS 150
#define LIGHT_PCS_init 7
#define INSTANCE_PCS_3D 150
#define INSTANCE_PCS_2D 80

//���_
struct Vertex {
	VECTOR3 Pos;       //�ʒu
	VECTOR3 normal;   //�@��
	VECTOR4 color;   //�F
	VECTOR2 tex;    //�e�N�X�`�����W
};

struct MY_VERTEX_MESH {
	VECTOR3 Pos;
	VECTOR3 normal;
	VECTOR2 tex;
};

struct MY_VERTEX2 {
	VECTOR3 Pos;       
	VECTOR4 color;
	VECTOR2 tex;    
};

//�V�F�[�_�[�󂯓n���p�o�b�t�@3D
struct CONSTANT_BUFFER {

	MATRIX World[INSTANCE_PCS_3D];
	MATRIX WVP[INSTANCE_PCS_3D];
	VECTOR4 C_Pos;       //���_�ʒu
	VECTOR4 AddObjColor;//�I�u�W�F�N�g�̐F�ω��p

	//�|�C���g���C�g
	VECTOR4 pLightPos[LIGHT_PCS];
	VECTOR4 pLightColor[LIGHT_PCS];
	VECTOR4 pLightst[LIGHT_PCS];
	VECTOR4 pShadowLow_Lpcs;//�e�̉����l, ���C�g��, ���C�g�L��

	//�f�B���N�V���i�����C�g
	VECTOR4 dDirection;
	VECTOR4 dLightColor;
	VECTOR4 dLightst;

	//�t�H�O
	VECTOR4  FogAmo_Density; //�t�H�O��x, �t�H�O�̖��xy, onoffz
	VECTOR4  FogColor;   //�t�H�O�̐F

	//�f�B�X�v���C�g�����g�}�b�s���O�̋N����x(0���͂̏ꍇ�f�t�H���g�l3�ɂȂ�)
	VECTOR4  DispAmount;

	//UV���W�ړ��p
	VECTOR4 pXpYmXmY;
};

struct CONSTANT_BUFFER_MESH {
	VECTOR4 vDiffuse;//�f�B�t���[�Y�F
	VECTOR4 tex_f;   //�e�N�X�`���L�薳��
};

struct CONSTANT_BUFFER2D {
	VECTOR4 Pos[INSTANCE_PCS_2D];
	VECTOR4 Color[INSTANCE_PCS_2D];
	VECTOR4 sizeXY[INSTANCE_PCS_2D];
};

struct CONSTANT_BUFFER_P {
	MATRIX  WV;
	MATRIX  Proj;
	VECTOR4 size;//x�p�[�e�B�N���傫��, y�p�[�e�B�N���������t���O, z�X�s�[�h
};
struct PartPos {
	VECTOR3 CurrentPos; //�`��Ɏg��
	VECTOR3 PosSt;     //�J�n�ʒu
	VECTOR3 PosEnd;   //�I���ʒu
	VECTOR4 Col;
};

//�|�C���g���C�g
struct PointLight {
	VECTOR4 LightPos[LIGHT_PCS];   //����
	VECTOR4 LightColor[LIGHT_PCS];//�F
	VECTOR4 Lightst[LIGHT_PCS];  //�����W,���邳,�����̑傫��,�I���I�t
	float        ShadowLow_val; //�e�̉����l
	int          LightPcs;     //���C�g��
};

//���s����
struct DirectionLight {
	VECTOR4 Direction;  //����
	VECTOR4 LightColor;//�F
	VECTOR4 Lightst;  //���邳x,�I���I�ty,�e�̉����l
};

//�t�H�O
struct Fog {
	VECTOR4  FogColor;//�t�H�O�̐F
	float         Amount;  //�t�H�O��
	float         Density;//���x
	float         on_off;
};

#endif