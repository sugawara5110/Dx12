///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                          ShaderSkinMesh.hlsl                                          //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

char *ShaderSkinMesh =
"Texture2D g_texColor : register(t0);\n"
"SamplerState g_samLinear : register(s0);\n"

"cbuffer global_0:register(b0)\n"
"{\n"
"    matrix g_World[150];\n"
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
"};\n"

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
"    float4 Col  : COLOR0;\n"
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
"    output.Col = g_Diffuse;\n"

"    return output;\n"
"}\n"
//****************************************メッシュ頂点**************************************************************//

//****************************************メッシュピクセル**********************************************************//
"float4 PSSkin(VS_OUTPUT input) : SV_Target\n"
"{\n"
//法線正規化
"    float3 N = normalize(input.Nor);\n"
//テクスチャ
"    float4 T = g_texColor.Sample(g_samLinear, input.Tex);\n"
//基本カラー
"    float4 C = input.Col;\n"

//フォグ計算
"    float fd;\n"//距離
"    float ff;\n"//フォグファクター
"    if(g_FogAmo_Density.z == 1.0f){\n"
"       fd = length(g_C_Pos.xyz - input.wPos.xyz) * 0.01f;\n"//距離計算, 0.01は補正値
"       ff = pow(2.71828, -fd * g_FogAmo_Density.y);\n"//フォグファクター計算(変化量)
"       ff *= g_FogAmo_Density.x;\n"//フォグ全体の量(小さい方が多くなる)
"       ff = saturate(ff);\n"
"       if(T.w > 0.3f){\n"
"         T = ff * T + (1.0f - ff) * g_FogColor;\n"
"       }\n"
"    }\n"

//アルファ値退避
"    float a = C.w;\n"
"    float3 Col = { 0.0f, 0.0f, 0.0f };\n"

//ライト計算
"    for (int i = 0; i < g_ShadowLow_Lpcs.y; i++){\n"
//頂点から光源までの距離を計算
"        float distance = length(g_LightPos[i].xyz - input.wPos.xyz);\n"

//ライトオフ, レンジ×3より外は飛ばす
"        if (g_Lightst[i].w == 1.0f && distance < g_Lightst[i].x * 3){\n"

//ライト方向正規化
"           float3 L = normalize(abs(g_LightPos[i].xyz - input.wPos.xyz));\n"

//デフォルト減衰率
"            float attenuation = 2.0f;\n"
//レンジ外減衰率増減適用
"            if (distance > g_Lightst[i].x){ attenuation = g_Lightst[i].z; }\n"
//減衰計算           
"            float r = g_Lightst[i].y / (pow(distance, attenuation) * 0.001f);\n"

//法線,ライト方向から陰影作成, N, Lの内積がg_ShadowLow.x未満の場合g_ShadowLow.xの値が適用される(距離による影は関係無し)
"           Col = Col + max(dot(N, L), g_ShadowLow_Lpcs.x) * C * r * g_LightColor[i];\n"
"        }\n"
"    }\n"

//平行光源計算
"    float3 D_LightDir;\n"
"    float NL;\n"
"    if(g_DLightst.y == 1.0f)\n"
"    {\n"
"       D_LightDir = normalize(g_DLightDirection);\n"
"       NL = max(saturate(dot(input.Nor, D_LightDir)), g_DLightst.z);\n"
"       Col = Col + g_DLightColor * g_DLightst.x * NL;\n"
"    }\n"

"    float4 color;\n"
"    color = float4(Col, a) * T + g_ObjCol;\n"
"    return color;\n"
"}\n";
//****************************************メッシュピクセル**********************************************************//
