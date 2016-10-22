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
//�e�N�X�`���L���̃t���O.x
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

//****************************************���b�V�����_**************************************************************//
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
//****************************************���b�V�����_**************************************************************//

//***************************************�n���V�F�[�_�[�R���X�^���g*************************************************//
"HS_CONSTANT_OUTPUT HSConstant(InputPatch<VS_OUTPUT, 3> ip, uint pid : SV_PrimitiveID)\n"
"{\n"
"	HS_CONSTANT_OUTPUT output;\n"

//���[���h�ϊ�(float3 ���� float4�̕ϊ��Y�ꖳ����)
"   float4 pos = float4(ip[0].Pos, 1.0f);\n"
"   float4 wPos = mul(pos, g_World[ip[0].instanceID]);\n"
//���_���猻�ݒn�܂ł̋������v�Z
"   float distance = length(g_C_Pos.xyz - wPos.xyz);\n"

//�����Ń|���S��������
"   float divide = 1;\n"
//�e�N�X�`���L�̏ꍇ�|���S�����B
"    if(g_Tex_f.x == 1.0f)\n"
"    {\n"
"       if(distance < 1000){divide = 2;}\n"
"       if(distance < 500){divide = 5;}\n"
"       if(distance < 300){divide = 10;}\n"
"    }\n"

"	output.factor[0] = divide;\n"
"	output.factor[1] = divide;\n"
"	output.factor[2] = divide;\n"
//u �c�̕������i���̃��C�������{�Ђ����j
"	output.inner_factor = divide;\n"

"	return output;\n"
"}\n"
//***************************************�n���V�F�[�_�[�R���X�^���g*************************************************//

//***************************************�n���V�F�[�_�[*************************************************************//
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
//***************************************�n���V�F�[�_�[*************************************************************//

//**************************************�h���C���V�F�[�_�[**********************************************************//
"[domain(\"tri\")]\n"
"DS_OUTPUT DSMesh(HS_CONSTANT_OUTPUT In, float3 UV : SV_DomaInLocation, const OutputPatch<HS_OUTPUT, 3> patch)\n"
"{\n"
"	DS_OUTPUT output;\n"

//�J���[���o��
"   output.Col = patch[0].Col;\n"

//UV���W�v�Z
"   float2 uv = patch[0].Tex * UV.x + patch[1].Tex * UV.y + patch[2].Tex * UV.z;\n"
"   output.Tex = uv;\n"

"   float4 height;\n"
"   float hei;\n"
//�e�N�X�`���L�̏ꍇ
"   if(g_Tex_f.x == 1.0f)\n"
"   {\n"
//�摜���獂�����Z�o
"      height = g_texColor.SampleLevel(g_samLinear, uv, 0) * g_DispAmount.x;\n"
"      hei = (height.x + height.y + height.z) / 3;\n"
"   }\n"

//pos���W�v�Z
"   float3 pos = patch[0].Pos * UV.x + patch[1].Pos * UV.y + patch[2].Pos * UV.z;\n"
"   output.Pos = float4(pos, 1);\n"

//���e�N�X�`�����̏ꍇ���̖@�����̂܂܎g�p
"   float4 nor = float4(patch[0].Nor, 0);\n"
//�e�N�X�`���L�̏ꍇ
"   if(g_Tex_f.x == 1.0f)\n"
"   {\n"
//���[�J���@���̕�����hei�����_�ړ�
"      output.Pos.xyz += hei * patch[0].Nor;\n"
//�摜����@���v�Z
"      nor = g_texColor.SampleLevel(g_samLinear, uv, 0) * 2 - 1;\n"
//���[�J���@���̐�Βl���傫�������ɖ@���x�N�g���ϊ�
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

//���W�ϊ�
"   output.wPos = mul(output.Pos, g_World[patch[0].instanceID]);\n"
"   output.Pos = mul(output.Pos, g_WVP[patch[0].instanceID]);\n"

//�o�͂���@���̍쐬
"   output.Nor = mul(Normal, (float3x3)g_World[patch[0].instanceID]);\n"

"	return output;\n"
"}\n"
//**************************************�h���C���V�F�[�_�[**********************************************************//

//****************************************���b�V���s�N�Z��**********************************************************//
"float4 PSMesh(DS_OUTPUT input) : SV_Target\n"
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
"       if(g_Tex_f.x == 1.0f && T.w > 0.3f){\n"
"         T = ff * T + (1.0f - ff) * g_FogColor;\n"
"       }\n"
"       if(g_Tex_f.x == 0.0f && C.w > 0.3f){\n"
"         C = ff * C + (1.0f - ff) * g_FogColor;\n"
"       }\n"
"    }\n"

//�A���t�@�l�ޔ�
"    float a = C.w;\n"
"    float3 Col = { 0.0f, 0.0f, 0.0f };\n"

//���C�g�v�Z
"    for (int i = 0; i < g_ShadowLow_Lpcs.y; i++){\n"
//���_��������܂ł̋������v�Z
"         float distance = length(g_LightPos[i].xyz - input.wPos.xyz);\n"

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
//�e�N�X�`���L
"    if(g_Tex_f.x == 1.0f)\n"
"    {\n"
"       color = float4(Col, a) * T + g_ObjCol;\n"
"    }\n"
//�e�N�X�`����
"    if (g_Tex_f.x == 0.0f)\n"
"    {\n"
"       color = float4(Col, a) + g_ObjCol;\n"
"    }\n"
"    return color;\n"
"}\n";
//****************************************���b�V���s�N�Z��**********************************************************//
