//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@	         Map�N���X                                       **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Map_Header
#define Class_Map_Header

#include "MovieSoundManager.h"
#include "Position.h"
#include "DxText.h"
#include "Hero.h"
#include <stdio.h>
#include <stdlib.h>
#include <TCHAR.h>

#define POS_CE  (posz * mxy.y * mxy.x + posy * mxy.x + posx)
#define POSY_U1 (posz * mxy.y * mxy.x + (posy + 1) * mxy.x + posx)
#define POSY_D1 (posz * mxy.y * mxy.x + (posy - 1) * mxy.x + posx)
#define POSX_U1 (posz * mxy.y * mxy.x + posy * mxy.x + (posx + 1))
#define POSX_D1 (posz * mxy.y * mxy.x + posy * mxy.x + (posx - 1))

#define MAP_COM_1   2
#define MAP_COM_2   3

class Map{

private:
	static int map_no_s;    //�}�b�v�i���o�[
	int map_no;            //�I�u�W�F�N�g�����p�}�b�v�i���o�[
	static int MapComCurr;   //obj�������̊��蓖�ăR�}���h���X�g�i���o�[
	int Map_Com = MAP_COM_2;//���R�}���h���X�g�i���o�[
	static MapStPos MPos;     //�}�b�v�X�^�[�g�ʒu
	static int boss_killed[5];//�e�{�X���j����
	Dx12Process *dx;
	DxText *text;
	T_float tfloat;
	Position::mapxy mxy;

	struct LightPos{
		float x, y, z;
		float r, g, b, a;
		float range;
		float brightness;
		float attenuation;
		bool on_off;
	};
	LightPos *light;
	int lightcount;

	float cax1;//���݈ʒu
	float cax2;//�����_
	float cay1;//���݈ʒu
	float cay2;//�����_
	float src_theta;//���݂̌����Ă����

	int posx;//�}�b�v�t�B���^�[�ʒu
	int posy;
	int posz;
	int woodcount;     //�،�
	int blockcountA;   //�Ǘp�����̂̎l�p�`�̌������K
	int blockcountB;   //�Ǘp�����̂̎l�p�`�̌���
	int blockcountC;   //�Ǘp�����̂̎l�p�`�̌���2
	int blockcountD;   //�Ǘp�����̂̎l�p�`�̌��n��
	int blockcountE;   //�Ǘp�����̂̎l�p�`�̌��R�P��
	int squarecount; //�Ǘp��
	int r_point_count;//�񕜃|�C���g��
	int mo_count;    //����e�N�X�`��,������
	int f_wall_count;//����e�N�X�`�����ǌ�
	int boss_count; //�{�X�o���|�C���g��
	int Elevator_count;//�G���x�[�^�[��
	float map_text_f;//�}�b�v��̃e�L�X�g�\���t���O
	TCHAR m_tx[30];//�\�����e
	bool recover_p_f;//���J�o�[�|�C���g���B����
	float recovPosX, recovPosY;//���J�o�[�|�C���g�����ʒu(�����ɂ���ꍇ�v�����ύX)
	bool boss_p_f;  //�{�X�o���|�C���g���B����
	bool elevator_UP; 
	bool elevator_DOWN;
	float elevator_step;

	struct OBJPosRandomValue{
		float x;
		float y;
		OBJPosRandomValue(){
			x = (rand() % 100) - 50.0f;
			y = (rand() % 100) - 50.0f;
		}
	};
	OBJPosRandomValue *wood, *wall1;
	
	MeshData mWood, mountain;
	PolygonData poWallA, poWallB, poWallC, poWallD, poWallE, poWall1[3], poF_Wall,
		poGroundF, poCeilingF, poGroundM, poCeilingM, poGroundE, poCeilingE,
		poBackground, poRain, poRecover, poRecoverLine[12], poBoss, poElevator, poEXIT;
	ParticleData poMo;
	Position::E_Pos e_pos[4];
	Position::H_Pos h_pos;
	Hero *he;//�ړ��p

	void Debug();
	Map(){};
	void Mapfilter_p(int k, int j, int i, int idx1, int idx2, int *cnt);
	void Mapfilter(Position::H_Pos *h_p);
	void Mapdraw_Wood();
	void Mapdraw_Mountain();
	void Mapcreate_Wall1(int i);
	void Mapdraw_Wall1();
	void Mapcreate_Wall(PolygonData *pd, int no1, int no2, float height, float adjust, float adjust2);
	void Mapcreate_Ground(PolygonData *pd, int pcsx, int pcsy, float height, float adjust);
	void Mapcreate_Ceiling(PolygonData *pd, int pcsx, int pcsy, float height, float adjust);
	void Mapcreate_Background(float st, float end);
	void Mapdraw_Rain();
	void Mapcreate_Recover();
	void Mapdraw_Recover();
	void Mapcreate_Ds();
	void Mapdraw_Ds();
	void Mapcreate_BossPoint();
	void Mapcreate_Elevator();
	void Mapcreate_EXIT(float x, float y, float z, float xsize);
	void MapdrawObj();
	bool MoveUpCond(int Ind);
	bool MoveDownCond(int Ind);
	Encount Move(MapState *mapstate, Directionkey direction);
	void MapText(TCHAR str[30]);
	bool ViewCulling(float obj_x, float obj_y, float obj_z);
	void HeroDraw(bool mf);
	bool CollisionDetection(float in_y, float in_x, Directionkey dir);

	//�l�̌ܓ�
	int rounding(int val, int digit_number) {
		int v = 5 * (int)pow(10.0, (double)digit_number - 1.0);
		val += v;
		return val -= (val % (int)pow(10.0, (double)digit_number));
	}
	//downNum�؂艺��
	//upNum�؂�グ
	int rounding2(int val, int digit_num, int downNum, int upNum) {
		int d = (int)pow(10.0, (double)digit_num - 1.0);
		int valT = val / d;
		int valT2 = valT / 10;
		if ((valT % 10) >= upNum)return (valT2 + 1) * d * 10;
		if ((valT % 10) <= downNum)return (valT2 - 1) * d * 10;
		return val;
	}
	
public:
	static int GetMapNo();
	static void SetMapNo(int no);
	static void SetBossKilled(int i, int f);
	static int *GetBossKilled();
	static int GetBossKilled(int map_no);
	Map(Position::H_Pos *h_p, Hero *hero);
	Encount Mapdraw(MapState *mapstate, Directionkey direction, Encount encount, bool menu, bool title, bool ending);
	Position::E_Pos *Getposition(int p);
	Position::H_Pos *Getposition();
	void Setposition(Position::H_Pos *pos);
	Position::mapxy *Getmap();
	~Map();
};

#endif