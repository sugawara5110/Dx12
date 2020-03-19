//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@    TextureBinaryLoader�N���X                               **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_TextureBinaryLoader_Header
#define Class_TextureBinaryLoader_Header

#include "../../Common/Direct3DWrapper/Dx12Process.h"
#include "../../Common/SearchFile\SearchFile.h"
#include "../../Common/TextureLoader/TextureLoader.h"

class TextureBinaryLoader {

private:
	static Texture* tex;
	static SearchFile* sf;
	static int texNum;

	TextureBinaryLoader() {}
	static void TextureGetBuffer(char* Bpass);
	static void TextureBinaryDecode(char* Bpass);
	static void TextureBinaryDecode(char* Bpass, bool UpKeep);//�Í����ς݉摜�o�C�i���f�R�[�h
	static void InputTextureFileName(char* Bpass);//�t�@�C�����ړǂݍ���

public:
	static void TextureGetBufferAll();
	static void TextureBinaryDecodeAll();
	static int getTexNum();
	static Texture* getTexture();
	static void DeleteTextureBinary();
	static void DeleteTextureStruct();
};

#endif
