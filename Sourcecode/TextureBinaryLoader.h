//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@    TextureBinaryLoader�N���X                               **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_TextureBinaryLoader_Header
#define Class_TextureBinaryLoader_Header

#include "./Direct3DWrapper/Dx12Process.h"

class TextureBinaryLoader {

private:
	static Texture *tex;
	static int texNum;

	TextureBinaryLoader() {}
	static void TextureGetBuffer(char *Bpass);
	static void TextureBinaryDecode(char *Bpass);
	static void TextureBinaryDecode(char *Bpass, bool UpKeep);//�Í����ς݉摜�o�C�i���f�R�[�h

public:
	static void TextureGetBufferAll();
	static void TextureBinaryDecodeAll();
	static void DeleteTextureBinary();
	static void DeleteTextureStruct();
};

#endif
