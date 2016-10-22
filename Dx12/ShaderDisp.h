///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           ShaderDisp.hlsl                                             //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

char *ShaderDisp =
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
"};\n"

"struct VS_OUTPUT_TCL\n"
"{\n"
"    float3 Pos        : POSITION;\n"
"    float3 Nor        : NORMAL;\n"
"    float4 Col        : COLOR;\n"
"    float2 Tex        : TEXCOORD;\n"
"    uint   instanceID : SV_InstanceID;\n"
"};\n"

"struct VS_OUTPUT_TC\n"
"{\n"
"    float3 Pos        : POSITION;\n"
"    float3 Nor        : NORMAL;\n"
"    float4 Col        : COLOR;\n"
"    float2 Tex        : TEXCOORD;\n"
"    uint   instanceID : SV_InstanceID;\n"
"};\n"

"struct HS_CONSTANT_OUTPUT\n"
"{\n"
"	 float factor[4]       : SV_TessFactor;\n"
"	 float inner_factor[2] : SV_InsideTessFactor;\n"
"};\n"

"struct HS_OUTPUT_TCL\n"
"{\n"
"    float3 Pos        : POSITION;\n"
"    float3 Nor        : NORMAL;\n"
"    float4 Col        : COLOR;\n"
"    float2 Tex        : TEXCOORD;\n"
"    uint   instanceID : SV_InstanceID;\n"
"};\n"

"struct HS_OUTPUT_TC\n"
"{\n"
"	 float3 Pos        : POSITION;\n"
"    float3 Nor        : NORMAL;\n"
"    float4 Col        : COLOR;\n"
"    float2 Tex        : TEXCOORD;\n"
"    uint   instanceID : SV_InstanceID;\n"
"};\n"

"struct DS_OUTPUT_TCL\n"
"{\n"
"    float4 Pos  : SV_POSITION;\n"
"    float4 wPos : POSITION;\n"
"    float3 Nor  : NORMAL;\n"
"    float4 Col  : COLOR;\n"
"    float2 Tex  : TEXCOORD;\n"
"};\n"

"struct DS_OUTPUT_TC\n"
"{\n"
"	 float4 Pos  : SV_POSITION;\n"
"    float4 wPos : POSITION;\n"
"    float3 Nor  : NORMAL;\n"
"    float4 Col  : COLOR;\n"
"    float2 Tex  : TEXCOORD;\n"
"};\n"

//*********************************************頂点シェーダー*******************************************************************//
//ライト有
"VS_OUTPUT_TCL VSDispL(float3 Pos : POSITION, float3 Nor : NORMAL, float4 Col : COLOR, float2 Tex : TEXCOORD, uint instanceID : SV_InstanceID)\n"
"{\n"
"    VS_OUTPUT_TCL output = (VS_OUTPUT_TCL)0;\n"
"    output.Pos = Pos;\n"
"    output.Nor = Nor;\n"
"    output.Col = Col;\n"
"    output.Tex = Tex;\n"
"    output.instanceID = instanceID;\n"
"    return output;\n"
"}\n"

//ライト無
"VS_OUTPUT_TC VSDisp(float3 Pos : POSITION, float3 Nor : NORMAL, float4 Col : COLOR, float2 Tex : TEXCOORD, uint instanceID : SV_InstanceID)\n"
"{\n"
"    VS_OUTPUT_TC output = (VS_OUTPUT_TC)0;\n"
"    output.Pos = Pos;\n"
"    output.Nor = Nor;\n"
"    output.Col = Col;\n"
"    output.Tex = Tex;\n"
"    output.instanceID = instanceID;\n"
"    return output;\n"
"}\n"
//*********************************************頂点シェーダー*******************************************************************//

//***************************************ハルシェーダーコンスタント*************************************************************//
//ライト有
"HS_CONSTANT_OUTPUT HSConstantL(InputPatch<VS_OUTPUT_TCL, 4> ipL, uint pidL : SV_PrimitiveID)\n"
"{\n"
"	HS_CONSTANT_OUTPUT output;\n"

//ワールド変換(float3 から float4の変換忘れ無い事)
"   float4 pos = float4(ipL[0].Pos, 1.0f);\n"
"   float4 wPos = mul(pos, g_World[ipL[0].instanceID]);\n"
//頂点から現在地までの距離を計算
"   float distance = length(g_C_Pos.xyz - wPos.xyz);\n"

//距離でポリゴン数決定
"   float divide = 1;\n"
"   if(distance < 1000){divide = 2;}\n"
"   if(distance < 500){divide = 50;}\n"
"   if(distance < 300){divide = 80;}\n"

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

//ライト無
"HS_CONSTANT_OUTPUT HSConstant(InputPatch<VS_OUTPUT_TC, 4> ip, uint pid : SV_PrimitiveID)\n"
"{\n"
"	HS_CONSTANT_OUTPUT output;\n"

//ワールド変換
"   float4 pos = float4(ip[0].Pos, 1.0f);\n"
"   float4 wPos = mul(pos, g_World[ip[0].instanceID]);\n"

//頂点から現在地までの距離を計算
"   float distance = length(g_C_Pos.xyz - wPos.xyz);\n"

//距離でポリゴン数決定
"   float divide = 1;\n"
"   if(distance < 1000){divide = 2;}\n"
"   if(distance < 500){divide = 50;}\n"
"   if(distance < 300){divide = 80;}\n"

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
//ライト有
"[domain(\"quad\")]\n"
"[partitioning(\"integer\")]\n"
"[outputtopology(\"triangle_ccw\")]\n"
"[outputcontrolpoints(4)]\n"
"[patchconstantfunc(\"HSConstantL\")]\n"
"HS_OUTPUT_TCL HSDispL(InputPatch<VS_OUTPUT_TCL, 4> ipL, uint cpidL : SV_OutputControlPointID, uint pidL : SV_PrimitiveID)\n"
"{\n"
"	HS_OUTPUT_TCL output;\n"
"	output.Pos = ipL[cpidL].Pos;\n"
"	output.Nor = ipL[cpidL].Nor;\n"
"	output.Col = ipL[cpidL].Col;\n"
"	output.Tex = ipL[cpidL].Tex;\n"
"   output.instanceID = ipL[cpidL].instanceID;\n"
"	return output;\n"
"}\n"

//ライト無
"[domain(\"quad\")]\n"
"[partitioning(\"integer\")]\n"
"[outputtopology(\"triangle_ccw\")]\n"
"[outputcontrolpoints(4)]\n"
"[patchconstantfunc(\"HSConstant\")]\n"
"HS_OUTPUT_TC HSDisp(InputPatch<VS_OUTPUT_TC, 4> ip, uint cpid : SV_OutputControlPointID, uint pid : SV_PrimitiveID)\n"
"{\n"
"	HS_OUTPUT_TC output;\n"
"	output.Pos = ip[cpid].Pos;\n"
"	output.Nor = ip[cpid].Nor;\n"
"	output.Col = ip[cpid].Col;\n"
"	output.Tex = ip[cpid].Tex;\n"
"   output.instanceID = ip[cpid].instanceID;\n"
"	return output;\n"
"}\n"
//***************************************ハルシェーダー*************************************************************************//

//**************************************ドメインシェーダー*********************************************************************//
//ライト有
"[domain(\"quad\")]\n"
"DS_OUTPUT_TCL DSDispL(HS_CONSTANT_OUTPUT InL, float2 UV : SV_DomaInLocation, const OutputPatch<HS_OUTPUT_TCL, 4> patchL)\n"
"{\n"
"	DS_OUTPUT_TCL output;\n"

//カラー取り出し
"   output.Col = patchL[0].Col;\n"

//UV座標計算
"   float2 top_uv = lerp(patchL[0].Tex, patchL[1].Tex, UV.x);\n"
"   float2 bottom_uv = lerp(patchL[3].Tex, patchL[2].Tex, UV.x);\n"
"   float2 uv = float2(lerp(top_uv, bottom_uv, UV.y));\n"
"   output.Tex = uv;\n"
//画像から高さを算出
"   float4 texheight = g_texColor.SampleLevel(g_samLinear, uv, 0);\n"
"   float4 height = texheight * g_DispAmount.x;\n"
"   float hei = (height.x + height.y + height.z) / 3;\n"
//画像から法線ベクトル生成
"   float4 nor = texheight * 2 - 1;\n"//-1.0～1.0にする為
//pos座標計算
"   float3 top_pos = lerp(patchL[0].Pos, patchL[1].Pos, UV.x);\n"
"   float3 bottom_pos = lerp(patchL[3].Pos, patchL[2].Pos, UV.x);\n"
"   output.Pos = float4(lerp(top_pos, bottom_pos, UV.y), 1);\n"
//ローカル法線の方向にhei分頂点移動
"   output.Pos.xyz += hei * patchL[0].Nor;\n"
//ローカル法線の絶対値が大きい方向に法線ベクトル変換
"   float3 nor1;\n"
"   float absNx = abs(patchL[0].Nor.x);\n"
"   float absNy = abs(patchL[0].Nor.y);\n"
"   float absNz = abs(patchL[0].Nor.z);\n"
"   if(absNx >= absNy && absNx >= absNz){\n"
"     if (patchL[0].Nor.x < 0){\n"
"        nor1.x = -nor.y;\n"
"        nor1.y =  nor.x;\n"
"        nor1.z =  nor.z;\n"
"     }\n"
"     if (patchL[0].Nor.x >= 0){\n"
"        nor1.x =  nor.y;\n"
"        nor1.y = -nor.x;\n"
"        nor1.z =  nor.z;\n"
"     }\n"
"   }\n"
"   if(absNy >= absNx && absNy >= absNz){\n"
"     if (patchL[0].Nor.y < 0){\n"
"        nor1.x = -nor.x;\n"
"        nor1.y = -nor.y;\n"
"        nor1.z =  nor.z;\n"
"     }\n"
"     if (patchL[0].Nor.y >= 0){\n"
"        nor1.x =  nor.x;\n"
"        nor1.y =  nor.y;\n"
"        nor1.z =  nor.z;\n"
"     }\n"
"   }\n"
"   if(absNz >= absNx && absNz >= absNy){\n"
"     if (patchL[0].Nor.z < 0){\n"
"        nor1.x =  nor.x;\n"
"        nor1.y =  nor.z;\n"
"        nor1.z = -nor.y;\n"
"     }\n"
"     if (patchL[0].Nor.z >= 0){\n"
"        nor1.x =  nor.x;\n"
"        nor1.y =  nor.z;\n"
"        nor1.z =  nor.y;\n"
"     }\n"
"   }\n"
"   output.wPos = mul(output.Pos, g_World[patchL[0].instanceID]);\n"
"   output.Pos = mul(output.Pos, g_WVP[patchL[0].instanceID]);\n"

//法線正規化
"   float3 Normal = normalize(nor1);\n"

//出力する法線の作成
"   output.Nor = mul(Normal, (float3x3)g_World[patchL[0].instanceID]);\n"

"	return output;\n"
"}\n"

//ライト無
"[domain(\"quad\")]\n"
"DS_OUTPUT_TC DSDisp(HS_CONSTANT_OUTPUT In, float2 UV : SV_DomaInLocation, const OutputPatch<HS_OUTPUT_TC, 4> patch)\n"
"{\n"
"	DS_OUTPUT_TC output;\n"

//カラー取り出し
"   output.Col = patch[0].Col;\n"

"   float2 top_uv = lerp(patch[0].Tex, patch[1].Tex, UV.x);\n"
"   float2 bottom_uv = lerp(patch[3].Tex, patch[2].Tex, UV.x);\n"
"   float2 uv = float2(lerp(top_uv, bottom_uv, UV.y));\n"
"   output.Tex = uv;\n"

"   float4 height = g_texColor.SampleLevel(g_samLinear, uv, 0) * g_DispAmount.x;\n"
"   float hei = (height.x + height.y + height.z) / 3;\n"

"   float3 top_pos = lerp(patch[0].Pos, patch[1].Pos, UV.x);\n"
"   float3 bottom_pos = lerp(patch[3].Pos, patch[2].Pos, UV.x);\n"
"   output.Pos = float4(lerp(top_pos, bottom_pos, UV.y), 1);\n"
//法線の方向によって頂点移動方向を決める
"   if (patch[0].Nor.x == -1.0f){\n"
"       output.Pos.x -= hei;\n"
"   }\n"
"   if (patch[0].Nor.x ==  1.0f){\n"
"       output.Pos.x += hei;\n"
"   }\n"
"   if (patch[0].Nor.y == -1.0f){\n"
"       output.Pos.y -= hei;\n"
"   }\n"
"   if (patch[0].Nor.y ==  1.0f){\n"
"       output.Pos.y += hei;\n"
"   }\n"
"   if (patch[0].Nor.z == -1.0f){\n"
"       output.Pos.z -= hei;\n"
"   }\n"
"   if (patch[0].Nor.z ==  1.0f){\n"
"       output.Pos.z += hei;\n"
"   }\n"
"   output.wPos = mul(output.Pos, g_World[patch[0].instanceID]);\n"
"   output.Pos = mul(output.Pos, g_WVP[patch[0].instanceID]);\n"

"	return output;\n"
"}\n"
//**************************************ドメインシェーダー*********************************************************************//

//**************************************ピクセルシェーダー********************************************************************//
//ライト有
"float4 PSDispL(DS_OUTPUT_TCL input) : SV_Target\n"
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

//アルファ値退避
"    float a = input.Col.w;\n"
"    float3 Col = { 0.0f, 0.0f, 0.0f };\n"

//ライト計算
"    for (int i = 0; i < g_ShadowLow_Lpcs.y; i++){\n"
//頂点から光源までの距離を計算
"        float distance = length(g_LightPos[i].xyz - input.wPos.xyz);\n"

//ライトオフ, レンジ×3より外は飛ばす
"        if (g_Lightst[i].w == 1.0f && distance < g_Lightst[i].x * 3){\n"

//ライト方向正規化
"            float3 L = normalize(abs(g_LightPos[i].xyz - input.wPos.xyz));\n"

//デフォルト減衰率
"            float attenuation = 2.0f;\n"
//レンジ外減衰率増減適用
"            if (distance > g_Lightst[i].x){ attenuation = g_Lightst[i].z; }\n"
//減衰計算           
"            float r = g_Lightst[i].y / (pow(distance, attenuation) * 0.001f);\n"

//法線,ライト方向から陰影作成, N, Lの内積がg_ShadowLow.x未満の場合g_ShadowLow.xの値が適用される(距離による影は関係無し)
"           Col = Col + max(dot(N, L), g_ShadowLow_Lpcs.x) * input.Col * r * g_LightColor[i];\n"
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
"    return float4(Col, a) * T + g_ObjCol;\n"
"}\n"

//ライト無
"float4 PSDisp(DS_OUTPUT_TC input) : SV_Target\n"
"{\n"
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

"   float4 col = T;\n"
"   return col * input.Col + g_ObjCol;\n"
"}\n";
//**************************************ピクセルシェーダー*******************************************************************//
