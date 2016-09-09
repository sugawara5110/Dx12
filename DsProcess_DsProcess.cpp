//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@          DsProcess�N���X                                   **//
//**                                   DsProcess�֐�                                     **//
//*****************************************************************************************//

#include "DsProcess.h"

DsProcess::DsProcess(){

	pGraphBuilder = NULL;
	pSampleGrabberFilter = NULL;
	pSampleGrabber = NULL;
	pVideoWindow = NULL;
	pMediaControl = NULL;
	pMediaPosition = NULL;
	pBasicAudio = NULL;

	// COM��������
	CoInitialize(NULL);

	// FilterGraph�𐶐�
	CoCreateInstance(CLSID_FilterGraph,
		NULL,
		CLSCTX_INPROC,
		IID_IGraphBuilder,
		(LPVOID *)&pGraphBuilder);

	// MediaControl�C���^�[�t�F�[�X�擾
	pGraphBuilder->QueryInterface(IID_IMediaControl,
		(LPVOID *)&pMediaControl);

	// IVideoWindow�C���^�[�t�F�[�X���擾���܂�
	pGraphBuilder->QueryInterface(IID_IVideoWindow, (void **)&pVideoWindow);

	//MediaPosition�C���^�[�t�F�[�X�擾
	pGraphBuilder->QueryInterface(IID_IMediaPosition,
		(LPVOID *)&pMediaPosition);

	//BasicAudio�C���^�[�t�F�[�X�擾
	pGraphBuilder->QueryInterface(IID_IBasicAudio,
		(LPVOID *)&pBasicAudio);
}

void DsProcess::BSTR_Convert(char *fname, BSTR *bstr){
	LPSTR lstr = fname;
	//BSTR�ɕK�v�ȃo�b�t�@�T�C�Y�����߂�(directshow�p)
	int bstrlen = (int)MultiByteToWideChar(
		CP_ACP,		 // �R�[�h�y�[�W ANSI �R�[�h�y�[�W
		0,			// �����̎�ނ��w�肷��t���O
		lstr,	   // �}�b�v��������̃A�h���X
		strlen(lstr), // �}�b�v��������̃o�C�g��
		NULL,		 // �}�b�v�惏�C�h�����������o�b�t�@�̃A�h���X
		0			// �o�b�t�@�̃T�C�Y
		);

	//�o�b�t�@���m�ۂ���
	*bstr = SysAllocStringLen(NULL, bstrlen);

	//BSTR�ɕ���
	MultiByteToWideChar(
		CP_ACP,
		0,
		lstr,
		strlen(lstr),
		*bstr,      //RenderFile�̈����Ɏg��
		bstrlen
		);
}

char *DsProcess::BinaryDecode(char *bpass){

	static char decfname[64];
	int size = 0;
	char *binary = NULL;

	int i1 = 0;
	do{
		strncpy(&decfname[i1], &bpass[i1], 1);
		i1++;
	} while (bpass[i1] != '.');
	strncpy(&decfname[i1++], ".", 1);
	strncpy(&decfname[i1++], "d", 1);
	strncpy(&decfname[i1++], "a", 1);
	strncpy(&decfname[i1++], "1", 1);
	strncpy(&decfname[i1], "\0", 1);

	Bdecode(bpass, &binary, &size);

	FILE *fp2 = fopen(decfname, "wb");

	for (int i = 0; i < size + 1; i++){
		fputc(binary[i], fp2);
	}

	fclose(fp2);
	free(binary);
	binary = NULL;

	return decfname;
}

DsProcess::~DsProcess(){

	RELEASE(pBasicAudio);
	RELEASE(pMediaPosition);
	RELEASE(pMediaControl);
	RELEASE(pVideoWindow);
	//����̏��Ԃ̊֌W�ł����ɋL�q
	RELEASE(pSampleGrabber);
	RELEASE(pSampleGrabberFilter);
	RELEASE(pGraphBuilder);

	// COM�I��
	CoUninitialize();
}
