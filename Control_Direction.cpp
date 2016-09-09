//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　            Controlクラス                                   **//
//**                                     Direction関数                                   **//
//*****************************************************************************************//

#include "Dx12Process.h"
#include "Control.h"

Directionkey Control::directionkey;

Control::Control(){

	keydownhistory = NOTPRESS;
	directionkey = NOTPRESS;
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
	return directionkey;
}