//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　          DsProcessクラス                                   **//
//**                                   DsProcess関数                                     **//
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

	// COMを初期化
	CoInitialize(NULL);

	// FilterGraphを生成
	CoCreateInstance(CLSID_FilterGraph,
		NULL,
		CLSCTX_INPROC,
		IID_IGraphBuilder,
		(LPVOID *)&pGraphBuilder);

	// MediaControlインターフェース取得
	pGraphBuilder->QueryInterface(IID_IMediaControl,
		(LPVOID *)&pMediaControl);

	// IVideoWindowインターフェースを取得します
	pGraphBuilder->QueryInterface(IID_IVideoWindow, (void **)&pVideoWindow);

	//MediaPositionインターフェース取得
	pGraphBuilder->QueryInterface(IID_IMediaPosition,
		(LPVOID *)&pMediaPosition);

	//BasicAudioインターフェース取得
	pGraphBuilder->QueryInterface(IID_IBasicAudio,
		(LPVOID *)&pBasicAudio);
}

void DsProcess::BSTR_Convert(char *fname, BSTR *bstr){
	LPSTR lstr = fname;
	//BSTRに必要なバッファサイズを求める(directshow用)
	int bstrlen = (int)MultiByteToWideChar(
		CP_ACP,		 // コードページ ANSI コードページ
		0,			// 文字の種類を指定するフラグ
		lstr,	   // マップ元文字列のアドレス
		strlen(lstr), // マップ元文字列のバイト数
		NULL,		 // マップ先ワイド文字列を入れるバッファのアドレス
		0			// バッファのサイズ
		);

	//バッファを確保する
	*bstr = SysAllocStringLen(NULL, bstrlen);

	//BSTRに複製
	MultiByteToWideChar(
		CP_ACP,
		0,
		lstr,
		strlen(lstr),
		*bstr,      //RenderFileの引数に使う
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
	//解放の順番の関係でここに記述
	RELEASE(pSampleGrabber);
	RELEASE(pSampleGrabberFilter);
	RELEASE(pGraphBuilder);

	// COM終了
	CoUninitialize();
}
