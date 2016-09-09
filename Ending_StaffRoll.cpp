//*****************************************************************************************//
//**                                                                                     **//
//**                   @@@@@@      EndingƒNƒ‰ƒX                                    **//
//**                                     StaffRollŠÖ”                                   **//
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
	TCHAR *str = L"ƒ^ƒCƒgƒ‹AƒGƒ“ƒfƒBƒ“ƒO‚a‚f‚lAŒø‰Ê‰¹’ñ‹Ÿ";
	text->UpDateText(&str, 110.0f, str_y, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	TCHAR *str2 = L"–‚‰¤°—l";
	text->UpDateText(&str2, 340.0f, str_y + 30.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	TCHAR *str3 = L"í“¬Aƒ}ƒbƒv‚a‚f‚l’ñ‹Ÿ";
	text->UpDateText(&str3, 250.0f, str_y + 200.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	TCHAR *str4 = L"‚v‚‰‚Ž‚‡‚Œ‚…‚“‚“‚r‚…‚’‚‚‚ˆ—l";
	text->UpDateText(&str4, 170.0f, str_y + 230.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	TCHAR *str5 = L"“G‰æ‘œ’ñ‹Ÿ";
	text->UpDateText(&str5, 320.0f, str_y + 400.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	TCHAR *str6 = L"‚g‚h|‚s‚h‚l‚d—l";
	text->UpDateText(&str6, 290.0f, str_y + 430.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	TCHAR *str7 = L"‚e‚‚Œ‚ƒ‚…|‚š‚…‚’‚…—l";
	text->UpDateText(&str7, 250.0f, str_y + 460.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	TCHAR *str8 = L"ƒGƒtƒFƒNƒg‰æ‘œ’ñ‹Ÿ";
	text->UpDateText(&str8, 260.0f, str_y + 600.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	TCHAR *str9 = L"‚Ò‚Û‚â—l";
	text->UpDateText(&str9, 340.0f, str_y + 630.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	TCHAR *str10 = L"ƒƒCƒ“ƒvƒƒOƒ‰ƒ}[";
	text->UpDateText(&str10, 260.0f, str_y + 1200.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	TCHAR *str11 = L"›Œ´‘";
	text->UpDateText(&str11, 350.0f, str_y + 1230.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	TCHAR *str12 = L"ƒGƒOƒ[ƒNƒeƒBƒuEƒvƒƒfƒ…[ƒT[";
	text->UpDateText(&str12, 160.0f, str_y + 1600.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	TCHAR *str13 = L"›Œ´‘";
	text->UpDateText(&str13, 350.0f, str_y + 1630.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	TCHAR *str14 = L"‚s‚g‚d‚d‚m‚c";
	text->UpDateText(&str14, 310.0f, str_y + 2500.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });

	if (str_y > -2200)str_y -= 0.4f;
}

Ending::~Ending(){

	MovieSoundManager::ObjDelete_ending();
}

