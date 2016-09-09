//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@          MeshData�N���X                                    **//
//**                                   GetVBarray�֐�                                    **//
//*****************************************************************************************//

#include "Dx12Process.h"

HANDLE *MeshData::MeshObj_H = NULL;
MeshData *MeshData::MeshObj = NULL;
char **MeshData::MeshPass = NULL;

void MeshData::GetVBarrayThreadArray(MeshData *meshObj, char **Mpass, int pcs){
	//obj�t�@�C���p�X�z��󂯎��
	MeshPass = Mpass;
	//������obj�|�C���^�󂯎��
	MeshObj = meshObj;
	//�n���h������
	MeshObj_H = (HANDLE*)malloc(sizeof(HANDLE) * pcs);

	//�X���b�h����
	if (pcs > 0)MeshObj_H[0] = (HANDLE)_beginthreadex(NULL, 0, GetVB0, NULL, 0, NULL);
	if (pcs > 1)MeshObj_H[1] = (HANDLE)_beginthreadex(NULL, 0, GetVB1, NULL, 0, NULL);
	if (pcs > 2)MeshObj_H[2] = (HANDLE)_beginthreadex(NULL, 0, GetVB2, NULL, 0, NULL);
	if (pcs > 3)MeshObj_H[3] = (HANDLE)_beginthreadex(NULL, 0, GetVB3, NULL, 0, NULL);
	if (pcs > 4)MeshObj_H[4] = (HANDLE)_beginthreadex(NULL, 0, GetVB4, NULL, 0, NULL);
	if (pcs > 5)MeshObj_H[5] = (HANDLE)_beginthreadex(NULL, 0, GetVB5, NULL, 0, NULL);
	if (pcs > 6)MeshObj_H[6] = (HANDLE)_beginthreadex(NULL, 0, GetVB6, NULL, 0, NULL);
	if (pcs > 7)MeshObj_H[7] = (HANDLE)_beginthreadex(NULL, 0, GetVB7, NULL, 0, NULL);
	if (pcs > 8)MeshObj_H[8] = (HANDLE)_beginthreadex(NULL, 0, GetVB8, NULL, 0, NULL);
	if (pcs > 9)MeshObj_H[9] = (HANDLE)_beginthreadex(NULL, 0, GetVB9, NULL, 0, NULL);
	if (pcs > 10)MeshObj_H[10] = (HANDLE)_beginthreadex(NULL, 0, GetVB10, NULL, 0, NULL);
	if (pcs > 11)MeshObj_H[11] = (HANDLE)_beginthreadex(NULL, 0, GetVB11, NULL, 0, NULL);
	if (pcs > 12)MeshObj_H[12] = (HANDLE)_beginthreadex(NULL, 0, GetVB12, NULL, 0, NULL);
	if (pcs > 13)MeshObj_H[13] = (HANDLE)_beginthreadex(NULL, 0, GetVB13, NULL, 0, NULL);
	if (pcs > 14)MeshObj_H[14] = (HANDLE)_beginthreadex(NULL, 0, GetVB14, NULL, 0, NULL);
	if (pcs > 15)MeshObj_H[15] = (HANDLE)_beginthreadex(NULL, 0, GetVB15, NULL, 0, NULL);
	if (pcs > 16)MeshObj_H[16] = (HANDLE)_beginthreadex(NULL, 0, GetVB16, NULL, 0, NULL);
	if (pcs > 17)MeshObj_H[17] = (HANDLE)_beginthreadex(NULL, 0, GetVB17, NULL, 0, NULL);
	if (pcs > 18)MeshObj_H[18] = (HANDLE)_beginthreadex(NULL, 0, GetVB18, NULL, 0, NULL);
	if (pcs > 19)MeshObj_H[19] = (HANDLE)_beginthreadex(NULL, 0, GetVB19, NULL, 0, NULL);
	if (pcs > 20)MeshObj_H[20] = (HANDLE)_beginthreadex(NULL, 0, GetVB20, NULL, 0, NULL);
	if (pcs > 21)MeshObj_H[21] = (HANDLE)_beginthreadex(NULL, 0, GetVB21, NULL, 0, NULL);
	if (pcs > 22)MeshObj_H[22] = (HANDLE)_beginthreadex(NULL, 0, GetVB22, NULL, 0, NULL);
	if (pcs > 23)MeshObj_H[23] = (HANDLE)_beginthreadex(NULL, 0, GetVB23, NULL, 0, NULL);
	if (pcs > 24)MeshObj_H[24] = (HANDLE)_beginthreadex(NULL, 0, GetVB24, NULL, 0, NULL);
	if (pcs > 25)MeshObj_H[25] = (HANDLE)_beginthreadex(NULL, 0, GetVB25, NULL, 0, NULL);
	if (pcs > 26)MeshObj_H[26] = (HANDLE)_beginthreadex(NULL, 0, GetVB26, NULL, 0, NULL);
	if (pcs > 27)MeshObj_H[27] = (HANDLE)_beginthreadex(NULL, 0, GetVB27, NULL, 0, NULL);
	if (pcs > 28)MeshObj_H[28] = (HANDLE)_beginthreadex(NULL, 0, GetVB28, NULL, 0, NULL);
	if (pcs > 29)MeshObj_H[29] = (HANDLE)_beginthreadex(NULL, 0, GetVB29, NULL, 0, NULL);
	if (pcs > 30)MeshObj_H[30] = (HANDLE)_beginthreadex(NULL, 0, GetVB30, NULL, 0, NULL);
	if (pcs > 31)MeshObj_H[31] = (HANDLE)_beginthreadex(NULL, 0, GetVB31, NULL, 0, NULL);
	if (pcs > 32)MeshObj_H[32] = (HANDLE)_beginthreadex(NULL, 0, GetVB32, NULL, 0, NULL);
	if (pcs > 33)MeshObj_H[33] = (HANDLE)_beginthreadex(NULL, 0, GetVB33, NULL, 0, NULL);
	if (pcs > 34)MeshObj_H[34] = (HANDLE)_beginthreadex(NULL, 0, GetVB34, NULL, 0, NULL);
	if (pcs > 35)MeshObj_H[35] = (HANDLE)_beginthreadex(NULL, 0, GetVB35, NULL, 0, NULL);

	//�S�X���b�h���I������܂Ń��[�v
	int cnt = 0;
	while (cnt != pcs){
		cnt = 0;
		for (int i = 0; i < pcs; i++){
			DWORD th_end;
			GetExitCodeThread(MeshObj_H[i], &th_end);//�n���h���̏��
			if (th_end != STILL_ACTIVE)cnt++;//��~���Ă�΃J�E���g
		}
	}
	for (int i = 0; i < pcs; i++){
		WaitForSingleObject(MeshObj_H[i], INFINITE);//�X���b�h���I������܂ő҂�
		CloseHandle(MeshObj_H[i]);                 //�n���h�������
		MeshObj_H[i] = NULL;
	}
	free(MeshObj_H);//�n���h���z����
	MeshObj_H = NULL;
	MeshObj = NULL;
	MeshPass = NULL;
}

//GetVBarrayThreadArray���Ŏg�p
unsigned __stdcall GetVB0(void *){
	MeshData::MeshObj[0].GetVBarray(MeshData::MeshPass[0]);
	return 0;
}
unsigned __stdcall GetVB1(void *){
	MeshData::MeshObj[1].GetVBarray(MeshData::MeshPass[1]);
	return 0;
}
unsigned __stdcall GetVB2(void *){
	MeshData::MeshObj[2].GetVBarray(MeshData::MeshPass[2]);
	return 0;
}
unsigned __stdcall GetVB3(void *){
	MeshData::MeshObj[3].GetVBarray(MeshData::MeshPass[3]);
	return 0;
}
unsigned __stdcall GetVB4(void *){
	MeshData::MeshObj[4].GetVBarray(MeshData::MeshPass[4]);
	return 0;
}
unsigned __stdcall GetVB5(void *){
	MeshData::MeshObj[5].GetVBarray(MeshData::MeshPass[5]);
	return 0;
}
unsigned __stdcall GetVB6(void *){
	MeshData::MeshObj[6].GetVBarray(MeshData::MeshPass[6]);
	return 0;
}
unsigned __stdcall GetVB7(void *){
	MeshData::MeshObj[7].GetVBarray(MeshData::MeshPass[7]);
	return 0;
}
unsigned __stdcall GetVB8(void *){
	MeshData::MeshObj[8].GetVBarray(MeshData::MeshPass[8]);
	return 0;
}
unsigned __stdcall GetVB9(void *){
	MeshData::MeshObj[9].GetVBarray(MeshData::MeshPass[9]);
	return 0;
}
unsigned __stdcall GetVB10(void *){
	MeshData::MeshObj[10].GetVBarray(MeshData::MeshPass[10]);
	return 0;
}
unsigned __stdcall GetVB11(void *){
	MeshData::MeshObj[11].GetVBarray(MeshData::MeshPass[11]);
	return 0;
}
unsigned __stdcall GetVB12(void *){
	MeshData::MeshObj[12].GetVBarray(MeshData::MeshPass[12]);
	return 0;
}
unsigned __stdcall GetVB13(void *){
	MeshData::MeshObj[13].GetVBarray(MeshData::MeshPass[13]);
	return 0;
}
unsigned __stdcall GetVB14(void *){
	MeshData::MeshObj[14].GetVBarray(MeshData::MeshPass[14]);
	return 0;
}
unsigned __stdcall GetVB15(void *){
	MeshData::MeshObj[15].GetVBarray(MeshData::MeshPass[15]);
	return 0;
}
unsigned __stdcall GetVB16(void *){
	MeshData::MeshObj[16].GetVBarray(MeshData::MeshPass[16]);
	return 0;
}
unsigned __stdcall GetVB17(void *){
	MeshData::MeshObj[17].GetVBarray(MeshData::MeshPass[17]);
	return 0;
}
unsigned __stdcall GetVB18(void *){
	MeshData::MeshObj[18].GetVBarray(MeshData::MeshPass[18]);
	return 0;
}
unsigned __stdcall GetVB19(void *){
	MeshData::MeshObj[19].GetVBarray(MeshData::MeshPass[19]);
	return 0;
}
unsigned __stdcall GetVB20(void *){
	MeshData::MeshObj[20].GetVBarray(MeshData::MeshPass[20]);
	return 0;
}
unsigned __stdcall GetVB21(void *){
	MeshData::MeshObj[21].GetVBarray(MeshData::MeshPass[21]);
	return 0;
}
unsigned __stdcall GetVB22(void *){
	MeshData::MeshObj[22].GetVBarray(MeshData::MeshPass[22]);
	return 0;
}
unsigned __stdcall GetVB23(void *){
	MeshData::MeshObj[23].GetVBarray(MeshData::MeshPass[23]);
	return 0;
}
unsigned __stdcall GetVB24(void *){
	MeshData::MeshObj[24].GetVBarray(MeshData::MeshPass[24]);
	return 0;
}
unsigned __stdcall GetVB25(void *){
	MeshData::MeshObj[25].GetVBarray(MeshData::MeshPass[25]);
	return 0;
}
unsigned __stdcall GetVB26(void *){
	MeshData::MeshObj[26].GetVBarray(MeshData::MeshPass[26]);
	return 0;
}
unsigned __stdcall GetVB27(void *){
	MeshData::MeshObj[27].GetVBarray(MeshData::MeshPass[27]);
	return 0;
}
unsigned __stdcall GetVB28(void *){
	MeshData::MeshObj[28].GetVBarray(MeshData::MeshPass[28]);
	return 0;
}
unsigned __stdcall GetVB29(void *){
	MeshData::MeshObj[29].GetVBarray(MeshData::MeshPass[29]);
	return 0;
}
unsigned __stdcall GetVB30(void *){
	MeshData::MeshObj[30].GetVBarray(MeshData::MeshPass[30]);
	return 0;
}
unsigned __stdcall GetVB31(void *){
	MeshData::MeshObj[31].GetVBarray(MeshData::MeshPass[31]);
	return 0;
}
unsigned __stdcall GetVB32(void *){
	MeshData::MeshObj[32].GetVBarray(MeshData::MeshPass[32]);
	return 0;
}
unsigned __stdcall GetVB33(void *){
	MeshData::MeshObj[33].GetVBarray(MeshData::MeshPass[33]);
	return 0;
}
unsigned __stdcall GetVB34(void *){
	MeshData::MeshObj[34].GetVBarray(MeshData::MeshPass[34]);
	return 0;
}
unsigned __stdcall GetVB35(void *){
	MeshData::MeshObj[35].GetVBarray(MeshData::MeshPass[35]);
	return 0;
}

MeshData::MeshData() {
	dx = Dx12Process::GetInstance();
	mDirectCmdListAlloc = dx->mDirectCmdListAlloc[0].Get();
	mCommandList = dx->mCommandList[0].Get();
	primType_create = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	primType_draw = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

MeshData::~MeshData() {
	S_DELETE(mObjectCB);
	S_DELETE(mObject_MESHCB);
	ARR_DELETE(pMaterial);
}

void MeshData::SetCommandList(int no) {
	com_no = no;
	mDirectCmdListAlloc = dx->mDirectCmdListAlloc[com_no].Get();
	mCommandList = dx->mCommandList[com_no].Get();
}

ID3D12PipelineState *MeshData::GetPipelineState() {
	return mPSO.Get();
}

void MeshData::GetShaderByteCode(bool disp) {

	if (disp == TRUE) {
		ps = dx->pHullShader_MESH_D.Get();
		ds = dx->pDomainShader_MESH_D.Get();
		vs = dx->pVertexShader_MESH_D.Get();
		ps = dx->pPixelShader_MESH_D.Get();
	}
	else {
		vs = dx->pVertexShader_MESH.Get();
		ps = dx->pPixelShader_MESH.Get();
	}
}

void MeshData::LoadMaterialFromFile(LPSTR FileName, MY_MATERIAL** ppMaterial) {

	//�}�e���A���t�@�C�����J���ē��e��ǂݍ���
	FILE* fp = NULL;
	fopen_s(&fp, FileName, "rt");
	char line[200] = { 0 };//1�s�ǂݍ��ݗp
	char key[110] = { 0 };//1�P��ǂݍ��ݗp
	VECTOR4 v = { 0, 0, 0, 1 };

	//�}�e���A�����𒲂ׂ�
	MaterialCount = 0;
	while (!feof(fp))
	{
		//�L�[���[�h�ǂݍ���
		fgets(line, sizeof(line), fp);
		sscanf_s(line, "%s ", key, sizeof(key));
		//�}�e���A����
		if (strcmp(key, "newmtl") == 0)
		{
			MaterialCount++;
		}
	}
	MY_MATERIAL* pMaterial = new MY_MATERIAL[MaterialCount]();

	//�{�ǂݍ���	
	fseek(fp, 0, SEEK_SET);
	INT iMCount = -1;

	while (!feof(fp))
	{
		//�L�[���[�h�ǂݍ���
		fgets(line, sizeof(line), fp);//1�s�ǂݍ���line�Ɋi�[,FILE�|�C���^1�s�i��
		sscanf_s(line, "%s ", key, sizeof(key));//�ǂݍ���1�s����"%s"�ŏ��̕�����1�ǂݍ���
		//�}�e���A����
		if (strcmp(key, "newmtl") == 0)
		{
			iMCount++;
			sscanf_s(&line[7], "%s ", key, sizeof(key));//line��7�v�f��(newmtl)�̒��ォ��1�ڂ̕������key�Ɋi�[
			strcpy_s(pMaterial[iMCount].MaterialName, key);
		}
		//Kd�@�f�B�t���[�Y
		if (strcmp(key, "Kd") == 0)
		{
			sscanf_s(&line[3], "%f %f %f", &v.x, &v.y, &v.z);
			pMaterial[iMCount].Kd = v;
		}
		//map_Kd�@�e�N�X�`���[
		if (strcmp(key, "map_Kd") == 0)
		{
			sscanf_s(&line[7], "%s", &pMaterial[iMCount].TextureName, sizeof(pMaterial[iMCount].TextureName));
			//Dx12�N���X������g�p����e�N�X�`���[�i���o�[�擾(atoi ���l�������int�^�ϊ�)
			pMaterial[iMCount].tex_no = atoi(pMaterial[iMCount].TextureName);
		}
	}
	fclose(fp);

	*ppMaterial = pMaterial;
}

void MeshData::SetState(bool al, bool bl, bool di) {
	alpha = al;
	blend = bl;
	disp = di;
	if (disp) {
		primType_create = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
		primType_draw = D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST;
	}
	else {
		primType_create = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		primType_draw = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	}
}

void MeshData::GetVBarray(LPSTR FileName) {

	GetShaderByteCode(disp);

	mObjectCB = new UploadBuffer<CONSTANT_BUFFER>(dx->md3dDevice.Get(), 1, true);

	CD3DX12_DESCRIPTOR_RANGE texTable;
	texTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	CD3DX12_ROOT_PARAMETER slotRootParameter[3];
	slotRootParameter[0].InitAsDescriptorTable(1, &texTable, D3D12_SHADER_VISIBILITY_ALL);
	slotRootParameter[1].InitAsConstantBufferView(0);
	slotRootParameter[2].InitAsConstantBufferView(1);

	auto staticSamplers = dx->GetStaticSamplers();

	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(3, slotRootParameter,
		(UINT)staticSamplers.size(), staticSamplers.data(),
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	Microsoft::WRL::ComPtr<ID3DBlob> serializedRootSig = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
	D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

	if (errorBlob != nullptr)
	{
		::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}
	//RootSignature����
	dx->md3dDevice->CreateRootSignature(
		0,
		serializedRootSig->GetBufferPointer(),
		serializedRootSig->GetBufferSize(),
		IID_PPV_ARGS(mRootSignature.GetAddressOf()));

	float x, y, z;
	int v1 = 0, v2 = 0, v3 = 0;
	int vn1 = 0, vn2 = 0, vn3 = 0;
	int vt1 = 0, vt2 = 0, vt3 = 0;
	int VCount = 0;//�ǂݍ��݃J�E���^�[
	int VNCount = 0;//�ǂݍ��݃J�E���^�[
	int VTCount = 0;//�ǂݍ��݃J�E���^�[
	FaceCount = 0;//�|���S�����J�E���^�[

	char line[200] = { 0 };
	char key[200] = { 0 };
	//OBJ�t�@�C�����J���ē��e��ǂݍ���
	FILE* fp = NULL;
	fopen_s(&fp, FileName, "rt");

	//���O�ɒ��_���A�|���S�����𒲂ׂ�
	while (!feof(fp))
	{
		//�L�[���[�h�ǂݍ���
		fgets(line, sizeof(line), fp);
		sscanf_s(line, "%s ", key, sizeof(key));
		//�}�e���A���ǂݍ���
		if (strcmp(key, "mtllib") == 0)
		{
			sscanf_s(&line[7], "%s ", key, sizeof(key));
			LoadMaterialFromFile(key, &pMaterial);
		}
		//���_
		if (strcmp(key, "v") == 0)
		{
			VCount++;
		}
		//�@��
		if (strcmp(key, "vn") == 0)
		{
			VNCount++;
		}
		//�e�N�X�`���[���W
		if (strcmp(key, "vt") == 0)
		{
			VTCount++;
		}
		//�t�F�C�X�i�|���S���j
		if (strcmp(key, "f") == 0)
		{
			FaceCount++;
		}
	}

	//�ꎞ�I�ȃ������m��
	VECTOR3* pvCoord = new VECTOR3[VCount]();
	VECTOR3* pvNormal = new VECTOR3[VNCount]();
	VECTOR2* pvTexture = new VECTOR2[VTCount]();

	//�{�ǂݍ���	
	fseek(fp, 0, SEEK_SET);
	VCount = 0;
	VNCount = 0;
	VTCount = 0;

	while (!feof(fp))
	{
		//�L�[���[�h �ǂݍ���
		ZeroMemory(key, sizeof(key));
		fgets(line, sizeof(line), fp);
		sscanf_s(line, "%s", key, sizeof(key));

		//���_ �ǂݍ���
		if (strcmp(key, "v") == 0)
		{
			sscanf_s(&line[2], "%f %f %f", &x, &y, &z);
			pvCoord[VCount].x = -x;
			pvCoord[VCount].y = z;
			pvCoord[VCount].z = y;
			VCount++;
		}

		//�@�� �ǂݍ���
		if (strcmp(key, "vn") == 0)
		{
			sscanf_s(&line[3], "%f %f %f", &x, &y, &z);
			pvNormal[VNCount].x = x;
			pvNormal[VNCount].y = -z;
			pvNormal[VNCount].z = -y;
			VNCount++;
		}

		//�e�N�X�`���[���W �ǂݍ���
		if (strcmp(key, "vt") == 0)
		{
			sscanf_s(&line[3], "%f %f", &x, &y);
			pvTexture[VTCount].x = x;
			pvTexture[VTCount].y = 1 - y;//OBJ�t�@�C����Y�������t�Ȃ̂ō��킹��
			VTCount++;
		}
	}

	mObject_MESHCB = new UploadBuffer<CONSTANT_BUFFER_MESH>(dx->md3dDevice.Get(), MaterialCount, true);//�A�h���X���炵�ĊeMaterial�A�N�Z�X
	for (int i = 0; i < MaterialCount; i++) {
		CONSTANT_BUFFER_MESH sg;
		sg.vDiffuse = pMaterial[i].Kd;//�f�B�t���[�Y�J���[���V�F�[�_�[�ɓn��
									  //�e�N�X�`���L���t���O
		if (pMaterial[i].TextureName[0] != NULL) {
			sg.tex_f = { 1.0f, 0.0f, 0.0f, 0.0f };
		}
		else {
			sg.tex_f = { 0.0f, 0.0f, 0.0f, 0.0f };
		}
		mObject_MESHCB->CopyData(i, sg);
	}

	//�}�e���A���̐�����Descriptor��p�ӂ���
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = MaterialCount;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	dx->md3dDevice->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&mSrvHeap));

	Vview = std::make_unique<VertexView>();
	Iview = std::make_unique<IndexView[]>(MaterialCount);

	//�t�F�C�X�@�ǂݍ��݁@�o���o���Ɏ��^����Ă���\��������̂ŁA�}�e���A�����𗊂�ɂȂ����킹��
	bool boFlag = false;
	piFaceBuffer = new int[MaterialCount * FaceCount * 3]();//3���_�Ȃ̂�3�C���f�b�N�X * Material��
	pvVertexBuffer = new MY_VERTEX_MESH[FaceCount * 3]();
	int FCount = 0;
	int dwPartFCount = 0;
	for (int i = 0; i < MaterialCount; i++)
	{
		dwPartFCount = 0;
		fseek(fp, 0, SEEK_SET);

		while (!feof(fp))
		{
			//�L�[���[�h �ǂݍ���
			ZeroMemory(key, sizeof(key));
			fgets(line, sizeof(line), fp);
			sscanf_s(line, "%s ", key, sizeof(key));

			//�t�F�C�X �ǂݍ��݁����_�C���f�b�N�X��
			if (strcmp(key, "usemtl") == 0)
			{
				sscanf_s(&line[7], "%s ", key, sizeof(key));
				if (strcmp(key, pMaterial[i].MaterialName) == 0)
				{
					boFlag = true;
				}
				else
				{
					boFlag = false;
				}
			}
			if (strcmp(key, "f") == 0 && boFlag == true)
			{
				if (pMaterial[i].tex_no != -1)//�e�N�X�`���[����T�[�t�F�C�X
				{
					sscanf_s(&line[2], "%d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
				}
				else//�e�N�X�`���[�����T�[�t�F�C�X
				{
					sscanf_s(&line[2], "%d//%d %d//%d %d//%d", &v1, &vn1, &v2, &vn2, &v3, &vn3);
					vt1 = vt2 = vt3 = 1;//���G���[�h�~
				}

				//�C���f�b�N�X�o�b�t�@�[
				piFaceBuffer[FaceCount * 3 * i + dwPartFCount * 3] = FCount * 3;
				piFaceBuffer[FaceCount * 3 * i + dwPartFCount * 3 + 1] = FCount * 3 + 1;
				piFaceBuffer[FaceCount * 3 * i + dwPartFCount * 3 + 2] = FCount * 3 + 2;
				//���_�\���̂ɑ��
				pvVertexBuffer[FCount * 3].Pos = pvCoord[v1 - 1];
				pvVertexBuffer[FCount * 3].normal = pvNormal[vn1 - 1];
				pvVertexBuffer[FCount * 3].tex = pvTexture[vt1 - 1];

				pvVertexBuffer[FCount * 3 + 1].Pos = pvCoord[v2 - 1];
				pvVertexBuffer[FCount * 3 + 1].normal = pvNormal[vn2 - 1];
				pvVertexBuffer[FCount * 3 + 1].tex = pvTexture[vt2 - 1];

				pvVertexBuffer[FCount * 3 + 2].Pos = pvCoord[v3 - 1];
				pvVertexBuffer[FCount * 3 + 2].normal = pvNormal[vn3 - 1];
				pvVertexBuffer[FCount * 3 + 2].tex = pvTexture[vt3 - 1];

				dwPartFCount++;
				FCount++;
			}
		}

		if (dwPartFCount == 0)//�g�p����Ă��Ȃ��}�e���A���΍􂪕K�v�ȏꍇ�����ǉ��BDraw�ɂ�
		{
			continue;
		}

		const UINT ibByteSize = (UINT)dwPartFCount * 3 * sizeof(UINT);

		D3DCreateBlob(ibByteSize, &Iview[i].IndexBufferCPU);
		CopyMemory(Iview[i].IndexBufferCPU->GetBufferPointer(), &piFaceBuffer[FaceCount * 3 * i], ibByteSize);

		Iview[i].IndexFormat = DXGI_FORMAT_R32_UINT;//����ԈႤ�ƑS�R�ς�钍��
		Iview[i].IndexBufferByteSize = ibByteSize;
		Iview[i].IndexCount = dwPartFCount * 3;
	}
	
	fclose(fp);

	const UINT vbByteSize = (UINT)FCount * 3 * sizeof(MY_VERTEX_MESH);

	D3DCreateBlob(vbByteSize, &Vview->VertexBufferCPU);
	CopyMemory(Vview->VertexBufferCPU->GetBufferPointer(), pvVertexBuffer, vbByteSize);

	Vview->VertexByteStride = sizeof(MY_VERTEX_MESH);
	Vview->VertexBufferByteSize = vbByteSize;

	//�ꎞ�I�ϐ����
	ARR_DELETE(pvCoord);
	ARR_DELETE(pvNormal);
	ARR_DELETE(pvTexture);

	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g����
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	psoDesc.InputLayout = { dx->pVertexLayout_MESH.data(), (UINT)dx->pVertexLayout_MESH.size() };
	psoDesc.pRootSignature = mRootSignature.Get();
	psoDesc.VS =
	{
		reinterpret_cast<BYTE*>(vs->GetBufferPointer()),
		vs->GetBufferSize()
	};
	if (hs != nullptr) {
		psoDesc.HS =
		{
			reinterpret_cast<BYTE*>(hs->GetBufferPointer()),
			hs->GetBufferSize()
		};
	}
	if (ds != nullptr) {
		psoDesc.DS =
		{
			reinterpret_cast<BYTE*>(ds->GetBufferPointer()),
			ds->GetBufferSize()
		};
	}
	psoDesc.PS =
	{
		reinterpret_cast<BYTE*>(ps->GetBufferPointer()),
		ps->GetBufferSize()
	};
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.BlendState.IndependentBlendEnable = FALSE;
	psoDesc.BlendState.AlphaToCoverageEnable = alpha;//�A���t�@�e�X�gon/off
	psoDesc.BlendState.RenderTarget[0].BlendEnable = blend;//�u�����hon/off
	psoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	psoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = primType_create;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = dx->mBackBufferFormat;
	psoDesc.SampleDesc.Count = dx->m4xMsaaState ? 4 : 1;
	psoDesc.SampleDesc.Quality = dx->m4xMsaaState ? (dx->m4xMsaaQuality - 1) : 0;
	psoDesc.DSVFormat = dx->mDepthStencilFormat;
	dx->md3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPSO));
}

void MeshData::GetTexture() {
	//���_�ǂݍ��݂̓s����CommandList�̏������u��

	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(mSrvHeap->GetCPUDescriptorHandleForHeapStart());
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	for (int i = 0; i < MaterialCount; i++) {
		srvDesc.Format = dx->texture[pMaterial[i].tex_no]->GetDesc().Format;
		srvDesc.Texture2D.MipLevels = dx->texture[pMaterial[i].tex_no]->GetDesc().MipLevels;
		dx->md3dDevice->CreateShaderResourceView(dx->texture[pMaterial[i].tex_no], &srvDesc, hDescriptor);
		hDescriptor.Offset(1, dx->mCbvSrvUavDescriptorSize);

		Iview[i].IndexBufferGPU = dx->CreateDefaultBuffer(dx->md3dDevice.Get(),
			mCommandList, &piFaceBuffer[FaceCount * 3 * i], Iview[i].IndexBufferByteSize, Iview[i].IndexBufferUploader);
	}
	ARR_DELETE(piFaceBuffer);

	Vview->VertexBufferGPU = dx->CreateDefaultBuffer(dx->md3dDevice.Get(),
		mCommandList, pvVertexBuffer, Vview->VertexBufferByteSize, Vview->VertexBufferUploader);
	ARR_DELETE(pvVertexBuffer);
}

void MeshData::InstancedMap(float x, float y, float z, float thetaZ, float thetaY, float thetaX, float size) {
	dx->InstancedMap(x, y, z, thetaZ, thetaY, thetaX, size);
}

void MeshData::Draw(float x, float y, float z, float r, float g, float b, float thetaZ, float thetaY, float thetaX, float size, float disp) {

	mCommandList->SetPipelineState(mPSO.Get());

	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
	dx->MatrixMap(mObjectCB, x, y, z, r, g, b, thetaZ, thetaY, thetaX, size, disp, 1.0f, 1.0f, 1.0f, 1.0f);

	mCommandList->RSSetViewports(1, &dx->mScreenViewport);
	mCommandList->RSSetScissorRects(1, &dx->mScissorRect);

	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(dx->mSwapChainBuffer[dx->mCurrBackBuffer].Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	//�����_�[�^�[�Q�b�g�̃Z�b�g
	mCommandList->OMSetRenderTargets(1, &CD3DX12_CPU_DESCRIPTOR_HANDLE(
		dx->mRtvHeap->GetCPUDescriptorHandleForHeapStart(),
		dx->mCurrBackBuffer,
		dx->mRtvDescriptorSize), true, &dx->mDsvHeap->GetCPUDescriptorHandleForHeapStart());

	ID3D12DescriptorHeap* descriptorHeaps[] = { mSrvHeap.Get() };
	mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	mCommandList->SetGraphicsRootSignature(mRootSignature.Get());

	mCommandList->IASetVertexBuffers(0, 1, &Vview->VertexBufferView());
	mCommandList->IASetPrimitiveTopology(primType_draw);

	CD3DX12_GPU_DESCRIPTOR_HANDLE tex(mSrvHeap->GetGPUDescriptorHandleForHeapStart());
	for (int i = 0; i < MaterialCount; i++) {

		mCommandList->IASetIndexBuffer(&Iview[i].IndexBufferView());

		mCommandList->SetGraphicsRootDescriptorTable(0, tex);
		tex.Offset(1, dx->mCbvSrvUavDescriptorSize);//�f�X�N���v�^�q�[�v�̃A�h���X�ʒu�I�t�Z�b�g�Ŏ��̃e�N�X�`����ǂݍ��܂���

		mCommandList->SetGraphicsRootConstantBufferView(1, mObjectCB->Resource()->GetGPUVirtualAddress());
		mCommandList->SetGraphicsRootConstantBufferView(2, mObject_MESHCB->Resource()->GetGPUVirtualAddress() + 256 * i);

		mCommandList->DrawIndexedInstanced(
			Iview[i].IndexCount,
			1 + dx->ins_no, 0, 0, 0);
	}

	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(dx->mSwapChainBuffer[dx->mCurrBackBuffer].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	dx->ins_no = 0;
}