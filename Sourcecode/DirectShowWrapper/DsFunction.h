//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@	         �ėp�֐�, �f�[�^�^.h                            **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef DsFunction_Header
#define DsFunction_Header

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h >

//�o�C�i���p�o�b�t�@�擾
void DsBGetBuffer(char *bpass, char **binary, int *size);
//�Í����ς݃o�C�i������
void DsBdecode(char *bpass, char **binary, int *size);

#endif