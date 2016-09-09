//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Mapクラス                                       **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Map_Header
#define Class_Map_Header

#include "MovieSoundManager.h"
#include "Position.h"
#include "DxText.h"
#include "Hero.h"
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
	static int map_no_s;    //マップナンバー
	int map_no;            //オブジェクト内部用マップナンバー
	static int MapComCurr;   //obj生成時の割り当てコマンドリストナンバー
	int Map_Com = MAP_COM_2;//現コマンドリストナンバー
	static MapStPos MPos;     //マップスタート位置
	static int boss_killed[5];//各ボス撃破履歴
	Dx12Process *dx;
	DxText *text;
	T_float tfloat;
	typedef struct{
		int *m;
		int x;
		int y;
		int z;
	}mapxy;
	mapxy mxy;

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

	float cax1;//現在位置
	float cax2;//注視点
	float cay1;//現在位置
	float cay2;//注視点
	float src_theta;//現在の向いてる方向

	int posx;//マップフィルター位置
	int posy;
	int posz;
	int woodcount;     //木個数
	int blockcountA;   //壁用立方体の四角形の個数レンガ
	int blockcountB;   //壁用立方体の四角形の個数岩
	int blockcountC;   //壁用立方体の四角形の個数岩2
	int blockcountD;   //壁用立方体の四角形の個数溶岩
	int blockcountE;   //壁用立方体の四角形の個数コケ岩
	int squarecount; //壁用板個数
	int r_point_count;//回復ポイント個数
	int mo_count;    //動画テクスチャ,松明個数
	int f_wall_count;//動画テクスチャ炎壁個数
	int boss_count; //ボス出現ポイント個数
	int Elevator_count;//エレベーター個数
	float map_text_f;//マップ上のテキスト表示フラグ
	TCHAR m_tx[30];//表示内容
	bool recover_p_f;//リカバーポイント到達履歴
	bool boss_p_f;  //ボス出現ポイント到達履歴
	bool elevator_UP; 
	bool elevator_DOWN;
	float elevator_step;

	bool moving;
	Directionkey direction_move;
	float m_theta;
	float stepx;
	float stepy;

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
		poBackground, poRain, poRecover, poRecoverLine[10], poMo, poBoss, poElevator, poEXIT;
	Position::E_Pos e_pos[4];
	Position::H_Pos h_pos;
	Hero *he;//移動用
	int walkI;//移動用

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
	void HeroDraw(Directionkey direction);

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
	~Map();
};

#endif