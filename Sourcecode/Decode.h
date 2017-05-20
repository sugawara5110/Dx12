//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Decode                                          **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Decode_Header
#define Decode_Header

//バイナリ用バッファ取得
void BinaryGetBuffer(char *bpass, char **binary, int *size);
//暗号化済みバイナリ復号
void BinaryDecode(char *bpass, char **binary, int *size);

#endif
