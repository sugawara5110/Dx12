//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Battleクラス                                    **//
//**                                      Fight関数                                      **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <new>     //placement new
#include <stdlib.h>
#include <time.h>
#include "Enemy.h"
#include "EnemySide.h"
#include "EnemyBoss.h"
#include "Battle.h"
#include "Hero.h"

Result Battle::Fight(Hero *hero, Directionkey direction, Result result) {

	Position::H_Pos h_posOut;//視点変換後用
	//視点初期値
	h_posOut.cx = h_pos->cx;//視点
	h_posOut.cy = h_pos->cy;//視点
	h_posOut.cx2 = h_pos->cx2;//注視点
	h_posOut.cy2 = h_pos->cy2;//注視点
	h_posOut.cz = h_pos->cz;
	h_posOut.theta = h_pos->theta;

	dx->Bigin(ENEMY_COM, nullptr);

	Debug(enemy);

	//戦闘開始直後視点
	if (!battlefirst) {
		battlefirst = Position::CamRecession(h_pos, &h_posOut, tfloat.Add(0.1f));
		if (battlefirst)Position::CamRecession(NULL, NULL, 0);//初期化(初期化しておかないと次回戦闘時機能しない)
	}

	if (CamActInd != -1) {
		Position::CamActionBackView(&b_pos[CamActInd], &h_posOut, tfloat.Add(0.1f), CamActOn, h_draw[CamActInd].action);
		char lr;
		if (CamActInd == 0 || CamActInd == 1)lr = 'L'; else lr = 'R';
		Position::CamActionRoundView(&b_pos[CamActInd], &h_posOut, tfloat.Add(0.3f), CamActOn, h_draw[CamActInd].action, lr);
	}

	float cx = h_posOut.cx;//視点
	float cy = h_posOut.cy;//視点
	float cx2 = h_posOut.cx2;//注視点
	float cy2 = h_posOut.cy2;//注視点
	float cz = h_posOut.cz;

	dx->Cameraset(cx, cx2, cy, cy2, cz, cz);
	Act_fin_flg act;
	//敵戦闘不能アクションフラグ
	//LOSTは発生したらずっとなので注意
	for (int i = 0; i < e_num; i++)if (enemy[i].Dieflg() == TRUE)e_draw[i].action = LOST;
	//敵表示
	for (int i = 0; i < e_num; i++) {
		if (e_draw[i].LOST_fin == FALSE) {//敵側はLOST_fin==TRUE状態だと表示されないようにする
			act = enemy[i].Enemydraw(this, &E_select_obj, e_draw[i].action, e_draw[i].Magrun);
			switch (act) {
			case AT_FIN://敵側攻撃,回復終了
				ValueDraw<Enemy>(enemy, h_draw, e_draw, 4, e_num);//フラグ, action = DAMAGE or RECOVER
				break;
			case LOST_FIN://敵側戦闘不能アクション終了
				e_draw[i].LOST_fin = TRUE;
				break;
			}
		}
		e_draw[i].action = enemy[i].Normal_act_get();//↑actionは一度入れればOKな構造なので入れたら即初期化する。これやらないとずっとactionしっぱなし。
	}

	//プレイヤー戦闘不能アクションフラグ
	//LOSTは発生したらずっとなので注意
	for (int i = 0; i < 4; i++)if (hero[i].Dieflg() == TRUE)h_draw[i].action = LOST;
	//プレイヤーステータス表示
	for (int i = 0; i < 4; i++) {
		act = hero[i].Statedraw(this, &select_obj, h_pos, e_pos, h_draw[i].AGmeter / METER_MAX, h_draw[i].command_run, h_draw[i].action, h_draw[i].Magrun);
		switch (act) {
		case AT_FIN://プレイヤー側攻撃,回復終了
			ValueDraw<Hero>(hero, e_draw, h_draw, e_num, 4);//フラグ, action = DAMAGE or RECOVER
			CamActOn = FALSE;
			break;
		case LOST_FIN:
			h_draw[i].LOST_fin = TRUE;
			break;
		}
		h_draw[i].action = NORMAL;
	}

	//Debug(enemy);//デバック用

	//GAMEOVER時ループスキップ
	if (result == DIE) {
		MovieSoundManager::Die_sound(TRUE);
		dx->End(ENEMY_COM);
		return DIE;
	}

	MovieSoundManager::Enemy_sound(TRUE);

	//初期視点終了まで繰り返し
	if (!battlefirst) {
		dx->End(ENEMY_COM); return IN_BATTLE;
	}

	//敵攻撃パターン決定1体ずつ
	E_com_select = E_AT_select(hero);//メーターMAX時行動は即決定するがテンプレート関数使用の関係でプレイヤー側と合わせている。

	//敵攻撃
	if (time_stop_flg == FALSE && E_com_select != NOSELECT && E_com_select != OTHER) {//上に同じ
		//数値保管初期化
		for (int i = 0; i < 4; i++)h_draw[i].DMdata = -1;
		for (int i = 0; i < e_num; i++)e_draw[i].RCVdata = -1;

		if (E_com_select == ATT) {
			for (int i = 0; i < e_num; i++) {
				if (enemy[i].Dieflg() == FALSE && e_draw[i].command_run == TRUE) {//command_run == TRUEは1体しかならない
					//↓time_stop始まり, action = ATTACK
					ATprocess<Hero, Enemy>(&hero[E_select_obj], &enemy[i], &h_draw[E_select_obj], &e_draw[i]);
					break;//1体検出したら後は無いので抜ける
				}
			}
		}
		if (E_com_select == MAG) {
			for (int i = 0; i < e_num; i++) {
				if (enemy[i].Dieflg() == FALSE && e_draw[i].command_run == TRUE) {
					//↓time_stop始まり, action = MAGIC
					MAGprocess<Hero, Enemy>(hero, enemy, &enemy[i], &e_draw[i], e_draw, h_draw, &E_select_obj, &E_MAG_select, E_ATT);
					break;
				}
			}
		}
		E_com_select = NOSELECT;
	}
	//敵回復
	for (int i = 0; i < e_num; i++) {
		E_drawPos(i);//↓の文字表示の位置計算
		RCVdraw<Enemy>(&enemy[i], &e_draw[i], -50.0f, 0.0f);//←time_stop終わり, フラグある限り全て実行
	}
	//プレイヤーダメージ
	for (int i = 0; i < 4; i++) {
		H_drawPos(i);
		DMdraw<Hero>(&hero[i], &h_draw[i], -100.0f, 0.0f);//←time_stop終わり, フラグある限り全て実行
	}

	//プレイヤー全滅
	if (hero[0].Dieflg() == TRUE && hero[1].Dieflg() == TRUE && hero[2].Dieflg() == TRUE && hero[3].Dieflg() == TRUE) {
		MovieSoundManager::Enemy_soundoff();
		MovieSoundManager::Die_sound(FALSE);
		SelectPermissionMove(hero);
		dx->End(ENEMY_COM);
		return DIE;
	}

	//戦闘不能時コマンド選択権破棄,アクション初期化これやらないと戦闘不能者の所でコマンド入力止まったまま
	for (int i = 0; i < 4; i++) {
		if (hero[i].Dieflg() == TRUE) {
			if (h_draw[i].command_run == TRUE) {
				SelectPermissionMove(hero);
				com_select = NOSELECT;
			}
			hero[i].Act_f_init();
		}
	}

	//コマンド入力
	//行動中(time_stop_flg == TRUE)でも選択可能状態に入ってる場合は選択のみ実行できる
	//コマンド入力後,行動終(time_stop_flg == FALSE)になるまでは↓に入れない(com_select != NOSELECTの為)
	if (com_select == NOSELECT || com_select == OTHER) {
		for (int i = 0; i < 4; i++) {
			com_select = H_AT_select(hero, i, direction);
			if (com_select != NOSELECT)break;
		}
	}

	//プレイヤー攻撃
	if (time_stop_flg == FALSE && com_select != NOSELECT && com_select != OTHER) {//選択済みの場合実行される
		//数値保管初期化
		for (int i = 0; i < e_num; i++)e_draw[i].DMdata = -1;
		for (int i = 0; i < 4; i++)h_draw[i].RCVdata = -1;

		if (com_select == ATT) {
			for (int i = 0; i < 4; i++) {
				if (hero[i].Dieflg() == FALSE && h_draw[i].command_run == TRUE) {
					//↓time_stop始まり, action = ATTACK
					ATprocess<Enemy, Hero>(&enemy[select_obj], &hero[i], &e_draw[select_obj], &h_draw[i]);
					bool f = FALSE;
					if (rand() % 2 == 0)f = TRUE;
					CamActOn = f;
					CamActInd = i;
					//選択権移動これやらないと他のメンバがコマンド選択できない
					SelectPermissionMove(hero);
					break;
				}
			}
		}
		if (com_select == MAG) {
			for (int i = 0; i < 4; i++) {
				if (hero[i].Dieflg() == FALSE && h_draw[i].command_run == TRUE) {
					//↓time_stop始まり, action = MAGIC
					MAGprocess<Enemy, Hero>(enemy, hero, &hero[i], &h_draw[i], h_draw, e_draw, &select_obj, &MAG_select, H_ATT);
					bool f = FALSE;
					if (rand() % 2 == 0)f = TRUE;
					CamActOn = f;
					CamActInd = i;
					//選択権移動これやらないと他のメンバがコマンド選択できない
					SelectPermissionMove(hero);
					break;
				}
			}
		}
		if (com_select == ESCAPE) {
			int h_esc = 0;
			int e_esc = 0;
			for (int i = 0; i < 4; i++) {
				if (hero[i].Dieflg() == TRUE)continue;
				h_esc += hero[i].s_AG();
			}
			for (int i = 0; i < e_num; i++) {
				if (enemy[i].Dieflg() == TRUE)continue;
				e_esc += enemy[i].s_AG();
			}
			//エスケープ可否確認
			bool es = TRUE;
			for (int i = 0; i < e_num; i++) {
				if (enemy[i].Dieflg() == TRUE)continue;
				if (enemy[i].s_esc() == FALSE) {
					es = FALSE;
					break;
				}
			}
			if (es == TRUE && h_esc > e_esc)Escape_s = TRUE;
			else Escape_s = FALSE;
			Escape_f = 300;
			SelectPermissionMove(hero);
			for (int i = 0; i < 4; i++)h_draw[i].AGmeter = 0;
		}
		com_select = NOSELECT;//これを実行しないとコマンド入力入れない
	}
	//プレイヤー回復
	for (int i = 0; i < 4; i++) {
		H_drawPos(i);
		RCVdraw<Hero>(&hero[i], &h_draw[i], -100.0f, 0.0f);//←time_stop終わり, フラグある限り全て実行
	}
	//敵ダメージ
	for (int i = 0; i < e_num; i++) {
		E_drawPos(i);
		DMdraw<Enemy>(&enemy[i], &e_draw[i], -50.0f, 0.0f);//←time_stop終わり, フラグある限り全て実行
	}

	//エスケープ表示
	if (Escapedraw() == FALSE) {
		if (Escape_s == TRUE) {
			dx->End(ENEMY_COM); return WIN;
		}
	}
	//プレイヤー全部メーター貯まってない状態での選択権破棄
	if (h_draw[0].AGmeter <= METER_MAX && h_draw[1].AGmeter <= METER_MAX && h_draw[2].AGmeter <= METER_MAX && h_draw[3].AGmeter <= METER_MAX) {
		SelectPermissionMove(hero);
	}

	//敵全滅→Exp処理
	if (e_draw[0].LOST_fin == TRUE &&
		(e_pos[1].element == FALSE || e_draw[1].LOST_fin == TRUE) &&
		(e_pos[2].element == FALSE || e_draw[2].LOST_fin == TRUE) &&
		(e_pos[3].element == FALSE || e_draw[3].LOST_fin == TRUE)) {
		int GetExp = 0;
		for (int i = 0; i < e_num; i++) {
			if (e_pos[i].element == TRUE)GetExp += enemy[i].s_Exp();
		}
		for (int i = 0; i < 4; i++) {
			if (hero[i].Dieflg() == TRUE)continue;
			hero[i].s_Exp(GetExp);
		}
		for (int i = 0; i < 4; i++) {
			hero[i].Act_f_init();
		}
		dx->End(ENEMY_COM);
		return WIN;
	}

	dx->End(ENEMY_COM);
	return IN_BATTLE;
}

Position::Bt_H_Pos *Battle::GetBtPos(Position::H_Pos *h_p){
	//戦闘中各オブジェクト配置位置
	static Position::Bt_H_Pos b_p[4];
	for (int i = 0; i < 4; i++){
		float ajst = ((float)i - 1.5f) * 20.0f;
		switch ((int)h_p->theta){
		case 360:
		case 0:
			b_p[i].BtPos_x1 = h_p->cx1 + ajst;
			b_p[i].BtPos_x2 = h_p->cx2 + ajst;
			b_p[i].BtPos_y1 = h_p->cy1;
			b_p[i].BtPos_y2 = h_p->cy2;
			break;
		case 90:
			b_p[i].BtPos_x1 = h_p->cx1;
			b_p[i].BtPos_x2 = h_p->cx2;
			b_p[i].BtPos_y1 = h_p->cy1 + ajst;
			b_p[i].BtPos_y2 = h_p->cy2 + ajst;
			break;
		case 180:
			b_p[i].BtPos_x1 = h_p->cx1 - ajst;
			b_p[i].BtPos_x2 = h_p->cx2 - ajst;
			b_p[i].BtPos_y1 = h_p->cy1;
			b_p[i].BtPos_y2 = h_p->cy2;
			break;
		case 270:
			b_p[i].BtPos_x1 = h_p->cx1;
			b_p[i].BtPos_x2 = h_p->cx2;
			b_p[i].BtPos_y1 = h_p->cy1 - ajst;
			b_p[i].BtPos_y2 = h_p->cy2 - ajst;
			break;
		}
	}
	return b_p;
}

void Battle::E_drawPos(int i){
	VECTOR3 p3;
	p3.as(e_pos[i].x, e_pos[i].y, e_pos[i].z);
	PolygonData2D::Pos2DCompute(&p3);
	e_draw[i].draw_x = p3.x;
	e_draw[i].draw_y = p3.y;
}

void Battle::H_drawPos(int i){
	VECTOR3 p3;
	p3.as(b_pos[i].BtPos_x1, b_pos[i].BtPos_y1, h_pos->pz * 100.0f);
	PolygonData2D::Pos2DCompute(&p3);
	h_draw[i].draw_x = p3.x;
	h_draw[i].draw_y = p3.y;
}

bool Battle::Escapedraw() {

	float m = tfloat.Add(0.1f);
	if (Escape_f != 0) {
		if ((Escape_f += m) < 330) {
			TCHAR *str = L"エスケープ成功";
			TCHAR *str1 = L"エスケープ失敗";
			if (Escape_s == TRUE)text->UpDateText(&str, 300.0f, 300.0f, 35.0f, { 0.0f, 0.7f, 1.0f, 1.0f });
			if (Escape_s == FALSE)text->UpDateText(&str1, 300.0f, 300.0f, 35.0f, { 0.0f, 0.7f, 1.0f, 1.0f });
			return TRUE;
		}
		else {
			Escape_f = 0;
			return FALSE;
		}
	}
	return TRUE;
}

bool Battle::GetH_DM(int element){

	if (h_draw[element].DMdata >= 0)return TRUE;

	return FALSE;
}

bool Battle::GetH_RCV(int element){

	if (h_draw[element].RCVdata >= 0)return TRUE;

	return FALSE;
}

bool Battle::GetE_DM(int element){

	if (e_pos[element].element == FALSE)return FALSE;
	if (e_draw[element].DMdata >= 0)return TRUE;

	return FALSE;
}

bool Battle::GetE_RCV(int element){

	if (e_pos[element].element == FALSE)return FALSE;
	if (e_draw[element].RCVdata >= 0)return TRUE;

	return FALSE;
}

void Battle::Debug(Enemy *enemy) {//デバック用

	for (int i = 0; i < 4; i++) {
		if (e_pos[i].element == FALSE)continue;
		text->UpDateValue(enemy[i].s_HP(), 500.0f, 10.0f + 90.0f * i, 15.0f, 5, { 1.0f, 1.0f, 1.0f, 1.0f });
		text->UpDateValue(enemy[i].s_MHP(), 580.0f, 10.0f + 90.0f * i, 15.0f, 5, { 1.0f, 1.0f, 1.0f, 1.0f });
		text->UpDateValue(enemy[i].s_MP(), 500.0f, 25.0f + 90.0f * i, 15.0f, 5, { 1.0f, 1.0f, 1.0f, 1.0f });
		text->UpDateValue(enemy[i].s_MMP(), 580.0f, 25.0f + 90.0f * i, 15.0f, 5, { 1.0f, 1.0f, 1.0f, 1.0f });
		text->UpDateValue((int)e_draw[i].AGmeter, 500.0f, 40.0f + 90.0f * i, 15.0f, 5, { 1.0f, 1.0f, 1.0f, 1.0f });
	}
}