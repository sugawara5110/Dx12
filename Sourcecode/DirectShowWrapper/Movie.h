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

	long nBufferSize;//バッファサイズサイズ
	BYTE *pBuffer;  //ピクセルデータバッファ
	int linesize;   //1ラインサイズ
	int xs, ys;    //画像サイズ
	int wid, hei; //格納時画像サイズ 
	int **m_pix; //受け渡し用ピクセルデータ

public:
	Movie() {}
	Movie(char *pass);
	int **GetFrame(int width, int height);
	virtual ~Movie();
};

#endif
