///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           ShaderMESH.hlsl                                             //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

//ShaderFunction.hに連結させて使う
char *ShaderMesh =
"cbuffer global_1:register(b1)\n"
"{\n"
//マテリアル毎の色
"    float4 g_Diffuse;\n"
"};\n"

"struct VS_OUTPUT\n"
"{\n"
"    float4 Pos  : SV_POSITION;\n"
"    float4 wPos : POSITION;\n"
"    float3 Nor  : NORMAL;\n"
"    float2 Tex  : TEXCOORD;\n"
"};\n"

//****************************************メッシュ頂点**************************************************************//
"VS_OUTPUT VSMesh(float4 Pos : POSITION, float4 Nor : NORMAL, float2 Tex : TEXCOORD, uint instanceID : SV_InstanceID)\n"
"{\n"
"    VS_OUTPUT output = (VS_OUTPUT)0;\n"
"    output.Pos = mul(Pos, g_WVP[instanceID]);\n"
"    output.wPos = mul(Pos, g_World[instanceID]);\n"
"    output.Nor = mul(Nor, (float3x3)g_World[instanceID]);\n"
"    output.Tex = Tex;\n"

"    return output;\n"
"}\n"
//****************************************メッシュ頂点**************************************************************//

//****************************************メッシュピクセル**********************************************************//
"float4 PSMesh(VS_OUTPUT input) : SV_Target\n"
"{\n"
//法線正規化
"    float3 N = normalize(input.Nor);\n"
//テクスチャ
"    float4 T1 = g_texColor.Sample(g_samLinear, input.Tex);\n"
//基本カラー
"    float4 C = g_Diffuse;\n"

//フォグ計算テクスチャに対して計算
"    float4 T = FogCom(g_FogColor, g_FogAmo_Density, g_C_Pos, input.wPos, T1);\n"

//アルファ値退避
"    float a = C.w;\n"

//ライト計算
"    float3 Col = { 0.0f, 0.0f, 0.0f };\n"
"    for (int i = 0; i < g_ShadowLow_Lpcs.y; i++){\n"
"        Col = Col + PointLightCom(C, N, g_ShadowLow_Lpcs, g_LightPos[i], input.wPos, g_Lightst[i], g_LightColor[i]);\n"
"    }\n"

//平行光源計算
"    Col = Col + DirectionalLightCom(input.Nor, g_DLightst, g_DLightDirection, g_DLightColor);\n"

"    float4 color;\n"
"    color = float4(Col, a) * T + g_ObjCol;\n"
"    return color;\n"
"}\n";
//****************************************メッシュピクセル**********************************************************//
