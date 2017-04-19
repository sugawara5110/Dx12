//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　    TextureBinaryLoaderクラス                               **//
//**                                                                                     **//
//*****************************************************************************************//

#include "TextureBinaryLoader.h"

void TextureBinaryLoader::TextureGetBufferAll() {

	Dx12Process *dx = Dx12Process::GetInstance();

	Dx12Process::Lock();
	//マップ0
	dx->TextureGetBuffer("./dat/texture/map/wall1.da", 0);
	dx->TextureGetBuffer("./dat/texture/map/ground1.da", 1);
	dx->TextureGetBuffer("./dat/texture/map/ceiling1.da", 2);
	//マップ1
	dx->TextureGetBuffer("./dat/texture/map/wall2.da", 4);
	dx->TextureGetBuffer("./dat/texture/map/ground2.da", 5);
	dx->TextureGetBuffer("./dat/texture/map/ceiling2.da", 6);
	dx->TextureGetBuffer("./dat/texture/map/wall2-1.da", 8);
	dx->TextureGetBuffer("./dat/texture/map/leaf.da", 9);
	dx->TextureGetBuffer("./dat/texture/map/wood.da", 10);
	//マップ2
	dx->TextureGetBuffer("./dat/texture/map/ceiling3_wall3.da", 11);
	dx->TextureGetBuffer("./dat/texture/map/ground3.da", 12);
	//マップ3
	dx->TextureGetBuffer("./dat/texture/map/ceiling4_ground4.da", 15);
	//マップ4
	dx->TextureGetBuffer("./dat/texture/map/wall5.da", 26);
	dx->TextureGetBuffer("./dat/texture/map/ground5.da", 27);
	dx->TextureGetBuffer("./dat/texture/map/ceiling5.da", 28);
	//通常敵
	dx->TextureGetBuffer("./dat/texture/enemy/enemy1.da", 30);
	dx->TextureGetBuffer("./dat/texture/enemy/enemy2.da", 31);
	dx->TextureGetBuffer("./dat/texture/enemy/enemy3.da", 32);
	dx->TextureGetBuffer("./dat/texture/enemy/enemy4.da", 33);
	dx->TextureGetBuffer("./dat/texture/enemy/enemy5.da", 34);
	dx->TextureGetBuffer("./dat/texture/enemy/enemy6.da", 35);
	dx->TextureGetBuffer("./dat/texture/enemy/enemy7.da", 36);
	dx->TextureGetBuffer("./dat/texture/enemy/enemy8.da", 37);
	dx->TextureGetBuffer("./dat/texture/enemy/enemy9.da", 38);
	dx->TextureGetBuffer("./dat/texture/enemy/enemy10.da", 39);
	dx->TextureGetBuffer("./dat/texture/enemy/enemy11.da", 40);
	dx->TextureGetBuffer("./dat/texture/enemy/enemy12.da", 41);
	dx->TextureGetBuffer("./dat/texture/enemy/enemy13.da", 42);
	dx->TextureGetBuffer("./dat/texture/enemy/enemy14.da", 43);
	dx->TextureGetBuffer("./dat/texture/enemy/enemy15.da", 44);
	dx->TextureGetBuffer("./dat/texture/enemy/enemy16.da", 45);
	dx->TextureGetBuffer("./dat/texture/enemy/enemy17.da", 46);
	dx->TextureGetBuffer("./dat/texture/enemy/enemy18.da", 47);
	dx->TextureGetBuffer("./dat/texture/enemy/enemy19.da", 48);
	dx->TextureGetBuffer("./dat/texture/enemy/enemy20.da", 49);
	//ボス
	dx->TextureGetBuffer("./dat/texture/enemy/boss1.da", 50);
	dx->TextureGetBuffer("./dat/texture/enemy/boss2.da", 51);
	dx->TextureGetBuffer("./dat/texture/enemy/boss3.da", 52);
	dx->TextureGetBuffer("./dat/texture/enemy/boss4.da", 53);
	//ラストボス
	dx->TextureGetBuffer("./dat/texture/enemy/lastboss.da", 59);
	//魔方陣通常
	dx->TextureGetBuffer("./dat/texture/magic/side_magic.da", 60);
	//魔方陣ボス
	dx->TextureGetBuffer("./dat/texture/magic/boss_magic.da", 61);
	dx->TextureGetBuffer("./dat/texture/magic/particle.da", 62);
	//回復ポイント
	dx->TextureGetBuffer("./dat/texture/magic/recover.da", 70);
	//エフェクト
	dx->TextureGetBuffer("./dat/texture/effect/e_att.da", 80);
	dx->TextureGetBuffer("./dat/texture/effect/h_att.da", 81);
	dx->TextureGetBuffer("./dat/texture/effect/flame.da", 82);
	dx->TextureGetBuffer("./dat/texture/effect/healing.da", 83);
	dx->TextureGetBuffer("./dat/texture/effect/recov.da", 84);
	//プレイヤー1テクスチャ
	dx->TextureGetBuffer("./dat/texture/player/p1/brown_eye.da", 90);
	dx->TextureGetBuffer("./dat/texture/player/p1/classicshoes_texture_diffuse.da", 91);
	dx->TextureGetBuffer("./dat/texture/player/p1/eyebrow001.da", 92);
	dx->TextureGetBuffer("./dat/texture/player/p1/jacket01_diffuse.da", 93);
	dx->TextureGetBuffer("./dat/texture/player/p1/jeans01_black_diffuse.da", 94);
	dx->TextureGetBuffer("./dat/texture/player/p1/male01_diffuse_black.da", 95);
	dx->TextureGetBuffer("./dat/texture/player/p1/young_lightskinned_male_diffuse.da", 96);
	//プレイヤー2テクスチャ
	dx->TextureGetBuffer("./dat/texture/player/p2/brown_eye.da", 100);
	dx->TextureGetBuffer("./dat/texture/player/p2/diffuse_black.da", 101);
	dx->TextureGetBuffer("./dat/texture/player/p2/eyebrow006.da", 102);
	dx->TextureGetBuffer("./dat/texture/player/p2/eyelashes03.da", 103);
	dx->TextureGetBuffer("./dat/texture/player/p2/shoes02_default.da", 104);
	dx->TextureGetBuffer("./dat/texture/player/p2/short01_black_diffuse.da", 105);
	dx->TextureGetBuffer("./dat/texture/player/p2/tshirt02_texture.da", 106);
	dx->TextureGetBuffer("./dat/texture/player/p2/young_lightskinned_female_diffuse.da", 107);
	//プレイヤー3テクスチャ
	dx->TextureGetBuffer("./dat/texture/player/p3/brown_eye.da", 110);
	dx->TextureGetBuffer("./dat/texture/player/p3/classicshoes_texture_diffuse.da", 111);
	dx->TextureGetBuffer("./dat/texture/player/p3/jeans_basic_diffuse.da", 112);
	dx->TextureGetBuffer("./dat/texture/player/p3/young_darkskinned_male_diffuse.da", 113);
	//プレイヤー4テクスチャ
	dx->TextureGetBuffer("./dat/texture/player/p4/afro.da", 120);
	dx->TextureGetBuffer("./dat/texture/player/p4/brown_eye.da", 121);
	dx->TextureGetBuffer("./dat/texture/player/p4/eyebrow007.da", 122);
	dx->TextureGetBuffer("./dat/texture/player/p4/eyelashes03.da", 123);
	dx->TextureGetBuffer("./dat/texture/player/p4/shoes02_default.da", 124);
	dx->TextureGetBuffer("./dat/texture/player/p4/short01_black_diffuse.da", 125);
	dx->TextureGetBuffer("./dat/texture/player/p4/tshirt_texture_blue.da", 126);
	dx->TextureGetBuffer("./dat/texture/player/p4/young_darkskinned_female_diffuse.da", 127);

	Dx12Process::Unlock();
}

void TextureBinaryLoader::TextureBinaryDecodeAll() {

	Dx12Process *dx = Dx12Process::GetInstance();

	//マップ0
	dx->TextureBinaryDecode("./dat/texture/map/wall1.da", 0);
	dx->TextureBinaryDecode("./dat/texture/map/ground1.da", 1);
	dx->TextureBinaryDecode("./dat/texture/map/ceiling1.da", 2);
	//マップ1
	dx->TextureBinaryDecode("./dat/texture/map/wall2.da", 4);
	dx->TextureBinaryDecode("./dat/texture/map/ground2.da", 5);
	dx->TextureBinaryDecode("./dat/texture/map/ceiling2.da", 6);
	dx->TextureBinaryDecode("./dat/texture/map/wall2-1.da", 8);
	dx->TextureBinaryDecode("./dat/texture/map/leaf.da", 9);
	dx->TextureBinaryDecode("./dat/texture/map/wood.da", 10);
	//マップ2
	dx->TextureBinaryDecode("./dat/texture/map/ceiling3_wall3.da", 11);
	dx->TextureBinaryDecode("./dat/texture/map/ground3.da", 12);
	//マップ3
	dx->TextureBinaryDecode("./dat/texture/map/ceiling4_ground4.da", 15);
	//マップ4
	dx->TextureBinaryDecode("./dat/texture/map/wall5.da", 26);
	dx->TextureBinaryDecode("./dat/texture/map/ground5.da", 27);
	dx->TextureBinaryDecode("./dat/texture/map/ceiling5.da", 28);
	//通常敵
	dx->TextureBinaryDecode("./dat/texture/enemy/enemy1.da", 30);
	dx->TextureBinaryDecode("./dat/texture/enemy/enemy2.da", 31);
	dx->TextureBinaryDecode("./dat/texture/enemy/enemy3.da", 32);
	dx->TextureBinaryDecode("./dat/texture/enemy/enemy4.da", 33);
	dx->TextureBinaryDecode("./dat/texture/enemy/enemy5.da", 34);
	dx->TextureBinaryDecode("./dat/texture/enemy/enemy6.da", 35);
	dx->TextureBinaryDecode("./dat/texture/enemy/enemy7.da", 36);
	dx->TextureBinaryDecode("./dat/texture/enemy/enemy8.da", 37);
	dx->TextureBinaryDecode("./dat/texture/enemy/enemy9.da", 38);
	dx->TextureBinaryDecode("./dat/texture/enemy/enemy10.da", 39);
	dx->TextureBinaryDecode("./dat/texture/enemy/enemy11.da", 40);
	dx->TextureBinaryDecode("./dat/texture/enemy/enemy12.da", 41);
	dx->TextureBinaryDecode("./dat/texture/enemy/enemy13.da", 42);
	dx->TextureBinaryDecode("./dat/texture/enemy/enemy14.da", 43);
	dx->TextureBinaryDecode("./dat/texture/enemy/enemy15.da", 44);
	dx->TextureBinaryDecode("./dat/texture/enemy/enemy16.da", 45);
	dx->TextureBinaryDecode("./dat/texture/enemy/enemy17.da", 46);
	dx->TextureBinaryDecode("./dat/texture/enemy/enemy18.da", 47);
	dx->TextureBinaryDecode("./dat/texture/enemy/enemy19.da", 48);
	dx->TextureBinaryDecode("./dat/texture/enemy/enemy20.da", 49);
	//ボス
	dx->TextureBinaryDecode("./dat/texture/enemy/boss1.da", 50);
	dx->TextureBinaryDecode("./dat/texture/enemy/boss2.da", 51);
	dx->TextureBinaryDecode("./dat/texture/enemy/boss3.da", 52);
	dx->TextureBinaryDecode("./dat/texture/enemy/boss4.da", 53);
	//ラストボス
	dx->TextureBinaryDecode("./dat/texture/enemy/lastboss.da", 59);
	//魔方陣通常
	dx->TextureBinaryDecode("./dat/texture/magic/side_magic.da", 60);
	//魔方陣ボス
	dx->TextureBinaryDecode("./dat/texture/magic/boss_magic.da", 61);
	dx->TextureBinaryDecode("./dat/texture/magic/particle.da", 62);
	//回復ポイント
	dx->TextureBinaryDecode("./dat/texture/magic/recover.da", 70);
	//エフェクト
	dx->TextureBinaryDecode("./dat/texture/effect/e_att.da", 80);
	dx->TextureBinaryDecode("./dat/texture/effect/h_att.da", 81);
	dx->TextureBinaryDecode("./dat/texture/effect/flame.da", 82);
	dx->TextureBinaryDecode("./dat/texture/effect/healing.da", 83);
	dx->TextureBinaryDecode("./dat/texture/effect/recov.da", 84);
	//プレイヤー1テクスチャ
	dx->TextureBinaryDecode("./dat/texture/player/p1/brown_eye.da", 90);
	dx->TextureBinaryDecode("./dat/texture/player/p1/classicshoes_texture_diffuse.da", 91);
	dx->TextureBinaryDecode("./dat/texture/player/p1/eyebrow001.da", 92);
	dx->TextureBinaryDecode("./dat/texture/player/p1/jacket01_diffuse.da", 93);
	dx->TextureBinaryDecode("./dat/texture/player/p1/jeans01_black_diffuse.da", 94);
	dx->TextureBinaryDecode("./dat/texture/player/p1/male01_diffuse_black.da", 95);
	dx->TextureBinaryDecode("./dat/texture/player/p1/young_lightskinned_male_diffuse.da", 96);
	//プレイヤー2テクスチャ
	dx->TextureBinaryDecode("./dat/texture/player/p2/brown_eye.da", 100);
	dx->TextureBinaryDecode("./dat/texture/player/p2/diffuse_black.da", 101);
	dx->TextureBinaryDecode("./dat/texture/player/p2/eyebrow006.da", 102);
	dx->TextureBinaryDecode("./dat/texture/player/p2/eyelashes03.da", 103);
	dx->TextureBinaryDecode("./dat/texture/player/p2/shoes02_default.da", 104);
	dx->TextureBinaryDecode("./dat/texture/player/p2/short01_black_diffuse.da", 105);
	dx->TextureBinaryDecode("./dat/texture/player/p2/tshirt02_texture.da", 106);
	dx->TextureBinaryDecode("./dat/texture/player/p2/young_lightskinned_female_diffuse.da", 107);
	//プレイヤー3テクスチャ
	dx->TextureBinaryDecode("./dat/texture/player/p3/brown_eye.da", 110);
	dx->TextureBinaryDecode("./dat/texture/player/p3/classicshoes_texture_diffuse.da", 111);
	dx->TextureBinaryDecode("./dat/texture/player/p3/jeans_basic_diffuse.da", 112);
	dx->TextureBinaryDecode("./dat/texture/player/p3/young_darkskinned_male_diffuse.da", 113);
	//プレイヤー4テクスチャ
	dx->TextureBinaryDecode("./dat/texture/player/p4/afro.da", 120);
	dx->TextureBinaryDecode("./dat/texture/player/p4/brown_eye.da", 121);
	dx->TextureBinaryDecode("./dat/texture/player/p4/eyebrow007.da", 122);
	dx->TextureBinaryDecode("./dat/texture/player/p4/eyelashes03.da", 123);
	dx->TextureBinaryDecode("./dat/texture/player/p4/shoes02_default.da", 124);
	dx->TextureBinaryDecode("./dat/texture/player/p4/short01_black_diffuse.da", 125);
	dx->TextureBinaryDecode("./dat/texture/player/p4/tshirt_texture_blue.da", 126);
	dx->TextureBinaryDecode("./dat/texture/player/p4/young_darkskinned_female_diffuse.da", 127);
}
