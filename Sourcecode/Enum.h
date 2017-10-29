//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	           列挙型                                        **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Enum_Header
#define Class_Enum_Header

enum Directionkey {
	TWOPRESS,
	NOTPRESS,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	ENTER,
	CANCEL,
	CAMPOS //視点座標用
};

enum Result {
	WIN,
	IN_BATTLE,
	DIE
};

enum Action {
	NORMAL,
	MOVE,
	ATTACK,
	MAGIC,
	DAMAGE,
	LOST,
	RECOVER
};

enum Act_fin_flg {
	AT_FIN,
	LOST_FIN,
	NOT_FIN
};

enum MenuSelect {
	MAIN_M,
	AT_M,
	MAG_AT_M,
	MAG_RE_M
};

enum MapMenuSelect {
	MAIN,
	ST,
	RE,
	PP,
	CAN
};

enum CommandSelect {
	NOSELECT,
	OTHER,
	ATT,
	MAG,
	ESCAPE,
	CANCEL_M
};

enum MagicSelect {
	FLAME,
	HEAL,
	RECOV,
	NOSEL
};

enum Encount {
	NOENCOUNT,
	SIDE,
	BOSS
};

enum MapState {
	NORMAL_MAP,
	RECOV_MAP,
	CHANGE_MAP
};

enum MapStPos {
	POS_ST,
	POS_EN1,
	POS_EN2
};

enum TemplateType {
	E_ATT,
	H_ATT
};

#endif
