//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　    TextureBinaryLoaderクラス                               **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_TextureBinaryLoader_Header
#define Class_TextureBinaryLoader_Header

#include "Dx12Process.h"

class TextureBinaryLoader {

private:
	TextureBinaryLoader(){}

public:
	static void TextureGetBufferAll();
	static void TextureBinaryDecodeAll();
};

#endif
