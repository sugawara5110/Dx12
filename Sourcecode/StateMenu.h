//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         StateMenuクラス                                 **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_StateMenu_Header
#define Class_StateMenu_Header

#include "MovieSoundManager.h"
#include "Position.h"
#include "DxText.h"
#define s_MAX_HP (savedata.p_data[i].Vitality * savedata.p_data[i].LV * 2 + 30)
#define s_MAX_MP (savedata.p_data[i].Magic * savedata.p_data[i].LV + 10)

class StateMenu{

private:
	Dx12Process *dx;
	PolygonData2D state, s_state, r_state, r_state1;
	DxText *text;
	MapMenuSelect menu_select;
	int comNo;
	int main_select;
	int st_select;
	int re_select;
	int re_select1;
	int re_select2;
	int pp_select;
	int pp_select1;

	typedef struct{
		Parameter::P_Data p_data[4];
		Position::H_Pos h_pos;
		int map_no;
		int boss_kil[5];//ボス撃破履歴
	}SaveData;
	SaveData savedata, key1, key2;
	unsigned int Encryptionkey;
	unsigned int Decryptionkey;

	void Menucreate();
	void St_create();
	void Re_create();
	void Re_create1();
	bool Main_Menu(Position::H_Pos *h_p, int map_no, int *boss_killed, Hero *hero, Directionkey direction);
	void St_Menu(Hero *hero, Directionkey direction);
	void Re_Menu(Hero *hero, Directionkey direction);
	void Pp_Menu(Hero *hero, Directionkey direction);

public:
	StateMenu();
	bool TitleMenu(Directionkey direction);
	Parameter::P_Data *SetP_Data(int i);
	Position::H_Pos *SetH_Pos();
	int Set_boss_kil(int i);
	int SetMap_No();
	void SetCommandList(int com_no);
	bool Menudraw(Position::H_Pos *h_p, int map_no, int *boss_killed, Hero *hero, Directionkey direction);
	~StateMenu();
};

#endif