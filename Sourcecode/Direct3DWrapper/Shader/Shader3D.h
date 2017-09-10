///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           Shader3D.hlsl                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

//ShaderFunction.hに連結させて使う
char *Shader3D =
"struct VS_OUTPUT_TCL\n"
"{\n"
"    float4 Pos        : SV_POSITION;\n"
"    float4 wPos       : POSITION;\n"
"    float3 Nor        : NORMAL;\n"
"    float2 Tex        : TEXCOORD;\n"
"};\n"

"struct VS_OUTPUT_TC\n"
"{\n"
"    float4 Pos        : SV_POSITION;\n"
"    float4 wPos       : POSITION;\n"
"    float2 Tex        : TEXCOORD;\n"
"};\n"

"struct VS_OUTPUT_BC\n"
"{\n"
"    float4 Pos        : SV_POSITION;\n"
"    float4 Col        : COLOR;\n"
"};\n"

//****************************************テクスチャ頂点**************************************************************//
//ライト有
"VS_OUTPUT_TCL VSTextureColorL(float4 Pos : POSITION, float4 Nor : NORMAL, float2 Tex : TEXCOORD, uint instanceID : SV_InstanceID)\n"
"{\n"
"    VS_OUTPUT_TCL output = (VS_OUTPUT_TCL)0;\n"
"    output.Pos = mul(Pos, g_WVP[instanceID]);\n"
"    output.wPos = mul(Pos, g_World[instanceID]);\n"
"    output.Nor = mul(Nor, (float3x3)g_World[instanceID]);\n"
"    output.Tex.x = Tex.x * g_pXpYmXmY.x + g_pXpYmXmY.x * g_pXpYmXmY.z;\n"
"    output.Tex.y = Tex.y * g_pXpYmXmY.y + g_pXpYmXmY.y * g_pXpYmXmY.w;\n"

"    return output;\n"
"}\n"

//ライト無
"VS_OUTPUT_TC VSTextureColor(float4 Pos : POSITION, float2 Tex : TEXCOORD, uint instanceID : SV_InstanceID)\n"
"{\n"
"    VS_OUTPUT_TC output = (VS_OUTPUT_TC)0;\n"
"    output.Pos = mul(Pos, g_WVP[instanceID]);\n"
"    output.wPos = mul(Pos, g_World[instanceID]);\n"
"    output.Tex.x = Tex.x * g_pXpYmXmY.x + g_pXpYmXmY.x * g_pXpYmXmY.z;\n"
"    output.Tex.y = Tex.y * g_pXpYmXmY.y + g_pXpYmXmY.y * g_pXpYmXmY.w;\n"

"    return output;\n"
"}\n"
//****************************************テクスチャ頂点**************************************************************//

//****************************************テクスチャピクセル**********************************************************//
//ライト有
"float4 PSTextureColorL(VS_OUTPUT_TCL input) : SV_Target\n"
"{\n"
//法線正規化
"    float3 N = normalize(input.Nor);\n"

//テクスチャ
"    float4 T1 = g_texColor.Sample(g_samLinear, input.Tex);\n"

//フォグ計算(テクスチャに対して計算)
"    float4 T = FogCom(g_FogColor, g_FogAmo_Density, g_C_Pos, input.wPos, T1);\n"

//ライト計算
"    float4 C = { 1.0f, 1.0f, 1.0f, 1.0f};\n"
"    float3 Col = { 0.0f, 0.0f, 0.0f };\n"
"    for (int i = 0; i < g_ShadowLow_Lpcs.y; i++){\n"
"        Col = Col + PointLightCom(C, N, g_ShadowLow_Lpcs, g_LightPos[i], input.wPos, g_Lightst[i], g_LightColor[i]);\n"
"    }\n"

//平行光源計算
"    Col = Col + DirectionalLightCom(input.Nor, g_DLightst, g_DLightDirection, g_DLightColor);\n"

"    if(T.w <= 0.0f)discard;\n"//アルファ値0の場合ピクセル破棄
"    return float4(Col, 1.0f) * T + g_ObjCol;\n"
"}\n"

//ライト無
"float4 PSTextureColor(VS_OUTPUT_TC input) : SV_Target\n"
"{\n"
//テクスチャ
"    float4 T1 = g_texColor.Sample(g_samLinear, input.Tex);\n"

//フォグ計算(テクスチャに対して計算)
"    float4 T = FogCom(g_FogColor, g_FogAmo_Density, g_C_Pos, input.wPos, T1);\n"

"    if(T.w <= 0.0f)discard;\n"//アルファ値0の場合ピクセル破棄
"    return T + g_ObjCol;\n"
"}\n"
//****************************************テクスチャピクセル**********************************************************//

//****************************************基本色頂点******************************************************************//
"VS_OUTPUT_BC VSBaseColor(float4 Pos : POSITION, float4 Col : COLOR, uint instanceID : SV_InstanceID)\n"
"{\n"
"    VS_OUTPUT_BC output = (VS_OUTPUT_BC)0;\n"
"    output.Pos = mul(Pos, g_WVP[instanceID]);\n"

"    output.Col = Col;\n"

"    return output;\n"
"}\n"
//****************************************基本色頂点******************************************************************//

//****************************************基本色ピクセル**************************************************************//
"float4 PSBaseColor(VS_OUTPUT_BC input) : SV_Target\n"
"{\n"
"   return input.Col + g_ObjCol;\n"
"}\n";
//****************************************基本色ピクセル**************************************************************//
