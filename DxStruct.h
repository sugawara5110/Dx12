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

//頂点
struct Vertex
{
	VECTOR3 Pos;     //位置
	VECTOR3 normal;  //法線
	VECTOR4 color;   //色
	VECTOR2 tex;    //テクスチャ座標
};

//シェーダー受け渡し用バッファ
struct CONSTANT_BUFFER
{
	MATRIX World;
	MATRIX WVP;
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