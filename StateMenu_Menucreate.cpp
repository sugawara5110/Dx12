//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         StateMenuクラス                                 **//
//**                                     Menucreate関数                                  **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "Hero.h"
#include "StateMenu.h"

StateMenu::StateMenu() {

	dx = Dx12Process::GetInstance();
	text = DxText::GetInstance();
	MovieSoundManager::ObjCreate_title();
	MovieSoundManager::Title_sound(FALSE);
	dx->Bigin(STATEMENU_COM);
	state.SetCommandList(STATEMENU_COM);
	state.GetVBarray2D(1);
	state.CreateBox(0.0f, 0.0f, 0.0f, 0.1f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, TRUE, TRUE);
	s_state.SetCommandList(STATEMENU_COM);
	s_state.GetVBarray2D(1);
	s_state.CreateBox(0.0f, 0.0f, 0.0f, 0.1f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, TRUE, TRUE);
	r_state.SetCommandList(STATEMENU_COM);
	r_state.GetVBarray2D(1);
	r_state.CreateBox(0.0f, 0.0f, 0.0f, 0.1f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, TRUE, TRUE);
	r_state1.SetCommandList(STATEMENU_COM);
	r_state1.GetVBarray2D(1);
	r_state1.CreateBox(0.0f, 0.0f, 0.0f, 0.1f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, TRUE, TRUE);
	dx->End(STATEMENU_COM);
	dx->WaitFenceCurrent();
	menu_select = MAIN;
	main_select = 0;
	st_select = 0;
	re_select = 0;
	re_select1 = -1;
	re_select2 = -1;
	pp_select = 0;
	pp_select1 = -1;
	Encryptionkey = 0xf0f0f0;
	Decryptionkey = 0x0f0f0f;
}

void StateMenu::Menucreate() {
	state.InstancedSetConstBf(0.0f, 0.0f, 0.9f, 1.0f, 1.0f, 1.0f, 0.7f, 500.0f, 430.0f);
	state.Draw(5.0f, 5.0f, 0.8f, 0.6f, 0.6f, 0.6f, 0.7f, 490.0f, 420.0f);
}

void StateMenu::St_create() {
	s_state.InstancedSetConstBf(110.0f, 0.0f, 0.7f, 1.0f, 1.0f, 1.0f, 0.7f, 390.0f, 430.0f);
	s_state.Draw(115.0f, 5.0f, 0.6f, 0.6f, 0.6f, 0.6f, 0.7f, 380.0f, 420.0f);
}

void StateMenu::Re_create() {
	r_state.InstancedSetConstBf(0.0f, 0.0f, 0.7f, 1.0f, 1.0f, 1.0f, 0.7f, 110.0f, 430.0f);
	r_state.Draw(5.0f, 5.0f, 0.6f, 0.6f, 0.6f, 0.6f, 0.7f, 100.0f, 420.0f);
}

void StateMenu::Re_create1() {
	r_state1.InstancedSetConstBf(0.0f, 150.0f, 0.5f, 1.0f, 1.0f, 1.0f, 0.7f, 135.0f, 280.0f);
	r_state1.Draw(5.0f, 155.0f, 0.4f, 0.6f, 0.6f, 0.6f, 0.7f, 125.0f, 270.0f);
}

StateMenu::~StateMenu(){

	MovieSoundManager::ObjDelete_title();
}