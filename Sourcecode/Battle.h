//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Battleクラス                                    **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Battle_Header
#define Class_Battle_Header

#include "MovieSoundManager.h"
#include "Position.h"
#include "../../Common/Direct3DWrapper/DxText.h"
#include "../../Common/Window/Control.h"
#include "Enum.h"

#define METER_MAX 25000
#define DrawYMAX -160
#define DrawYMIN -180

//前方宣言
class Map;
class Parameter;
class Hero;
class Enemy;
class EnemySide;
class EnemyBoss;
//前方宣言

class Battle {

private:
	//初期パラメーター
	Hero* he_para;
	Position::E_Pos* e_po_para;
	Position::H_Pos* h_po_para;
	Encount encount_para;
	int no_para, e_nu_para;

	Dx12Process* dx;
	DxText* text;
	T_float tfloat;
	int comNo;
	PolygonData2D command, h_select;//コマンド選択ウインドウ, 回復選択カーソル
	PolygonData E_select;//敵選択カーソル
	Enemy* enemy;
	EnemySide* enemyside;
	EnemyBoss* enemyboss;
	int e_num;            //敵出現数
	bool command_run_first_flg;//コマンド選択権無の状態 == FALSE、コマンド選択後TRUEとなる。1体ずつ
	bool time_stop_flg;       //時間ストップフラグ, 1連のアクション～HPMP増減は1体ずつ処理なのでフラグは1個でok
	float Escape_f;          //エスケープ表示フラグ,座標
	bool Escape_s;          //エスケープ成功
	bool battlefirst;      //開始直後のカメラ移動完了フラグ
	bool CamActOn;        //
	int CamActInd;
	bool CreateFin;

	CommandSelect com_select;   //コマンド入力(1ループに付き1体ずつなので1個でok↓も同様)
	bool          comDraw;
	CommandSelect E_com_select;//敵用(テンプレート関数使用の為引数揃える)
	MagicSelect MAG_select;   //選択したマジック
	MagicSelect E_MAG_select;//敵用
	int select_obj;           //選択対象番号敵,プレイヤー共通,エフェクト選択(4==全体)
	int E_select_obj;        //敵用(エフェクト選択用)(4==全体)

	typedef struct {
		float AGmeter;        //メーター
		Action action;
		MagicSelect Magrun;//選択マジック
		float RCVdrawY;     //回復表示フラグ,表示座標
		int RCVdata;       //回復数保管
		bool Recov_f;      //リカバリー開始フラグ終了時FALSE
		float DMdrawY;    //ダメージ表示フラグ,表示座標Yの上下量
		int DMdata;      //ダメージ数保管
		float draw_x;   //数字表示位置
		float draw_y;   //数字表示位置
		bool command_run;  //コマンドアクセス権状態,攻撃スタートフラグ
		bool LOST_fin;    //LOSTアクション終了フラグ
		//↓コマンド入力
		MenuSelect manu;//現選択メニュー(以下コマンド選択関連)
		int M_select;    //メインメニューカーソル位置
		int A_select;   //攻撃対象カーソル位置
		int MA_select; //マジック対象カーソル位置
		int R_select; //回復対象カーソル位置 
	}Draw;
	Draw h_draw[4];
	Draw* e_draw;
	Position::E_Pos e_pos[4];
	Position::H_Pos h_pos;
	Position::Bt_H_Pos* b_pos;

	void Debug(Enemy* enemy);//デバック用
	void Menucreate();
	void Cursor_h(int no);
	void Cursor_e(int select, float* r, float* b);
	void SelectPermissionMove(Hero* hero);
	CommandSelect E_AT_select(Hero* hero);
	CommandSelect Menu_main(Hero* hero, int i, Directionkey direction);
	CommandSelect Menu_AT(int i, Directionkey direction);
	CommandSelect Menu_MAG_AT(int i, Directionkey direction);
	CommandSelect Menu_RCV(Hero* hero, int i, Directionkey direction);
	CommandSelect H_AT_select(Hero* hero, int i, Directionkey direction);
	bool Escapedraw();
	void E_drawPos(int i);
	void H_drawPos(int i);

	template<typename T_rcv>
	void ValueDraw(T_rcv* rcv, Draw* dm, Draw* rc, int dmI, int rcI) {
		//↓攻撃対象又は回復対象のアクション,データ処理開始
		bool draw_flg = FALSE;
		for (int i1 = 0; i1 < dmI; i1++) {
			if (dm[i1].DMdata >= 0) {
				dm[i1].action = DAMAGE;//対象がダメージ受けたのでこの後ダメージ処理
				dm[i1].DMdrawY = DrawYMIN;//パラメーター間違えると数字表示されないので注意,フラグオン
				draw_flg = TRUE;
			}
		}
		for (int i1 = 0; i1 < rcI; i1++) {
			if (rc[i1].RCVdata >= 0) {
				if (rc[i1].Recov_f == TRUE) {
					rcv[i1].Dieflg(FALSE);
					rc[i1].action = RECOVER;//対象がリカバリーを受けたのでこの後処理
					rc[i1].LOST_fin = FALSE;
				}
				else {
					rc[i1].action = rcv[i1].Normal_act_get();//対象が回復受けたのでこの後回復処理(回復用actは無い)
				}
				rc[i1].RCVdrawY = DrawYMIN;//パラメーター間違えると数字表示されないので注意,フラグオン
				rc[i1].Recov_f = FALSE;//actionにRECOVERが入った時点でもう用無し,これをここでやらない場合はHEALに追加必要
				draw_flg = TRUE;
			}
		}
		if (draw_flg == FALSE)time_stop_flg = FALSE;//何も行動無しなのでストップ解除(MP足りない場合)
	}

	template<typename T_dm, typename T_att>
	void ATprocess(T_dm* dm, T_att* att, Draw* d, Draw* at) {
		if (dm->Dieflg() == FALSE) {
			at->action = ATTACK;
			d->DMdata = att->GetAttack();
			time_stop_flg = TRUE;
		}
		at->AGmeter = 0;
	}

	template<typename T_dm, typename T_att>
	void MAGprocess(T_dm* p_dm, T_att* p_att, T_att* att, Draw* at, Draw* p_at, Draw* p_d, int* select_ob, MagicSelect* select_M, TemplateType type) {
		int att_n;
		int dm_n;
		if (type == E_ATT) {
			att_n = e_num;
			dm_n = 4;
		}
		if (type == H_ATT) {
			att_n = 4;
			dm_n = e_num;
		}
		int cnt = 0;
		at->action = MAGIC;
		//MP処理
		if (att->DownMP(*select_M) == FALSE) {
			//MP足りない場合の処理
			at->AGmeter = 0;
			at->Magrun = NOSEL;
			time_stop_flg = TRUE;
			return;
		}
		//マジック攻撃処理
		switch (*select_M) {
		case FLAME:
			att->s_Fp(1);
			//全体攻撃
			if (*select_ob == 4) {
				for (int i = 0; i < dm_n; i++) {
					if (p_dm[i].Dieflg() == FALSE)cnt++;//対象人数でダメージ変化
				}
				for (int i = 0; i < dm_n; i++) {
					if (p_dm[i].Dieflg() == TRUE)continue;
					p_d[i].DMdata = att->GetMagic(FLAME, cnt);
				}
			}
			else {
				//単体攻撃
				if (p_dm[*select_ob].Dieflg() == TRUE) {
					//選択後攻撃直前に対象LOSTの場合の処理
					at->action = NORMAL; at->AGmeter = 0; return;
				}
				p_d[*select_ob].DMdata = att->GetMagic(FLAME, 1);
			}
			break;
		case HEAL:
			att->s_Hp(1);
			//全体攻撃
			if (*select_ob == 4) {
				for (int i = 0; i < att_n; i++) {
					if (p_att[i].Dieflg() == FALSE)cnt++;
				}
				for (int i = 0; i < att_n; i++) {
					if (p_att[i].Dieflg() == TRUE)continue;
					p_at[i].RCVdata = att->GetMagic(HEAL, cnt);
				}
			}
			else {
				//単体攻撃
				if (p_att[*select_ob].Dieflg() == TRUE) {
					//選択後回復直前に対象LOSTの場合の処理
					at->action = NORMAL;  at->AGmeter = 0; return;
				}
				p_at[*select_ob].RCVdata = att->GetMagic(HEAL, 1);
			}
			break;
		case RECOV:
			att->s_Rp(1);
			//戦闘不能者選択:復活  それ以外選択:小回復
			p_at[*select_ob].RCVdata = att->GetMagic(RECOV, 1);
			if (p_att[*select_ob].Dieflg() == TRUE) {
				p_at[*select_ob].Recov_f = TRUE;//リカバリーフラグ
			}
			break;
		}
		at->AGmeter = 0;
		time_stop_flg = TRUE;
	}

	template<typename T_rcv>
	void RCVdraw(T_rcv* rcv, Draw* at, float adjustX, float adjustY) {
		if (at->RCVdrawY != 0) {
			if ((at->RCVdrawY += tfloat.Add(0.1f)) < DrawYMAX) {
				text->UpDateValue(at->RCVdata, at->draw_x + adjustX, at->draw_y + at->RCVdrawY + adjustY, 30.0f, 5, { 0.3f, 1.0f, 0.3f, 1.0f });
			}
			else {
				if (at->RCVdrawY >= DrawYMAX && at->RCVdrawY < DrawYMAX + 20.0f) {
					text->UpDateValue(at->RCVdata, at->draw_x + adjustX, at->draw_y + DrawYMAX + adjustY, 30.0f, 5, { 0.3f, 1.0f, 0.3f, 1.0f });
				}
				else {
					rcv->UpHP(at->RCVdata);
					at->RCVdrawY = 0;//ここで初期化しないと数値表示されっぱなし
					time_stop_flg = FALSE;
				}
			}
		}
	}

	template<typename T_dm>
	void DMdraw(T_dm* dm, Draw* d, float adjustX, float adjustY) {
		if (d->DMdrawY != 0) {
			if ((d->DMdrawY += tfloat.Add(0.1f)) < DrawYMAX) {
				text->UpDateValue(d->DMdata, d->draw_x + adjustX, d->draw_y + d->DMdrawY + adjustY, 30.0f, 5, { 1.0f, 0.3f, 0.3f, 1.0f });
			}
			else {
				if (d->DMdrawY >= DrawYMAX && d->DMdrawY < DrawYMAX + 20.0f) {
					text->UpDateValue(d->DMdata, d->draw_x + adjustX, d->draw_y + DrawYMAX + adjustY, 30.0f, 5, { 1.0f, 0.3f, 0.3f, 1.0f });
				}
				else {
					dm->DownHP(d->DMdata);
					d->DMdrawY = 0;//ここで初期化しないと数値表示されっぱなし
					time_stop_flg = FALSE;
				}
			}
		}
	}

public:
	Battle() {}
	void SetParameter(Hero* he, Position::E_Pos* e_pos, Position::H_Pos* h_pos, Encount encount, int no, int e_nu);
	void Init();
	void SetVertex();
	void SetCommandList(int com_no);
	void CreateBattle();
	bool CreateB_Fin();
	Result FightUpdate(Hero* he, Directionkey direction, Result result);
	void FightDraw(Encount enc);
	void StreamOutput(Encount enc);
	void StreamOutputAfterDraw(Encount enc);
	void Draw2D(Encount enc);
	Position::Bt_H_Pos* GetBtPos(Position::H_Pos* h_p);
	bool GetH_DM(int element);
	bool GetH_RCV(int element);
	bool GetE_DM(int element);
	bool GetE_RCV(int element);
	bool GetBossEffectState(float* blur);
	~Battle();

	ParameterDXR** getParameterDXR(int* numPara);
	void setPointLightNo();
};

#endif