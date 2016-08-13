//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@    Dx12TestMain                                        **//
//**                                                                                     **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Dx12Process.h"

#pragma comment(lib,"winmm.lib")

//-------------------------------------------------------------
// ���b�Z�[�W�����p�R�[���o�b�N�֐�
// ����
//		hWnd	: �E�B���h�E�n���h��
//		msg		: ���b�Z�[�W
//		wParam	: ���b�Z�[�W�̍ŏ��̃p�����[�^(�����ꂽ�L�[�̓��蓙�Ɏg�p)
//		lParam	: ���b�Z�[�W��2�Ԗڂ̃p�����[�^
// �߂�l
//		���b�Z�[�W��������
//-------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch (msg) {
	case WM_CLOSE:			//�~�{�^��
		PostQuitMessage(0);//�A�v���P�[�V�����I������,���b�Z�[�W�L���[��WM_QUIT���|�X�g
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);//�A�v���P�[�V�������������Ȃ��E�B���h�E���b�Z�[�W�ɑ΂��Ă̊���̏������s
}

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

	srand((unsigned)time(NULL));
	TCHAR clsName[] = L"3DRPG";// �E�B���h�E�N���X��

	HWND hWnd;//�E�B���h�E�n���h��
	MSG msg; //���b�Z�[�W
	//�E�C���h�E�N���X�̏�����
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX); //���̍\���̂̃T�C�Y
	wcex.style = NULL;               //�E�C���h�E�X�^�C��(default)
	wcex.lpfnWndProc = WindowProc;  //���b�Z�[�W�����֐��̓o�^
	wcex.cbClsExtra = 0;       //�ʏ��0	                
	wcex.cbWndExtra = 0;      //�ʏ��0					
	wcex.hInstance = hInstance; //�C���X�^���X�ւ̃n���h��				
	wcex.hIcon = NULL;         //�A�C�R�� (����)				
	wcex.hCursor = NULL;      //�J�[�\���̌`				
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); //�w�i				
	wcex.lpszMenuName = NULL;                       //���j���[����				
	wcex.lpszClassName = (TCHAR*)clsName;          //�N���X��               
	wcex.hIconSm = NULL;                          //���A�C�R��			   

	//�E�C���h�E�N���X�̓o�^(RegisterClassEx�֐�)
	if (!RegisterClassEx(&wcex))return -1;

	//�E�C���h�E�����E�C���h�E���[�h
	if (!(hWnd = CreateWindow(clsName, //�o�^�N���X��
		clsName,                      //�E�C���h�E��
		WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE,//�E�C���h�E�X�^�C��
		CW_USEDEFAULT, //�E�C���h�E���ʒu
		0,            //�E�C���h�E�c�ʒu
		800,             //�E�C���h�E��
		600,            //�E�C���h�E����
		NULL,          //�e�E�C���h�E�n���h��
		NULL,         //���j���[,�q�E�C���h�E�n���h��
		hInstance,   //�A�v���P�[�V�����C���X�^���X�n���h��
		NULL)))     //�E�C���h�E�쐬�f�[�^
		return -1;

	// �E�B���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);
	// WM_PAINT���Ă΂�Ȃ��悤�ɂ���
	ValidateRect(hWnd, 0);

	//Directx������
	Dx12Process::InstanceCreate();
	Dx12Process *dx = Dx12Process::GetInstance();

	if (!dx->Initialize(hWnd)) return -1;

	PolygonData a[1];
	for (int i = 0; i < 1; i++) {
		a[i].GetVBarray(CONTROL_POINT, true, 1);
		//����
		a[i].SetVertex(0, 0,
			-10.0f, -10.0f, -10.0f,
			0.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			0.0f, 0.0f);

		//����
		a[i].SetVertex(1, 1,
			-10.0f, 10.0f, -10.0f,
			0.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			0.0f, 1.0f);

		//�E��
		a[i].SetVertex(2, 2,
			10.0f, 10.0f, -10.0f,
			0.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f);

		//�E��
		a[i].SetVertex(3, 3,
			10.0f, -10.0f, -10.0f,
			0.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f);
		a[i].Create();
	}
	float theta = 0.0f;
	while (1){//�A�v�����s�����[�v
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {	// PostQuitMessage()���Ă΂ꂽ(�~�����ꂽ)
				break;	//���[�v�̏I��
			}
			else {
				// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`WindowProc�Ăяo��
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		//MainLoop
		dx->Sclear();
		dx->Cameraset(0.0f, 0.0f, 0.5f, 0.0f, 40.0f, 0.0f);
		dx->P_ShadowBright(0.3f);
		dx->PointLightPosSet(0, 0, 40, 30, 1, 1, 1, 1, 100,
			8.0f, 2, true);//0:���_, 1:���X�{�X, 2:�o�����, 3456:�퓬
		for (int i = 0; i < 1; i++)
			a[i].Draw(i * 10, i * 10, i * 10, 0, 0, 0, (int)(theta+=0.1f)%360, 1.0f);
		dx->Drawscreen();
		T_float::GetTime(hWnd);
	}

	Dx12Process::DeleteInstance();
	return (int)msg.wParam;
}
