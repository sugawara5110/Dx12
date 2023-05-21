//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@�@      Ending�N���X                                    **//
//**                                     StaffRoll�֐�                                   **//
//*****************************************************************************************//

#include "Ending.h"

Ending::Ending() {

	text = DxText::GetInstance();
	str_y = 600;
	MovieSoundManager::ObjCreate_ending();
	MovieSoundManager::Ending_sound(FALSE);
}

void Ending::StaffRoll() {

	MovieSoundManager::Ending_sound(TRUE);
	text->UpDateText(L"�^�C�g���A�G���f�B���O�a�f�l�A���ʉ���", 110.0f, str_y, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateText(L"�������l", 340.0f, str_y + 30.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateText(L"�퓬�A�}�b�v�a�f�l��", 250.0f, str_y + 200.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateText(L"�v���������������r�����������l", 170.0f, str_y + 230.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateText(L"�G�摜��", 320.0f, str_y + 400.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateText(L"�g�h�|�s�h�l�d�l", 290.0f, str_y + 430.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateText(L"�e���������|���������l", 250.0f, str_y + 460.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateText(L"�G�t�F�N�g�摜��", 260.0f, str_y + 600.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateText(L"�҂ۂ�l", 340.0f, str_y + 630.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateText(L"���C���v���O���}�[", 260.0f, str_y + 1200.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateText(L"������", 350.0f, str_y + 1230.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateText(L"�G�O�[�N�e�B�u�E�v���f���[�T�[", 160.0f, str_y + 1600.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateText(L"������", 350.0f, str_y + 1630.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateText(L"�s�g�d�d�m�c", 310.0f, str_y + 2500.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });

	if (str_y > -2200)str_y -= 0.4f;
}

Ending::~Ending(){

	MovieSoundManager::ObjDelete_ending();
}

