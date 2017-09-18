//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　    TextureBinaryLoaderクラス                               **//
//**                                                                                     **//
//*****************************************************************************************//

#include "TextureBinaryLoader.h"
#include "Decode.h"

Texture *TextureBinaryLoader::tex = NULL;
int TextureBinaryLoader::texNum = 0;

void TextureBinaryLoader::TextureGetBuffer(char *Bpass) {
	BinaryGetBuffer(Bpass, &tex[texNum].binary_ch, &tex[texNum].binary_size);
	texNum++;
}

void TextureBinaryLoader::TextureBinaryDecode(char *Bpass) {
	TextureBinaryDecode(Bpass, FALSE);
}

void TextureBinaryLoader::TextureBinaryDecode(char *Bpass, bool UpKeep) {
	BinaryDecode(Bpass, &tex[texNum].binary_ch, &tex[texNum].binary_size);
	tex[texNum].texName = Dx12Process::GetInstance()->GetNameFromPass(Bpass);
	tex[texNum].UpKeep = UpKeep;
	texNum++;
}

void TextureBinaryLoader::TextureGetBufferAll() {

	tex = new Texture[150];

	texNum = 0;
	//マップ0
	TextureGetBuffer("./dat/texture/map/wall1.da");
	TextureGetBuffer("./dat/texture/map/wall1Nor.da");
	TextureGetBuffer("./dat/texture/map/ground1.da");
	TextureGetBuffer("./dat/texture/map/ground1Nor.da");
	TextureGetBuffer("./dat/texture/map/ceiling1.da");
	TextureGetBuffer("./dat/texture/map/ceiling1Nor.da");
	//マップ1
	TextureGetBuffer("./dat/texture/map/wall2.da");
	TextureGetBuffer("./dat/texture/map/ground2.da");
	TextureGetBuffer("./dat/texture/map/ground2Nor.da");
	TextureGetBuffer("./dat/texture/map/ceiling2.da");
	TextureGetBuffer("./dat/texture/map/wall2-1.da");
	TextureGetBuffer("./dat/texture/map/wall2Nor.da");
	TextureGetBuffer("./dat/texture/map/leaf.da");
	TextureGetBuffer("./dat/texture/map/wood.da");
	//マップ2
	TextureGetBuffer("./dat/texture/map/ceiling3_wall3.da");
	TextureGetBuffer("./dat/texture/map/ceiling3_wall3Nor.da");
	TextureGetBuffer("./dat/texture/map/ground3.da");
	TextureGetBuffer("./dat/texture/map/ground3Nor.da");
	TextureGetBuffer("./dat/texture/map/wave.da");
	TextureGetBuffer("./dat/texture/map/waveNor.da");
	//マップ3
	TextureGetBuffer("./dat/texture/map/ceiling4_ground4.da");
	TextureGetBuffer("./dat/texture/map/ceiling4_ground4Nor.da");
	//マップ4
	TextureGetBuffer("./dat/texture/map/wall5.da");
	TextureGetBuffer("./dat/texture/map/wall5Nor.da");
	TextureGetBuffer("./dat/texture/map/ground5.da");
	TextureGetBuffer("./dat/texture/map/ground5Nor.da");
	TextureGetBuffer("./dat/texture/map/ceiling5.da");
	TextureGetBuffer("./dat/texture/map/ceiling5Nor.da");
	//通常敵
	TextureGetBuffer("./dat/texture/enemy/enemy1.da");
	TextureGetBuffer("./dat/texture/enemy/enemy2.da");
	TextureGetBuffer("./dat/texture/enemy/enemy3.da");
	TextureGetBuffer("./dat/texture/enemy/enemy4.da");
	TextureGetBuffer("./dat/texture/enemy/enemy5.da");
	TextureGetBuffer("./dat/texture/enemy/enemy6.da");
	TextureGetBuffer("./dat/texture/enemy/enemy7.da");
	TextureGetBuffer("./dat/texture/enemy/enemy8.da");
	TextureGetBuffer("./dat/texture/enemy/enemy9.da");
	TextureGetBuffer("./dat/texture/enemy/enemy10.da");
	TextureGetBuffer("./dat/texture/enemy/enemy11.da");
	TextureGetBuffer("./dat/texture/enemy/enemy12.da");
	TextureGetBuffer("./dat/texture/enemy/enemy13.da");
	TextureGetBuffer("./dat/texture/enemy/enemy14.da");
	TextureGetBuffer("./dat/texture/enemy/enemy15.da");
	TextureGetBuffer("./dat/texture/enemy/enemy16.da");
	TextureGetBuffer("./dat/texture/enemy/enemy17.da");
	TextureGetBuffer("./dat/texture/enemy/enemy18.da");
	TextureGetBuffer("./dat/texture/enemy/enemy19.da");
	TextureGetBuffer("./dat/texture/enemy/enemy20.da");
	//ボス
	TextureGetBuffer("./dat/texture/enemy/boss1.da");
	TextureGetBuffer("./dat/texture/enemy/boss1_normal.da");
	TextureGetBuffer("./dat/texture/enemy/boss2.da");
	TextureGetBuffer("./dat/texture/enemy/boss2_normal.da");
	TextureGetBuffer("./dat/texture/enemy/boss3.da");
	TextureGetBuffer("./dat/texture/enemy/boss4.da");
	TextureGetBuffer("./dat/texture/enemy/boss4_normal.da");
	//ラストボス
	TextureGetBuffer("./dat/texture/enemy/lastboss.da");
	TextureGetBuffer("./dat/texture/enemy/lastboss_normal.da");
	//魔方陣通常
	TextureGetBuffer("./dat/texture/magic/side_magic.da");
	//魔方陣ボス
	TextureGetBuffer("./dat/texture/magic/boss_magic.da");
	TextureGetBuffer("./dat/texture/magic/particle.da");
	//回復ポイント
	TextureGetBuffer("./dat/texture/magic/recover.da");
	//エフェクト
	TextureGetBuffer("./dat/texture/effect/e_att.da");
	TextureGetBuffer("./dat/texture/effect/h_att.da");
	TextureGetBuffer("./dat/texture/effect/flame.da");
	TextureGetBuffer("./dat/texture/effect/healing.da");
	TextureGetBuffer("./dat/texture/effect/recov.da");
	//プレイヤー1テクスチャ
	TextureGetBuffer("./dat/texture/player/p1/brown_eye.da");
	TextureGetBuffer("./dat/texture/player/p1/classicshoes_texture_diffuse.da");
	TextureGetBuffer("./dat/texture/player/p1/classicshoes_texture_normals.da");
	TextureGetBuffer("./dat/texture/player/p1/eyebrow001.da");
	TextureGetBuffer("./dat/texture/player/p1/jacket01_diffuse.da");
	TextureGetBuffer("./dat/texture/player/p1/jacket01_normals.da");
	TextureGetBuffer("./dat/texture/player/p1/jeans01_black_diffuse.da");
	TextureGetBuffer("./dat/texture/player/p1/jeans01_normals.da");
	TextureGetBuffer("./dat/texture/player/p1/male01_diffuse_black.da");
	TextureGetBuffer("./dat/texture/player/p1/young_lightskinned_male_diffuse.da");
	//プレイヤー2テクスチャ
	TextureGetBuffer("./dat/texture/player/p2/brown_eye.da");
	TextureGetBuffer("./dat/texture/player/p2/diffuse_black.da");
	TextureGetBuffer("./dat/texture/player/p2/normals.da");
	TextureGetBuffer("./dat/texture/player/p2/eyebrow006.da");
	TextureGetBuffer("./dat/texture/player/p2/eyelashes03.da");
	TextureGetBuffer("./dat/texture/player/p2/shoes02_default.da");
	TextureGetBuffer("./dat/texture/player/p2/short01_black_diffuse.da");
	TextureGetBuffer("./dat/texture/player/p2/short01_normals.da");
	TextureGetBuffer("./dat/texture/player/p2/tshirt02_texture.da");
	TextureGetBuffer("./dat/texture/player/p2/tshirt02_normals.da");
	TextureGetBuffer("./dat/texture/player/p2/young_lightskinned_female_diffuse.da");
	//プレイヤー3テクスチャ
	TextureGetBuffer("./dat/texture/player/p3/brown_eye.da");
	TextureGetBuffer("./dat/texture/player/p3/classicshoes_texture_diffuse.da");
	TextureGetBuffer("./dat/texture/player/p3/classicshoes_texture_normals.da");
	TextureGetBuffer("./dat/texture/player/p3/jeans_basic_diffuse.da");
	TextureGetBuffer("./dat/texture/player/p3/jeans01_normals.da");
	TextureGetBuffer("./dat/texture/player/p3/young_darkskinned_male_diffuse.da");
	//プレイヤー4テクスチャ
	TextureGetBuffer("./dat/texture/player/p4/afro.da");
	TextureGetBuffer("./dat/texture/player/p4/brown_eye.da");
	TextureGetBuffer("./dat/texture/player/p4/eyebrow007.da");
	TextureGetBuffer("./dat/texture/player/p4/eyelashes03.da");
	TextureGetBuffer("./dat/texture/player/p4/shoes02_default.da");
	TextureGetBuffer("./dat/texture/player/p4/short01_black_diffuse.da");
	TextureGetBuffer("./dat/texture/player/p4/short01_normals.da");
	TextureGetBuffer("./dat/texture/player/p4/tshirt_texture_blue.da");
	TextureGetBuffer("./dat/texture/player/p4/tshirt_normal.da");
	TextureGetBuffer("./dat/texture/player/p4/young_darkskinned_female_diffuse.da");
}

void TextureBinaryLoader::TextureBinaryDecodeAll() {

	texNum = 0;
	//マップ0
	TextureBinaryDecode("./dat/texture/map/wall1.da");
	TextureBinaryDecode("./dat/texture/map/wall1Nor.da");
	TextureBinaryDecode("./dat/texture/map/ground1.da");
	TextureBinaryDecode("./dat/texture/map/ground1Nor.da");
	TextureBinaryDecode("./dat/texture/map/ceiling1.da");
	TextureBinaryDecode("./dat/texture/map/ceiling1Nor.da");
	//マップ1
	TextureBinaryDecode("./dat/texture/map/wall2.da");
	TextureBinaryDecode("./dat/texture/map/ground2.da");
	TextureBinaryDecode("./dat/texture/map/ground2Nor.da");
	TextureBinaryDecode("./dat/texture/map/ceiling2.da");
	TextureBinaryDecode("./dat/texture/map/wall2-1.da");
	TextureBinaryDecode("./dat/texture/map/wall2Nor.da");
	TextureBinaryDecode("./dat/texture/map/leaf.da");
	TextureBinaryDecode("./dat/texture/map/wood.da");
	//マップ2
	TextureBinaryDecode("./dat/texture/map/ceiling3_wall3.da");
	TextureBinaryDecode("./dat/texture/map/ceiling3_wall3Nor.da");
	TextureBinaryDecode("./dat/texture/map/ground3.da");
	TextureBinaryDecode("./dat/texture/map/ground3Nor.da");
	TextureBinaryDecode("./dat/texture/map/wave.da");
	TextureBinaryDecode("./dat/texture/map/waveNor.da");
	//マップ3
	TextureBinaryDecode("./dat/texture/map/ceiling4_ground4.da");
	TextureBinaryDecode("./dat/texture/map/ceiling4_ground4Nor.da");
	//マップ4
	TextureBinaryDecode("./dat/texture/map/wall5.da");
	TextureBinaryDecode("./dat/texture/map/wall5Nor.da");
	TextureBinaryDecode("./dat/texture/map/ground5.da");
	TextureBinaryDecode("./dat/texture/map/ground5Nor.da");
	TextureBinaryDecode("./dat/texture/map/ceiling5.da");
	TextureBinaryDecode("./dat/texture/map/ceiling5Nor.da");
	//通常敵
	TextureBinaryDecode("./dat/texture/enemy/enemy1.da");
	TextureBinaryDecode("./dat/texture/enemy/enemy2.da");
	TextureBinaryDecode("./dat/texture/enemy/enemy3.da");
	TextureBinaryDecode("./dat/texture/enemy/enemy4.da");
	TextureBinaryDecode("./dat/texture/enemy/enemy5.da");
	TextureBinaryDecode("./dat/texture/enemy/enemy6.da");
	TextureBinaryDecode("./dat/texture/enemy/enemy7.da");
	TextureBinaryDecode("./dat/texture/enemy/enemy8.da");
	TextureBinaryDecode("./dat/texture/enemy/enemy9.da");
	TextureBinaryDecode("./dat/texture/enemy/enemy10.da");
	TextureBinaryDecode("./dat/texture/enemy/enemy11.da");
	TextureBinaryDecode("./dat/texture/enemy/enemy12.da");
	TextureBinaryDecode("./dat/texture/enemy/enemy13.da");
	TextureBinaryDecode("./dat/texture/enemy/enemy14.da");
	TextureBinaryDecode("./dat/texture/enemy/enemy15.da");
	TextureBinaryDecode("./dat/texture/enemy/enemy16.da");
	TextureBinaryDecode("./dat/texture/enemy/enemy17.da");
	TextureBinaryDecode("./dat/texture/enemy/enemy18.da");
	TextureBinaryDecode("./dat/texture/enemy/enemy19.da");
	TextureBinaryDecode("./dat/texture/enemy/enemy20.da");
	//ボス
	TextureBinaryDecode("./dat/texture/enemy/boss1.da");
	TextureBinaryDecode("./dat/texture/enemy/boss1_normal.da");
	TextureBinaryDecode("./dat/texture/enemy/boss2.da");
	TextureBinaryDecode("./dat/texture/enemy/boss2_normal.da");
	TextureBinaryDecode("./dat/texture/enemy/boss3.da");
	TextureBinaryDecode("./dat/texture/enemy/boss4.da");
	TextureBinaryDecode("./dat/texture/enemy/boss4_normal.da");
	//ラストボス
	TextureBinaryDecode("./dat/texture/enemy/lastboss.da");
	TextureBinaryDecode("./dat/texture/enemy/lastboss_normal.da");
	//魔方陣通常
	TextureBinaryDecode("./dat/texture/magic/side_magic.da");
	//魔方陣ボス
	TextureBinaryDecode("./dat/texture/magic/boss_magic.da", TRUE);
	TextureBinaryDecode("./dat/texture/magic/particle.da");
	//回復ポイント
	TextureBinaryDecode("./dat/texture/magic/recover.da");
	//エフェクト
	TextureBinaryDecode("./dat/texture/effect/e_att.da");
	TextureBinaryDecode("./dat/texture/effect/h_att.da");
	TextureBinaryDecode("./dat/texture/effect/flame.da");
	TextureBinaryDecode("./dat/texture/effect/healing.da");
	TextureBinaryDecode("./dat/texture/effect/recov.da");
	//プレイヤー1テクスチャ
	TextureBinaryDecode("./dat/texture/player/p1/brown_eye.da");
	TextureBinaryDecode("./dat/texture/player/p1/classicshoes_texture_diffuse.da");
	TextureBinaryDecode("./dat/texture/player/p1/classicshoes_texture_normals.da");
	TextureBinaryDecode("./dat/texture/player/p1/eyebrow001.da");
	TextureBinaryDecode("./dat/texture/player/p1/jacket01_diffuse.da");
	TextureBinaryDecode("./dat/texture/player/p1/jacket01_normals.da");
	TextureBinaryDecode("./dat/texture/player/p1/jeans01_black_diffuse.da");
	TextureBinaryDecode("./dat/texture/player/p1/jeans01_normals.da");
	TextureBinaryDecode("./dat/texture/player/p1/male01_diffuse_black.da");
	TextureBinaryDecode("./dat/texture/player/p1/young_lightskinned_male_diffuse.da");
	//プレイヤー2テクスチャ
	TextureBinaryDecode("./dat/texture/player/p2/brown_eye.da");
	TextureBinaryDecode("./dat/texture/player/p2/diffuse_black.da");
	TextureBinaryDecode("./dat/texture/player/p2/normals.da");
	TextureBinaryDecode("./dat/texture/player/p2/eyebrow006.da");
	TextureBinaryDecode("./dat/texture/player/p2/eyelashes03.da");
	TextureBinaryDecode("./dat/texture/player/p2/shoes02_default.da");
	TextureBinaryDecode("./dat/texture/player/p2/short01_black_diffuse.da");
	TextureBinaryDecode("./dat/texture/player/p2/short01_normals.da");
	TextureBinaryDecode("./dat/texture/player/p2/tshirt02_texture.da");
	TextureBinaryDecode("./dat/texture/player/p2/tshirt02_normals.da");
	TextureBinaryDecode("./dat/texture/player/p2/young_lightskinned_female_diffuse.da");
	//プレイヤー3テクスチャ
	TextureBinaryDecode("./dat/texture/player/p3/brown_eye.da");
	TextureBinaryDecode("./dat/texture/player/p3/classicshoes_texture_diffuse.da");
	TextureBinaryDecode("./dat/texture/player/p3/classicshoes_texture_normals.da");
	TextureBinaryDecode("./dat/texture/player/p3/jeans_basic_diffuse.da");
	TextureBinaryDecode("./dat/texture/player/p3/jeans01_normals.da");
	TextureBinaryDecode("./dat/texture/player/p3/young_darkskinned_male_diffuse.da");
	//プレイヤー4テクスチャ
	TextureBinaryDecode("./dat/texture/player/p4/afro.da");
	TextureBinaryDecode("./dat/texture/player/p4/brown_eye.da");
	TextureBinaryDecode("./dat/texture/player/p4/eyebrow007.da");
	TextureBinaryDecode("./dat/texture/player/p4/eyelashes03.da");
	TextureBinaryDecode("./dat/texture/player/p4/shoes02_default.da");
	TextureBinaryDecode("./dat/texture/player/p4/short01_black_diffuse.da");
	TextureBinaryDecode("./dat/texture/player/p4/short01_normals.da");
	TextureBinaryDecode("./dat/texture/player/p4/tshirt_texture_blue.da");
	TextureBinaryDecode("./dat/texture/player/p4/tshirt_normal.da");
	TextureBinaryDecode("./dat/texture/player/p4/young_darkskinned_female_diffuse.da");

	Dx12Process::GetInstance()->SetTextureBinary(tex, texNum);
}

void TextureBinaryLoader::DeleteTextureBinary() {
	for (int i = 0; i < 150; i++) {
		if (tex[i].binary_size == 0)continue;
		free(tex[i].binary_ch);
		tex[i].binary_ch = NULL;
	}
}

void TextureBinaryLoader::DeleteTextureStruct() {
	ARR_DELETE(tex);
}
