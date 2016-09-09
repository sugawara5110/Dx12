//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Heroクラス                                      **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Hero_Header
#define Class_Hero_Header

#include "Parameter.h"
#include "Position.h"
#include "Battle.h"

#define HERO_COM 5

//前方宣言
class Battle;
//前方宣言

class Hero:public Parameter{

protected:
	int o_no;//オブジェクトナンバー
	MeshData *map_walk;//Mapクラスから操作(o_no=1のみ生成)
	char **map_walk_pass;
	MeshData *p_att;     //アタックアニメーション用
	char **p_att_pass;    //objファイルパス
	float p_att_cnt;      //アニメーションカウント
	int p_att_Ind;       //アニメーション描画時のオブジェクトインデックス
	int ObjCntMax;      //アニメーションオブジェクト個数
	float LA, LA_x, LA_y; //LostAction用thetaZによって変わる

	PolygonData2D state, meter;
	PolygonData mag, effect[4];
	int effect_no = 0;
	float mov_y;
	float mov_x;
	float mov_z;
	Action act_f;
	bool up;
	float count;
	bool effect_f;//エフェクトON,OFF
	float tx, ty;//エフェクト用テクスチャ座標
	float tt;     //エフェクト速度調整用

	void Statecreate(bool command_run);
	void Metercreate(float me);
	void Magiccreate();
	bool Effectdraw(Battle *battle, int *select_obj, Position::H_Pos *h_pos, Position::E_Pos *e_pos);

public:
	Hero();
	Hero(int no);
	void P_DataInput(P_Data *p_dat);
	void OBJWalkDraw(float x, float y, float z, float r, float g, float b, float theta, int walkI);//walkIはMapクラスから操作(-1～)
	Act_fin_flg Statedraw(Battle *battle, int *select_obj, Position::H_Pos *h_pos, Position::E_Pos *e_pos, float me, bool command_run, Action action, MagicSelect H_Magrun);
	Action Normal_act_get();
	void Act_f_init();
	virtual ~Hero();
};

#endif