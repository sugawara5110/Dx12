//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@    MovieSoundManager�N���X                                 **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_MovieSoundManager_Header
#define Class_MovieSoundManager_Header

#include "DsProcess.h"

class MovieSoundManager{

private:
	class Movie :public DsProcess{

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
		Movie();
		Movie(int no);
		int **GetFrame(int width, int height);
		virtual ~Movie();
	};

	class Sound_ :public DsProcess{

	public:
		Sound_();
		Sound_(int no);
		void sound(bool repeat, long volume);//volume -10000�`0
		void soundoff();
		void soundloop(bool repeat, long volume, REFTIME start, REFTIME end);
	};

	static Movie *mo;
	static Movie *f_wall;
	static Sound_ *dungeon_so[5];
	static Sound_ *rain_so;
	static Sound_ *enemy_so[2];
	static Sound_ *title_so;
	static Sound_ *die_so;
	static Sound_ *att_so, *flame_so, *heal_so, *magic_so;
	static Sound_ *select_so, *enter_so;
	static Sound_ *ending_so;
	static Sound_ *bosslost_so;
	static int map_n;//dungeon_so[5]�Y����
	
	MovieSoundManager();

public:
	static void ObjInit();
	static void ObjCreate_title();
	static void ObjDelete_title();
	static void ObjCreate_map(int map_no);
	static void ObjChange_map();
	static void ObjDelete_map();
	static void ObjCreate_battle(int n);
	static void ObjDelete_battle();
	static void ObjCreate_ending();
	static void ObjDelete_ending();
	static void ObjDelete();
	static int **Torch_GetFrame(int width, int height);
	static int **FireWall_GetFrame(int width, int height);
	static void Dungeon_sound(bool repeat, int map);
	static void Dungeon_soundoff(int map);
	static void Rain_sound(bool repeat);
	static void Rain_soundoff();
	static void Enemy_sound(bool repeat);
	static void Enemy_soundoff();
	static void Bosslost_sound(bool repeat);
	static void Title_sound(bool repeat);
	static void Title_soundoff();
	static void Ending_sound(bool repeat);
	static void Die_sound(bool repeat);
	static void Die_soundoff();
	static void Att_sound(bool repeat);
	static void Flame_sound(bool repeat);
	static void Heal_sound(bool repeat);
	static void Magic_sound(bool repeat);
	static void Select_sound(bool repeat);
	static void Enter_sound(bool repeat);
};

#endif