///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           ShaderMESH_D.hlsl                                           //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

//ShaderFunction.hに連結させて使う
char *ShaderMesh_D =
"cbuffer global_1:register(b1)\n"
"{\n"
//マテリアル毎の色
"    float4 g_Diffuse;\n"
"};\n"

"struct VS_OUTPUT\n"
"{\n"
"    float3 Pos        : POSITION;\n"
"    float3 Nor        : NORMAL;\n"
"    float2 Tex        : TEXCOORD;\n"
"    uint   instanceID : SV_InstanceID;\n"
"};\n"

"struct HS_CONSTANT_OUTPUT\n"
"{\n"
"	 float factor[3]    : SV_TessFactor;\n"
"	 float inner_factor : SV_InsideTessFactor;\n"
"};\n"

"struct HS_OUTPUT\n"
"{\n"
"    float3 Pos        : POSITION;\n"
"    float3 Nor        : NORMAL;\n"
"    float2 Tex        : TEXCOORD;\n"
"    uint   instanceID : SV_InstanceID;\n"
"};\n"

"struct DS_OUTPUT\n"
"{\n"
"    float4 Pos  : SV_POSITION;\n"
"    float4 wPos : POSITION;\n"
"    float3 Nor  : NORMAL;\n"
"    float2 Tex  : TEXCOORD;\n"
"};\n"

//****************************************メッシュ頂点**************************************************************//
"VS_OUTPUT VSMesh(float3 Pos : POSITION, float3 Nor : NORMAL, float2 Tex : TEXCOORD, uint instanceID : SV_InstanceID)\n"
"{\n"
"    VS_OUTPUT output = (VS_OUTPUT)0;\n"
"    output.Pos = Pos;\n"
"    output.Nor = Nor;\n"
"    output.Tex = Tex;\n"
"    output.instanceID = instanceID;\n"
"    return output;\n"
"}\n"
//****************************************メッシュ頂点**************************************************************//

//***************************************ハルシェーダーコンスタント*************************************************//
"HS_CONSTANT_OUTPUT HSConstant(InputPatch<VS_OUTPUT, 3> ip, uint pid : SV_PrimitiveID)\n"
"{\n"
"	HS_CONSTANT_OUTPUT output;\n"

//ワールド変換(float3 から float4の変換忘れ無い事)
"   float4 pos = float4(ip[0].Pos, 1.0f);\n"
"   float4 wPos = mul(pos, g_World[ip[0].instanceID]);\n"
//頂点から現在地までの距離を計算
"   float distance = length(g_C_Pos.xyz - wPos.xyz);\n"

//距離でポリゴン数決定
"   float divide = 1;\n"

"   if(distance < 1000){divide = 2;}\n"
"   if(distance < 500){divide = 5;}\n"
"   if(distance < 300){divide = 10;}\n"

"	output.factor[0] = divide;\n"
"	output.factor[1] = divide;\n"
"	output.factor[2] = divide;\n"
//u 縦の分割数（横のラインを何本ひくか）
"	output.inner_factor = divide;\n"

"	return output;\n"
"}\n"
//***************************************ハルシェーダーコンスタント*************************************************//

//***************************************ハルシェーダー*************************************************************//
"[domain(\"tri\")]\n"
"[partitioning(\"integer\")]\n"
"[outputtopology(\"triangle_ccw\")]\n"
"[outputcontrolpoints(3)]\n"
"[patchconstantfunc(\"HSConstant\")]\n"
"HS_OUTPUT HSMesh(InputPatch<VS_OUTPUT, 3> ip, uint cpid : SV_OutputControlPointID, uint pid : SV_PrimitiveID)\n"
"{\n"
"	HS_OUTPUT output;\n"
"	output.Pos = ip[cpid].Pos;\n"
"	output.Nor = ip[cpid].Nor;\n"
"	output.Tex = ip[cpid].Tex;\n"
"   output.instanceID = ip[cpid].instanceID;\n"
"	return output;\n"
"}\n"
//***************************************ハルシェーダー*************************************************************//

//**************************************ドメインシェーダー**********************************************************//
"[domain(\"tri\")]\n"
"DS_OUTPUT DSMesh(HS_CONSTANT_OUTPUT In, float3 UV : SV_DomaInLocation, const OutputPatch<HS_OUTPUT, 3> patch)\n"
"{\n"
"	DS_OUTPUT output;\n"

//UV座標計算
"   float2 uv = patch[0].Tex * UV.x + patch[1].Tex * UV.y + patch[2].Tex * UV.z;\n"
"   output.Tex = uv;\n"

"   float4 height;\n"
"   float hei;\n"

//画像から高さを算出
"   height = g_texColor.SampleLevel(g_samLinear, uv, 0) * g_DispAmount.x;\n"
"   hei = (height.x + height.y + height.z) / 3;\n"

//pos座標計算
"   float3 pos = patch[0].Pos * UV.x + patch[1].Pos * UV.y + patch[2].Pos * UV.z;\n"
"   output.Pos = float4(pos, 1);\n"

"   float4 nor = float4(patch[0].Nor, 0);\n"

//ローカル法線の方向にhei分頂点移動, ポリゴンが切れるのでパッチの端は移動させない
"   if(UV.x != 1.0f && UV.x != 0.0f && UV.y != 1.0f && UV.y != 0.0f && UV.z != 1.0f && UV.z != 0.0f)\n"
"   {\n"
"      output.Pos.xyz += hei * patch[0].Nor;\n"
"   }\n"
//画像から法線計算用ベクトル生成
"   nor = g_texColor.SampleLevel(g_samLinear, uv, 0) * 2 - 1;\n"
//画像から生成したベクトルにローカル法線を足し法線ベクトルとする
"   float3 nor1;\n"
"   nor1 = nor.xyz + patch[0].Nor;\n"
"   nor.xyz = nor1;\n"

"   float3 Normal = normalize(nor.xyz);\n"

//座標変換
"   output.wPos = mul(output.Pos, g_World[patch[0].instanceID]);\n"
"   output.Pos = mul(output.Pos, g_WVP[patch[0].instanceID]);\n"

//出力する法線の作成
"   output.Nor = mul(Normal, (float3x3)g_World[patch[0].instanceID]);\n"

"	return output;\n"
"}\n"
//**************************************ドメインシェーダー**********************************************************//

//****************************************メッシュピクセル**********************************************************//
"float4 PSMesh(DS_OUTPUT input) : SV_Target\n"
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
