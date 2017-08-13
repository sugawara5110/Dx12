///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           ShaderWaveDraw.hlsl                                         //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

char *ShaderWaveDraw =
"Texture2D g_texColor : register(t0);\n"
"SamplerState g_samLinear : register(s0);\n"
"struct WaveData\n"
"{\n"
"	float sinWave;\n"
"   float theta;\n"
"};\n"
"StructuredBuffer<WaveData> gInput : register(t1);\n"

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
"};\n"

//wave
"cbuffer cbWave  : register(b1)\n"
"{\n"
//x:waveHeight, y:分割数
"    float4 g_wHei_divide;\n"
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
"	 float factor[4]       : SV_TessFactor;\n"
"	 float inner_factor[2] : SV_InsideTessFactor;\n"
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

//*********************************************頂点シェーダー*******************************************************************//
"VS_OUTPUT VSWave(float3 Pos : POSITION, float3 Nor : NORMAL, float2 Tex : TEXCOORD, uint instanceID : SV_InstanceID)\n"
"{\n"
"    VS_OUTPUT output = (VS_OUTPUT)0;\n"
"    output.Pos = Pos;\n"
"    output.Nor = Nor;\n"
"    output.Tex = Tex;\n"
"    output.instanceID = instanceID;\n"
"    return output;\n"
"}\n"
//*********************************************頂点シェーダー*******************************************************************//

//***************************************ハルシェーダーコンスタント*************************************************************//
"HS_CONSTANT_OUTPUT HSConstant(InputPatch<VS_OUTPUT, 4> ipL, uint pidL : SV_PrimitiveID)\n"
"{\n"
"	HS_CONSTANT_OUTPUT output;\n"

//ワールド変換(float3 から float4の変換忘れ無い事)
"   float4 pos = float4(ipL[0].Pos, 1.0f);\n"
"   float4 wPos = mul(pos, g_World[ipL[0].instanceID]);\n"
//頂点から現在地までの距離を計算
"   float distance = length(g_C_Pos.xyz - wPos.xyz);\n"

//ポリゴン数決定
"   float divide = g_wHei_divide.y;\n"

"	output.factor[0] = divide;\n"
"	output.factor[1] = divide;\n"
"	output.factor[2] = divide;\n"
"	output.factor[3] = divide;\n"
//u 縦の分割数（横のラインを何本ひくか）
"	output.inner_factor[0] = divide;\n"
//v
"	output.inner_factor[1] = divide;\n"

"	return output;\n"
"}\n"
//***************************************ハルシェーダーコンスタント*************************************************************//

//***************************************ハルシェーダー*************************************************************************//
"[domain(\"quad\")]\n"
"[partitioning(\"integer\")]\n"
"[outputtopology(\"triangle_ccw\")]\n"
"[outputcontrolpoints(4)]\n"
"[patchconstantfunc(\"HSConstant\")]\n"
"HS_OUTPUT HSWave(InputPatch<VS_OUTPUT, 4> ipL, uint cpidL : SV_OutputControlPointID, uint pidL : SV_PrimitiveID)\n"
"{\n"
"	HS_OUTPUT output;\n"
"	output.Pos = ipL[cpidL].Pos;\n"
"	output.Nor = ipL[cpidL].Nor;\n"
"	output.Tex = ipL[cpidL].Tex;\n"
"   output.instanceID = ipL[cpidL].instanceID;\n"
"	return output;\n"
"}\n"
//***************************************ハルシェーダー*************************************************************************//

//**************************************ドメインシェーダー*********************************************************************//
"[domain(\"quad\")]\n"
"DS_OUTPUT DSWave(HS_CONSTANT_OUTPUT InL, float2 UV : SV_DomaInLocation, const OutputPatch<HS_OUTPUT, 4> patchL)\n"
"{\n"
"	DS_OUTPUT output;\n"

//UV座標計算
"   float2 top_uv = lerp(patchL[0].Tex, patchL[1].Tex, UV.x);\n"
"   float2 bottom_uv = lerp(patchL[3].Tex, patchL[2].Tex, UV.x);\n"
"   float2 uv = float2(lerp(top_uv, bottom_uv, UV.y));\n"
"   output.Tex = uv;\n"
//画像から高さを算出
"   float4 texheight = g_texColor.SampleLevel(g_samLinear, uv, 0);\n"
"   float4 height = texheight * g_DispAmount.x;\n"
"   float hei = (height.x + height.y + height.z) / 3;\n"
//コンピュートシェーダーで計算したsin波取り出し
"   float uvy = g_wHei_divide.y * (g_wHei_divide.y - 1.0f);\n"
"   float uvx = g_wHei_divide.y;\n"
"   float sinwave = gInput[uvy * uv.y + uvx * uv.x].sinWave;\n"
//画像から法線計算用ベクトル生成
"   float4 nor = texheight * 2 - 1;\n"//-1.0～1.0にする為
//pos座標計算
"   float3 top_pos = lerp(patchL[0].Pos, patchL[1].Pos, UV.x);\n"
"   float3 bottom_pos = lerp(patchL[3].Pos, patchL[2].Pos, UV.x);\n"
"   output.Pos = float4(lerp(top_pos, bottom_pos, UV.y), 1);\n"
//ローカル法線の方向にhei分頂点移動
"   output.Pos.xyz += hei * patchL[0].Nor;\n"
//ローカル法線の方向にsin波を生成
"   float3 sinwave3 = patchL[0].Nor * sinwave;\n"
//頂点にsin波合成
"   output.Pos.xyz += sinwave3;\n"
//画像から生成したベクトルにローカル法線を足し法線ベクトルとする
"   float3 nor1;\n"
"   nor1 = nor.xyz + patchL[0].Nor;\n"
"   output.wPos = mul(output.Pos, g_World[patchL[0].instanceID]);\n"
"   output.Pos = mul(output.Pos, g_WVP[patchL[0].instanceID]);\n"

//法線正規化
"   float3 Normal = normalize(nor1);\n"

//出力する法線の作成
"   output.Nor = mul(Normal, (float3x3)g_World[patchL[0].instanceID]);\n"

"	return output;\n"
"}\n"
//**************************************ドメインシェーダー*********************************************************************//

//**************************************ピクセルシェーダー********************************************************************//
"float4 PSWave(DS_OUTPUT input) : SV_Target\n"
"{\n"
//法線正規化
"    float3 N = normalize(input.Nor);\n"
//テクスチャ
"    float4 T = g_texColor.Sample(g_samLinear, input.Tex);\n"

//フォグ計算テクスチャに対して計算
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

"    float3 Col = { 0.0f, 0.0f, 0.0f };\n"

//ライト計算
"    for (int i = 0; i < g_ShadowLow_Lpcs.y; i++){\n"
//頂点から光源までの距離を計算
"        float distance = length(g_LightPos[i].xyz - input.wPos.xyz);\n"

//ライトオフ, レンジ×3より外は飛ばす
"        if (g_Lightst[i].w == 1.0f && distance < g_Lightst[i].x * 3){\n"

//ライト方向正規化
"            float3 L = normalize(g_LightPos[i].xyz - input.wPos.xyz);\n"

//デフォルト減衰率
"            float attenuation = 2.0f;\n"
//レンジ外減衰率増減適用
"            if (distance > g_Lightst[i].x){ attenuation = g_Lightst[i].z; }\n"
//減衰計算           
"            float r = g_Lightst[i].y / (pow(distance, attenuation) * 0.001f);\n"

//法線,ライト方向から陰影作成, N, Lの内積がg_ShadowLow.x未満の場合g_ShadowLow.xの値が適用される(距離による影は関係無し)
"           Col = Col + max(dot(N, L), g_ShadowLow_Lpcs.x) * r * g_LightColor[i];\n"
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

//最後に基本色にテクスチャの色を掛け合わせる
"    return float4(Col, 1.0f) * T + g_ObjCol;\n"
"}\n";
