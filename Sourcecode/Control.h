//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Controlクラス                                   **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Control_Header
#define Class_Control_Header

#include "./Direct3DWrapper/Dx12Process.h"
#include "./Direct3DWrapper//DxText.h"
#include "Enum.h"

class Control {

private:
	static Control *co;
	Directionkey keydownhistory;
	Directionkey directionkey;
	Directionkey directionkey2[2];
	int keyOffCount;

	Control();

public:
	static Control *GetInstance();
	static void DeleteInstance();

	void Input(UINT msg, WPARAM wParam);
	Directionkey Direction();
	Directionkey Direction(bool f);
};

#endif