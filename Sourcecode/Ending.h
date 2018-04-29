//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　　      Endingクラス                                    **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Ending_Header
#define Class_Ending_Header

#include "MovieSoundManager.h"
#include "../../Common/Direct3DWrapper/DxText.h"

class Ending{

private:
	Dx12Process *dx;
	DxText *text;
	float str_y;

public:
	Ending();
	void StaffRoll();
	~Ending();
};

#endif