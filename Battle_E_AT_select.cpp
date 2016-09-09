//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Battleクラス                                    **//
//**                                    E_AT_select関数                                  **//
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

CommandSelect Battle::E_AT_select(Hero *hero){

	int rnd;
	int r = -1;
	int r1;
	if (time_stop_flg == FALSE){
		for (int i = 0; i < e_num; i++)e_draw[i].command_run = FALSE;//コマンド入力が無いので初期化後すぐ下で選択権取得
		for (int i = 0; i < e_num; i++){
			if (enemy[i].Dieflg() == FALSE && (e_draw[i].AGmeter += tfloat.Add(enemy[i].GetAgility())) > METER_MAX){
				e_draw[i].command_run = TRUE;
				if (enemy[i].M_run_flg() == FALSE){
					do{
						if (hero[0].Dieflg() == TRUE && hero[1].Dieflg() == TRUE && hero[2].Dieflg() == TRUE && hero[3].Dieflg() == TRUE){
							e_draw[i].AGmeter = 0;
							return NOSELECT;//ここ来た場合何もしてない状態なのでタイムストップしないで抜ける。
						}
						rnd = rand() % 4;//ランダム選択
					} while (hero[rnd].Dieflg() == TRUE);
					E_select_obj = rnd;
					return ATT;
				}
				else{
					//マジック選択
					enemy[i].M_select(&r, &r1);

					switch (r){
					case 0:
						e_draw[i].Magrun = FLAME;
						E_MAG_select = FLAME;
						if (r1 == 0){
							E_select_obj = 4;
						}
						else{
							do{
								if (hero[0].Dieflg() == TRUE && hero[1].Dieflg() == TRUE && hero[2].Dieflg() == TRUE && hero[3].Dieflg() == TRUE){
									e_draw[i].AGmeter = 0;
									return NOSELECT;//ここ来た場合何もしてない状態なのでタイムストップしないで抜ける。
								}
								r1 = rand() % 4;//ランダム選択
							} while (hero[r1].Dieflg() == TRUE);
							E_select_obj = r1;
						}
						return MAG;
						break;

					case 1:
						e_draw[i].Magrun = HEAL;
						E_MAG_select = HEAL;
						if (r1 == 0){
							E_select_obj = 4;
						}
						else{
							do{
								if (enemy[0].Dieflg() == TRUE &&
									(e_pos[1].element == FALSE || enemy[1].Dieflg() == TRUE) &&
									(e_pos[2].element == FALSE || enemy[2].Dieflg() == TRUE) &&
									(e_pos[3].element == FALSE || enemy[3].Dieflg() == TRUE)){
									e_draw[i].AGmeter = 0;
									return NOSELECT;//ここ来た場合何もしてない状態なのでタイムストップしないで抜ける。
								}
								r1 = rand() % e_num;//ランダム選択
							} while (enemy[r1].Dieflg() == TRUE);
							E_select_obj = r1;
						}
						return MAG;
						break;

					case 2:
						int i1 = -1;
						e_draw[i].Magrun = RECOV;
						E_MAG_select = RECOV;
						for (int i2 = 0; i2 < e_num; i2++){
							if (enemy[i2].Dieflg() == TRUE && e_pos[i2].element == TRUE){//戦闘不能者探索
								i1 = i2;
								break;
							}
						}
						if (i1 != -1){//戦闘不能者有
							E_select_obj = i1;
						}
						else {
							//全員生きてる場合は生きてるやつの最初にヒットしたやつにかける(小回復)
							for (int i2 = 0; i2 < e_num; i2++){
								if (e_pos[i2].element == FALSE)continue;
								E_select_obj = i2;
								break;
							}
						}
						return MAG;
						break;
					}
				}
			}
		}
	}

	return NOSELECT;
}
