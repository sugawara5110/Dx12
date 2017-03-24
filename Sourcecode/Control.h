//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Controlクラス                                   **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Control_Header
#define Class_Control_Header

#include "Dx12Process.h"

class Control{

private:
	static Control *co;
	Directionkey keydownhistory;
	Directionkey directionkey;
	Directionkey directionkey2;

	Control();

public:
	static Control *GetInstance();
	static void DeleteInstance();

	void Input(UINT msg, WPARAM wParam);
	Directionkey Direction();
	Directionkey Direction(bool f);
};

#endif