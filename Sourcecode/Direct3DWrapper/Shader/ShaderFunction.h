///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                       ShaderFunction.hlsl                                             //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

char *ShaderFunction =
////////////////////////////////フォグ計算(テクスチャに対して計算)////////////////////////////////////////
"float4 FogCom(float4 FogCol, float4 Fog, float4 CPos, float4 wPos, float4 Tex)\n"
"{\n"
"    float fd;\n"//距離
"    float ff;\n"//フォグファクター
"    if(Fog.z == 1.0f){\n"
"       fd = length(CPos.xyz - wPos.xyz) * 0.01f;\n"//距離計算, 0.01は補正値
"       ff = pow(2.71828, -fd * Fog.y);\n"//フォグファクター計算(変化量)
"       ff *= Fog.x;\n"//フォグ全体の量(小さい方が多くなる)
"       ff = saturate(ff);\n"
"       if(Tex.w > 0.3f){\n"
"         Tex = ff * Tex + (1.0f - ff) * FogCol;\n"
"       }\n"
"    }\n"
"   return Tex;\n"
"}\n"
//////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////ポイントライト計算(ライト数分ループさせて使用する)////////////////////////////////
"float3 PointLightCom(float4 Diffuse, float3 Nor, float4 shadow, float4 lightPos, float4 wPos, float4 lightSt, float4 lightCol)\n"
"{\n"
//出力用Col
"    float3 Col = { 0.0f, 0.0f, 0.0f };\n"
//頂点から光源までの距離を計算
"    float distance = length(lightPos.xyz - wPos.xyz);\n"

//ライトオフ, レンジ×3より外は飛ばす
"    if (lightSt.w == 1.0f && distance < lightSt.x * 3){\n"

//ライト方向正規化
"       float3 L = normalize(lightPos.xyz - wPos.xyz);\n"

//デフォルト減衰率
"       float attenuation = 2.0f;\n"
//レンジ外減衰率増減適用
"       if (distance > lightSt.x){ attenuation = lightSt.z; }\n"
//減衰計算           
"       float r = lightSt.y / (pow(distance, attenuation) * 0.001f);\n"

//法線,ライト方向から陰影作成, N, Lの内積がg_ShadowLow.x未満の場合g_ShadowLow.xの値が適用される(距離による影は関係無し)
"       Col = max(dot(Nor, L), shadow.x) * Diffuse * r * lightCol;\n"
"    }\n"
"    return Col;\n"
"}\n"
//////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////平行光源計算/////////////////////////////////////////////////////////
"float3 DirectionalLightCom(float3 Nor, float4 DlightSt, float4 Dir, float4 DCol)\n"
"{\n"
//出力用Col
"    float3 Col = { 0.0f, 0.0f, 0.0f };\n"

"    float3 D_LightDir;\n"
"    float NL;\n"
"    if(DlightSt.y == 1.0f)\n"
"    {\n"
"       D_LightDir = normalize(Dir);\n"
"       NL = max(saturate(dot(Nor, D_LightDir)), DlightSt.z);\n"
"       Col = DCol * DlightSt.x * NL;\n"
"    }\n"
"    return Col;\n"
"}\n"
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////共通パラメーター////////////////////////////////////////////////////////
"Texture2D g_texColor : register(t0);\n"
"SamplerState g_samLinear : register(s0);\n"

"cbuffer global  : register(b0)\n"
"{\n"
"    matrix g_World[150]; \n"
"    matrix g_WVP[150];\n"
//現在位置
"    float4 g_C_Pos;\n"
//オブジェクト追加カラー
"    float4 g_ObjCol;\n"
//光源位置
"    float4 g_LightPos[150];\n"
//ライト色
"    float4 g_LightColor[150];\n"
//レンジ, 明るさ, 減衰の大きさ, オンオフ
"    float4 g_Lightst[150];\n"
//影の下限値x, ライト個数y
"    float4 g_ShadowLow_Lpcs;\n"
//平行光源方向
"    float4 g_DLightDirection;\n"
//平行光源色
"    float4 g_DLightColor;\n"
//平行光源明るさx,オンオフy, 影の下限値z
"    float4 g_DLightst;\n"
//フォグ量x, 密度y, onoffz
"    float4 g_FogAmo_Density;\n"
//フォグ色
"    float4 g_FogColor;\n"
//ディスプ起伏量x
"    float4 g_DispAmount;\n"
//UV座標移動用
"    float4 g_pXpYmXmY;\n"
"};\n";
/////////////////////////////////////////////////////////////////////////////////////////////////////////