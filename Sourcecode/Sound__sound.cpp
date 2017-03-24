//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@�@  Sound_�N���X                                        **//
//**                                 sound�֐�                                           **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include "MovieSoundManager.h"

MovieSoundManager::Sound_::Sound_(){}

MovieSoundManager::Sound_::Sound_(int no){

	char *fname = NULL;

	//��������
	switch (no){
		//���ʉ�
	case 0:
		fname = BinaryDecode("./dat/movie/att.da");
		break;
	case 1:
		fname = BinaryDecode("./dat/movie/flame.da");
		break;
	case 2:
		fname = BinaryDecode("./dat/movie/heal.da");
		break;
	case 3:
		fname = BinaryDecode("./dat/movie/magic.da");
		break;
	case 4:
		fname = BinaryDecode("./dat/movie/select.da");
		break;
	case 5:
		fname = BinaryDecode("./dat/movie/enter.da");
		break;
		//�^�C�g��
	case 20:
		fname = BinaryDecode("./dat/movie/title.da");
		break;
		//�_���W����
	case 21:
		fname = BinaryDecode("./dat/movie/dungeon0.da");
		break;
	case 22:
		fname = BinaryDecode("./dat/movie/dungeon1.da");
		break;
	case 23:
		fname = BinaryDecode("./dat/movie/rain.da");
		break;
	case 24:
		fname = BinaryDecode("./dat/movie/dungeon2.da");
		break;
	case 25:
		fname = BinaryDecode("./dat/movie/dungeon3.da");
		break;
	case 26:
		fname = BinaryDecode("./dat/movie/dungeon4.da");
		break;
		//�퓬
	case 30:
		fname = BinaryDecode("./dat/movie/die.da");
		break;
	case 31:
		fname = BinaryDecode("./dat/movie/side_enemy.da");
		break;
	case 32:
		fname = BinaryDecode("./dat/movie/boss_enemy.da");
		break;
	case 33:
		fname = BinaryDecode("./dat/movie/boss_enemy2.da");
		break;
	case 34:
		fname = BinaryDecode("./dat/movie/lastboss.da");
		break;
	case 35:
		fname = BinaryDecode("./dat/movie/ending.da");
		break;
	case 36:
		fname = BinaryDecode("./dat/movie/bosslost.da");
		break;
	}

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

	remove(fname);//�t�@�C���폜�A�����ɍ폜����Ȃ����g�p�����ƃA�N�Z�X�ł��Ȃ��݂���
}

void MovieSoundManager::Sound_::sound(bool repeat, long volume){

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

void MovieSoundManager::Sound_::soundoff(){
	pBasicAudio->put_Volume(-10000);//����OFF
}

void MovieSoundManager::Sound_::soundloop(bool repeat, long volume, REFTIME start, REFTIME end){

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