///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           ShaderMESH_D.hlsl                                           //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

//ShaderFunction.hに連結させて使う
char *ShaderMesh_D =
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
"	HS_CONSTANT_OUTPUT output = (HS_CONSTANT_OUTPUT)0;\n"

//ワールド変換
"   float3 wPos = mul(ip[0].Pos, (float3x3)g_World[ip[0].instanceID]);\n"
//頂点から現在地までの距離を計算
"   float distance = length(g_C_Pos.xyz - wPos);\n"

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
"	HS_OUTPUT output = (HS_OUTPUT)0;\n"
"	output.Pos = ip[cpid].Pos;\n"
"	output.Nor = ip[cpid].Nor;\n"
"	output.Tex = ip[cpid].Tex;\n"
"   output.instanceID = ip[cpid].instanceID;\n"
"	return output;\n"
"}\n"
//***************************************ハルシェーダー*************************************************************//

//**************************************ドメインシェーダー**********************************************************//
"[domain(\"tri\")]\n"
"PS_INPUT DSMesh(HS_CONSTANT_OUTPUT In, float3 UV : SV_DomaInLocation, const OutputPatch<HS_OUTPUT, 3> patch)\n"
"{\n"
"	PS_INPUT output = (PS_INPUT)0;\n"

//UV座標計算
"   float2 uv = patch[0].Tex * UV.x + patch[1].Tex * UV.y + patch[2].Tex * UV.z;\n"
"   output.Tex = uv;\n"

//画像から高さを算出
"   float4 height = g_texColor.SampleLevel(g_samLinear, uv, 0) * g_DispAmount.x;\n"
"   float hei = (height.x + height.y + height.z) / 3;\n"

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
"   float3 nor1 = nor.xyz + patch[0].Nor;\n"
"   nor.xyz = nor1;\n"

"   float3 Normal = normalize(nor.xyz);\n"

//座標変換
"   output.wPos = mul(output.Pos, g_World[patch[0].instanceID]);\n"
"   output.Pos = mul(output.Pos, g_WVP[patch[0].instanceID]);\n"

//出力する法線の作成
"   output.Nor = mul(Normal, (float3x3)g_World[patch[0].instanceID]);\n"

"	return output;\n"
"}\n";
//**************************************ドメインシェーダー**********************************************************//
