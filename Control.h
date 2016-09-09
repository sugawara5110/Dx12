//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Controlクラス                                   **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Control_Header
#define Class_Control_Header

class Control{

private:
	Directionkey keydownhistory;

public:
	static Directionkey directionkey;
	
	Control();
	Directionkey Direction();
	Directionkey Direction(bool f);
};

#endif