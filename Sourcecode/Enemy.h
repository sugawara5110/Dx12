//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Enemyクラス                                     **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Enemy_Header
#define Class_Enemy_Header

#include "Parameter.h"
#include "Position.h"

#define ENEMY_COM 4

//前方宣言
class Battle;
//前方宣言

class Enemy :public Parameter{

protected:
	int o_no;//オブジェクトナンバー
	int e_no;//敵ナンバー
	float pos_offset;//ボス位置オフセット
	Position::H_Pos *h_pos;
	Position::E_Pos *e_pos;
	PolygonData *en, *mag, effect[4];
	int effect_no = 0;
	MeshData *en_boss_att0;       //boss2のみ使用
	SkinMesh *en_boss_att;       //ボスアニメーション
	bool attOn, attFin;         //attアニメーション中
	bool magicAttOn;            //magicアニメーションフラグ
	ParticleData *mag_boss;    //ボス用
	float mag_size;           //ボス用

	bool esc;//エスケープ可否 TRUE==エスケープ可
	float mov_z;
	float mov_y;
	float mov_x;
	float size_x;
	float size_y;
	Action normal_action;//通常時のアクション
	Action act_f;
	int M_run_flg_rnd;  //マジック実行フラグ生成用ランダム
	bool up;
	bool zoom;
	float count;
	bool effect_f;//エフェクトON,OFF
	float tx, ty;//エフェクト用テクスチャ座標
	float tt;     //エフェクト速度調整用
	float cr, cg, cb;//敵画像色

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
	virtual bool M_run_flg();//マジック実行フラグ
	virtual void M_select(int *r, int *r1);//マジック選択
	bool s_esc();
	virtual ~Enemy();
};

#endif
