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
//���݈ʒu
"    float4 g_C_Pos;\n"
//�I�u�W�F�N�g�ǉ��J���[
"    float4 g_ObjCol;\n"
//�����ʒu
"    float4 g_LightPos[150];\n"
//���C�g�F
"    float4 g_LightColor[150];\n"
//�����W, ���邳, �����̑傫��, �I���I�t
"    float4 g_Lightst[150];\n"
//�e�̉����lx, ���C�g��y
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

"cbuffer global_1:register(b1)\n"
"{\n"
//�}�e���A�����̐F
"    float4 g_Diffuse;\n"
"};\n"

"cbuffer global_bones : register(b2)\n"//�{�[���̃|�[�Y�s�񂪓���
"{\n"
"   matrix g_mConstBoneWorld[150];\n"
"};\n"

//�X�L�j���O��̒��_�E�@��������
"struct Skin\n"
"{\n"
"   float4 Pos : POSITION;\n"
"   float3 Nor : NORMAL;\n"
"};\n"
//�o�[�e�b�N�X�o�b�t�@�[�̓���
"struct VSSkinIn\n"
"{\n"
"   float3 Pos : POSITION;\n"//�ʒu   
"   float3 Nor : NORMAL;\n"//���_�@��
"   float2 Tex : TEXCOORD;\n"//�e�N�X�`���[���W
"   uint4  Bones : BONE_INDEX;\n"//�{�[���̃C���f�b�N�X
"   float4 Weights : BONE_WEIGHT;\n"//�{�[���̏d��
"};\n"

"struct VS_OUTPUT\n"
"{\n"
"    float4 Pos  : SV_POSITION;\n"
"    float4 wPos : POSITION;\n"
"    float3 Nor  : NORMAL;\n"
"    float2 Tex  : TEXCOORD;\n"
"    float4 Col  : COLOR0;\n"
"};\n"

//�w�肵���ԍ��̃{�[���̃|�[�Y�s���Ԃ��@�T�u�֐��i�o�[�e�b�N�X�V�F�[�_�[�Ŏg�p�j
"matrix FetchBoneMatrix(uint iBone)\n"
"{\n"
"   return g_mConstBoneWorld[iBone];\n"
"}\n"

//���_���X�L�j���O
"Skin SkinVert(VSSkinIn Input)\n"
"{\n"
"   Skin Output = (Skin)0;\n"

"   float4 Pos = float4(Input.Pos, 1);\n"
"   Pos.x *= -1;\n"//FBX�͉E����W�n�Ȃ̂�x���邢�͂��𔽓]
"   float3 Nor = Input.Nor;\n"
//�{�[��0
"   uint iBone = Input.Bones.x;\n"
"   float fWeight = Input.Weights.x;\n"
"   matrix m = FetchBoneMatrix(iBone);\n"
"   Output.Pos += fWeight * mul(Pos, m);\n"
"   Output.Nor += fWeight * mul(Nor, (float3x3)m);\n"
//�{�[��1
"   iBone = Input.Bones.y;\n"
"   fWeight = Input.Weights.y;\n"
"   m = FetchBoneMatrix(iBone);\n"
"   Output.Pos += fWeight * mul(Pos, m);\n"
"   Output.Nor += fWeight * mul(Nor, (float3x3)m);\n"
//�{�[��2
"   iBone = Input.Bones.z;\n"
"   fWeight = Input.Weights.z;\n"
"   m = FetchBoneMatrix(iBone);\n"
"   Output.Pos += fWeight * mul(Pos, m);\n"
"   Output.Nor += fWeight * mul(Nor, (float3x3)m);\n"
//�{�[��3
"   iBone = Input.Bones.w;\n"
"   fWeight = Input.Weights.w;\n"
"   m = FetchBoneMatrix(iBone);\n"
"   Output.Pos += fWeight * mul(Pos, m);\n"
"   Output.Nor += fWeight * mul(Nor, (float3x3)m);\n"

"   return Output;\n"
"}\n"

//****************************************���b�V�����_**************************************************************//
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
//****************************************���b�V�����_**************************************************************//

//****************************************���b�V���s�N�Z��**********************************************************//
"float4 PSSkin(VS_OUTPUT input) : SV_Target\n"
"{\n"
//�@�����K��
"    float3 N = normalize(input.Nor);\n"
//�e�N�X�`��
"    float4 T = g_texColor.Sample(g_samLinear, input.Tex);\n"
//��{�J���[
"    float4 C = input.Col;\n"

//�t�H�O�v�Z
"    float fd;\n"//����
"    float ff;\n"//�t�H�O�t�@�N�^�[
"    if(g_FogAmo_Density.z == 1.0f){\n"
"       fd = length(g_C_Pos.xyz - input.wPos.xyz) * 0.01f;\n"//�����v�Z, 0.01�͕␳�l
"       ff = pow(2.71828, -fd * g_FogAmo_Density.y);\n"//�t�H�O�t�@�N�^�[�v�Z(�ω���)
"       ff *= g_FogAmo_Density.x;\n"//�t�H�O�S�̗̂�(���������������Ȃ�)
"       ff = saturate(ff);\n"
"       if(T.w > 0.3f){\n"
"         T = ff * T + (1.0f - ff) * g_FogColor;\n"
"       }\n"
"    }\n"

//�A���t�@�l�ޔ�
"    float a = C.w;\n"
"    float3 Col = { 0.0f, 0.0f, 0.0f };\n"

//���C�g�v�Z
"    for (int i = 0; i < g_ShadowLow_Lpcs.y; i++){\n"
//���_��������܂ł̋������v�Z
"        float distance = length(g_LightPos[i].xyz - input.wPos.xyz);\n"

//���C�g�I�t, �����W�~3���O�͔�΂�
"        if (g_Lightst[i].w == 1.0f && distance < g_Lightst[i].x * 3){\n"

//���C�g�������K��
"           float3 L = normalize(abs(g_LightPos[i].xyz - input.wPos.xyz));\n"

//�f�t�H���g������
"            float attenuation = 2.0f;\n"
//�����W�O�����������K�p
"            if (distance > g_Lightst[i].x){ attenuation = g_Lightst[i].z; }\n"
//�����v�Z           
"            float r = g_Lightst[i].y / (pow(distance, attenuation) * 0.001f);\n"

//�@��,���C�g��������A�e�쐬, N, L�̓��ς�g_ShadowLow.x�����̏ꍇg_ShadowLow.x�̒l���K�p�����(�����ɂ��e�͊֌W����)
"           Col = Col + max(dot(N, L), g_ShadowLow_Lpcs.x) * C * r * g_LightColor[i];\n"
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

"    float4 color;\n"
"    color = float4(Col, a) * T + g_ObjCol;\n"
"    return color;\n"
"}\n";
//****************************************���b�V���s�N�Z��**********************************************************//
