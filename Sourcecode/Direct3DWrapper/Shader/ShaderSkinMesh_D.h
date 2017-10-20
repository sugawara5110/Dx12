///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                          ShaderSkinMesh_D.hlsl                                        //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

//ShaderFunction.hに連結させて使う
char *ShaderSkinMesh_D =
"cbuffer global_bones : register(b2)\n"//ボーンのポーズ行列が入る
"{\n"
"   matrix g_mConstBoneWorld[150];\n"
"};\n"

//スキニング後の頂点・法線が入る
"struct Skin\n"
"{\n"
"   float4 Pos : POSITION;\n"
"   float3 Nor : NORMAL;\n"
"   float3 GNor :GEO_NORMAL;\n"
"};\n"
//バーテックスバッファーの入力
"struct VSSkinIn\n"
"{\n"
"   float3 Pos : POSITION;\n"//頂点   
"   float3 Nor : NORMAL;\n"//法線
"   float3 GNor : GEO_NORMAL;\n"//ジオメトリ法線
"   float2 Tex : TEXCOORD;\n"//テクスチャー座標
"   uint4  Bones : BONE_INDEX;\n"//ボーンのインデックス
"   float4 Weights : BONE_WEIGHT;\n"//ボーンの重み
"};\n"

"struct VS_OUTPUT\n"
"{\n"
"    float4 Pos    : POSITION;\n"
"    float3 Nor    : NORMAL;\n"
"    float3 GNor   : GEO_NORMAL;\n"
"    float2 Tex        : TEXCOORD;\n"
"    uint   instanceID : SV_InstanceID;\n"
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
"   float3 Nor = Input.Nor;\n"
"   float3 GNor = Input.GNor;\n"
//ボーン0
"   uint iBone = Input.Bones.x;\n"
"   float fWeight = Input.Weights.x;\n"
"   matrix m = FetchBoneMatrix(iBone);\n"
"   Output.Pos += fWeight * mul(Pos, m);\n"
"   Output.Nor += fWeight * mul(Nor, (float3x3)m);\n"
"   Output.GNor += fWeight * mul(GNor, (float3x3)m);\n"
//ボーン1
"   iBone = Input.Bones.y;\n"
"   fWeight = Input.Weights.y;\n"
"   m = FetchBoneMatrix(iBone);\n"
"   Output.Pos += fWeight * mul(Pos, m);\n"
"   Output.Nor += fWeight * mul(Nor, (float3x3)m);\n"
"   Output.GNor += fWeight * mul(GNor, (float3x3)m);\n"
//ボーン2
"   iBone = Input.Bones.z;\n"
"   fWeight = Input.Weights.z;\n"
"   m = FetchBoneMatrix(iBone);\n"
"   Output.Pos += fWeight * mul(Pos, m);\n"
"   Output.Nor += fWeight * mul(Nor, (float3x3)m);\n"
"   Output.GNor += fWeight * mul(GNor, (float3x3)m);\n"
//ボーン3
"   iBone = Input.Bones.w;\n"
"   fWeight = Input.Weights.w;\n"
"   m = FetchBoneMatrix(iBone);\n"
"   Output.Pos += fWeight * mul(Pos, m);\n"
"   Output.Nor += fWeight * mul(Nor, (float3x3)m);\n"
"   Output.GNor += fWeight * mul(GNor, (float3x3)m);\n"

"   return Output;\n"
"}\n"

//****************************************バーテックスシェーダー****************************************************//
"VS_OUTPUT VS(VSSkinIn input, uint instanceID : SV_InstanceID)\n"
"{\n"
"    VS_OUTPUT output = (VS_OUTPUT)0;\n"

"    Skin vSkinned = SkinVert(input);\n"

"    output.Pos = vSkinned.Pos;\n"
"    output.Nor = vSkinned.Nor;\n"
"    output.GNor = vSkinned.GNor;\n"
"    output.Tex = input.Tex;\n"
"    output.instanceID = instanceID;\n"

"    return output;\n"
"}\n";
//****************************************バーテックスシェーダー****************************************************//
