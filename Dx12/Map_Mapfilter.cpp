//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Mapクラス                                       **//
//**                                    Mapfilter関数                                    **//
//*****************************************************************************************//

#include <string.h>
#include "Map.h"

void Map::Mapfilter_p(int k, int j, int i, int idx1, int idx2, int *cnt){
	//手前方向検査
	if (k < mxy.z - 1 && mxy.m[(k + 1) * mxy.y * mxy.x + j * mxy.x + i] != idx1 &&
		mxy.m[(k + 1) * mxy.y * mxy.x + j * mxy.x + i] != idx2)(*cnt)++;
	//奥方向検査
	if (k > 0 && mxy.m[(k - 1) * mxy.y * mxy.x + j * mxy.x + i] != idx1 &&
		mxy.m[(k - 1) * mxy.y * mxy.x + j * mxy.x + i] != idx2)(*cnt)++;
	//上方向検査
	if (j > 0 && mxy.m[k * mxy.y * mxy.x + (j - 1) * mxy.x + i] != idx1 &&
		mxy.m[k * mxy.y * mxy.x + (j - 1) * mxy.x + i] != idx2)(*cnt)++;
	//下方向検査
	if (j < mxy.y - 1 && mxy.m[k * mxy.y * mxy.x + (j + 1) * mxy.x + i] != idx1 &&
		mxy.m[k * mxy.y * mxy.x + (j + 1) * mxy.x + i] != idx2)(*cnt)++;
	//左方向検査
	if (i > 0 && mxy.m[k * mxy.y * mxy.x + j * mxy.x + (i - 1)] != idx1 &&
		mxy.m[k * mxy.y * mxy.x + j * mxy.x + (i - 1)] != idx2)(*cnt)++;
	//右方向検査
	if (i < mxy.x - 1 && mxy.m[k * mxy.y * mxy.x + j * mxy.x + (i + 1)] != idx1 &&
		mxy.m[k * mxy.y * mxy.x + j * mxy.x + (i + 1)] != idx2)(*cnt)++;
}

void Map::Mapfilter(Position::H_Pos *h_p){

	char *mapfilter;
	blockcountA = 0;
	blockcountB = 0;
	blockcountC = 0;
	blockcountD = 0;
	blockcountE = 0;
	squarecount = 0;
	r_point_count = 0;
	mo_count = 0;
	f_wall_count = 0;
	boss_count = 0;
	Elevator_count = 0;
	lightcount = 0;
	woodcount = 0;

	switch (map_no){
	case 0:
		mxy.z = 1; mxy.y = 35; mxy.x = 30;
		break;
	case 1:
		mxy.z = 1; mxy.y = 40; mxy.x = 40;
		break;
	case 2:
		mxy.z = 2; mxy.y = 40; mxy.x = 30;
		break;
	case 3:
		mxy.z = 2; mxy.y = 35; mxy.x = 30;
		break;
	case 4:
		mxy.z = 8; mxy.y = 30; mxy.x = 30;
		break;
	}

	mapfilter = (char*)malloc(sizeof(char) * mxy.z * mxy.y * mxy.x);
	// J:壁B74 K:壁C75 L:壁D76 M:壁E77 O:松明有り壁C79
	// 0:通路48  1:壁A49  2:回復ポイント50  3:ボス出現ポイント51  4:松明有り壁A52
	// 5:壁(板)53  6:出口1 54  7:入口55  8:出口2 56  I:炎壁73 W:木87
	// N:松明有り壁E78  -:透明壁45 A:エレベーター上65 B:エレベーター下66
	switch (map_no){
	case 0:
	{
		char mapfilter0[1][35][31] =
		{
			"000101000000000000000000000000",
			"000161000000000000000000000000",
			"000101000000000000000000000000",
			"000101000000000000000000000000",
			"000101000000000000000000000000",
			"000404111111111111111111110000",
			"004000411110000001111111010000",
			"010000011011011111111111010000",
			"001434111011011111111111011111",
			"000101110011011111111110000010",
			"111101110141011111111110111110",
			"110000000000000000111100100000",
			"111111111141111140411101100000",
			"000000100111111110011101100000",
			"000000100000010000011000100000",
			"000000111111000011111100100000",
			"000000110000001114111100100000",
			"000000111114100000000000100000",
			"000000100000000111110001100000",
			"000000400011111111110011000000",
			"000000100111111000000111111110",
			"000000100000000001111111110010",
			"000000110000111111111111110010",
			"000000011001111111111110000110",
			"000000001001111111111110111111",
			"000000004001111111111110100001",
			"000000001001111111111110111111",
			"000000001004141111111110100000",
			"000000001000000000000000100000",
			"000000001110011111111111100000",
			"000000000010021000000000000000",
			"000000000140041000000000000000",
			"000000000140041000000000000000",
			"000000000140041000000000000000",
			"000000000011110000000000000000"
		};
		for (int k = 0; k < mxy.z; k++){
			for (int j = 0; j < mxy.y; j++){
				for (int i = 0; i < mxy.x; i++){
					mapfilter[k * mxy.y * mxy.x + j * mxy.x + i] = mapfilter0[k][j][i];
				}
			}
		}
	}
	if (h_p == NULL && MPos == POS_ST){
		cax1 = 1150.0f;
		cax2 = 1150.0f;
		cay1 = 3350.0f;
		cay2 = 3280.0f;
		posx = 11;
		posy = 33;
		posz = 0;
		src_theta = 0.0f;
	}
	if (h_p == NULL && MPos == POS_EN1){
		cax1 = 450.0f;
		cax2 = 450.0f;
		cay1 = 250.0f;
		cay2 = 320.0f;
		posx = 4;
		posy = 2;
		posz = 0;
		src_theta = 180.0f;
	}
	break;

	case 1:
	{
		char mapfilter1[1][40][41] =
		{
			"0000000000000000000000000000000000000000",
			"00000000KOO00000000000000000000000000000",
			"00000000K0000000000000000000000000000000",
			"00000000K0K00000000000000000000000000000",
			"00000000K0K00000000000000000000000000000",

			"055JJJJJJ0JJJJJJJJJJJJJJ5550000000000000",
			"0555WWWWW6WWWWWW00000WWWWWW0000000000000",
			"0555W000W0W00000000000WWW550000000000000",
			"0555W000W3W0000000000000W550000000000000",
			"0555W0000000000000000000W550000000000000",
			"055WWWWWWW00000WWWW0000WW050000000000000",
			"055W000000000000WWWWWWWW5050000000000000",
			"055W0000000000000000000W5050000000000000",
			"055W00000000000WWWW000WW5055000000000000",
			"055W0000000WW0000000000W5555000000000000",
			"055W00000000000000000000W555000000000000",
			"055W000000000000WW00WW00W555555550000000",
			"055W00WWWWWWW000000WWW00W555555550000000",
			"055W0000000000000000000WWWWWWWW550000000",
			"055W00000000000000000000000000W550000000",
			"055W000000000000000000WWWWWW00W555000000",
			"055W00000WW00W00WW0WW000W50W00W555000000",
			"055W00000000WWW000000WWW550W00W555000000",
			"055W0000000000000000000W550W00W555000000",
			"055W000000000000000000WW055W00W555000000",
			"055W000WWWW0000000WWWWWWWW5W00W000000000",
			"055W0000WW000000000000000W5W00W000000000",
			"055W000000WWW00000000000W55W00W000000000",
			"055W00000000000000000000W55W00WWWWWWWWW5",
			"055W200000WWWWW00000WW00WW5W0000000I0085",
			"055W00WW00000000000000000W5W00WWWWWWWWW5",
			"055W000000000000000000000W5W00W500000000",
			"055W000000J0J000000000000WWW00W500000000",
			"055W000000J7J000000000000WWWWWW550000000",
			"0JJJJJJJJJJ0JJJJJJJJJJJJJJJJ000000000000",

			"0000000000101000000000000000000000000000",
			"0000000000404000000000000000000000000000",
			"0000000000400000000000000000000000000000",
			"0000000000041100000000000000000000000000",
			"0000000000000000000000000000000000000000"
		};
		for (int k = 0; k < mxy.z; k++){
			for (int j = 0; j < mxy.y; j++){
				for (int i = 0; i < mxy.x; i++){
					mapfilter[k * mxy.y * mxy.x + j * mxy.x + i] = mapfilter1[k][j][i];
				}
			}
		}
	}
	if (h_p == NULL && MPos == POS_ST){
		cax1 = 1150.0f;
		cax2 = 1150.0f;
		cay1 = 3250.0f;
		cay2 = 3180.0f;
		posx = 11;
		posy = 32;
		posz = 0;
		src_theta = 0.0f;
	}
	if (h_p == NULL && MPos == POS_EN1){
		cax1 = 950.0f;
		cax2 = 950.0f;
		cay1 = 750.0f;
		cay2 = 820.0f;
		posx = 9;
		posy = 7;
		posz = 0;
		src_theta = 180.0f;
	}
	if (h_p == NULL && MPos == POS_EN2){
		cax1 = 3750.0f;
		cax2 = 3680.0f;
		cay1 = 2950.0f;
		cay2 = 2950.0f;
		posx = 37;
		posy = 29;
		posz = 0;
		src_theta = 270.0f;
	}
	break;

	case 2:
	{
		char mapfilter2[2][40][31] =
		{
			{
				"LLLL00000000000000000000000000",
				"000000000000000000000000000000",
				"III000000000000000000000000000",
				"I00000000000000000000000000000",
				"I0I000000000000000000000000000",

				"K0KKKKKKKKKKKKKKKKKKKKKKKKKKKK",//0
				"K6K00000000000000000000000000K",//1
				"K0K0000000K00000K000000000000K",//2
				"K0K00KKKK0K00000K000000000000K",//3
				"K0K0000000K00000K000000000000K",//4
				"K3K0000000K00KKKK000000000000K",//5
				"K0K0KKK000K00000K000000000000K",//6
				"K0K0000000KKKK00KKKKKKKKKK000K",//7
				"K0K0000000K00000K000000000000K",//8
				"K0K0000000K00000K000000000000K",//9
				"K0KKK00KKKKKKKKKK000000000000K",//10
				"K000000000000000K0000000KKKKKK",//11
				"KKKKKKKKK000000KK000000000000K",//12
				"K000000000000000KKKK000000000K",//13
				"KKKKKKKKKKKKKKKKK000000000000K",//14
				"K0K00000000000000000000000000K",//15
				"KKKKKKKKKKKKKKKKKKKKKKKKKKK00K",//16
				"K0000000000000000000000000000K",//17
				"K000000000000000000000000KKKKK",//18
				"KKKKKKKKKKKKKKKKKKK0000000000K",//19
				"K00000000000000000K0000000000K",//20
				"K00000000000000000KKKKKK00000K",//21
				"K0K00000000000000000000K00000K",//22
				"K0K00000000000000000000K00000K",//23
				"K0K00000000020000000000K00KKKK",//24
				"K0OKKKKKKKKKKKKKKK00000K00000K",//25
				"K0K00000000000000000000K00000K",//26
				"K0K00000000000000000000K00000K",//27
				"K0K00000000000000000000000000K",//28
				"K0KKKKKKKKKKKKKKKKKKKKKKKKKKKK",//29
				"K0K000000000000000000000000000",
				"K0K000000000000000000000000000",
				"K0K000000000000000000000000000",
				"K7K000000000000000000000000000",
				"K0K000000000000000000000000000"
			},
			{
				"000000000000000000000000000000",
				"000000000000000000000000000000",
				"000000000000000000000000000000",
				"000000000000000000000000000000",
				"000000000000000000000000000000",

				"KKKKKKKKKKKKKKKKKKKKKKKKKKKKKK",//0
				"K0K00000000000000000000000000K",//1
				"K0K0000000K00000K000000000000K",//2
				"K0K00KKKK0K00000K000000000000K",//3
				"K0K0000000K00000K000000000000K",//4
				"K0K0000000K00KKKK000000000000K",//5
				"K0K0KKK000K00000K000000000000K",//6
				"K0K0000000KKKK00KKKKKKKKKK000K",//7
				"K0K0000000K00000K000000000000K",//8
				"K0K0000000K00000K000000000000K",//9
				"K0KKK00KKKKKKKKKK000000000000K",//10
				"K0K0000000000000K0000000KKKKKK",//11
				"K0KKKKKKK000000KK000000000000K",//12
				"K000000000000000KKKK000000000K",//13
				"K0KKKKKKKKKKKKKKK000000000000K",//14
				"KKK00000000000000000000000000K",//15
				"KKKKKKKKKKKKKKKKKKKKKKKKKKK00K",//16
				"K0000000000000000000000000000K",//17
				"K000000000000000000000000KKKKK",//18
				"KKKKKKKKKKKKKKKKKKK0000000000K",//19
				"K00000000000000000K0000000000K",//20
				"K00000000000000000KKKKKK00000K",//21
				"K0K00000000000000000000K00000K",//22
				"K0K00000000000000000000K00000K",//23
				"K0K00000000000000000000K00KKKK",//24
				"K0KKKKKKKKKKKKKKKK00000K00000K",//25
				"K0K00000000000000000000K00000K",//26
				"K0K00000000000000000000K00000K",//27
				"K0K00000000000000000000000000K",//28
				"K0KKKKKKKKKKKKKKKKKKKKKKKKKKKK",//29
				"K0K000000000000000000000000000",
				"K0K000000000000000000000000000",
				"K0K000000000000000000000000000",
				"K0K000000000000000000000000000",
				"KKK000000000000000000000000000"
			}
		};
		for (int k = 0; k < mxy.z; k++){
			for (int j = 0; j < mxy.y; j++){
				for (int i = 0; i < mxy.x; i++){
					mapfilter[k * mxy.y * mxy.x + j * mxy.x + i] = mapfilter2[k][j][i];
				}
			}
		}
	}
	if (h_p == NULL && MPos == POS_ST){
		cax1 = 150.0f;
		cax2 = 150.0f;
		cay1 = 3750.0f;
		cay2 = 3680.0f;
		posx = 1;
		posy = 37;
		posz = 0;
		src_theta = 0.0f;
	}
	if (h_p == NULL && MPos == POS_EN1){
		cax1 = 150.0f;
		cax2 = 150.0f;
		cay1 = 750.0f;
		cay2 = 820.0f;
		posx = 1;
		posy = 7;
		posz = 0;
		src_theta = 180.0f;
	}
	break;

	case 3:
	{
		char mapfilter3[2][35][31] =
		{
			{
				"LLLLLLLLLLLLLLLLLLLLLLLLLLLLLL",//0
				"LIIIIIIIIIIIIIIIIIIIIIIIIIIIIL",//1
				"LI00000I00000000000000000000IL",//2
				"LI00000I00000000000000000000IL",//3
				"LI00000I00000000000000000000IL",//4
				"LIII3III00000000000000000000IL",//5
				"LI0I0I0000000IIIIIII00IIIIIIIL",//6
				"LI0I0I0000000I00000000000000IL",//7
				"LI00000000000I00000000000000IL",//8
				"LI00000000000I00000000000000IL",//9
				"LIIIIIIIIIIIIIIIIIIIIIIII000IL",//10
				"LI00000000000000000000000000IL",//11
				"LI00000000000000000000000000IL",//12
				"LIIIII00IIIIIIIIIIIIIIIIIIIIIL",//13
				"LI00000000000000000000000000IL",//14
				"LI00000000000000000000000000IL",//15
				"LI00000000000000000000000000IL",//16
				"LI00000000000000000000000000IL",//17
				"LI0000IIIIIIIIIIIIIIIIIIIIIIIL",//18
				"LI00000000000000000000000000IL",//19
				"LI00000000000000000000000000IL",//20
				"LI00000000000000000000000000IL",//21
				"LIIIIIIIIIIIIIIIIIIIIII00000IL",//22
				"LI00000000I00000000000000000IL",//23
				"LI00000002I00I00000000000000IL",//24
				"LI0I000000I00I000IIIIIIIIIIIIL",//25
				"LI0I000000000I00000000000000IL",//26
				"LI0I000000I00I00000000000000IL",//27
				"LI7IIIIIIIIIIIIIIIIIIIIIIIIIIL",//28
				"LL0LLLLLLLLLLLLLLLLLLLLLLLLLLL",//29

				"0K0K00000000000000000000000000",
				"0O0O00000000000000000000000000",
				"0K0000000000000000000000000000",
				"0KKK00000000000000000000000000",
				"000000000000000000000000000000"
			},
			{
				"LLLLLLLLLLLLLLLLLLLLLLLLLLLLLL",//0
				"L0000000000000000000000000000L",//1
				"L0000000000000000000000000000L",//2
				"L0000000000000000000000000000L",//3
				"L0000000000000000000000000000L",//4
				"L0000000000000000000000000000L",//5
				"L0000000000000000000000000000L",//6
				"L0000000000000000000000000000L",//7
				"L0000000000000000000000000000L",//8
				"L0000000000000000000000000000L",//9
				"L0000000000000000000000000000L",//10
				"L0000000000000000000000000000L",//11
				"L0000000000000000000000000000L",//12
				"L0000000000000000000000000000L",//13
				"L0000000000000000000000000000L",//14
				"L0000000000000000000000000000L",//15
				"L0000000000000000000000000000L",//16
				"L0000000000000000000000000000L",//17
				"L0000000000000000000000000000L",//18
				"L0000000000000000000000000000L",//19
				"L0000000000000000000000000000L",//20
				"L0000000000000000000000000000L",//21
				"L0000000000000000000000000000L",//22
				"L0000000000000000000000000000L",//23
				"L0000000000000000000000000000L",//24
				"L0000000000000000000000000000L",//25
				"L0000000000000000000000000000L",//26
				"L0000000000000000000000000000L",//27
				"L0000000000000000000000000000L",//28
				"LLLLLLLLLLLLLLLLLLLLLLLLLLLLLL",//29

				"000000000000000000000000000000",
				"000000000000000000000000000000",
				"000000000000000000000000000000",
				"000000000000000000000000000000",
				"000000000000000000000000000000"
			}
		};
		for (int k = 0; k < mxy.z; k++){
			for (int j = 0; j < mxy.y; j++){
				for (int i = 0; i < mxy.x; i++){
					mapfilter[k * mxy.y * mxy.x + j * mxy.x + i] = mapfilter3[k][j][i];
				}
			}
		}
	}
	if (h_p == NULL && MPos == POS_ST){
		cax1 = 250.0f;
		cax2 = 250.0f;
		cay1 = 2750.0f;
		cay2 = 2680.0f;
		posx = 2;
		posy = 27;
		posz = 0;
		src_theta = 0.0f;
	}
	break;

	case 4:
	{
		char mapfilter4[8][30][31] =
		{
			{
				//1階
				"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",//0
				"MA00000000M000000M00000000000M",//1
				"M000000000M000000M00000000000M",//2
				"MMMMMMM000M000000N00000000000M",//3
				"M000000000M000000000000000000M",//4
				"M000000000M000000N00000000000M",//5
				"M0000000000000000M00000000000M",//6
				"MMMMMMMMMMMMMMMMMMMMMMM000000M",//7
				"M00000000000000MM000000000000M",//8
				"M000000000000000MM0000MMMMMMMM",//9
				"M0000000000000000MM0000000000M",//10
				"M0000000000000000000000000000M",//11
				"M0MMMMMMMMMMMMMMMMMMMMMMMMMMMM",//12
				"M0000000000000000000000000000M",//13
				"M000000000000M000000000000000M",//14
				"MA00000000000M000000000000000M",//15
				"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",//16
				"M000000000000000AM0000000000AM",//17
				"M0000000000000000M00000000000M",//18
				"M0000000000000000M00000000000M",//19
				"MA000000000000000M00000000000M",//20
				"MMMMMMMMMMMMMMMMMMMMM0000000AM",//21
				"M0000000000000000000MMMMMMMMMM",//22
				"M000000000A002000000M00000000M",//23
				"M00000000M000M000000M00000000M",//24
				"M00000000M000M000000M00000000M",//25
				"M00000000MN0NM000000M00000000M",//26
				"M00000000MN0NM000000000000000M",//27
				"MMMMMMMMMMN7NMMMMMMMMMMMMMMMMM",//28
				"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM" //29
			},
			{
				"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",//0
				"M0000000000000000000000000000M",//1
				"M0000000000000000000000000000M",//2
				"M0000000000000000000000000000M",//3
				"M0000000000000000000000000000M",//4
				"M0000000000000000000000000000M",//5
				"M0000000000000000000000000000M",//6
				"M0000000000000000000000000000M",//7
				"M0000000000000000000000000000M",//8
				"M0000000000000000000000000000M",//9
				"M0000000000000000000000000000M",//10
				"M0000000000000000000000000000M",//11
				"M0000000000000000000000000000M",//12
				"M0000000000000000000000000000M",//13
				"M0000000000000000000000000000M",//14
				"M0000000000000000000000000000M",//15
				"M0000000000000000000000000000M",//16
				"M0000000000000000000000000000M",//17
				"M0000000000000000000000000000M",//18
				"M0000000000000000000000000000M",//19
				"M0000000000000000000000000000M",//20
				"M0000000000000000000000000000M",//21
				"M0000000000000000000000000000M",//22
				"M0000000000000000000000000000M",//23
				"M0000000000000000000000000000M",//24
				"M0000000000000000000000000000M",//25
				"M0000000000000000000000000000M",//26
				"M0000000000000000000000000000M",//27
				"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",//28
				"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM" //29
			},
			{
				//2階の底
				"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",//0
				"M0MMM000000000000000000000000M",//1
				"M000M000000000000000000000000M",//2
				"M000M000000000000000000000000M",//3
				"M000M000000000000000000000000M",//4
				"M000M000000000000000000000000M",//5
				"M000M000000000000000000000000M",//6
				"M000M000000000000000000000000M",//7
				"M000M000000000000000000000000M",//8
				"M000M000000000000000000000000M",//9
				"M000M000000000000000000000000M",//10
				"M000M000000000000000000000000M",//11
				"M000M000000000000000000000000M",//12
				"M000M000000000000000000000000M",//13
				"M000M000000000000000000000000M",//14
				"M000M00000000000000000000000MM",//15
				"MM00M00000000000000000000000MM",//16
				"M000M000000000000MMM000000000M",//17
				"M000M000000000000000000000000M",//18
				"M000M00000M000000000000000000M",//19
				"M000M00000M000000000000000000M",//20
				"MM00M00000M000000000000000000M",//21
				"MM00N00000MMMMMMMMMMMMMMM000MM",//22
				"MM00N0000000000000000000M000MM",//23
				"M000NMM00000000000000000M000MM",//24
				"M000N0000000000000000000M000MM",//25
				"M000N00000000000000000000000MM",//26
				"M000N000000000000000000000000M",//27
				"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",//28
				"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM" //29
			},
			{
				//2階 壁透明
				"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",//0
				"M0000-00000000000000000000000M",//1
				"M---0-00000000000000000000000M",//2
				"M00-0-00000000000000000000000M",//3
				"M00-0-00000000000000000000000M",//4
				"M00-0-00000000000000000000000M",//5
				"M00-0-00000000000000000000000M",//6
				"M00-0-00000000000000000000000M",//7
				"M00-0-00000000000000000000000M",//8
				"M00-0-00000000000000000000000M",//9
				"M00-0-00000000000000000000000M",//10
				"M00-0-00000000000000000000000M",//11
				"M00-0-00000000000000000000000M",//12
				"M00-0-000000000000000000000--M",//13
				"M---0-000000000000000000000-BM",//14
				"M0--0-000000000000000000000-0M",//15
				"M0--0-000000000-------00000-0M",//16
				"MB--0-000---000-0000B-00000-0M",//17
				"M---0-000-B-000-------00000--M",//18
				"M---0-000-0-00000000000000000M",//19
				"M0--0-000-0-00000000000000000M",//20
				"M0--0-000-0---------------0--M",//21
				"M0--0-000-000000000000000-0-0M",//22
				"M0--0-----0-------------0-0-0M",//23
				"MB--000B----00000000000-0-0-0M",//24
				"M---0----00000000000000-0-0-0M",//25
				"M00-0-B-000000000000000-B-0-0M",//26
				"M00-A---000000000000000---0-BM",//27
				"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",//28
				"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM" //29
			},
			{
				"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",//0
				"M0000000000000000000000000000M",//1
				"M0000000000000000000000000000M",//2
				"M0000000000000000000000000000M",//3
				"M0000000000000000000000000000M",//4
				"M0000000000000000000000000000M",//5
				"M0000000000000000000000000000M",//6
				"M0000000000000000000000000000M",//7
				"M0000000000000000000000000000M",//8
				"M0000000000000000000000000000M",//9
				"M0000000000000000000000000000M",//10
				"M0000000000000000000000000000M",//11
				"M0000000000000000000000000000M",//12
				"M0000000000000000000000000000M",//13
				"M0000000000000000000000000000M",//14
				"M0000000000000000000000000000M",//15
				"M0000000000000000000000000000M",//16
				"M0000000000000000000000000000M",//17
				"M0000000000000000000000000000M",//18
				"M0000000000000000000000000000M",//19
				"M0000000000000000000000000000M",//20
				"M0000000000000000000000000000M",//21
				"M0000000000000000000000000000M",//22
				"M0000000000000000000000000000M",//23
				"M0000000000000000000000000000M",//24
				"M0000000000000000000000000000M",//25
				"M0000000000000000000000000000M",//26
				"M0000000000000000000000000000M",//27
				"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",//28
				"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM" //29
			},
			{
				//3階底
				"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",//0
				"M0000000000000000000000000000M",//1
				"M0000000000000000000000000000M",//2
				"M0000000000000000000000000000M",//3
				"M0000000000000000000000000000M",//4
				"M0000000000000000000000000000M",//5
				"M0000000000000000000000000000M",//6
				"M0000000000000000000000000000M",//7
				"M0000000000000000000000000000M",//8
				"M00000000000NNNNN000000000000M",//9
				"M0000000000NMMMMMN00000000000M",//10
				"M000000000NMMMMMMMN0000000000M",//11
				"M0000000000NNMMMNN00000000000M",//12
				"M000000000000NMN0000000000000M",//13
				"M0000000000000M00000000000000M",//14
				"M0000000000000M00000000000000M",//15
				"M0000000000000M00000000000000M",//16
				"M0000000000000M00000000000000M",//17
				"M0000000000000M00000000000000M",//18
				"M0000000000000M00000000000000M",//19
				"M0000000000000M00000000000000M",//20
				"M0000000000000M00000000000000M",//21
				"M0000000000000M00000000000000M",//22
				"M0000000000000M00000000000000M",//23
				"M0000000000000M00000000000000M",//24
				"M0000000000000M00000000000000M",//25
				"M0000000000000M00000000000000M",//26
				"M0000MMMMMMMMMM00000000000000M",//27
				"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",//28
				"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM" //29
			},
			{
				//3階 壁透明
				"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",//0
				"M0000000000000000000000000000M",//1
				"M0000000000000000000000000000M",//2
				"M0000000000000000000000000000M",//3
				"M0000000000000000000000000000M",//4
				"M0000000000000000000000000000M",//5
				"M0000000000000000000000000000M",//6
				"M0000000000000000000000000000M",//7
				"M0000000000000000000000000000M",//8
				"M0000000000000000000000000000M",//9
				"M0000000000000000000000000000M",//10
				"M000000000000---0000000000000M",//11
				"M000000000000-0-0000000000000M",//12
				"M000000000000-3-0000000000000M",//13
				"M000000000000-0-0000000000000M",//14
				"M000000000000-0-0000000000000M",//15
				"M000000000000-0-0000000000000M",//16
				"M000000000000-0-0000000000000M",//17
				"M000000000000-0-0000000000000M",//18
				"M000000000000-0-0000000000000M",//19
				"M000000000000-0-0000000000000M",//20
				"M000000000000-0-0000000000000M",//21
				"M000000000000-0-0000000000000M",//22
				"M000000000000-0-0000000000000M",//23
				"M000000000000-0-0000000000000M",//24
				"M0000---00000-0-0000000000000M",//25
				"M00---B-------0-0000000000000M",//26
				"M---00000000000-0000000000000M",//27
				"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",//28
				"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM" //29
			},
			{
				//3階 上側(天井まで狭いから付け足し)
				"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",//0
				"M0000000000000000000000000000M",//1
				"M0000000000000000000000000000M",//2
				"M0000000000000000000000000000M",//3
				"M0000000000000000000000000000M",//4
				"M0000000000000000000000000000M",//5
				"M0000000000000000000000000000M",//6
				"M0000000000000000000000000000M",//7
				"M0000000000000000000000000000M",//8
				"M0000000000000000000000000000M",//9
				"M0000000000000000000000000000M",//10
				"M0000000000000000000000000000M",//11
				"M0000000000000000000000000000M",//12
				"M0000000000000000000000000000M",//13
				"M0000000000000000000000000000M",//14
				"M0000000000000000000000000000M",//15
				"M0000000000000000000000000000M",//16
				"M0000000000000000000000000000M",//17
				"M0000000000000000000000000000M",//18
				"M0000000000000000000000000000M",//19
				"M0000000000000000000000000000M",//20
				"M0000000000000000000000000000M",//21
				"M0000000000000000000000000000M",//22
				"M0000000000000000000000000000M",//23
				"M0000000000000000000000000000M",//24
				"M0000000000000000000000000000M",//25
				"M0000000000000000000000000000M",//26
				"M0000000000000000000000000000M",//27
				"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",//28
				"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM" //29
			}
		};
		for (int k = 0; k < mxy.z; k++){
			for (int j = 0; j < mxy.y; j++){
				for (int i = 0; i < mxy.x; i++){
					mapfilter[k * mxy.y * mxy.x + j * mxy.x + i] = mapfilter4[k][j][i];
				}
			}
		}
	}
	if (h_p == NULL && MPos == POS_ST){
		cax1 = 1150.0f;
		cax2 = 1150.0f;
		cay1 = 2750.0f;
		cay2 = 2680.0f;
		posx = 11;
		posy = 27;
		posz = 0;
		src_theta = 0.0f;
	}
	break;
	}

	mxy.m = (int*)malloc(sizeof(int) * mxy.z * mxy.y * mxy.x);

	//配列コピー
	for (int k = 0; k < mxy.z; k++){
		for (int j = 0; j < mxy.y; j++){
			for (int i = 0; i < mxy.x; i++){
				mxy.m[k * mxy.y * mxy.x + j * mxy.x + i] = mapfilter[k * mxy.y * mxy.x + j * mxy.x + i];
			}
		}
	}

	//頂点バッファ確保の為の個数カウント
	for (int k = 0; k < mxy.z; k++){
		for (int j = 0; j < mxy.y; j++){
			for (int i = 0; i < mxy.x; i++){
				if (mxy.m[k * mxy.y * mxy.x + j * mxy.x + i] == 49 || mxy.m[k * mxy.y * mxy.x + j * mxy.x + i] == 52){
					//各方向にブロックが無いか検査, 有る場合はその面は描画しないのでバッファ確保しない
					Mapfilter_p(k, j, i, 49, 52, &blockcountA);
				}
				if (mxy.m[k * mxy.y * mxy.x + j * mxy.x + i] == 74){
					Mapfilter_p(k, j, i, 74, 74, &blockcountB);
				}
				if (mxy.m[k * mxy.y * mxy.x + j * mxy.x + i] == 75 || mxy.m[k * mxy.y * mxy.x + j * mxy.x + i] == 79){
					Mapfilter_p(k, j, i, 75, 79, &blockcountC);
				}
				if (mxy.m[k * mxy.y * mxy.x + j * mxy.x + i] == 76){
					Mapfilter_p(k, j, i, 76, 76, &blockcountD);
				}
				if (mxy.m[k * mxy.y * mxy.x + j * mxy.x + i] == 77 || mxy.m[k * mxy.y * mxy.x + j * mxy.x + i] == 78){
					Mapfilter_p(k, j, i, 77, 78, &blockcountE);
				}

				if (mxy.m[k * mxy.y * mxy.x + j * mxy.x + i] == 50)r_point_count++;//2のアスキーコード50
				if (mxy.m[k * mxy.y * mxy.x + j * mxy.x + i] == 52 ||
					mxy.m[k * mxy.y * mxy.x + j * mxy.x + i] == 78 ||
					mxy.m[k * mxy.y * mxy.x + j * mxy.x + i] == 79)mo_count++;
				if (mxy.m[k * mxy.y * mxy.x + j * mxy.x + i] == 87)woodcount++;
				if (mxy.m[k * mxy.y * mxy.x + j * mxy.x + i] == 53)squarecount++;
				if (mxy.m[k * mxy.y * mxy.x + j * mxy.x + i] == 51){
					if (boss_killed[map_no] == 1){
						mxy.m[k * mxy.y * mxy.x + j * mxy.x + i] = 48;
					}
					else boss_count++;
				}
				if (mxy.m[k * mxy.y * mxy.x + j * mxy.x + i] == 65 ||
					mxy.m[k * mxy.y * mxy.x + j * mxy.x + i] == 66)Elevator_count++;
				if (mxy.m[k * mxy.y * mxy.x + j * mxy.x + i] == 73){
					if (map_no == 1 && boss_killed[3] == 1){
						mxy.m[k * mxy.y * mxy.x + j * mxy.x + i] = 48;
					}
					else Mapfilter_p(k, j, i, 73, 73, &f_wall_count);
				}
			}
		}
	}
	lightcount = mo_count * 4;//1ブロック最大4光源
	free(mapfilter);
	mapfilter = NULL;

	if (h_p != NULL){
		cax1 = h_p->cx1;
		cax2 = h_p->cx2;
		cay1 = h_p->cy1;
		cay2 = h_p->cy2;
		posx = h_p->px;
		posy = h_p->py;
		posz = h_p->pz;
		src_theta = h_p->theta;
	}
}