//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Parameterクラス                                 **//
//**                                      GetAttack関数                                  **//
//*****************************************************************************************//

#include "Parameter.h"

Parameter::Parameter(){

	dx = Dx12Process::GetInstance();
	text = DxText::GetInstance();
}

int Parameter::GetAttack(){

	return p_data.Attack * p_data.Agility;
}

int Parameter::GetMagic(MagicSelect mag, int p){

	switch (mag){
	case FLAME:
		return p_data.Magic * p_data.FlameATT_LV / p;
	case HEAL:
		return p_data.Magic * p_data.Healing_LV / p;
	case RECOV:
		return p_data.Magic * p_data.Recover_LV / 2;
	}
	return 0;
}

void Parameter::UpHP(int up){

	if (p_data.HP <= 0)return;
	p_data.HP += up;
	if (p_data.HP > MAX_HP)p_data.HP = MAX_HP;
}

void Parameter::DownHP(int down){//downにGetAttackを突っ込む

	p_data.HP -= down;
}

bool Parameter::DownMP(MagicSelect mag){

	switch (mag){
	case FLAME:
		if (p_data.FlameATT_LV > p_data.MP)return FALSE;
		p_data.MP -= p_data.FlameATT_LV;
		break;
	case HEAL:
		if (p_data.Healing_LV > p_data.MP)return FALSE;
		p_data.MP -= p_data.Healing_LV;
		break;
	case RECOV:
		if (p_data.Recover_LV > p_data.MP)return FALSE;
		p_data.MP -= p_data.Recover_LV;
		break;
	}
	return TRUE;
}

float Parameter::GetAgility(){

	return (p_data.Agility + 50.0f) / 4.0f;
}

int Parameter::GetFlameATT_LV(){

	return p_data.FlameATT_LV;
}

int Parameter::GetHealing_LV(){

	return p_data.Healing_LV;
}

int Parameter::GetRecover_LV(){

	return p_data.Recover_LV;
}

bool Parameter::Dieflg(){

	if (p_data.HP > 0){ return FALSE; }
	else{
		p_data.HP = 0;
		return TRUE;
	}
}

bool Parameter::Dieflg(bool f){

	if (f == TRUE){
		p_data.HP = 0;
		return TRUE;
	}
	if (f == FALSE){
		p_data.HP = 1;
		return FALSE;
	}
	return FALSE;
}

int Parameter::s_ATT(){
	return p_data.Attack;
}

void  Parameter::s_ATT(bool f){
	p_data.Attack++;
}

int Parameter::s_MAG(){
	return p_data.Magic;
}

void Parameter::s_MAG(bool f){
	p_data.Magic++;
	if (p_data.Magic >= 6){
		if (p_data.FlameATT_LV == 0)p_data.FlameATT_LV = 1;
		if (p_data.Healing_LV == 0)p_data.Healing_LV = 1;
		if (p_data.Recover_LV == 0)p_data.Recover_LV = 1;
	}
}

int Parameter::s_AG(){
	return p_data.Agility;
}

void Parameter::s_AG(bool f){
	p_data.Agility++;
}

int Parameter::s_VIT(){
	return p_data.Vitality;
}

void Parameter::s_VIT(bool f){
	p_data.Vitality++;
}

int Parameter::s_MHP(){
	return MAX_HP;
}

int Parameter::s_HP(){
	return p_data.HP;
}

int Parameter::s_MMP(){
	return MAX_MP;
}

int Parameter::s_MP(){
	return p_data.MP;
}

int Parameter::s_Exp(){
	return p_data.Exp;
}

bool Parameter::s_Exp(int down){

	if (p_data.Exp - down <= 0){
		int getp = down;
		while (p_data.Exp - getp <= 0){
			getp -= p_data.Exp;
			p_data.LV++;
			p_data.ParameterPoint += 4;
			p_data.Exp = p_data.LV * 10;
		}
		p_data.Exp -= getp;
		return TRUE;
	}
	p_data.Exp -= down;
	return FALSE;
}

int Parameter::s_LV(){
	return p_data.LV;
}

int Parameter::s_ParameterPoint(){
	return p_data.ParameterPoint;
}

bool Parameter::s_ParameterPoint(int updown){

	if (updown < 0 && p_data.ParameterPoint < -updown)return FALSE;
	p_data.ParameterPoint += updown;
	return TRUE;
}

int Parameter::s_Fp(){
	return p_data.Fpoint;
}

bool Parameter::s_Fp(int down){
	
	if (p_data.FlameATT_LV == 0)return FALSE;//LV=0の場合ポイント加算無し
	if ((p_data.Fpoint -= down) <= 0){
		p_data.FlameATT_LV++;
		p_data.Fpoint = p_data.FlameATT_LV * 10;
		return TRUE;
	}
	return FALSE;
}

int Parameter::s_Hp(){
	return p_data.Hpoint;
}

bool Parameter::s_Hp(int down){

	if (p_data.Healing_LV == 0)return FALSE;
	if ((p_data.Hpoint -= down) <= 0){
		p_data.Healing_LV++;
		p_data.Hpoint = p_data.Healing_LV * 10;
		return TRUE;
	}
	return FALSE;
}

int Parameter::s_Rp(){
	return p_data.Rpoint;
}

bool Parameter::s_Rp(int down){

	if (p_data.Recover_LV == 0)return FALSE;
	if ((p_data.Rpoint -= down) <= 0){
		p_data.Recover_LV++;
		p_data.Rpoint = p_data.Recover_LV * 10;
		return TRUE;
	}
	return FALSE;
}

void Parameter::all_recover(){

	p_data.HP = s_MHP();
	p_data.MP = s_MMP();
}

Parameter::~Parameter(){}