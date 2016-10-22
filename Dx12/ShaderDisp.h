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

//*********************************************���_�V�F�[�_�[*******************************************************************//
//���C�g�L
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

//���C�g��
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
//*********************************************���_�V�F�[�_�[*******************************************************************//

//***************************************�n���V�F�[�_�[�R���X�^���g*************************************************************//
//���C�g�L
"HS_CONSTANT_OUTPUT HSConstantL(InputPatch<VS_OUTPUT_TCL, 4> ipL, uint pidL : SV_PrimitiveID)\n"
"{\n"
"	HS_CONSTANT_OUTPUT output;\n"

//���[���h�ϊ�(float3 ���� float4�̕ϊ��Y�ꖳ����)
"   float4 pos = float4(ipL[0].Pos, 1.0f);\n"
"   float4 wPos = mul(pos, g_World[ipL[0].instanceID]);\n"
//���_���猻�ݒn�܂ł̋������v�Z
"   float distance = length(g_C_Pos.xyz - wPos.xyz);\n"

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

//���C�g��
"HS_CONSTANT_OUTPUT HSConstant(InputPatch<VS_OUTPUT_TC, 4> ip, uint pid : SV_PrimitiveID)\n"
"{\n"
"	HS_CONSTANT_OUTPUT output;\n"

//���[���h�ϊ�
"   float4 pos = float4(ip[0].Pos, 1.0f);\n"
"   float4 wPos = mul(pos, g_World[ip[0].instanceID]);\n"

//���_���猻�ݒn�܂ł̋������v�Z
"   float distance = length(g_C_Pos.xyz - wPos.xyz);\n"

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
//���C�g�L
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

//���C�g��
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
//***************************************�n���V�F�[�_�[*************************************************************************//

//**************************************�h���C���V�F�[�_�[*********************************************************************//
//���C�g�L
"[domain(\"quad\")]\n"
"DS_OUTPUT_TCL DSDispL(HS_CONSTANT_OUTPUT InL, float2 UV : SV_DomaInLocation, const OutputPatch<HS_OUTPUT_TCL, 4> patchL)\n"
"{\n"
"	DS_OUTPUT_TCL output;\n"

//�J���[���o��
"   output.Col = patchL[0].Col;\n"

//UV���W�v�Z
"   float2 top_uv = lerp(patchL[0].Tex, patchL[1].Tex, UV.x);\n"
"   float2 bottom_uv = lerp(patchL[3].Tex, patchL[2].Tex, UV.x);\n"
"   float2 uv = float2(lerp(top_uv, bottom_uv, UV.y));\n"
"   output.Tex = uv;\n"
//�摜���獂�����Z�o
"   float4 texheight = g_texColor.SampleLevel(g_samLinear, uv, 0);\n"
"   float4 height = texheight * g_DispAmount.x;\n"
"   float hei = (height.x + height.y + height.z) / 3;\n"
//�摜����@���x�N�g������
"   float4 nor = texheight * 2 - 1;\n"//-1.0�`1.0�ɂ����
//pos���W�v�Z
"   float3 top_pos = lerp(patchL[0].Pos, patchL[1].Pos, UV.x);\n"
"   float3 bottom_pos = lerp(patchL[3].Pos, patchL[2].Pos, UV.x);\n"
"   output.Pos = float4(lerp(top_pos, bottom_pos, UV.y), 1);\n"
//���[�J���@���̕�����hei�����_�ړ�
"   output.Pos.xyz += hei * patchL[0].Nor;\n"
//���[�J���@���̐�Βl���傫�������ɖ@���x�N�g���ϊ�
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

//�@�����K��
"   float3 Normal = normalize(nor1);\n"

//�o�͂���@���̍쐬
"   output.Nor = mul(Normal, (float3x3)g_World[patchL[0].instanceID]);\n"

"	return output;\n"
"}\n"

//���C�g��
"[domain(\"quad\")]\n"
"DS_OUTPUT_TC DSDisp(HS_CONSTANT_OUTPUT In, float2 UV : SV_DomaInLocation, const OutputPatch<HS_OUTPUT_TC, 4> patch)\n"
"{\n"
"	DS_OUTPUT_TC output;\n"

//�J���[���o��
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
//�@���̕����ɂ���Ē��_�ړ����������߂�
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
//**************************************�h���C���V�F�[�_�[*********************************************************************//

//**************************************�s�N�Z���V�F�[�_�[********************************************************************//
//���C�g�L
"float4 PSDispL(DS_OUTPUT_TCL input) : SV_Target\n"
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
"         T = ff * T + (1.0f - ff) * g_FogColor;\n"
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

//�Ō�Ɋ�{�F�Ƀe�N�X�`���̐F���|�����킹��
"    return float4(Col, a) * T + g_ObjCol;\n"
"}\n"

//���C�g��
"float4 PSDisp(DS_OUTPUT_TC input) : SV_Target\n"
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
"         T = ff * T + (1.0f - ff) * g_FogColor;\n"
"       }\n"
"    }\n"

"   float4 col = T;\n"
"   return col * input.Col + g_ObjCol;\n"
"}\n";
//**************************************�s�N�Z���V�F�[�_�[*******************************************************************//
