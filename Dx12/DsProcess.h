//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@          DsProcess�N���X                                   **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_DsProcess_Header
#define Class_DsProcess_Header

#define _CRT_SECURE_NO_WARNINGS
#include "Dx12Process.h"
#include <dshow.h>
#include <qedit.h>

#pragma comment(lib,"strmiids.lib")
#pragma comment(lib,"quartz.lib")

class DsProcess{

protected:
	IGraphBuilder *pGraphBuilder;      //�C���^�[�t�F�[�X,�O���t�ւ̃t�B���^�̒ǉ��A2 �̃s���̐ڑ���
	IBaseFilter *pSampleGrabberFilter;//�C���^�[�t�F�[�X,�t�B���^�𐧌�(Movie�g�p��)
	ISampleGrabber *pSampleGrabber;  //�C���^�[�t�F�[�X,�t�B���^ �O���t����ʂ�X�̃��f�B�A �T���v�����擾(Movie�g�p��)
	IVideoWindow *pVideoWindow;     //�C���^�[�t�F�[�X,�r�f�I �E�B���h�E�̃v���p�e�B��ݒ�
	IMediaControl *pMediaControl;  //�C���^�[�t�F�[�X,�t�B���^ �O���t��ʂ�f�[�^ �t���[�𐧌�
	IMediaPosition *pMediaPosition;  //�C���^�[�t�F�[�X,�X�g���[�����̈ʒu���V�[�N
	REFTIME time2;                   //����̑S�Đ�����
	REFTIME time1;                  //����̌��Đ��ʒu
	IBasicAudio *pBasicAudio;      //�C���^�[�t�F�[�X,�I�[�f�B�I �X�g���[���̃{�����[���ƃo�����X�𐧌�

	DsProcess();
	void BSTR_Convert(char *fname, BSTR *bstr);
	char *BinaryDecode(char *bpass);

public:
	virtual ~DsProcess();
};

#endif