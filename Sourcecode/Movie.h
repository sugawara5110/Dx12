//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@           Movie�N���X                                      **//
//**                                                                                     **//
//*****************************************************************************************//
#ifndef Class_Movie_Header
#define Class_Movie_Header

#include "DsProcess.h"

class Movie :public DsProcess {

protected:
	VIDEOINFOHEADER *pVideoInfoHeader;//�\����,�r�f�I �C���[�W�̃r�b�g�}�b�v�ƐF���
	AM_MEDIA_TYPE am_media_type;     //���f�B�A �T���v���� ���W���[ �^�C�v���w�肷��O���[�o����ӎ��ʎq (GUID)

	long nBufferSize;//�o�b�t�@�T�C�Y�T�C�Y
	BYTE *pBuffer;  //�s�N�Z���f�[�^�o�b�t�@
	int linesize;   //1���C���T�C�Y
	int xs, ys;    //�摜�T�C�Y
	int wid, hei; //�i�[���摜�T�C�Y 
	int **m_pix; //�󂯓n���p�s�N�Z���f�[�^

public:
	Movie() {}
	Movie(char *pass);
	int **GetFrame(int width, int height);
	virtual ~Movie();
};

#endif
