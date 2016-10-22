///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           ShaderMESH_D.hlsl                                           //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

char *ShaderMesh_D =
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
//テクスチャ有無のフラグ.x
"    float4 g_Tex_f;\n"
"};\n"

"struct VS_OUTPUT\n"
"{\n"
"    float3 Pos        : POSITION;\n"
"    float3 Nor        : NORMAL;\n"
"    float4 Col        : COLOR0;\n"
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
"    float4 Col        : COLOR;\n"
"    float2 Tex        : TEXCOORD;\n"
"    uint   instanceID : SV_InstanceID;\n"
"};\n"

"struct DS_OUTPUT\n"
"{\n"
"    float4 Pos  : SV_POSITION;\n"
"    float4 wPos : POSITION;\n"
"    float3 Nor  : NORMAL;\n"
"    float4 Col  : COLOR;\n"
"    float2 Tex  : TEXCOORD;\n"
"};\n"

//****************************************メッシュ頂点**************************************************************//
"VS_OUTPUT VSMesh(float3 Pos : POSITION, float3 Nor : NORMAL, float2 Tex : TEXCOORD, uint instanceID : SV_InstanceID)\n"
"{\n"
"    VS_OUTPUT output = (VS_OUTPUT)0;\n"
"    output.Pos = Pos;\n"
"    output.Nor = Nor;\n"
"    output.Tex = Tex;\n"
"    output.Col = g_Diffuse;\n"
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
//テクスチャ有の場合ポリゴン増殖
"    if(g_Tex_f.x == 1.0f)\n"
"    {\n"
"       if(distance < 1000){divide = 2;}\n"
"       if(distance < 500){divide = 5;}\n"
"       if(distance < 300){divide = 10;}\n"
"    }\n"

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
"	output.Col = ip[cpid].Col;\n"
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

//カラー取り出し
"   output.Col = patch[0].Col;\n"

//UV座標計算
"   float2 uv = patch[0].Tex * UV.x + patch[1].Tex * UV.y + patch[2].Tex * UV.z;\n"
"   output.Tex = uv;\n"

"   float4 height;\n"
"   float hei;\n"
//テクスチャ有の場合
"   if(g_Tex_f.x == 1.0f)\n"
"   {\n"
//画像から高さを算出
"      height = g_texColor.SampleLevel(g_samLinear, uv, 0) * g_DispAmount.x;\n"
"      hei = (height.x + height.y + height.z) / 3;\n"
"   }\n"

//pos座標計算
"   float3 pos = patch[0].Pos * UV.x + patch[1].Pos * UV.y + patch[2].Pos * UV.z;\n"
"   output.Pos = float4(pos, 1);\n"

//↓テクスチャ無の場合この法線そのまま使用
"   float4 nor = float4(patch[0].Nor, 0);\n"
//テクスチャ有の場合
"   if(g_Tex_f.x == 1.0f)\n"
"   {\n"
//ローカル法線の方向にhei分頂点移動
"      output.Pos.xyz += hei * patch[0].Nor;\n"
//画像から法線計算
"      nor = g_texColor.SampleLevel(g_samLinear, uv, 0) * 2 - 1;\n"
//ローカル法線の絶対値が大きい方向に法線ベクトル変換
"      float3 nor1;\n"
"      float absNx = abs(patch[0].Nor.x);\n"
"      float absNy = abs(patch[0].Nor.y);\n"
"      float absNz = abs(patch[0].Nor.z);\n"
"      if(absNx >= absNy && absNx >= absNz){\n"
"        if (patch[0].Nor.x < 0){\n"
"          nor1.x = -nor.y;\n"
"          nor1.y =  nor.x;\n"
"          nor1.z =  nor.z;\n"
"        }\n"
"        if (patch[0].Nor.x >= 0){\n"
"          nor1.x =  nor.y;\n"
"          nor1.y = -nor.x;\n"
"          nor1.z =  nor.z;\n"
"        }\n"
"      }\n"
"      if(absNy >= absNx && absNy >= absNz){\n"
"        if (patch[0].Nor.y < 0){\n"
"          nor1.x = -nor.x;\n"
"          nor1.y = -nor.y;\n"
"          nor1.z =  nor.z;\n"
"        }\n"
"        if (patch[0].Nor.y >= 0){\n"
"          nor1.x =  nor.x;\n"
"          nor1.y =  nor.y;\n"
"          nor1.z =  nor.z;\n"
"        }\n"
"      }\n"
"      if(absNz >= absNx && absNz >= absNy){\n"
"        if (patch[0].Nor.z < 0){\n"
"          nor1.x =  nor.x;\n"
"          nor1.y =  nor.z;\n"
"          nor1.z = -nor.y;\n"
"        }\n"
"        if (patch[0].Nor.z >= 0){\n"
"          nor1.x =  nor.x;\n"
"          nor1.y =  nor.z;\n"
"          nor1.z =  nor.y;\n"
"        }\n"
"      }\n"
"      nor.xyz = nor1;\n"
"   }\n"

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
"       if(g_Tex_f.x == 1.0f && T.w > 0.3f){\n"
"         T = ff * T + (1.0f - ff) * g_FogColor;\n"
"       }\n"
"       if(g_Tex_f.x == 0.0f && C.w > 0.3f){\n"
"         C = ff * C + (1.0f - ff) * g_FogColor;\n"
"       }\n"
"    }\n"

//アルファ値退避
"    float a = C.w;\n"
"    float3 Col = { 0.0f, 0.0f, 0.0f };\n"

//ライト計算
"    for (int i = 0; i < g_ShadowLow_Lpcs.y; i++){\n"
//頂点から光源までの距離を計算
"         float distance = length(g_LightPos[i].xyz - input.wPos.xyz);\n"

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
//テクスチャ有
"    if(g_Tex_f.x == 1.0f)\n"
"    {\n"
"       color = float4(Col, a) * T + g_ObjCol;\n"
"    }\n"
//テクスチャ無
"    if (g_Tex_f.x == 0.0f)\n"
"    {\n"
"       color = float4(Col, a) + g_ObjCol;\n"
"    }\n"
"    return color;\n"
"}\n";
//****************************************メッシュピクセル**********************************************************//
