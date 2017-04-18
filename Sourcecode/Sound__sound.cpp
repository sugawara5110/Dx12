//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@�@  Sound_�N���X                                        **//
//**                                 sound�֐�                                           **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include "Sound_.h"

Sound_::Sound_(){}

Sound_::Sound_(char *pass) {

	char *fname = NULL;

	fname = BinaryDecode(pass);

	BSTR bstr;
	BSTR_Convert(fname, &bstr);

	// Graph�𐶐�
	pMediaControl->RenderFile(bstr);
	SysFreeString(bstr);//bstr���

	pVideoWindow->put_AutoShow(OAFALSE);  //�����\�����Ȃ��悤�ɂ���

	// �Đ��J�n
	pMediaControl->Run();

	pBasicAudio->put_Volume(-10000);//���ʒ���(-10000 �` 0)

	//�X�g���[���̎��ԕ����擾(�ŏ���1��擾�����ok)
	pMediaPosition->get_Duration(&time2);

	remove(fname);//�t�@�C���폜�B�����폜����Ă�
}

void Sound_::sound(bool repeat, long volume){

	pBasicAudio->put_Volume(volume);//����ON

	if (repeat == FALSE){
		pMediaPosition->put_CurrentPosition(0);//�Đ��ʒu���X�^�[�g�ʒu�ɃZ�b�g
	}
	else{
		//�X�g���[���̍��v���ԕ�����Ƃ���A���݂̈ʒu���擾����B
		pMediaPosition->get_CurrentPosition(&time1);

		//���ʒu�ƏI���ʒu�������ꍇ�X�^�[�g�ʒu�ɃZ�b�g
		if (time1 == time2)pMediaPosition->put_CurrentPosition(0);
	}
}

void Sound_::soundoff(){
	pBasicAudio->put_Volume(-10000);//����OFF
}

void Sound_::soundloop(bool repeat, long volume, REFTIME start, REFTIME end){

	REFTIME s = time2 * start / 100.0;
	REFTIME e = time2 * end / 100.0;

	pBasicAudio->put_Volume(volume);//����ON

	if (repeat == FALSE){
		pMediaPosition->put_CurrentPosition(s);//�Đ��ʒu���X�^�[�g�ʒu�ɃZ�b�g
	}
	else{
		//�X�g���[���̍��v���ԕ�����Ƃ���A���݂̈ʒu���擾����B
		pMediaPosition->get_CurrentPosition(&time1);

		//���ʒu�ƏI���ʒu�������ꍇ�X�^�[�g�ʒu�ɃZ�b�g
		if (time1 >= e)pMediaPosition->put_CurrentPosition(s);
	}
}