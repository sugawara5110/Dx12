///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           ShaderDisp.hlsl                                             //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

//ShaderFunction.h�ɘA�������Ďg��
char *ShaderDisp =
"Texture2D g_texNormal : register(t1);\n"
"struct VS_OUTPUT_TC\n"
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

"struct HS_OUTPUT_TC\n"
"{\n"
"	 float3 Pos        : POSITION;\n"
"    float3 Nor        : NORMAL;\n"
"    float2 Tex        : TEXCOORD;\n"
"    uint   instanceID : SV_InstanceID;\n"
"};\n"

"struct DS_OUTPUT_TC\n"
"{\n"
"	 float4 Pos  : SV_POSITION;\n"
"    float4 wPos : POSITION;\n"
"    float3 Nor  : NORMAL;\n"
"    float2 Tex  : TEXCOORD;\n"
"};\n"

//*********************************************���_�V�F�[�_�[*******************************************************************//
"VS_OUTPUT_TC VSDisp(float3 Pos : POSITION, float3 Nor : NORMAL, float2 Tex : TEXCOORD, uint instanceID : SV_InstanceID)\n"
"{\n"
"    VS_OUTPUT_TC output = (VS_OUTPUT_TC)0;\n"
"    output.Pos = Pos;\n"
"    output.Nor = Nor;\n"
"    output.Tex = Tex;\n"
"    output.instanceID = instanceID;\n"
"    return output;\n"
"}\n"
//*********************************************���_�V�F�[�_�[*******************************************************************//

//***************************************�n���V�F�[�_�[�R���X�^���g*************************************************************//
"HS_CONSTANT_OUTPUT HSConstant(InputPatch<VS_OUTPUT_TC, 4> ip, uint pid : SV_PrimitiveID)\n"
"{\n"
"	HS_CONSTANT_OUTPUT output = (HS_CONSTANT_OUTPUT)0;\n"

//���[���h�ϊ�
"   float3 wPos = mul(ip[0].Pos, (float3x3)g_World[ip[0].instanceID]);\n"
//���_���猻�ݒn�܂ł̋������v�Z
"   float distance = length(g_C_Pos.xyz - wPos);\n"

//�����Ń|���S��������
"   float divide = 1;\n"
"   if(distance < 1000){divide = 2;}\n"
"   if(distance < 500){divide = 50;}\n"
"   if(distance < 300){divide = 80;}\n"

"	output.factor[0] = divide;\n"
"	output.factor[1] = divide;\n"
"	output.factor[2] = divide;\n"
"	output.factor[3] = divide;\n"
//u �c�̕������i���̃��C�������{�Ђ����j
"	output.inner_factor[0] = divide;\n"
//v
"	output.inner_factor[1] = divide;\n"

"	return output;\n"
"}\n"
//***************************************�n���V�F�[�_�[�R���X�^���g*************************************************************//

//***************************************�n���V�F�[�_�[*************************************************************************//
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
"	output.Tex = ip[cpid].Tex;\n"
"   output.instanceID = ip[cpid].instanceID;\n"
"	return output;\n"
"}\n"
//***************************************�n���V�F�[�_�[*************************************************************************//

//**************************************�h���C���V�F�[�_�[*********************************************************************//
"[domain(\"quad\")]\n"
"DS_OUTPUT_TC DSDisp(HS_CONSTANT_OUTPUT In, float2 UV : SV_DomaInLocation, const OutputPatch<HS_OUTPUT_TC, 4> patch)\n"
"{\n"
"	DS_OUTPUT_TC output = (DS_OUTPUT_TC)0;\n"

//UV���W�v�Z
"   float2 top_uv = lerp(patch[0].Tex, patch[1].Tex, UV.x);\n"
"   float2 bottom_uv = lerp(patch[3].Tex, patch[2].Tex, UV.x);\n"
"   float2 uv = float2(lerp(top_uv, bottom_uv, UV.y));\n"
"   output.Tex = uv;\n"
//�摜���獂�����Z�o
"   float4 texheight = g_texColor.SampleLevel(g_samLinear, uv, 0);\n"
"   float4 height = texheight * g_DispAmount.x;\n"
"   float hei = (height.x + height.y + height.z) / 3;\n"
//�摜����@���v�Z�p�x�N�g������
"   float4 nor = texheight * 2 - 1;\n"//-1.0�`1.0�ɂ����
//pos���W�v�Z
"   float3 top_pos = lerp(patch[0].Pos, patch[1].Pos, UV.x);\n"
"   float3 bottom_pos = lerp(patch[3].Pos, patch[2].Pos, UV.x);\n"
"   output.Pos = float4(lerp(top_pos, bottom_pos, UV.y), 1);\n"
//���[�J���@���̕�����hei�����_�ړ�
"   output.Pos.xyz += hei * patch[0].Nor;\n"
//�摜���琶�������x�N�g���Ƀ��[�J���@���𑫂��@���x�N�g���Ƃ���
"   float3 nor1 = nor.xyz + patch[0].Nor;\n"
"   output.wPos = mul(output.Pos, g_World[patch[0].instanceID]);\n"
"   output.Pos = mul(output.Pos, g_WVP[patch[0].instanceID]);\n"

//�@�����K��
"   float3 Normal = normalize(nor1);\n"

//�o�͂���@���̍쐬
"   output.Nor = mul(Normal, (float3x3)g_World[patch[0].instanceID]);\n"

"	return output;\n"
"}\n"
//**************************************�h���C���V�F�[�_�[*********************************************************************//

//**************************************�s�N�Z���V�F�[�_�[********************************************************************//
////////////////////////////////////////////���C�g�L/////////////////////////////////////////////////////////////////
"float4 PSDispL(DS_OUTPUT_TC input) : SV_Target\n"
"{\n"
//�@�����K��
"    float3 N = normalize(input.Nor);\n"
//�e�N�X�`��
"    float4 T1 = g_texColor.Sample(g_samLinear, input.Tex);\n"

//�t�H�O�v�Z(�e�N�X�`���ɑ΂��Čv�Z)
"    float4 T = FogCom(g_FogColor, g_FogAmo_Density, g_C_Pos, input.wPos, T1);\n"

//���C�g�v�Z
"    float4 C = { 1.0f, 1.0f, 1.0f, 1.0f};\n"
"    float3 Col = { 0.0f, 0.0f, 0.0f };\n"
"    for (int i = 0; i < g_ShadowLow_Lpcs.y; i++){\n"
"        Col = Col + PointLightCom(C, C, N, g_ShadowLow_Lpcs, g_LightPos[i], input.wPos, g_Lightst[i], g_LightColor[i], g_C_Pos);\n"
"    }\n"

//���s�����v�Z
"    Col = Col + DirectionalLightCom(C, C, N, g_DLightst, g_DLightDirection, g_DLightColor, input.wPos, g_C_Pos);\n"

//�Ō�Ɋ�{�F�Ƀe�N�X�`���̐F���|�����킹��
"    return float4(Col, 1.0f) * T + g_ObjCol;\n"
"}\n"
////////////////////////////////////////////���C�g�L/////////////////////////////////////////////////////////////////

/////////////////////////////////////���C�g�L�o���v�}�b�v////////////////////////////////////////////////////////////
"float4 PSDispLBump(DS_OUTPUT_TC input) : SV_Target\n"
"{\n"
//�e�N�X�`��
"    float4 T1 = g_texColor.Sample(g_samLinear, input.Tex);\n"
"    float4 T2 = g_texNormal.Sample(g_samLinear, input.Tex);\n"
//NormalMap�Ɩ@�����|�����킹�Đ��K��
"    float3 N = normalize(input.Nor * T2.xyz);\n"
//�t�H�O�v�Z(�e�N�X�`���ɑ΂��Čv�Z)
"    float4 T = FogCom(g_FogColor, g_FogAmo_Density, g_C_Pos, input.wPos, T1);\n"

//���C�g�v�Z
"    float4 C = { 1.0f, 1.0f, 1.0f, 1.0f};\n"
"    float3 Col = { 0.0f, 0.0f, 0.0f };\n"
"    for (int i = 0; i < g_ShadowLow_Lpcs.y; i++){\n"
"        Col = Col + PointLightCom(C, C, N, g_ShadowLow_Lpcs, g_LightPos[i], input.wPos, g_Lightst[i], g_LightColor[i], g_C_Pos);\n"
"    }\n"

//���s�����v�Z
"    Col = Col + DirectionalLightCom(C, C, N, g_DLightst, g_DLightDirection, g_DLightColor, input.wPos, g_C_Pos);\n"

//�Ō�Ɋ�{�F�Ƀe�N�X�`���̐F���|�����킹��
"    return float4(Col, 1.0f) * T + g_ObjCol;\n"
"}\n"
/////////////////////////////////////���C�g�L�o���v�}�b�v////////////////////////////////////////////////////////////

/////////////////////////////////////////���C�g����/////////////////////////////////////////////////////////////////
"float4 PSDisp(DS_OUTPUT_TC input) : SV_Target\n"
"{\n"
//�e�N�X�`��
"    float4 T1 = g_texColor.Sample(g_samLinear, input.Tex);\n"

//�t�H�O�v�Z�e�N�X�`���ɑ΂��Čv�Z
"    float4 T = FogCom(g_FogColor, g_FogAmo_Density, g_C_Pos, input.wPos, T1);\n"

"    float4 col = T;\n"
"    return col + g_ObjCol;\n"
"}\n";
/////////////////////////////////////////���C�g����/////////////////////////////////////////////////////////////////
//**************************************�s�N�Z���V�F�[�_�[*******************************************************************//
