///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           ShaderMESH_D.hlsl                                           //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

//ShaderFunction.h�ɘA�������Ďg��
char *ShaderMesh_D =
"cbuffer global_1:register(b1)\n"
"{\n"
//�}�e���A�����̐F
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

//****************************************���b�V�����_**************************************************************//
"VS_OUTPUT VSMesh(float3 Pos : POSITION, float3 Nor : NORMAL, float2 Tex : TEXCOORD, uint instanceID : SV_InstanceID)\n"
"{\n"
"    VS_OUTPUT output = (VS_OUTPUT)0;\n"
"    output.Pos = Pos;\n"
"    output.Nor = Nor;\n"
"    output.Tex = Tex;\n"
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

"   if(distance < 1000){divide = 2;}\n"
"   if(distance < 500){divide = 5;}\n"
"   if(distance < 300){divide = 10;}\n"

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

//UV���W�v�Z
"   float2 uv = patch[0].Tex * UV.x + patch[1].Tex * UV.y + patch[2].Tex * UV.z;\n"
"   output.Tex = uv;\n"

"   float4 height;\n"
"   float hei;\n"

//�摜���獂�����Z�o
"   height = g_texColor.SampleLevel(g_samLinear, uv, 0) * g_DispAmount.x;\n"
"   hei = (height.x + height.y + height.z) / 3;\n"

//pos���W�v�Z
"   float3 pos = patch[0].Pos * UV.x + patch[1].Pos * UV.y + patch[2].Pos * UV.z;\n"
"   output.Pos = float4(pos, 1);\n"

"   float4 nor = float4(patch[0].Nor, 0);\n"

//���[�J���@���̕�����hei�����_�ړ�, �|���S�����؂��̂Ńp�b�`�̒[�͈ړ������Ȃ�
"   if(UV.x != 1.0f && UV.x != 0.0f && UV.y != 1.0f && UV.y != 0.0f && UV.z != 1.0f && UV.z != 0.0f)\n"
"   {\n"
"      output.Pos.xyz += hei * patch[0].Nor;\n"
"   }\n"
//�摜����@���v�Z�p�x�N�g������
"   nor = g_texColor.SampleLevel(g_samLinear, uv, 0) * 2 - 1;\n"
//�摜���琶�������x�N�g���Ƀ��[�J���@���𑫂��@���x�N�g���Ƃ���
"   float3 nor1;\n"
"   nor1 = nor.xyz + patch[0].Nor;\n"
"   nor.xyz = nor1;\n"

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
"        Col = Col + PointLightCom(C, N, g_ShadowLow_Lpcs, g_LightPos[i], input.wPos, g_Lightst[i], g_LightColor[i]);\n"
"    }\n"

//���s�����v�Z
"    Col = Col + DirectionalLightCom(input.Nor, g_DLightst, g_DLightDirection, g_DLightColor);\n"

"    float4 color;\n"
"    color = float4(Col, a) * T + g_ObjCol;\n"
"    return color;\n"
"}\n";
//****************************************���b�V���s�N�Z��**********************************************************//
