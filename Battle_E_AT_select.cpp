//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@	         Battle�N���X                                    **//
//**                                    E_AT_select�֐�                                  **//
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
		for (int i = 0; i < e_num; i++)e_draw[i].command_run = FALSE;//�R�}���h���͂������̂ŏ������シ�����őI�����擾
		for (int i = 0; i < e_num; i++){
			if (enemy[i].Dieflg() == FALSE && (e_draw[i].AGmeter += tfloat.Add(enemy[i].GetAgility())) > METER_MAX){
				e_draw[i].command_run = TRUE;
				if (enemy[i].M_run_flg() == FALSE){
					do{
						if (hero[0].Dieflg() == TRUE && hero[1].Dieflg() == TRUE && hero[2].Dieflg() == TRUE && hero[3].Dieflg() == TRUE){
							e_draw[i].AGmeter = 0;
							return NOSELECT;//���������ꍇ�������ĂȂ���ԂȂ̂Ń^�C���X�g�b�v���Ȃ��Ŕ�����B
						}
						rnd = rand() % 4;//�����_���I��
					} while (hero[rnd].Dieflg() == TRUE);
					E_select_obj = rnd;
					return ATT;
				}
				else{
					//�}�W�b�N�I��
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
									return NOSELECT;//���������ꍇ�������ĂȂ���ԂȂ̂Ń^�C���X�g�b�v���Ȃ��Ŕ�����B
								}
								r1 = rand() % 4;//�����_���I��
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
									return NOSELECT;//���������ꍇ�������ĂȂ���ԂȂ̂Ń^�C���X�g�b�v���Ȃ��Ŕ�����B
								}
								r1 = rand() % e_num;//�����_���I��
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
							if (enemy[i2].Dieflg() == TRUE && e_pos[i2].element == TRUE){//�퓬�s�\�ҒT��
								i1 = i2;
								break;
							}
						}
						if (i1 != -1){//�퓬�s�\�җL
							E_select_obj = i1;
						}
						else {
							//�S�������Ă�ꍇ�͐����Ă��̍ŏ��Ƀq�b�g������ɂ�����(����)
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
