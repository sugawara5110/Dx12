//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@�@      Ending�N���X                                    **//
//**                                     StaffRoll�֐�                                   **//
//*****************************************************************************************//

#include "Ending.h"

Ending::Ending(){

	dx = Dx12Process::GetInstance();
	text = DxText::GetInstance();
	str_y = 600;
	MovieSoundManager::ObjCreate_ending();
	MovieSoundManager::Ending_sound(FALSE);
}

void Ending::StaffRoll() {

	MovieSoundManager::Ending_sound(TRUE);
	TCHAR *str = L"�^�C�g���A�G���f�B���O�a�f�l�A���ʉ���";
	text->UpDateText(&str, 110.0f, str_y, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	TCHAR *str2 = L"�������l";
	text->UpDateText(&str2, 340.0f, str_y + 30.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	TCHAR *str3 = L"�퓬�A�}�b�v�a�f�l��";
	text->UpDateText(&str3, 250.0f, str_y + 200.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	TCHAR *str4 = L"�v���������������r�����������l";
	text->UpDateText(&str4, 170.0f, str_y + 230.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	TCHAR *str5 = L"�G�摜��";
	text->UpDateText(&str5, 320.0f, str_y + 400.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	TCHAR *str6 = L"�g�h�|�s�h�l�d�l";
	text->UpDateText(&str6, 290.0f, str_y + 430.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	TCHAR *str7 = L"�e���������|���������l";
	text->UpDateText(&str7, 250.0f, str_y + 460.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	TCHAR *str8 = L"�G�t�F�N�g�摜��";
	text->UpDateText(&str8, 260.0f, str_y + 600.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	TCHAR *str9 = L"�҂ۂ�l";
	text->UpDateText(&str9, 340.0f, str_y + 630.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	TCHAR *str10 = L"���C���v���O���}�[";
	text->UpDateText(&str10, 260.0f, str_y + 1200.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	TCHAR *str11 = L"������";
	text->UpDateText(&str11, 350.0f, str_y + 1230.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	TCHAR *str12 = L"�G�O�[�N�e�B�u�E�v���f���[�T�[";
	text->UpDateText(&str12, 160.0f, str_y + 1600.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	TCHAR *str13 = L"������";
	text->UpDateText(&str13, 350.0f, str_y + 1630.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	TCHAR *str14 = L"�s�g�d�d�m�c";
	text->UpDateText(&str14, 310.0f, str_y + 2500.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });

	if (str_y > -2200)str_y -= 0.4f;
}

Ending::~Ending(){

	MovieSoundManager::ObjDelete_ending();
}

