///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           ShaderMESH.hlsl                                             //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

//ShaderFunction.h�ɘA�������Ďg��
char *ShaderMesh =
"cbuffer global_1:register(b1)\n"
"{\n"
//�}�e���A�����̐F
"    float4 g_Diffuse;\n"
"    float4 g_Speculer; \n"
"};\n"

"struct VS_OUTPUT\n"
"{\n"
"    float4 Pos  : SV_POSITION;\n"
"    float4 wPos : POSITION;\n"
"    float3 Nor  : NORMAL;\n"
"    float2 Tex  : TEXCOORD;\n"
"};\n"

//****************************************���b�V�����_**************************************************************//
"VS_OUTPUT VSMesh(float4 Pos : POSITION, float4 Nor : NORMAL, float2 Tex : TEXCOORD, uint instanceID : SV_InstanceID)\n"
"{\n"
"    VS_OUTPUT output = (VS_OUTPUT)0;\n"
"    output.Pos = mul(Pos, g_WVP[instanceID]);\n"
"    output.wPos = mul(Pos, g_World[instanceID]);\n"
"    output.Nor = mul(Nor.xyz, (float3x3)g_World[instanceID]);\n"
"    output.Tex = Tex;\n"

"    return output;\n"
"}\n"
//****************************************���b�V�����_**************************************************************//

//****************************************���b�V���s�N�Z��**********************************************************//
"float4 PSMesh(VS_OUTPUT input) : SV_Target\n"
"{\n"
//�@�����K��
"    float3 N = normalize(input.Nor);\n"
//�e�N�X�`��
"    float4 T1 = g_texColor.Sample(g_samLinear, input.Tex);\n"
//��{�J���[
"    float4 C = g_Diffuse;\n"

//�t�H�O�v�Z�e�N�X�`���ɑ΂��Čv�Z
"    float4 T = FogCom(g_FogColor, g_FogAmo_Density, g_C_Pos, input.wPos, T1);\n"

//�A���t�@�l�ޔ�
"    float a = C.w;\n"

//���C�g�v�Z
"    float3 Col = { 0.0f, 0.0f, 0.0f };\n"
"    for (int i = 0; i < g_ShadowLow_Lpcs.y; i++){\n"
"        Col = Col + PointLightCom(g_Speculer, C, N, g_ShadowLow_Lpcs, g_LightPos[i], input.wPos, g_Lightst[i], g_LightColor[i], g_C_Pos);\n"
"    }\n"

//���s�����v�Z
"    Col = Col + DirectionalLightCom(g_Speculer, C, N, g_DLightst, g_DLightDirection, g_DLightColor, input.wPos, g_C_Pos);\n"

"    float4 color = float4(Col, a) * T + g_ObjCol;\n"
"    return color;\n"
"}\n";
//****************************************���b�V���s�N�Z��**********************************************************//
