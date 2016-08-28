//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　　  Sound_クラス                                        **//
//**                                 sound関数                                           **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include "MovieSoundManager.h"

MovieSoundManager::Sound_::Sound_(){}

MovieSoundManager::Sound_::Sound_(int no){

	char *fname = NULL;

	//復号処理
	switch (no){
		//効果音
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
		//タイトル
	case 20:
		fname = BinaryDecode("./dat/movie/title.da");
		break;
		//ダンジョン
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
		//戦闘
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

	// Graphを生成
	pMediaControl->RenderFile(bstr);
	SysFreeString(bstr);//bstr解放

	pVideoWindow->put_AutoShow(OAFALSE);  //自動表示しないようにする

	// 再生開始
	pMediaControl->Run();

	pBasicAudio->put_Volume(-10000);//音量調整(-10000 ～ 0)

	//ストリームの時間幅を取得(最初に1回取得すればok)
	pMediaPosition->get_Duration(&time2);

	remove(fname);//ファイル削除、すぐに削除されないが使用中だとアクセスできないみたい
}

void MovieSoundManager::Sound_::sound(bool repeat, long volume){

	pBasicAudio->put_Volume(volume);//音声ON

	if (repeat == FALSE){
		pMediaPosition->put_CurrentPosition(0);//再生位置をスタート位置にセット
	}
	else{
		//ストリームの合計時間幅を基準とする、現在の位置を取得する。
		pMediaPosition->get_CurrentPosition(&time1);

		//現位置と終了位置が同じ場合スタート位置にセット
		if (time1 == time2)pMediaPosition->put_CurrentPosition(0);
	}
}

void MovieSoundManager::Sound_::soundoff(){
	pBasicAudio->put_Volume(-10000);//音声OFF
}

void MovieSoundManager::Sound_::soundloop(bool repeat, long volume, REFTIME start, REFTIME end){

	REFTIME s = time2 * start / 100.0;
	REFTIME e = time2 * end / 100.0;

	pBasicAudio->put_Volume(volume);//音声ON

	if (repeat == FALSE){
		pMediaPosition->put_CurrentPosition(s);//再生位置をスタート位置にセット
	}
	else{
		//ストリームの合計時間幅を基準とする、現在の位置を取得する。
		pMediaPosition->get_CurrentPosition(&time1);

		//現位置と終了位置が同じ場合スタート位置にセット
		if (time1 >= e)pMediaPosition->put_CurrentPosition(s);
	}
}