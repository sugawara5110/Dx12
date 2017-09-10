///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                          ShaderSkinMesh.hlsl                                          //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

//ShaderFunction.hに連結させて使う
char *ShaderSkinMesh =
"cbuffer global_1:register(b1)\n"
"{\n"
//マテリアル毎の色
"    float4 g_Diffuse;\n"
"};\n"

"cbuffer global_bones : register(b2)\n"//ボーンのポーズ行列が入る
"{\n"
"   matrix g_mConstBoneWorld[150];\n"
"};\n"

//スキニング後の頂点・法線が入る
"struct Skin\n"
"{\n"
"   float4 Pos : POSITION;\n"
"   float3 Nor : NORMAL;\n"
"};\n"
//バーテックスバッファーの入力
"struct VSSkinIn\n"
"{\n"
"   float3 Pos : POSITION;\n"//位置   
"   float3 Nor : NORMAL;\n"//頂点法線
"   float2 Tex : TEXCOORD;\n"//テクスチャー座標
"   uint4  Bones : BONE_INDEX;\n"//ボーンのインデックス
"   float4 Weights : BONE_WEIGHT;\n"//ボーンの重み
"};\n"

"struct VS_OUTPUT\n"
"{\n"
"    float4 Pos  : SV_POSITION;\n"
"    float4 wPos : POSITION;\n"
"    float3 Nor  : NORMAL;\n"
"    float2 Tex  : TEXCOORD;\n"
"};\n"

//指定した番号のボーンのポーズ行列を返す　サブ関数（バーテックスシェーダーで使用）
"matrix FetchBoneMatrix(uint iBone)\n"
"{\n"
"   return g_mConstBoneWorld[iBone];\n"
"}\n"

//頂点をスキニング
"Skin SkinVert(VSSkinIn Input)\n"
"{\n"
"   Skin Output = (Skin)0;\n"

"   float4 Pos = float4(Input.Pos, 1);\n"
"   Pos.x *= -1;\n"//FBXは右手座標系なのでxあるいはｚを反転
"   float3 Nor = Input.Nor;\n"
//ボーン0
"   uint iBone = Input.Bones.x;\n"
"   float fWeight = Input.Weights.x;\n"
"   matrix m = FetchBoneMatrix(iBone);\n"
"   Output.Pos += fWeight * mul(Pos, m);\n"
"   Output.Nor += fWeight * mul(Nor, (float3x3)m);\n"
//ボーン1
"   iBone = Input.Bones.y;\n"
"   fWeight = Input.Weights.y;\n"
"   m = FetchBoneMatrix(iBone);\n"
"   Output.Pos += fWeight * mul(Pos, m);\n"
"   Output.Nor += fWeight * mul(Nor, (float3x3)m);\n"
//ボーン2
"   iBone = Input.Bones.z;\n"
"   fWeight = Input.Weights.z;\n"
"   m = FetchBoneMatrix(iBone);\n"
"   Output.Pos += fWeight * mul(Pos, m);\n"
"   Output.Nor += fWeight * mul(Nor, (float3x3)m);\n"
//ボーン3
"   iBone = Input.Bones.w;\n"
"   fWeight = Input.Weights.w;\n"
"   m = FetchBoneMatrix(iBone);\n"
"   Output.Pos += fWeight * mul(Pos, m);\n"
"   Output.Nor += fWeight * mul(Nor, (float3x3)m);\n"

"   return Output;\n"
"}\n"

//****************************************メッシュ頂点**************************************************************//
"VS_OUTPUT VSSkin(VSSkinIn input)\n"
"{\n"
"    VS_OUTPUT output = (VS_OUTPUT)0;\n"

"    Skin vSkinned = SkinVert(input);\n"

"    output.Pos = mul(vSkinned.Pos, g_WVP[0]);\n"
"    output.wPos = mul(vSkinned.Pos, g_World[0]);\n"
"    output.Nor = mul(vSkinned.Nor, (float3x3)g_World[0]);\n"
"    output.Tex = input.Tex;\n"

"    return output;\n"
"}\n"
//****************************************メッシュ頂点**************************************************************//

//****************************************メッシュピクセル**********************************************************//
"float4 PSSkin(VS_OUTPUT input) : SV_Target\n"
"{\n"
//法線正規化
"    float3 N = normalize(input.Nor);\n"
//テクスチャ
"    float4 T1 = g_texColor.Sample(g_samLinear, input.Tex);\n"
//基本カラー
"    float4 C = g_Diffuse;\n"

//フォグ計算テクスチャに対して計算
"    float4 T = FogCom(g_FogColor, g_FogAmo_Density, g_C_Pos, input.wPos, T1);\n"

//ライト計算
"    float3 Col = { 0.0f, 0.0f, 0.0f };\n"
"    for (int i = 0; i < g_ShadowLow_Lpcs.y; i++){\n"
"        Col = Col + PointLightCom(C, N, g_ShadowLow_Lpcs, g_LightPos[i], input.wPos, g_Lightst[i], g_LightColor[i]);\n"
"    }\n"

//平行光源計算
"    Col = Col + DirectionalLightCom(input.Nor, g_DLightst, g_DLightDirection, g_DLightColor);\n"

"    float4 color;\n"
"    color = float4(Col, 1.0f) * T + g_ObjCol;\n"
"    return color;\n"
"}\n";
//****************************************メッシュピクセル**********************************************************//
