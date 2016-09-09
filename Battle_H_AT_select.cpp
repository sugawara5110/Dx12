//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@	         Battle�N���X                                    **//
//**                                   H_AT_select�֐�                                   **//
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

CommandSelect Battle::H_AT_select(Hero *hero, int i, Directionkey direction){

	CommandSelect f, f1;
	if (hero[i].Dieflg() == TRUE){ h_draw[i].AGmeter = 0; return NOSELECT; }
	if (h_draw[i].AGmeter > METER_MAX || time_stop_flg == FALSE && (h_draw[i].AGmeter += tfloat.Add(hero[i].GetAgility())) > METER_MAX){
		h_draw[i].AGmeter = METER_MAX + 1;//�͈͊O�h�~(�R�}���h�I��ۗ��ő��������邩��)
		//�I�����N�������̏�Ԃ���ŏ��̑I�����擾�҂̏ꍇ�����őI�����擾
		if (command_run_first_flg == FALSE){
			h_draw[i].command_run = TRUE;
			command_run_first_flg = TRUE;//������Ȃ��Ɩ���I�����ړ����܂���
		}
		//�I�����L�̂ݎ��s, 1�̂��ƂȂ�
		if (h_draw[i].command_run == TRUE){
			switch (h_draw[i].manu){
			case MAIN_M:
				f = Menu_main(hero, i, direction);
				if (f != NOSELECT){
					if (f == ATT){
						h_draw[i].manu = AT_M;
						return OTHER;
					}
					if (f == MAG){
						if (MAG_select == FLAME)h_draw[i].manu = MAG_AT_M;
						else h_draw[i].manu = MAG_RE_M;
						return OTHER;
					}
					if (f == ESCAPE){
						return ESCAPE;
					}
					if (f == OTHER){
						return OTHER;//�I�����ړ��㑦���[�v�������Ȃ��Ƃ��̂܂ܑI�����ړ���̏����ɂȂ�̂Ő؂�ւ��Ȃ�
					}
				}
				break;
			case AT_M:
				f1 = Menu_AT(i, direction);
				if (f1 != NOSELECT){
					if (f1 == ATT){
						h_draw[i].manu = MAIN_M;
						return ATT;
					}
					if (f1 == CANCEL_M){
						h_draw[i].manu = MAIN_M;
						return OTHER;
					}
				}
				break;
			case MAG_AT_M:
				f1 = Menu_MAG_AT(i, direction);
				if (f1 != NOSELECT){
					if (f1 == MAG){
						h_draw[i].manu = MAIN_M;
						return MAG;
					}
					if (f1 == CANCEL_M){
						h_draw[i].manu = MAIN_M;
						return OTHER;
					}
				}
				break;
			case MAG_RE_M:
				f1 = Menu_RCV(hero, i, direction);
				if (f1 != NOSELECT){
					if (f1 == MAG){
						h_draw[i].manu = MAIN_M;
						return MAG;
					}
					if (f1 == CANCEL_M){
						h_draw[i].manu = MAIN_M;
						return OTHER;
					}
				}
				break;
			}
		}
	}

	return NOSELECT;
}
