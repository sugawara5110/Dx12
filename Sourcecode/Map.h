//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Mapクラス                                       **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Map_Header
#define Class_Map_Header

#include "MovieSoundManager.h"
#include "Position.h"
#include "../../Common/Window/Control.h"
#include "../../Common/Direct3DWrapper/DxText.h"
#include "Hero.h"
#include <stdio.h>
#include <stdlib.h>
#include <TCHAR.h>
#include "Enum.h"

#define POS_CE  (posz * mxy.y * mxy.x + posy * mxy.x + posx)
#define POSY_U1 (posz * mxy.y * mxy.x + (posy + 1) * mxy.x + posx)
#define POSY_D1 (posz * mxy.y * mxy.x + (posy - 1) * mxy.x + posx)
#define POSX_U1 (posz * mxy.y * mxy.x + posy * mxy.x + (posx + 1))
#define POSX_D1 (posz * mxy.y * mxy.x + posy * mxy.x + (posx - 1))

class Map {

private:
	//地図
	static MapHistoryData maphis;
	static UINT* maphistory[5];
	UINT** mapdata;
	PolygonData2D MapHistory;

	static int map_no_s;    //マップナンバー
	int map_no;            //オブジェクト内部用マップナンバー
	int comNo;
	static MapStPos MPos;     //マップスタート位置
	static int boss_killed[5];//各ボス撃破履歴
	Dx12Process* dx;
	DxText* text;
	T_float tfloat;
	Position::mapxy mxy;
	bool menuSt = FALSE;

	struct LightPos {
		float x, y, z;
		float r, g, b, a;
		float range;
		float brightness;
		float attenuation;
		bool on_off;
	};
	LightPos* light;
	int lightcount;

	float cax1;//現在位置
	float cax2;//注視点
	float cay1;//現在位置
	float cay2;//注視点
	float src_theta;//現在の向いてる方向
	//GetCamDelayPos使用
	float outx, outy;
	bool GetCamPosInit = false;

	int posx;//マップフィルター位置
	int posy;
	int posz;
	int woodcount;     //木個数
	int blockcountA;   //壁用立方体の四角形の個数レンガ
	int blockcountB;   //壁用立方体の四角形の個数岩
	int blockcountC;   //壁用立方体の四角形の個数岩2
	int blockcountD;   //壁用立方体の四角形の個数溶岩
	int blockcountE;   //壁用立方体の四角形の個数コケ岩
	int squarecount; //壁用板個数
	int r_point_count;//回復ポイント個数
	int mo_count;    //動画テクスチャ,松明個数
	int f_wall_count;//動画テクスチャ炎壁個数
	int boss_count; //ボス出現ポイント個数
	int Elevator_count;//エレベーター個数
	float map_text_f;//マップ上のテキスト表示フラグ
	TCHAR m_tx[30];//表示内容
	bool recover_p_f;//リカバーポイント到達履歴
	float recovPosX, recovPosY;//リカバーポイント光源位置(複数にする場合要処理変更)
	bool boss_p_f;  //ボス出現ポイント到達履歴
	bool elevator_UP;
	bool elevator_DOWN;
	float elevator_step;

	struct OBJPosRandomValue {
		float x;
		float y;
		OBJPosRandomValue() {
			x = (rand() % 100) - 50.0f;
			y = (rand() % 100) - 50.0f;
		}
	};
	OBJPosRandomValue* wood, * wall1;

	Wave* wav;
	MeshData* mWood, * mountain;
	PolygonData* poWallA, * poWallB, * poWallC, * poWallD, * poWallE, * poWall1[3], * poF_Wall,
		* poGroundF, * poCeilingF, * poGroundM, * poCeilingM, * poGroundE, * poCeilingE,
		* poBackground, poRain, poRecover, poRecoverLine[12], poBoss, poElevator,
		* poDirectionLight;
	EmissiveObj_Pa* poMo;
	EmissiveObj_Po* poEXIT;
	Position::E_Pos e_pos[4];
	Position::H_Pos h_pos;
	Hero* he;//移動用
	std::unique_ptr<MaterialType[]> materialType;
	std::unique_ptr<ParameterDXR* []> pdx;

	void Debug();
	void Mapfilter_p(int k, int j, int i, int idx1, int idx2, int* cnt);
	void Mapfilter(Position::H_Pos* h_p);
	void MapupdateWave();
	void Mapupdate_Wood();
	void Mapdraw_Wood();
	void Mapupdate_Mountain();
	void Mapdraw_Mountain();
	void Mapcreate_Wall1(int i);
	void Mapupdate_Wall1();
	void Mapdraw_Wall1();
	void Mapcreate_Wall(int numB, PolygonData* pd, int no1, int no2, float height, float adjust, float adjust2);
	void Mapcreate_Ground(PolygonData* pd, int pcsx, int pcsy, float height, float adjust);
	void Mapcreate_Ceiling(PolygonData* pd, int pcsx, int pcsy, float height, float adjust);
	void Mapcreate_Background(float st, float end);
	void Mapupdate_Rain();
	void Mapdraw_Rain();
	void Mapcreate_Recover(int num);
	void Mapupdate_Recover();
	void Mapdraw_Recover();
	void Mapcreate_Ds(int num);
	void Mapupdate_Ds();
	void Mapdraw_Ds();
	void Mapcreate_BossPoint(int num);
	void Mapcreate_Elevator(int num);
	void Mapcreate_EXIT(float x, float y, float z, float xsize);
	void MapUpdateObj();
	void MapdrawObj();
	bool MoveUpCond(int Ind);
	bool MoveDownCond(int Ind);
	bool MoveCamCond(int Ind);
	Encount Move(MapState* mapstate, Directionkey direction);
	void MapText(TCHAR str[30]);
	bool ViewCulling(float obj_x, float obj_y, float obj_z);
	void HeroUpdate(bool mf);
	bool CollisionDetection(float in_y, float in_x, Directionkey dir);
	void GetCamDelayPos(Directionkey dir, float inX, float inY, float* outX, float* outY);
	void RecordMap();

	//四捨五入
	int rounding(int val, int digit_number) {
		int v = 5 * (int)pow(10.0, (double)digit_number - 1.0);
		val += v;
		return val -= (val % (int)pow(10.0, (double)digit_number));
	}
	//downNum切り下げ
	//upNum切り上げ
	int rounding2(int val, int digit_num, int downNum, int upNum) {
		int d = (int)pow(10.0, (double)digit_num - 1.0);
		int valT = val / d;
		int valT2 = valT / 10;
		if ((valT % 10) >= upNum)return (valT2 + 1) * d * 10;
		if ((valT % 10) <= downNum)return (valT2 - 1) * d * 10;
		return val;
	}

public:
	static int GetMapNo();
	static void SetMapNo(int no);
	static void SetBossKilled(int i, int f);
	static int* GetBossKilled();
	static int GetBossKilled(int map_no);
	static void SetMapHistory(MapHistoryData* maphis);
	static MapHistoryData* GetMapHistory();
	Map() {}
	Map(Position::H_Pos* h_p, Hero* hero);
	void SetVertex();
	void SetCommandList(int com_no);
	void CreateMap();
	Encount MapUpdate(MapState* mapstate, Directionkey direction, Encount encount, bool menu, bool title, bool ending);
	void SetMovie();
	void MapDraw();
	void StreamOutput();
	void StreamOutputAfterDraw();
	Position::E_Pos* Getposition(int p);
	Position::H_Pos* Getposition();
	void Setposition(Position::H_Pos* pos);
	Position::mapxy* Getmap();
	bool GetMenuState(int* cnt);
	~Map();
	ParameterDXR** getParameterDXR(int* numPara);
	void setPointLightNo();
};

#endif