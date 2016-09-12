//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         StateMenuクラス                                 **//
//**                                      Menudraw関数                                   **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "Hero.h"
#include "StateMenu.h"

bool StateMenu::TitleMenu(Directionkey direction) {

	MovieSoundManager::Title_sound(TRUE);
	static int select = 0;
	float r1, r2, g1, g2;
	r1 = r2 = g1 = g2 = 0.0f;

	switch (select) {
	case 0:
		r2 = g2 = 1.0f;
		r1 = 0.0f;
		g1 = 0.7f;
		break;
	case 1:
		r1 = g1 = 1.0f;
		r2 = 0.0f;
		g2 = 0.7f;
		break;
	}

	switch (direction) {
	case UP:
		MovieSoundManager::Select_sound(FALSE);
		select = 0;
		break;
	case DOWN:
		MovieSoundManager::Select_sound(FALSE);
		select = 1;
		break;
	case ENTER:
		MovieSoundManager::Enter_sound(FALSE);
		if (select == 0) {
			for (int i = 0; i < 4; i++) {

				/*savedata.p_data[i].Attack = 1;
				savedata.p_data[i].Magic = 1;
				savedata.p_data[i].Agility = 1;
				savedata.p_data[i].Vitality = 1;
				savedata.p_data[i].Exp = 10;
				savedata.p_data[i].LV = 1;
				savedata.p_data[i].ParameterPoint = 0;
				savedata.p_data[i].HP = s_MAX_HP;
				savedata.p_data[i].MP = s_MAX_MP;
				savedata.p_data[i].FlameATT_LV = 0;
				savedata.p_data[i].Fpoint = 10;
				savedata.p_data[i].Healing_LV = 0;
				savedata.p_data[i].Hpoint = 10;
				savedata.p_data[i].Recover_LV = 0;
				savedata.p_data[i].Rpoint = 10;*/

				/*
				savedata.p_data[i].Attack = 10;
				savedata.p_data[i].Magic = 10;
				savedata.p_data[i].Agility = 10;
				savedata.p_data[i].Vitality = 10;
				savedata.p_data[i].Exp = 100;
				savedata.p_data[i].LV = 10;
				savedata.p_data[i].ParameterPoint = 0;
				savedata.p_data[i].HP = s_MAX_HP;
				savedata.p_data[i].MP = s_MAX_MP;
				savedata.p_data[i].FlameATT_LV = 10;
				savedata.p_data[i].Fpoint = 100;
				savedata.p_data[i].Healing_LV = 10;
				savedata.p_data[i].Hpoint = 100;
				savedata.p_data[i].Recover_LV = 10;
				savedata.p_data[i].Rpoint = 100;
				*/
				
				savedata.p_data[i].Attack = 100;
				savedata.p_data[i].Magic = 100;
				savedata.p_data[i].Agility = 100;
				savedata.p_data[i].Vitality = 100;
				savedata.p_data[i].Exp = 1000;
				savedata.p_data[i].LV = 100;
				savedata.p_data[i].ParameterPoint = 0;
				savedata.p_data[i].HP = s_MAX_HP;
				savedata.p_data[i].MP = s_MAX_MP;
				savedata.p_data[i].FlameATT_LV = 100;
				savedata.p_data[i].Fpoint = 1000;
				savedata.p_data[i].Healing_LV = 100;
				savedata.p_data[i].Hpoint = 1000;
				savedata.p_data[i].Recover_LV = 100;
				savedata.p_data[i].Rpoint = 1000;
				
			}

			savedata.h_pos.cx1 = 450.0f;//ボス前
			savedata.h_pos.cx2 = 450.0f;
			savedata.h_pos.cy1 = 1050.0f;
			savedata.h_pos.cy2 = 980.0f;
			savedata.h_pos.px = 4;
			savedata.h_pos.py = 10;//ボス前

			//通常のスタート位置
			savedata.h_pos.cx1 = 1150.0f;
			savedata.h_pos.cx2 = 1150.0f;
			savedata.h_pos.cy1 = 3350.0f;
			savedata.h_pos.cy2 = 3280.0f;
			savedata.h_pos.px = 11;
			savedata.h_pos.py = 33;
			savedata.h_pos.pz = 0;
			savedata.h_pos.theta = 0.0f;
			savedata.map_no = 0;
			//通常のスタート位置
			/*
			savedata.h_pos.cx1 = 950.0f;//ボス前
			savedata.h_pos.cx2 = 950.0f;
			savedata.h_pos.cy1 = 950.0f;
			savedata.h_pos.cy2 = 880.0f;
			savedata.h_pos.px = 9;
			savedata.h_pos.py = 9;//ボス前
			savedata.h_pos.cx1 = 1150.0f;
			savedata.h_pos.cx2 = 1150.0f;
			savedata.h_pos.cy1 = 3250.0f;
			savedata.h_pos.cy2 = 3180.0f;
			savedata.h_pos.px = 11;
			savedata.h_pos.py = 32;
			savedata.h_pos.pz = 0;
			savedata.h_pos.theta = 0.0f;
			savedata.map_no = 1;
			*/
			/*
			savedata.h_pos.cx1 = 150.0f;//ボス前
			savedata.h_pos.cx2 = 150.0f;
			savedata.h_pos.cy1 = 1250.0f;
			savedata.h_pos.cy2 = 1180.0f;
			savedata.h_pos.px = 1;
			savedata.h_pos.py = 12;//ボス前
			savedata.h_pos.cx1 = 150.0f;
			savedata.h_pos.cx2 = 150.0f;
			savedata.h_pos.cy1 = 3750.0f;
			savedata.h_pos.cy2 = 3680.0f;
			savedata.h_pos.px = 1;
			savedata.h_pos.py = 37;
			savedata.h_pos.pz = 0;
			savedata.h_pos.theta = 0.0f;
			savedata.map_no = 2;
			*/
			/*
			savedata.h_pos.cx1 = 450.0f;//ボス前
			savedata.h_pos.cx2 = 450.0f;
			savedata.h_pos.cy1 = 750.0f;
			savedata.h_pos.cy2 = 680.0f;
			savedata.h_pos.px = 4;
			savedata.h_pos.py = 7;//ボス前
			savedata.h_pos.cx1 = 250.0f;
			savedata.h_pos.cx2 = 250.0f;
			savedata.h_pos.cy1 = 2750.0f;
			savedata.h_pos.cy2 = 2680.0f;
			savedata.h_pos.px = 2;
			savedata.h_pos.py = 27;
			savedata.h_pos.pz = 0;
			savedata.h_pos.theta = 0.0f;
			savedata.map_no = 3;
			*/

			//ラスダン
			/*
			savedata.h_pos.cx1 = 1450.0f;//ボス前
			savedata.h_pos.cx2 = 1450.0f;
			savedata.h_pos.cy1 = 1550.0f;
			savedata.h_pos.cy2 = 1480.0f;
			savedata.h_pos.px = 14;
			savedata.h_pos.py = 15;
			savedata.h_pos.pz = 6;//ボス前
			savedata.h_pos.cx1 = 1150.0f;
			savedata.h_pos.cx2 = 1150.0f;
			savedata.h_pos.cy1 = 2750.0f;
			savedata.h_pos.cy2 = 2680.0f;
			savedata.h_pos.px = 11;
			savedata.h_pos.py = 27;
			savedata.h_pos.pz = 0;
			savedata.h_pos.theta = 0.0f;
			savedata.map_no = 4;
			*/
		}
		if (select == 1) {
			FILE *fp = fopen("./dat/save/save.da1", "rb");
			FILE *fp2 = fopen("./dat/save/save.da2", "rb");
			if (fp == NULL || fp2 == NULL)return TRUE;//読み込み失敗
			int cnt1 = fread(&key1, sizeof(key1), 1, fp);
			int cnt2 = fread(&key2, sizeof(key2), 1, fp2);
			fclose(fp); //ファイルを閉じる
			fclose(fp2);
			if (cnt1 < 1 || cnt2 < 1)return TRUE;//読み込みエラー
			//セーブデータチェック
			// 論理和 fは全て1111, 0は元数値と変わらず
			//論理積 0は全て0000,fは元数値と変わらず
			for (int i = 0; i < 4; i++) {
				//論理和チェック
				if (((unsigned int)key1.p_data[i].Attack | Decryptionkey) != 0xffffff ||
					((unsigned int)key1.p_data[i].Magic | Decryptionkey) != 0xffffff ||
					((unsigned int)key1.p_data[i].Agility | Decryptionkey) != 0xffffff ||
					((unsigned int)key1.p_data[i].Vitality | Decryptionkey) != 0xffffff ||
					((unsigned int)key1.p_data[i].Exp | Decryptionkey) != 0xffffff ||
					((unsigned int)key1.p_data[i].LV | Decryptionkey) != 0xffffff ||
					((unsigned int)key1.p_data[i].ParameterPoint | Decryptionkey) != 0xffffff ||
					((unsigned int)key1.p_data[i].HP | Decryptionkey) != 0xffffff ||
					((unsigned int)key1.p_data[i].MP | Decryptionkey) != 0xffffff ||
					((unsigned int)key1.p_data[i].FlameATT_LV | Decryptionkey) != 0xffffff ||
					((unsigned int)key1.p_data[i].Fpoint | Decryptionkey) != 0xffffff ||
					((unsigned int)key1.p_data[i].Healing_LV | Decryptionkey) != 0xffffff ||
					((unsigned int)key1.p_data[i].Hpoint | Decryptionkey) != 0xffffff ||
					((unsigned int)key1.p_data[i].Recover_LV | Decryptionkey) != 0xffffff ||
					((unsigned int)key1.p_data[i].Rpoint | Decryptionkey) != 0xffffff ||
					//論理積チェック
					((unsigned int)key2.p_data[i].Attack         & Decryptionkey) != 0x000000 ||
					((unsigned int)key2.p_data[i].Magic          & Decryptionkey) != 0x000000 ||
					((unsigned int)key2.p_data[i].Agility        & Decryptionkey) != 0x000000 ||
					((unsigned int)key2.p_data[i].Vitality       & Decryptionkey) != 0x000000 ||
					((unsigned int)key2.p_data[i].Exp            & Decryptionkey) != 0x000000 ||
					((unsigned int)key2.p_data[i].LV             & Decryptionkey) != 0x000000 ||
					((unsigned int)key2.p_data[i].ParameterPoint & Decryptionkey) != 0x000000 ||
					((unsigned int)key2.p_data[i].HP             & Decryptionkey) != 0x000000 ||
					((unsigned int)key2.p_data[i].MP             & Decryptionkey) != 0x000000 ||
					((unsigned int)key2.p_data[i].FlameATT_LV    & Decryptionkey) != 0x000000 ||
					((unsigned int)key2.p_data[i].Fpoint         & Decryptionkey) != 0x000000 ||
					((unsigned int)key2.p_data[i].Healing_LV     & Decryptionkey) != 0x000000 ||
					((unsigned int)key2.p_data[i].Hpoint         & Decryptionkey) != 0x000000 ||
					((unsigned int)key2.p_data[i].Recover_LV     & Decryptionkey) != 0x000000 ||
					((unsigned int)key2.p_data[i].Rpoint         & Decryptionkey) != 0x000000)return TRUE;
			}
			//論理和チェック
			if (((unsigned int)key1.h_pos.cx1 | Decryptionkey) != 0xffffff ||
				((unsigned int)key1.h_pos.cx2 | Decryptionkey) != 0xffffff ||
				((unsigned int)key1.h_pos.cy1 | Decryptionkey) != 0xffffff ||
				((unsigned int)key1.h_pos.cy2 | Decryptionkey) != 0xffffff ||
				((unsigned int)key1.h_pos.px | Decryptionkey) != 0xffffff ||
				((unsigned int)key1.h_pos.py | Decryptionkey) != 0xffffff ||
				((unsigned int)key1.h_pos.pz | Decryptionkey) != 0xffffff ||
				((unsigned int)key1.h_pos.theta | Decryptionkey) != 0xffffff ||
				((unsigned int)key1.map_no | Decryptionkey) != 0xffffff ||
				((unsigned int)key1.boss_kil[0] | Decryptionkey) != 0xffffff ||
				((unsigned int)key1.boss_kil[1] | Decryptionkey) != 0xffffff ||
				((unsigned int)key1.boss_kil[2] | Decryptionkey) != 0xffffff ||
				((unsigned int)key1.boss_kil[3] | Decryptionkey) != 0xffffff ||
				((unsigned int)key1.boss_kil[4] | Decryptionkey) != 0xffffff ||
				//論理積チェック
				((unsigned int)key2.h_pos.cx1 & Decryptionkey) != 0x000000 ||
				((unsigned int)key2.h_pos.cx2 & Decryptionkey) != 0x000000 ||
				((unsigned int)key2.h_pos.cy1 & Decryptionkey) != 0x000000 ||
				((unsigned int)key2.h_pos.cy2 & Decryptionkey) != 0x000000 ||
				((unsigned int)key2.h_pos.px  & Decryptionkey) != 0x000000 ||
				((unsigned int)key2.h_pos.py  & Decryptionkey) != 0x000000 ||
				((unsigned int)key2.h_pos.pz  & Decryptionkey) != 0x000000 ||
				((unsigned int)key2.h_pos.theta & Decryptionkey) != 0x000000 ||
				((unsigned int)key2.map_no    & Decryptionkey) != 0x000000 ||
				((unsigned int)key2.boss_kil[0] & Decryptionkey) != 0x000000 ||
				((unsigned int)key2.boss_kil[1] & Decryptionkey) != 0x000000 ||
				((unsigned int)key2.boss_kil[2] & Decryptionkey) != 0x000000 ||
				((unsigned int)key2.boss_kil[3] & Decryptionkey) != 0x000000 ||
				((unsigned int)key2.boss_kil[4] & Decryptionkey) != 0x000000)return TRUE;

			//セーブデータ復元
			for (int i = 0; i < 4; i++) {
				savedata.p_data[i].Attack = (key2.p_data[i].Attack + ((unsigned int)key1.p_data[i].Attack         & Decryptionkey));
				savedata.p_data[i].Magic = (key2.p_data[i].Magic + ((unsigned int)key1.p_data[i].Magic          & Decryptionkey));
				savedata.p_data[i].Agility = (key2.p_data[i].Agility + ((unsigned int)key1.p_data[i].Agility        & Decryptionkey));
				savedata.p_data[i].Vitality = (key2.p_data[i].Vitality + ((unsigned int)key1.p_data[i].Vitality       & Decryptionkey));
				savedata.p_data[i].Exp = (key2.p_data[i].Exp + ((unsigned int)key1.p_data[i].Exp            & Decryptionkey));
				savedata.p_data[i].LV = (key2.p_data[i].LV + ((unsigned int)key1.p_data[i].LV             & Decryptionkey));
				savedata.p_data[i].ParameterPoint = (key2.p_data[i].ParameterPoint + ((unsigned int)key1.p_data[i].ParameterPoint & Decryptionkey));
				savedata.p_data[i].HP = (key2.p_data[i].HP + ((unsigned int)key1.p_data[i].HP             & Decryptionkey));
				savedata.p_data[i].MP = (key2.p_data[i].MP + ((unsigned int)key1.p_data[i].MP             & Decryptionkey));
				savedata.p_data[i].FlameATT_LV = (key2.p_data[i].FlameATT_LV + ((unsigned int)key1.p_data[i].FlameATT_LV    & Decryptionkey));
				savedata.p_data[i].Fpoint = (key2.p_data[i].Fpoint + ((unsigned int)key1.p_data[i].Fpoint         & Decryptionkey));
				savedata.p_data[i].Healing_LV = (key2.p_data[i].Healing_LV + ((unsigned int)key1.p_data[i].Healing_LV     & Decryptionkey));
				savedata.p_data[i].Hpoint = (key2.p_data[i].Hpoint + ((unsigned int)key1.p_data[i].Hpoint         & Decryptionkey));
				savedata.p_data[i].Recover_LV = (key2.p_data[i].Recover_LV + ((unsigned int)key1.p_data[i].Recover_LV     & Decryptionkey));
				savedata.p_data[i].Rpoint = (key2.p_data[i].Rpoint + ((unsigned int)key1.p_data[i].Rpoint         & Decryptionkey));
			}
			savedata.h_pos.cx1 = (key2.h_pos.cx1 + ((unsigned int)key1.h_pos.cx1   & Decryptionkey));
			savedata.h_pos.cx2 = (key2.h_pos.cx2 + ((unsigned int)key1.h_pos.cx2   & Decryptionkey));
			savedata.h_pos.cy1 = (key2.h_pos.cy1 + ((unsigned int)key1.h_pos.cy1   & Decryptionkey));
			savedata.h_pos.cy2 = (key2.h_pos.cy2 + ((unsigned int)key1.h_pos.cy2   & Decryptionkey));
			savedata.h_pos.px = (key2.h_pos.px + ((unsigned int)key1.h_pos.px    & Decryptionkey));
			savedata.h_pos.py = (key2.h_pos.py + ((unsigned int)key1.h_pos.py    & Decryptionkey));
			savedata.h_pos.pz = (key2.h_pos.pz + ((unsigned int)key1.h_pos.pz    & Decryptionkey));
			savedata.h_pos.theta = (key2.h_pos.theta + ((unsigned int)key1.h_pos.theta & Decryptionkey));
			savedata.map_no = (key2.map_no + ((unsigned int)key1.map_no      & Decryptionkey));
			for (int i = 0; i < 5; i++)savedata.boss_kil[i] = (key2.boss_kil[i] + ((unsigned int)key1.boss_kil[i] & Decryptionkey));
		}
		MovieSoundManager::ObjDelete_title();
		return FALSE;
		break;
	}
	TCHAR *str = L"ＮＥＷＧＡＭＥ";
	text->UpDateText(&str, 320.0f, 250.0f, 30.0f, { r1, g1, 1.0f, 1.0f });
	TCHAR *str1 = L"ＣＯＮＴＩＮＵＥ";
	text->UpDateText(&str1, 320.0f, 290.0f, 30.0f, { r2, g2, 1.0f, 1.0f });
	return TRUE;
}

Parameter::P_Data *StateMenu::SetP_Data(int i){
	return &savedata.p_data[i];
}

Position::H_Pos *StateMenu::SetH_Pos(){
	return &savedata.h_pos;
}

int StateMenu::SetMap_No(){
	return savedata.map_no;
}

int StateMenu::Set_boss_kil(int i){
	return savedata.boss_kil[i];
}

bool StateMenu::Menudraw(Position::H_Pos *h_p, int map_no, int *boss_killed, Hero *hero, Directionkey direction) {
	dx->Bigin(STATEMENU_COM, nullptr);
	bool m_ref = FALSE;
	switch (menu_select) {
	case MAIN:
		m_ref = Main_Menu(h_p, map_no, boss_killed, hero, direction);
		dx->End(STATEMENU_COM);
		return m_ref;
	case ST:
		St_Menu(hero, direction);
		break;
	case RE:
		Re_Menu(hero, direction);
		break;
	case PP:
		Pp_Menu(hero, direction);
		break;
	}
	dx->End(STATEMENU_COM);
	return TRUE;
}

bool StateMenu::Main_Menu(Position::H_Pos *h_p, int map_no, int *boss_killed, Hero *hero, Directionkey direction) {

	float fsize = 25.0f;

	Menucreate();
	for (int i = 0; i < 4; i++) {
		TCHAR *str = L"囚人Ｎｏ";
		text->UpDateText(&str, 160.0f, 10.0f + 110.0f * i, fsize, { 1.0f, 1.0f, 1.0f, 1.0f });
		text->UpDateValue(i, 270.0f, 10.0f + 110.0f * i, fsize, 1, { 1.0f, 1.0f, 1.0f, 1.0f });
		TCHAR *str1 = L"ＨＰ";
		text->UpDateText(&str1, 160.0f, 40.0f + 110.0f * i, fsize, { 1.0f, 1.0f, 1.0f, 1.0f });
		text->UpDateValue(hero[i].s_HP(), 220.0f, 40.0f + 110.0f * i, fsize, 5, { 1.0f, 1.0f, 1.0f, 1.0f });
		text->UpDateValue(hero[i].s_MHP(), 370.0f, 40.0f + 110.0f * i, fsize, 5, { 1.0f, 1.0f, 1.0f, 1.0f });
		TCHAR *str2 = L"ＭＰ";
		text->UpDateText(&str2, 160.0f, 70.0f + 110.0f * i, fsize, { 1.0f, 1.0f, 1.0f, 1.0f });
		text->UpDateValue(hero[i].s_MP(), 220.0f, 70.0f + 110.0f * i, fsize, 5, { 1.0f, 1.0f, 1.0f, 1.0f });
		text->UpDateValue(hero[i].s_MMP(), 370.0f, 70.0f + 110.0f * i, fsize, 5, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	float r1, r2, r3, r4, g1, g2, g3, g4;
	r1 = r2 = r3 = r4 = g1 = g2 = g3 = g4 = 1.0f;

	switch (main_select) {
	case 0:
		r1 = 0.0f;
		g1 = 0.7f;
		break;
	case 1:
		r2 = 0.0f;
		g2 = 0.7f;
		break;
	case 2:
		r3 = 0.0f;
		g3 = 0.7f;
		break;
	case 3:
		r4 = 0.0f;
		g4 = 0.7f;
		break;
	}

	switch (direction) {
	case LEFT:
		break;
	case RIGHT:
		break;
	case UP:
		MovieSoundManager::Select_sound(FALSE);
		if (main_select == 0)break;
		main_select--;
		break;
	case DOWN:
		MovieSoundManager::Select_sound(FALSE);
		if (main_select == 3)break;
		main_select++;
		break;
	case ENTER:
		MovieSoundManager::Enter_sound(FALSE);
		switch (main_select) {
		case 0:
			menu_select = ST;
			break;
		case 1:
			menu_select = RE;
			break;
		case 2:
			menu_select = PP;
			break;
		case 3:
			//セーブデータ作成
			// 論理和 fは全て1111, 0は元数値と変わらず
			//論理積 0は全て0000,fは元数値と変わらず
			for (int i = 0; i < 4; i++) {
				//論理和データ
				key1.p_data[i].Attack = ((unsigned int)hero[i].s_ATT() | Encryptionkey);
				key1.p_data[i].Magic = ((unsigned int)hero[i].s_MAG() | Encryptionkey);
				key1.p_data[i].Agility = ((unsigned int)hero[i].s_AG() | Encryptionkey);
				key1.p_data[i].Vitality = ((unsigned int)hero[i].s_VIT() | Encryptionkey);
				key1.p_data[i].Exp = ((unsigned int)hero[i].s_Exp() | Encryptionkey);
				key1.p_data[i].LV = ((unsigned int)hero[i].s_LV() | Encryptionkey);
				key1.p_data[i].ParameterPoint = ((unsigned int)hero[i].s_ParameterPoint() | Encryptionkey);
				key1.p_data[i].HP = ((unsigned int)hero[i].s_HP() | Encryptionkey);
				key1.p_data[i].MP = ((unsigned int)hero[i].s_MP() | Encryptionkey);
				key1.p_data[i].FlameATT_LV = ((unsigned int)hero[i].GetFlameATT_LV() | Encryptionkey);
				key1.p_data[i].Fpoint = ((unsigned int)hero[i].s_Fp() | Encryptionkey);
				key1.p_data[i].Healing_LV = ((unsigned int)hero[i].GetHealing_LV() | Encryptionkey);
				key1.p_data[i].Hpoint = ((unsigned int)hero[i].s_Hp() | Encryptionkey);
				key1.p_data[i].Recover_LV = ((unsigned int)hero[i].GetRecover_LV() | Encryptionkey);
				key1.p_data[i].Rpoint = ((unsigned int)hero[i].s_Rp() | Encryptionkey);
				//論理積データ
				key2.p_data[i].Attack = ((unsigned int)hero[i].s_ATT() & Encryptionkey);
				key2.p_data[i].Magic = ((unsigned int)hero[i].s_MAG() & Encryptionkey);
				key2.p_data[i].Agility = ((unsigned int)hero[i].s_AG() & Encryptionkey);
				key2.p_data[i].Vitality = ((unsigned int)hero[i].s_VIT() & Encryptionkey);
				key2.p_data[i].Exp = ((unsigned int)hero[i].s_Exp() & Encryptionkey);
				key2.p_data[i].LV = ((unsigned int)hero[i].s_LV() & Encryptionkey);
				key2.p_data[i].ParameterPoint = ((unsigned int)hero[i].s_ParameterPoint() & Encryptionkey);
				key2.p_data[i].HP = ((unsigned int)hero[i].s_HP() & Encryptionkey);
				key2.p_data[i].MP = ((unsigned int)hero[i].s_MP() & Encryptionkey);
				key2.p_data[i].FlameATT_LV = ((unsigned int)hero[i].GetFlameATT_LV() & Encryptionkey);
				key2.p_data[i].Fpoint = ((unsigned int)hero[i].s_Fp() & Encryptionkey);
				key2.p_data[i].Healing_LV = ((unsigned int)hero[i].GetHealing_LV() & Encryptionkey);
				key2.p_data[i].Hpoint = ((unsigned int)hero[i].s_Hp() & Encryptionkey);
				key2.p_data[i].Recover_LV = ((unsigned int)hero[i].GetRecover_LV() & Encryptionkey);
				key2.p_data[i].Rpoint = ((unsigned int)hero[i].s_Rp() & Encryptionkey);
			}
			//論理和データ
			key1.h_pos.cx1 = (float)((unsigned int)(h_p->px * 100 + 50) | Encryptionkey);
			key1.h_pos.cx2 = (float)((unsigned int)(h_p->px * 100 + 50) | Encryptionkey);
			key1.h_pos.cy1 = (float)((unsigned int)(h_p->py * 100 + 50) | Encryptionkey);
			key1.h_pos.cy2 = (float)((unsigned int)(h_p->py * 100) | Encryptionkey);
			key1.h_pos.px = ((unsigned int)h_p->px | Encryptionkey);
			key1.h_pos.py = ((unsigned int)h_p->py | Encryptionkey);
			key1.h_pos.pz = ((unsigned int)h_p->pz | Encryptionkey);
			key1.h_pos.theta = (float)((unsigned int)0.0f | Encryptionkey);
			key1.map_no = ((unsigned int)map_no | Encryptionkey);
			for (int i = 0; i < 5; i++)key1.boss_kil[i] = ((unsigned int)boss_killed[i] | Encryptionkey);
			//論理積データ
			key2.h_pos.cx1 = (float)((unsigned int)(h_p->px * 100 + 50) & Encryptionkey);
			key2.h_pos.cx2 = (float)((unsigned int)(h_p->px * 100 + 50) & Encryptionkey);
			key2.h_pos.cy1 = (float)((unsigned int)(h_p->py * 100 + 50) & Encryptionkey);
			key2.h_pos.cy2 = (float)((unsigned int)(h_p->py * 100) & Encryptionkey);
			key2.h_pos.px = ((unsigned int)h_p->px & Encryptionkey);
			key2.h_pos.py = ((unsigned int)h_p->py & Encryptionkey);
			key2.h_pos.pz = ((unsigned int)h_p->pz & Encryptionkey);
			key2.h_pos.theta = (float)((unsigned int)0.0f & Encryptionkey);
			key2.map_no = ((unsigned int)map_no & Encryptionkey);
			for (int i = 0; i < 5; i++)key2.boss_kil[i] = ((unsigned int)boss_killed[i] & Encryptionkey);

			FILE *fp = fopen("./dat/save/save.da1", "wb");
			FILE *fp2 = fopen("./dat/save/save.da2", "wb");
			fwrite(&key1, sizeof(key1), 1, fp);
			fwrite(&key2, sizeof(key2), 1, fp2);
			fclose(fp); //ファイルを閉じる
			fclose(fp2);
			break;
		}
		break;
	case CANCEL:
		MovieSoundManager::Select_sound(FALSE);
		return FALSE;
	}

	fsize = 15.0f;
	TCHAR *str3 = L"ステータス";
	text->UpDateText(&str3, 10, 10, fsize, { r1, g1, 1.0f, 1.0f });
	TCHAR *str4 = L"回復";
	text->UpDateText(&str4, 10, 40, fsize, { r2, g2, 1.0f, 1.0f });
	TCHAR *str5 = L"ＰＰ振り分け";
	text->UpDateText(&str5, 10, 70, fsize, { r3, g3, 1.0f, 1.0f });
	TCHAR *str6 = L"セーブ";
	text->UpDateText(&str6, 10, 100, fsize, { r4, g4, 1.0f, 1.0f });

	return TRUE;
}

void StateMenu::St_Menu(Hero *hero, Directionkey direction) {

	float fsize = 25.0f;
	St_create();
	TCHAR *str = L"囚人Ｎｏ";
	text->UpDateText(&str, 120.0f, 10.0f, fsize, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateValue(st_select, 230.0f, 10.0f, fsize, 1, { 1.0f, 1.0f, 1.0f, 1.0f });
	TCHAR *str1 = L"ＬＶ";
	text->UpDateText(&str1, 120.0f, 40.0f, fsize, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateValue(hero[st_select].s_LV(), 300.0f, 40.0f, fsize, 3, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateValue(hero[st_select].s_LV() * 10 - hero[st_select].s_Exp(), 180.0f, 70.0f, fsize, 4, { 1.0f, 1.0f, 1.0f, 1.0f });
	TCHAR *str2 = L"/";
	text->UpDateText(&str2, 280.0f, 70.0f, fsize, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateValue(hero[st_select].s_LV() * 10, 305.0f, 70.0f, fsize, 4, { 1.0f, 1.0f, 1.0f, 1.0f });
	TCHAR *str3 = L"力";
	text->UpDateText(&str3, 120.0f, 100.0f, fsize, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateValue(hero[st_select].s_ATT(), 200.0f, 100.0f, fsize, 3, { 1.0f, 1.0f, 1.0f, 1.0f });
	TCHAR *str4 = L"魔力";
	text->UpDateText(&str4, 120.0f, 130.0f, fsize, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateValue(hero[st_select].s_MAG(), 200.0f, 130.0f, fsize, 3, { 1.0f, 1.0f, 1.0f, 1.0f });
	TCHAR *str5 = L"素早さ";
	text->UpDateText(&str5, 120.0f, 160.0f, fsize, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateValue(hero[st_select].s_AG(), 200.0f, 160.0f, fsize, 3, { 1.0f, 1.0f, 1.0f, 1.0f });
	TCHAR *str6 = L"体力";
	text->UpDateText(&str6, 120.0f, 190.0f, fsize, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateValue(hero[st_select].s_VIT(), 200.0f, 190.0f, fsize, 3, { 1.0f, 1.0f, 1.0f, 1.0f });

	if (hero[st_select].GetFlameATT_LV() != 0) {
		TCHAR *str7 = L"フレイムＬＶ";
		text->UpDateText(&str7, 120.0f, 220.0f, fsize, { 1.0f, 1.0f, 1.0f, 1.0f });
		text->UpDateValue(hero[st_select].GetFlameATT_LV(), 300.0f, 220.0f, fsize, 3, { 1.0f, 1.0f, 1.0f, 1.0f });
		text->UpDateValue(hero[st_select].GetFlameATT_LV() * 10 - hero[st_select].s_Fp(), 180.0f, 250.0f, fsize, 4, { 1.0f, 1.0f, 1.0f, 1.0f });
		TCHAR *str8 = L"/";
		text->UpDateText(&str8, 280.0f, 250.0f, fsize, { 1.0f, 1.0f, 1.0f, 1.0f });
		text->UpDateValue(hero[st_select].GetFlameATT_LV() * 10, 305.0f, 250.0f, fsize, 4, { 1.0f, 1.0f, 1.0f, 1.0f });
	}
	if (hero[st_select].GetHealing_LV() != 0) {
		TCHAR *str9 = L"ヒーリングＬＶ";
		text->UpDateText(&str9, 120.0f, 280.0f, fsize, { 1.0f, 1.0f, 1.0f, 1.0f });
		text->UpDateValue(hero[st_select].GetHealing_LV(), 300.0f, 280.0f, fsize, 3, { 1.0f, 1.0f, 1.0f, 1.0f });
		text->UpDateValue(hero[st_select].GetHealing_LV() * 10 - hero[st_select].s_Hp(), 180.0f, 310.0f, fsize, 4, { 1.0f, 1.0f, 1.0f, 1.0f });
		TCHAR *str10 = L"/";
		text->UpDateText(&str10, 280.0f, 310.0f, fsize, { 1.0f, 1.0f, 1.0f, 1.0f });
		text->UpDateValue(hero[st_select].GetHealing_LV() * 10, 305.0f, 310.0f, fsize, 4, { 1.0f, 1.0f, 1.0f, 1.0f });
	}
	if (hero[st_select].GetRecover_LV() != 0) {
		TCHAR *str11 = L"リカバリＬＶ";
		text->UpDateText(&str11, 120.0f, 340.0f, fsize, { 1.0f, 1.0f, 1.0f, 1.0f });
		text->UpDateValue(hero[st_select].GetRecover_LV(), 300.0f, 340.0f, fsize, 3, { 1.0f, 1.0f, 1.0f, 1.0f });
		text->UpDateValue(hero[st_select].GetRecover_LV() * 10 - hero[st_select].s_Rp(), 180.0f, 370.0f, fsize, 4, { 1.0f, 1.0f, 1.0f, 1.0f });
		TCHAR *str12 = L"/";
		text->UpDateText(&str12, 280.0f, 370.0f, fsize, { 1.0f, 1.0f, 1.0f, 1.0f });
		text->UpDateValue(hero[st_select].GetRecover_LV() * 10, 305.0f, 370.0f, fsize, 4, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	switch (direction) {
	case LEFT:
		MovieSoundManager::Select_sound(FALSE);
		if (st_select == 0)break;
		st_select--;
		break;
	case RIGHT:
		MovieSoundManager::Select_sound(FALSE);
		if (st_select == 3)break;
		st_select++;
		break;
	case UP:
		break;
	case DOWN:
		break;
	case ENTER:
		break;
	case CANCEL:
		MovieSoundManager::Select_sound(FALSE);
		menu_select = MAIN;
		break;
	}
}

void StateMenu::Re_Menu(Hero *hero, Directionkey direction) {

	float nr[4], ng[4], r[4], g[4], mr[2], mg[2];
	float fsize0 = 15.0f;
	float fsize = 25.0f;
	St_create();
	Re_create();
	int cnt;
	nr[0] = nr[1] = nr[2] = nr[3] = ng[0] = ng[1] = ng[2] = ng[3] = 1.0f;
	r[0] = r[1] = r[2] = r[3] = g[0] = g[1] = g[2] = g[3] = 1.0f;
	mr[0] = mr[1] = mg[0] = mg[1] = 1.0f;

	if (re_select2 == 4) {
		for (int i = 0; i < 4; i++) {
			r[i] = 0.0f;
			g[i] = 0.7f;
		}
	}
	if (re_select2 != -1 && re_select2 != 4) {
		r[re_select2] = 0.0f;
		g[re_select2] = 0.7f;
	}

	nr[re_select] = 0.0f;
	ng[re_select] = 0.7f;

	if (re_select1 != -1) {

		if (re_select1 == 0) {
			if (hero[re_select].GetRecover_LV() >= 1)re_select1 = 2;
			if (hero[re_select].GetHealing_LV() >= 1)re_select1 = 1;
		}
		switch (re_select1) {
		case 1:
			if (hero[re_select].GetHealing_LV() >= 1) {
				mr[0] = 0.0f;
				mg[0] = 0.7f;
			}
			break;
		case 2:
			if (hero[re_select].GetRecover_LV() >= 1) {
				mr[1] = 0.0f;
				mg[1] = 0.7f;
			}
			break;
		}
	}

	switch (direction) {
	case LEFT:
		MovieSoundManager::Select_sound(FALSE);
		if (re_select1 == 1 && re_select2 != -1) {
			if (re_select2 == 4) { re_select2 = 0; break; }
			if (re_select2 != 4) { re_select2 = 4; break; }
		}
		break;
	case RIGHT:
		MovieSoundManager::Select_sound(FALSE);
		if (re_select1 == 1 && re_select2 != -1) {
			if (re_select2 == 4) { re_select2 = 0; break; }
			if (re_select2 != 4) { re_select2 = 4; break; }
		}
		break;
	case UP:
		MovieSoundManager::Select_sound(FALSE);
		if (re_select2 != -1) {
			if (re_select2 == 4) {
				re_select2 = 3; break;
			}
			if (re_select2 == 0)break;
			re_select2--;
			break;
		}
		if (re_select1 != -1) {
			if (hero[re_select].GetHealing_LV() >= 1) {
				re_select1 = 1;
				break;
			}
			if (hero[re_select].GetRecover_LV() >= 1) {
				re_select1 = 2;
				break;
			}
			break;
		}
		if (re_select == 0)break;
		re_select--;
		break;
	case DOWN:
		MovieSoundManager::Select_sound(FALSE);
		if (re_select2 != -1) {
			if (re_select2 == 4) {
				re_select2 = 0; break;
			}
			if (re_select2 == 3)break;
			re_select2++;
			break;
		}
		if (re_select1 != -1) {
			if (hero[re_select].GetRecover_LV() >= 1) {
				re_select1 = 2;
				break;
			}
			if (hero[re_select].GetHealing_LV() >= 1) {
				re_select1 = 1;
				break;
			}
			break;
		}
		if (re_select == 3)break;
		re_select++;
		break;
	case ENTER:
		MovieSoundManager::Enter_sound(FALSE);
		if (hero[re_select].Dieflg() == TRUE)break;//戦闘不能中選択不可
		if (re_select1 == -1) {
			re_select1 = 0;
			break;
		}
		if (re_select1 >= 1 && re_select2 == -1) {
			re_select2 = 0;
			break;
		}

		//回復処理
		int rcv;
		cnt = 0;
		MagicSelect mag;
		if (re_select1 == 1)mag = HEAL;
		if (re_select1 == 2)mag = RECOV;
		if (hero[re_select].DownMP(mag) == FALSE)break;
		if (re_select2 == 4) {
			for (int i = 0; i < 4; i++) {
				if (hero[i].Dieflg() == FALSE)cnt++;
			}
			for (int i = 0; i < 4; i++) {
				rcv = hero[re_select].GetMagic(HEAL, cnt);
				if (hero[i].Dieflg() == TRUE)continue;
				hero[i].UpHP(rcv);
			}
		}
		else {
			if (re_select1 == 1) {
				rcv = hero[re_select].GetMagic(HEAL, 1);
				if (hero[re_select2].Dieflg() == TRUE)break;
				hero[re_select2].UpHP(rcv);
			}
			if (re_select1 == 2) {
				rcv = hero[re_select].GetMagic(RECOV, 1);
				if (hero[re_select2].Dieflg() == TRUE)hero[re_select2].Dieflg(FALSE);
				hero[re_select2].UpHP(rcv);
			}
		}
		break;
	case CANCEL:
		MovieSoundManager::Select_sound(FALSE);
		if (re_select2 != -1) {
			re_select2 = -1;
			break;
		}
		if (re_select1 != -1) {
			re_select1 = -1;
			break;
		}
		menu_select = MAIN;
		break;
	}

	for (int i = 0; i < 4; i++) {
		TCHAR *str = L"囚人Ｎｏ";
		text->UpDateText(&str, 10.0f, 10.0f + 20.0f * i, fsize0, { nr[i], ng[i], 1.0f, 1.0f });
		text->UpDateValue(i, 75.0f, 10.0f + 20.0f * i, fsize0, 1, { nr[i], ng[i], 1.0f, 1.0f });
		TCHAR *str1 = L"ＨＰ";
		text->UpDateText(&str1, 160.0f, 40.0f + 110.0f * i, fsize, { r[i], g[i], 1.0f, 1.0f });
		text->UpDateValue(hero[i].s_HP(), 220.0f, 40.0f + 110.0f * i, fsize, 5, { r[i], g[i], 1.0f, 1.0f });
		text->UpDateValue(hero[i].s_MHP(), 370.0f, 40.0f + 110.0f * i, fsize, 5, { r[i], g[i], 1.0f, 1.0f });
		TCHAR *str2 = L"ＭＰ";
		text->UpDateText(&str2, 160.0f, 70.0f + 110.0f * i, fsize, { 1.0f, 1.0f, 1.0f, 1.0f });
		text->UpDateValue(hero[i].s_MP(), 220.0f, 70.0f + 110.0f * i, fsize, 5, { 1.0f, 1.0f, 1.0f, 1.0f });
		text->UpDateValue(hero[i].s_MMP(), 370.0f, 70.0f + 110.0f * i, fsize, 5, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	if (re_select1 != -1) {
		Re_create1();
		if (hero[re_select].GetHealing_LV() >= 1) {
			TCHAR *str3 = L"ヒーリング";
			text->UpDateText(&str3, 10.0f, 160.0f, fsize0, { mr[0], mg[0], 1.0f, 1.0f });
		}
		if (hero[re_select].GetRecover_LV() >= 1) {
			TCHAR *str4 = L"リカバリ";
			text->UpDateText(&str4, 10.0f, 190.0f, fsize0, { mr[1], mg[1], 1.0f, 1.0f });
		}
	}
}

void StateMenu::Pp_Menu(Hero *hero, Directionkey direction) {

	float nr[4], ng[4], r[4], g[4];
	float fsize0 = 15.0f;
	float fsize = 25.0f;
	nr[0] = nr[1] = nr[2] = nr[3] = ng[0] = ng[1] = ng[2] = ng[3] = 1.0f;
	r[0] = r[1] = r[2] = r[3] = g[0] = g[1] = g[2] = g[3] = 1.0f;
	St_create();
	Re_create();

	nr[pp_select] = 0.0f;
	ng[pp_select] = 0.7f;

	switch (direction) {
	case LEFT:
		break;
	case RIGHT:
		break;
	case UP:
		MovieSoundManager::Select_sound(FALSE);
		if (pp_select1 != -1) {
			if (pp_select1 == 0)break;
			pp_select1--;
			break;
		}
		if (pp_select == 0)break;
		pp_select--;
		break;
	case DOWN:
		MovieSoundManager::Select_sound(FALSE);
		if (pp_select1 != -1) {
			if (pp_select1 == 3)break;
			pp_select1++;
			break;
		}
		if (pp_select == 3)break;
		pp_select++;
		break;
	case ENTER:
		MovieSoundManager::Enter_sound(FALSE);
		if (pp_select1 != -1) {
			if (hero[pp_select].s_ParameterPoint(-1) == FALSE)break;//PP足りない処理
			switch (pp_select1) {
			case 0:
				hero[pp_select].s_ATT(TRUE);
				break;
			case 1:
				hero[pp_select].s_MAG(TRUE);
				break;
			case 2:
				hero[pp_select].s_AG(TRUE);
				break;
			case 3:
				hero[pp_select].s_VIT(TRUE);
				break;
			}
			break;
		}
		pp_select1 = 0;
		break;
	case CANCEL:
		MovieSoundManager::Select_sound(FALSE);
		if (pp_select1 != -1) {
			pp_select1 = -1;
			break;
		}
		menu_select = MAIN;
		break;
	}

	for (int i = 0; i < 4; i++) {
		TCHAR *str = L"囚人Ｎｏ";
		text->UpDateText(&str, 10.0f, 10.0f + 20.0f * i, fsize0, { nr[i], ng[i], 1.0f, 1.0f });
		text->UpDateValue(i, 75.0f, 10.0f + 20.0f * i, fsize0, 1, { nr[i], ng[i], 1.0f, 1.0f });
	}

	if (pp_select1 != -1) {
		r[pp_select1] = 0.0f;
		g[pp_select1] = 0.7f;
		TCHAR *str1 = L"囚人Ｎｏ";
		text->UpDateText(&str1, 120.0f, 10.0f, fsize, { 1.0f, 1.0f, 1.0f, 1.0f });
		text->UpDateValue(pp_select, 230.0f, 10.0f, fsize, 1, { 1.0f, 1.0f, 1.0f, 1.0f });
		TCHAR *str2 = L"ＬＶ";
		text->UpDateText(&str2, 120.0f, 40.0f, fsize, { 1.0f, 1.0f, 1.0f, 1.0f });
		text->UpDateValue(hero[pp_select].s_LV(), 300.0f, 40.0f, fsize, 3, { 1.0f, 1.0f, 1.0f, 1.0f });
		TCHAR *str3 = L"力";
		text->UpDateText(&str3, 120.0f, 100.0f, fsize, { r[0], g[0], 1.0f, 1.0f });
		text->UpDateValue(hero[pp_select].s_ATT(), 200.0f, 100.0f, fsize, 3, { r[0], g[0], 1.0f, 1.0f });
		TCHAR *str4 = L"魔力";
		text->UpDateText(&str4, 120.0f, 130.0f, fsize, { r[1], g[1], 1.0f, 1.0f });
		text->UpDateValue(hero[pp_select].s_MAG(), 200.0f, 130.0f, fsize, 3, { r[1], g[1], 1.0f, 1.0f });
		TCHAR *str5 = L"素早さ";
		text->UpDateText(&str5, 120.0f, 160.0f, fsize, { r[2], g[2], 1.0f, 1.0f });
		text->UpDateValue(hero[pp_select].s_AG(), 200.0f, 160.0f, fsize, 3, { r[2], g[2], 1.0f, 1.0f });
		TCHAR *str6 = L"体力";
		text->UpDateText(&str6, 120.0f, 190.0f, fsize, { r[3], g[3], 1.0f, 1.0f });
		text->UpDateValue(hero[pp_select].s_VIT(), 200.0f, 190.0f, fsize, 3, { r[3], g[3], 1.0f, 1.0f });
		TCHAR *str7 = L"ＰＰ";
		text->UpDateText(&str7, 120.0f, 250.0f, fsize, { 1.0f, 1.0f, 1.0f, 1.0f });
		text->UpDateValue(hero[pp_select].s_ParameterPoint(), 200.0f, 250.0f, fsize, 3, { 1.0f, 1.0f, 1.0f, 1.0f });
	}
}