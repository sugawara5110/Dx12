//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Parameterクラス                                 **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Parameter_Header
#define Class_Parameter_Header

#include "DxText.h"
#include "MovieSoundManager.h"
#define MAX_HP (p_data.Vitality * p_data.LV * 2 + 30)
#define MAX_MP (p_data.Magic * p_data.LV + 10)

class Parameter{

public:
	typedef struct{
		//基本パラメーター
		int Attack;
		int Magic;//× LV + 10がMaxMP
		int Agility;
		int Vitality;//× LV × 2 + 30がMaxHP
		int Exp;//敵は獲得数,プレイヤー側はレベルアップまでの数値
		int LV;
		int ParameterPoint;
		int HP;
		int MP;

		//マジックレベル
		int FlameATT_LV, Fpoint;//次のレベルまでのポイント
		int Healing_LV, Hpoint;
		int Recover_LV, Rpoint;
	}P_Data;

protected:
	Dx12Process *dx;
	P_Data p_data;
	DxText *text;
	T_float tfloat;

	Parameter();
	
public:
	int GetAttack();
	int GetMagic(MagicSelect mag, int p);
	void UpHP(int up);
	void DownHP(int down);
	bool DownMP(MagicSelect mag);
	float GetAgility();
	int GetFlameATT_LV();
	int GetHealing_LV();
	int GetRecover_LV();
	bool Dieflg();
	bool Dieflg(bool f);
	int s_ATT();
	void s_ATT(bool f);
	int s_MAG();
	void s_MAG(bool f);
	int s_AG();
	void s_AG(bool f);
	int s_VIT();
	void s_VIT(bool f);
	int s_MHP();
	int s_HP();
	int s_MMP();
	int s_MP();
	int s_Exp();
	bool s_Exp(int down);
	int s_LV();
	int s_ParameterPoint();
	bool s_ParameterPoint(int updown);
	int s_Fp();
	bool s_Fp(int down);
	int s_Hp();
	bool s_Hp(int down);
	int s_Rp();
	bool s_Rp(int down);
	void all_recover();
	virtual ~Parameter();
};

#endif