//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@    3DRPG_Main                                          **//
//**                                                                                     **//
//*****************************************************************************************//

#include "Main.h"

//-------------------------------------------------------------
// �A�v���P�[�V�����̃G���g���|�C���g
// ����
//		hInstance     : ���݂̃C���X�^���X�̃n���h��
//		hPrevInstance : �ȑO�̃C���X�^���X�̃n���h��(win16�ł̕ʃC���X�^���X�m�F�pwin32�ł͏��NULL)
//		lpCmdLine	  : �R�}���h���C���p�����[�^
//		nCmdShow	  : �E�B���h�E�̕\�����
// �߂�l
//		����������0�ȊO�̒l
//-------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	Main main;
	main.Init(hInstance, nCmdShow);
	main.Loop();

	return (int)main.msg.wParam;
}