//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         汎用関数, データ型.h                            **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef DsFunction_Header
#define DsFunction_Header

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h >

//バイナリ用バッファ取得
void DsBGetBuffer(char *bpass, char **binary, int *size);
//暗号化済みバイナリ復号
void DsBdecode(char *bpass, char **binary, int *size);

#endif