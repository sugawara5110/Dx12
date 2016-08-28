///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           Shader3D.hlsl                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

char *Shader3D =
"Texture2D g_texColor : register(t0);\n"
"SamplerState g_samLinear : register(s0);\n"

"cbuffer global  : register(b0)\n"
"{\n"
"    matrix g_World[150];\n"
"    matrix g_WVP[150];\n"
//���݈ʒu
"    float4 g_C_Pos;\n"
//�I�u�W�F�N�g�ǉ��J���[
"    float4 g_ObjCol;\n"
//�_�����ʒu
"    float4 g_LightPos[150];\n"
//�_���C�g�F
"    float4 g_LightColor[150];\n"
//�_���C�g�����W, ���邳, �����̑傫��, �I���I�t
"    float4 g_Lightst[150];\n"
//�_���C�g�e�̉����lx, ���C�g��y, ���C�g�L��z
"    float4 g_ShadowLow_Lpcs;\n"
//���s��������
"    float4 g_DLightDirection;\n"
//���s�����F
"    float4 g_DLightColor;\n"
//���s�������邳x,�I���I�ty, �e�̉����lz
"    float4 g_DLightst;\n"
//�t�H�O��x, ���xy, onoffz
"    float4 g_FogAmo_Density;\n"
//�t�H�O�F
"    float4 g_FogColor;\n"
//�f�B�X�v�N����x
"    float4 g_DispAmount;\n"
//UV���W�ړ��p
"    float4 g_pXpYmXmY;\n"
"};\n"

"struct VS_OUTPUT_TCL\n"
"{\n"
"    float4 Pos        : SV_POSITION;\n"
"    float4 wPos       : POSITION;\n"
"    float3 Nor        : NORMAL;\n"
"    float4 Col        : COLOR;\n"
"    float2 Tex        : TEXCOORD;\n"
"};\n"

"struct VS_OUTPUT_TC\n"
"{\n"
"    float4 Pos        : SV_POSITION;\n"
"    float4 wPos       : POSITION;\n"
"    float4 Col        : COLOR;\n"
"    float2 Tex        : TEXCOORD;\n"
"};\n"

"struct VS_OUTPUT_BC\n"
"{\n"
"    float4 Pos        : SV_POSITION;\n"
"    float4 Col        : COLOR;\n"
"};\n"

//****************************************�e�N�X�`�����_**************************************************************//
//���C�g�L
"VS_OUTPUT_TCL VSTextureColorL(float4 Pos : POSITION, float4 Nor : NORMAL, float4 Col : COLOR, float2 Tex : TEXCOORD, uint instanceID : SV_InstanceID)\n"
"{\n"
"    VS_OUTPUT_TCL output = (VS_OUTPUT_TCL)0;\n"
"    output.Pos = mul(Pos, g_WVP[instanceID]);\n"
"    output.wPos = mul(Pos, g_World[instanceID]);\n"
"    output.Nor = mul(Nor, (float3x3)g_World[instanceID]);\n"
"    output.Col = Col;\n"
"    output.Tex.x = Tex.x * g_pXpYmXmY.x + g_pXpYmXmY.x * g_pXpYmXmY.z;\n"
"    output.Tex.y = Tex.y * g_pXpYmXmY.y + g_pXpYmXmY.y * g_pXpYmXmY.w;\n"

"    return output;\n"
"}\n"

//���C�g��
"VS_OUTPUT_TC VSTextureColor(float4 Pos : POSITION, float4 Col : COLOR, float2 Tex : TEXCOORD, uint instanceID : SV_InstanceID)\n"
"{\n"
"    VS_OUTPUT_TC output = (VS_OUTPUT_TC)0;\n"
"    output.Pos = mul(Pos, g_WVP[instanceID]);\n"
"    output.wPos = mul(Pos, g_World[instanceID]);\n"
"    output.Col = Col;\n"
"    output.Tex.x = Tex.x * g_pXpYmXmY.x + g_pXpYmXmY.x * g_pXpYmXmY.z;\n"
"    output.Tex.y = Tex.y * g_pXpYmXmY.y + g_pXpYmXmY.y * g_pXpYmXmY.w;\n"

"    return output;\n"
"}\n"
//****************************************�e�N�X�`�����_**************************************************************//

//****************************************�e�N�X�`���s�N�Z��**********************************************************//
//���C�g�L
"float4 PSTextureColorL(VS_OUTPUT_TCL input) : SV_Target\n"
"{\n"
//�@�����K��
"    float3 N = normalize(input.Nor);\n"

//�e�N�X�`��
"    float4 T = g_texColor.Sample(g_samLinear, input.Tex);\n"

//�t�H�O�v�Z�e�N�X�`���ɑ΂��Čv�Z
"    float fd;\n"//����
"    float ff;\n"//�t�H�O�t�@�N�^�[
"    if(g_FogAmo_Density.z == 1.0f){\n"
"       fd = length(g_C_Pos.xyz - input.wPos.xyz) * 0.01f;\n"//�����v�Z, 0.01�͕␳�l
"       ff = pow(2.71828, -fd * g_FogAmo_Density.y);\n"//�t�H�O�t�@�N�^�[�v�Z(�ω���)
"       ff *= g_FogAmo_Density.x;\n"//�t�H�O�S�̗̂�(���������������Ȃ�)
"       ff = saturate(ff);\n"
"       if(T.w > 0.3f){\n"
"          T = ff * T + (1.0f - ff) * g_FogColor;\n"
"       }\n"
"    }\n"

//�A���t�@�l�ޔ�
"    float a = input.Col.w;\n"
"    float3 Col = { 0.0f, 0.0f, 0.0f };\n"

//���C�g�v�Z
"    for (int i = 0; i < g_ShadowLow_Lpcs.y; i++){\n"
//���_��������܂ł̋������v�Z
"        float distance = length(g_LightPos[i].xyz - input.wPos.xyz);\n"

//���C�g�I�t, �����W�~3���O�͔�΂�
"        if (g_Lightst[i].w == 1.0f && distance < g_Lightst[i].x * 3){\n"

//���C�g�������K��
"            float3 L = normalize(abs(g_LightPos[i].xyz - input.wPos.xyz));\n"

//�f�t�H���g������
"            float attenuation = 2.0f;\n"
//�����W�O�����������K�p
"            if (distance > g_Lightst[i].x){ attenuation = g_Lightst[i].z; }\n"
//�����v�Z           
"            float r = g_Lightst[i].y / (pow(distance, attenuation) * 0.001f);\n"

//�@��,���C�g��������A�e�쐬, N, L�̓��ς�g_ShadowLow.x�����̏ꍇg_ShadowLow.x�̒l���K�p�����(�����ɂ��e�͊֌W����)
"           Col = Col + max(dot(N, L), g_ShadowLow_Lpcs.x) * input.Col * r * g_LightColor[i];\n"
"        }\n"
"    }\n"

//���s�����v�Z
"    float3 D_LightDir;\n"
"    float NL;\n"
"    if(g_DLightst.y == 1.0f)\n"
"    {\n"
"       D_LightDir = normalize(g_DLightDirection);\n"
"       NL = max(saturate(dot(input.Nor, D_LightDir)), g_DLightst.z);\n"
"       Col = Col + g_DLightColor * g_DLightst.x * NL;\n"
"    }\n"
"    if(T.w <= 0.0f)discard;\n"//�A���t�@�l0�̏ꍇ�s�N�Z���j��
"    return float4(Col, a) * T + g_ObjCol;\n"
"}\n"

//���C�g��
"float4 PSTextureColor(VS_OUTPUT_TC input) : SV_Target\n"
"{\n"
//�e�N�X�`��
"    float4 T = g_texColor.Sample(g_samLinear, input.Tex);\n"

//�t�H�O�v�Z�e�N�X�`���ɑ΂��Čv�Z
"    float fd;\n"//����
"    float ff;\n"//�t�H�O�t�@�N�^�[
"    if(g_FogAmo_Density.z == 1.0f){\n"
"       fd = length(g_C_Pos.xyz - input.wPos.xyz) * 0.01f;\n"//�����v�Z, 0.01�͕␳�l
"       ff = pow(2.71828, -fd * g_FogAmo_Density.y);\n"//�t�H�O�t�@�N�^�[�v�Z(�ω���)
"       ff *= g_FogAmo_Density.x;\n"//�t�H�O�S�̗̂�(���������������Ȃ�)
"       ff = saturate(ff);\n"
"       if(T.w > 0.3f){\n"
"          T = ff * T + (1.0f - ff) * g_FogColor;\n"
"       }\n"
"    }\n"
"    if(T.w <= 0.0f)discard;\n"//�A���t�@�l0�̏ꍇ�s�N�Z���j��
"    return input.Col * T + g_ObjCol;\n"
"}\n"
//****************************************�e�N�X�`���s�N�Z��**********************************************************//

//****************************************��{�F���_******************************************************************//
"VS_OUTPUT_BC VSBaseColor(float4 Pos : POSITION, float4 Col : COLOR, uint instanceID : SV_InstanceID)\n"
"{\n"
"    VS_OUTPUT_BC output = (VS_OUTPUT_BC)0;\n"
"    output.Pos = mul(Pos, g_WVP[instanceID]);\n"

"    output.Col = Col;\n"

"    return output;\n"
"}\n"
//****************************************��{�F���_******************************************************************//

//****************************************��{�F�s�N�Z��**************************************************************//
"float4 PSBaseColor(VS_OUTPUT_BC input) : SV_Target\n"
"{\n"
"   return input.Col + g_ObjCol;\n"
"}\n";
//****************************************��{�F�s�N�Z��**************************************************************//
