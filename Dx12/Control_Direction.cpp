//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　            Controlクラス                                   **//
//**                                     Direction関数                                   **//
//*****************************************************************************************//

#include "Control.h"

Control *Control::co = NULL;

Control *Control::GetInstance() {
	if (co == NULL)co = new Control();
	return co;
}

Control::Control(){

	keydownhistory = NOTPRESS;
	directionkey = NOTPRESS;
}

void Control::DeleteInstance() {
	delete co;
}

void Control::Input(UINT msg, WPARAM wParam) {
	switch (msg) {
	case WM_CLOSE:			//×ボタン
		PostQuitMessage(0);//アプリケーション終了処理,メッセージキューにWM_QUITをポスト
		break;
	case WM_KEYDOWN:
		switch ((CHAR)wParam) {
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		case VK_LEFT:
			directionkey = LEFT;
			break;
		case VK_RIGHT:
			directionkey = RIGHT;
			break;
		case VK_UP:
			directionkey = UP;
			break;
		case VK_DOWN:
			directionkey = DOWN;
			break;
		case VK_CONTROL:
			directionkey = ENTER;
			break;
		case VK_DELETE:
			directionkey = CANCEL;
			break;
		default:
			directionkey = NOTPRESS;
			break;
		}
		break;
	default:
		directionkey = NOTPRESS;
		break;
	}
	directionkey2 = directionkey;
}

Directionkey Control::Direction(){

	switch (directionkey){

	case LEFT:
		if (keydownhistory == LEFT)directionkey = TWOPRESS;
		else keydownhistory = LEFT;
		break;
	case RIGHT:
		if (keydownhistory == RIGHT)directionkey = TWOPRESS;
		else keydownhistory = RIGHT;
		break;
	case UP:
		if (keydownhistory == UP)directionkey = TWOPRESS;
		else keydownhistory = UP;
		break;
	case DOWN:
		if (keydownhistory == DOWN)directionkey = TWOPRESS;
		else keydownhistory = DOWN;
		break;
	case ENTER:
		if (keydownhistory == ENTER)directionkey = TWOPRESS;
		else keydownhistory = ENTER;
		break;
	case CANCEL:
		if (keydownhistory == CANCEL)directionkey = TWOPRESS;
		else keydownhistory = CANCEL;
		break;
	case NOTPRESS:
		keydownhistory = NOTPRESS;
		break;
	}
	return directionkey;
}

Directionkey Control::Direction(bool f){
	return directionkey2;
}