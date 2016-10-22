//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　           DxStruct.h                                       **//
//**                                   Directx用構造体                                   **//
//*****************************************************************************************//

#ifndef Class_DxStruct_Header
#define Class_DxStruct_Header

#include "Function.h"

#define LIGHT_PCS 150
#define LIGHT_PCS_init 7
#define INSTANCE_PCS_3D 150
#define INSTANCE_PCS_2D 80

//頂点
struct Vertex {
	VECTOR3 Pos;       //位置
	VECTOR3 normal;   //法線
	VECTOR4 color;   //色
	VECTOR2 tex;    //テクスチャ座標
};

struct MY_VERTEX_MESH {
	VECTOR3 Pos;
	VECTOR3 normal;
	VECTOR2 tex;
};

struct MY_VERTEX2 {
	VECTOR3 Pos;       
	VECTOR4 color;
	VECTOR2 tex;    
};

//シェーダー受け渡し用バッファ3D
struct CONSTANT_BUFFER {

	MATRIX World[INSTANCE_PCS_3D];
	MATRIX WVP[INSTANCE_PCS_3D];
	VECTOR4 C_Pos;       //視点位置
	VECTOR4 AddObjColor;//オブジェクトの色変化用

	//ポイントライト
	VECTOR4 pLightPos[LIGHT_PCS];
	VECTOR4 pLightColor[LIGHT_PCS];
	VECTOR4 pLightst[LIGHT_PCS];
	VECTOR4 pShadowLow_Lpcs;//影の下限値, ライト個数, ライト有無

	//ディレクショナルライト
	VECTOR4 dDirection;
	VECTOR4 dLightColor;
	VECTOR4 dLightst;

	//フォグ
	VECTOR4  FogAmo_Density; //フォグ量x, フォグの密度y, onoffz
	VECTOR4  FogColor;   //フォグの色

	//ディスプレイトメントマッピングの起伏量x(0入力の場合デフォルト値3になる)
	VECTOR4  DispAmount;

	//UV座標移動用
	VECTOR4 pXpYmXmY;
};

struct CONSTANT_BUFFER_MESH {
	VECTOR4 vDiffuse;//ディフューズ色
	VECTOR4 tex_f;   //テクスチャ有り無し
};

struct CONSTANT_BUFFER2D {
	VECTOR4 Pos[INSTANCE_PCS_2D];
	VECTOR4 Color[INSTANCE_PCS_2D];
	VECTOR4 sizeXY[INSTANCE_PCS_2D];
};

struct CONSTANT_BUFFER_P {
	MATRIX  WV;
	MATRIX  Proj;
	VECTOR4 size;//xパーティクル大きさ, yパーティクル初期化フラグ, zスピード
};
struct PartPos {
	VECTOR3 CurrentPos; //描画に使う
	VECTOR3 PosSt;     //開始位置
	VECTOR3 PosEnd;   //終了位置
	VECTOR4 Col;
};

//ポイントライト
struct PointLight {
	VECTOR4 LightPos[LIGHT_PCS];   //光源
	VECTOR4 LightColor[LIGHT_PCS];//色
	VECTOR4 Lightst[LIGHT_PCS];  //レンジ,明るさ,減衰の大きさ,オンオフ
	float        ShadowLow_val; //影の下限値
	int          LightPcs;     //ライト個数
};

//平行光源
struct DirectionLight {
	VECTOR4 Direction;  //方向
	VECTOR4 LightColor;//色
	VECTOR4 Lightst;  //明るさx,オンオフy,影の下限値
};

//フォグ
struct Fog {
	VECTOR4  FogColor;//フォグの色
	float         Amount;  //フォグ量
	float         Density;//密度
	float         on_off;
};

#endif