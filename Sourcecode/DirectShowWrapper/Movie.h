//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　           Movieクラス                                      **//
//**                                                                                     **//
//*****************************************************************************************//
#ifndef Class_Movie_Header
#define Class_Movie_Header

#include "DsProcess.h"

class Movie :public DsProcess {

protected:
	VIDEOINFOHEADER *pVideoInfoHeader;//構造体,ビデオ イメージのビットマップと色情報
	AM_MEDIA_TYPE am_media_type;     //メディア サンプルの メジャー タイプを指定するグローバル一意識別子 (GUID)

	long nBufferSize;//バッファサイズ
	BYTE *pBuffer;  //ピクセルデータバッファ
	int linesize;   //1ラインサイズ
	int xs, ys;    //画像サイズ
	int wid, hei; //格納時画像サイズ 
	UINT **m_pix; //受け渡し用ピクセルデータ(1要素1ピクセル)

public:
	Movie() {}
	Movie(char *fileName);//デコード後のファイルネーム
	UINT **GetFrame(int width, int height);
	virtual ~Movie();
};

#endif
