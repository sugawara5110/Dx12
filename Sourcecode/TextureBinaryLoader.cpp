//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　    TextureBinaryLoaderクラス                               **//
//**                                                                                     **//
//*****************************************************************************************//

#include "TextureBinaryLoader.h"
#include "Decode.h"

Texture *TextureBinaryLoader::tex = NULL;
int TextureBinaryLoader::texNum = 0;
SearchFile *TextureBinaryLoader::sf = nullptr;

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

void TextureBinaryLoader::InputTextureFileName(char *Bpass) {
	tex[texNum].texName = Bpass;
	texNum++;
}

void TextureBinaryLoader::SetUpKeep(char *Bpass, bool UpKeep) {
	tex[Dx12Process::GetInstance()->GetTexNumber(Dx12Process::GetInstance()->GetNameFromPass(Bpass))].UpKeep = UpKeep;
}

void TextureBinaryLoader::TextureGetBufferAll() {

	tex = new Texture[150];

	texNum = 0;

	sf = new SearchFile(8);
	char **str = new char*[1];
	str[0] = "da";
	sf->Search(L"./dat/texture/map/*", 0, str, 1);
	sf->Search(L"./dat/texture/enemy/*", 1, str, 1);
	sf->Search(L"./dat/texture/magic/*", 2, str, 1);
	sf->Search(L"./dat/texture/effect/*", 3, str, 1);
	sf->Search(L"./dat/texture/player/p1/*", 4, str, 1);
	sf->Search(L"./dat/texture/player/p2/*", 5, str, 1);
	sf->Search(L"./dat/texture/player/p3/*", 6, str, 1);
	sf->Search(L"./dat/texture/player/p4/*", 7, str, 1);
	delete[]str;
	str = nullptr;

	for (UINT k = 0; k < 8; k++) {
		for (UINT j = 0; j < sf->GetFileNum(k); j++) {
			TextureGetBuffer(sf->GetFileName(k, j));
		}
	}
}

void TextureBinaryLoader::TextureBinaryDecodeAll() {

	texNum = 0;
	for (UINT k = 0; k < 8; k++) {
		for (UINT j = 0; j < sf->GetFileNum(k); j++) {
			TextureBinaryDecode(sf->GetFileName(k, j));
		}
	}

	Dx12Process::GetInstance()->SetTextureBinary(tex, texNum);
	SetUpKeep("dat/texture/magic/boss_magic.da", TRUE);
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
	S_DELETE(sf);
}
