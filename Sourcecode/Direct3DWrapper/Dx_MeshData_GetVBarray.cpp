//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　          MeshDataクラス                                    **//
//**                                   GetVBarray関数                                    **//
//*****************************************************************************************//

#include "Dx12Process.h"

std::mutex MeshData::mtx;

MeshData::MeshData() {
	dx = Dx12Process::GetInstance();
	mCommandList = dx->dx_sub[0].mCommandList.Get();
	addDiffuse = 0.0f;
	addSpecular = 0.0f;
}

MeshData::~MeshData() {
	S_DELETE(mObjectCB);
	S_DELETE(mObject_MESHCB);
	ARR_DELETE(pMaterial);
	RELEASE(texture);
	RELEASE(textureUp);
}

void MeshData::SetCommandList(int no) {
	com_no = no;
	mCommandList = dx->dx_sub[com_no].mCommandList.Get();
}

ID3D12PipelineState *MeshData::GetPipelineState() {
	return mPSO.Get();
}

void MeshData::GetShaderByteCode(bool disp) {

	vs = dx->pVertexShader_MESH.Get();
	if (disp) {
		vsB = dx->pVertexShader_MESH_D.Get();
		hs = dx->pHullShaderTriangle.Get();
		ds = dx->pDomainShaderTriangle.Get();
	}
	else {
		vsB = dx->pVertexShader_MESH.Get();
		hs = nullptr;
		ds = nullptr;
	}
	ps = dx->pPixelShader_3D.Get();
	psB = dx->pPixelShader_Bump.Get();
}

void MeshData::LoadMaterialFromFile(char *FileName, MY_MATERIAL** ppMaterial) {

	//マテリアルファイルを開いて内容を読み込む
	FILE* fp = NULL;
	fopen_s(&fp, FileName, "rt");
	char line[200] = { 0 };//1行読み込み用
	char key[110] = { 0 };//1単語読み込み用
	VECTOR4 v = { 0, 0, 0, 1 };

	//マテリアル数を調べる
	MaterialCount = 0;
	while (!feof(fp))
	{
		//キーワード読み込み
		fgets(line, sizeof(line), fp);
		sscanf_s(line, "%s ", key, (unsigned int)sizeof(key));
		//マテリアル名
		if (strcmp(key, "newmtl") == 0)
		{
			MaterialCount++;
		}
	}

	MY_MATERIAL* pMaterial = new MY_MATERIAL[MaterialCount]();

	//本読み込み	
	fseek(fp, 0, SEEK_SET);
	INT iMCount = -1;
	texNum = 0;

	while (!feof(fp))
	{
		//キーワード読み込み
		fgets(line, sizeof(line), fp);//1行読み込みlineに格納,FILEポインタ1行進む
		sscanf_s(line, "%s ", key, (unsigned int)sizeof(key));//読み込んだ1行から"%s"最初の文字列1個読み込み
		//マテリアル名
		if (strcmp(key, "newmtl") == 0)
		{
			iMCount++;
			sscanf_s(&line[7], "%s ", key, (unsigned int)sizeof(key));//lineの7要素目(newmtl)の直後から1個目の文字列をkeyに格納
			strcpy_s(pMaterial[iMCount].MaterialName, key);
		}
		//Kd　ディフューズ
		if (strcmp(key, "Kd") == 0)
		{
			sscanf_s(&line[3], "%f %f %f", &v.x, &v.y, &v.z);
			pMaterial[iMCount].Kd = v;
		}
		//Ks　スペキュラー
		if (strcmp(key, "Ks") == 0)
		{
			sscanf_s(&line[3], "%f %f %f", &v.x, &v.y, &v.z);
			pMaterial[iMCount].Ks = v;
		}
		//map_Kd　テクスチャー
		if (strcmp(key, "map_Kd") == 0)
		{
			sscanf_s(&line[7], "%s", &pMaterial[iMCount].TextureName, (unsigned int)sizeof(pMaterial[iMCount].TextureName));
			pMaterial[iMCount].tex_no = dx->GetTexNumber(pMaterial[iMCount].TextureName);
			texNum++;
		}
		//map_bump　テクスチャー
		if (strcmp(key, "map_bump") == 0)
		{
			sscanf_s(&line[7], "%s", &pMaterial[iMCount].norTextureName, (unsigned int)sizeof(pMaterial[iMCount].norTextureName));
			pMaterial[iMCount].nortex_no = dx->GetTexNumber(pMaterial[iMCount].norTextureName);
			texNum++;
		}
	}
	fclose(fp);

	*ppMaterial = pMaterial;
}

void MeshData::SetState(bool al, bool bl, bool di) {
	SetState(al, bl, di, 0.0f, 0.0f);
}

void MeshData::SetState(bool al, bool bl, bool di, float diffuse, float specu) {
	alpha = al;
	blend = bl;
	disp = di;
	addDiffuse = diffuse;
	addSpecular = specu;

	if (disp) {
		primType_draw = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		primType_drawB = D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST;
	}
	else {
		primType_draw = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		primType_drawB = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	}
}

void MeshData::GetBuffer(char *FileName) {

	strcpy_s(mFileName, FileName);

	mObjectCB = new UploadBuffer<CONSTANT_BUFFER>(dx->md3dDevice.Get(), 1, true);

	int VCount = 0;//読み込みカウンター
	int VNCount = 0;//読み込みカウンター
	int VTCount = 0;//読み込みカウンター
	FaceCount = 0;//ポリゴン数カウンター

	char line[200] = { 0 };
	char key[200] = { 0 };
	//OBJファイルを開いて内容を読み込む
	FILE* fp = NULL;
	fopen_s(&fp, mFileName, "rt");

	//事前に頂点数、ポリゴン数を調べる
	while (!feof(fp))
	{
		//キーワード読み込み
		fgets(line, sizeof(line), fp);
		sscanf_s(line, "%s ", key, (unsigned int)sizeof(key));
		//マテリアル読み込み
		if (strcmp(key, "mtllib") == 0)
		{
			sscanf_s(&line[7], "%s ", key, (unsigned int)sizeof(key));
			LoadMaterialFromFile(key, &pMaterial);
		}
		//頂点
		if (strcmp(key, "v") == 0)
		{
			VCount++;
		}
		//法線
		if (strcmp(key, "vn") == 0)
		{
			VNCount++;
		}
		//テクスチャー座標
		if (strcmp(key, "vt") == 0)
		{
			VTCount++;
		}
		//フェイス（ポリゴン）
		if (strcmp(key, "f") == 0)
		{
			FaceCount++;
		}
	}
	fclose(fp);

	//一時的なメモリ確保
	pvCoord = new VECTOR3[VCount]();
	pvNormal = new VECTOR3[VNCount]();
	pvTexture = new VECTOR2[VTCount]();

	mObject_MESHCB = new UploadBuffer<CONSTANT_BUFFER2>(dx->md3dDevice.Get(), MaterialCount, true);//アドレスずらして各Materialアクセス
	Vview = std::make_unique<VertexView>();
	Iview = std::make_unique<IndexView[]>(MaterialCount);

	piFaceBuffer = new int[MaterialCount * FaceCount * 3]();//3頂点なので3インデックス * Material個数
	pvVertexBuffer = new VertexM[FaceCount * 3]();
}

void MeshData::SetVertex() {

	float x, y, z;
	int v1 = 0, v2 = 0, v3 = 0;
	int vn1 = 0, vn2 = 0, vn3 = 0;
	int vt1 = 0, vt2 = 0, vt3 = 0;
	int VCount = 0;//読み込みカウンター
	int VNCount = 0;//読み込みカウンター
	int VTCount = 0;//読み込みカウンター

	char line[200] = { 0 };
	char key[200] = { 0 };
	//OBJファイルを開いて内容を読み込む
	FILE* fp = NULL;
	fopen_s(&fp, mFileName, "rt");

	//本読み込み	
	while (!feof(fp))
	{
		//キーワード 読み込み
		ZeroMemory(key, sizeof(key));
		fgets(line, sizeof(line), fp);
		sscanf_s(line, "%s", key, (unsigned int)sizeof(key));

		//頂点 読み込み
		if (strcmp(key, "v") == 0)
		{
			sscanf_s(&line[2], "%f %f %f", &x, &y, &z);
			pvCoord[VCount].x = -x;
			pvCoord[VCount].y = z;
			pvCoord[VCount].z = y;
			VCount++;
		}

		//法線 読み込み
		if (strcmp(key, "vn") == 0)
		{
			sscanf_s(&line[3], "%f %f %f", &x, &y, &z);
			pvNormal[VNCount].x = x;
			pvNormal[VNCount].y = -z;
			pvNormal[VNCount].z = -y;
			VNCount++;
		}

		//テクスチャー座標 読み込み
		if (strcmp(key, "vt") == 0)
		{
			sscanf_s(&line[3], "%f %f", &x, &y);
			pvTexture[VTCount].x = x;
			pvTexture[VTCount].y = 1 - y;//OBJファイルはY成分が逆なので合わせる
			VTCount++;
		}
	}

	//同一座標頂点リスト
	SameVertexList *svList = new SameVertexList[VCount];

	for (int i = 0; i < MaterialCount; i++) {
		CONSTANT_BUFFER2 sg;
		pMaterial[i].Kd.x += addDiffuse;
		pMaterial[i].Kd.y += addDiffuse;
		pMaterial[i].Kd.z += addDiffuse;
		pMaterial[i].Ks.x += addSpecular;
		pMaterial[i].Ks.y += addSpecular;
		pMaterial[i].Ks.z += addSpecular;
		sg.vDiffuse = pMaterial[i].Kd;//ディフューズカラーをシェーダーに渡す
		sg.vDiffuse = pMaterial[i].Ks;//スペキュラーをシェーダーに渡す
		mObject_MESHCB->CopyData(i, sg);
	}

	//フェイス　読み込み　バラバラに収録されている可能性があるので、マテリアル名を頼りにつなぎ合わせる
	bool boFlag = false;

	int FCount = 0;
	int dwPartFCount = 0;
	for (int i = 0; i < MaterialCount; i++)
	{
		dwPartFCount = 0;
		fseek(fp, 0, SEEK_SET);

		while (!feof(fp))
		{
			//キーワード 読み込み
			ZeroMemory(key, sizeof(key));
			fgets(line, sizeof(line), fp);
			sscanf_s(line, "%s ", key, (unsigned int)sizeof(key));

			//フェイス 読み込み→頂点インデックスに
			if (strcmp(key, "usemtl") == 0)
			{
				sscanf_s(&line[7], "%s ", key, (unsigned int)sizeof(key));
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
				if (pMaterial[i].tex_no != -1)//テクスチャーありサーフェイス
				{
					sscanf_s(&line[2], "%d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
				}
				else//テクスチャー無しサーフェイス
				{
					sscanf_s(&line[2], "%d//%d %d//%d %d//%d", &v1, &vn1, &v2, &vn2, &v3, &vn3);
					vt1 = vt2 = vt3 = 1;//↓エラー防止
				}

				//インデックスバッファー
				piFaceBuffer[FaceCount * 3 * i + dwPartFCount * 3] = FCount * 3;
				piFaceBuffer[FaceCount * 3 * i + dwPartFCount * 3 + 1] = FCount * 3 + 1;
				piFaceBuffer[FaceCount * 3 * i + dwPartFCount * 3 + 2] = FCount * 3 + 2;
				//頂点構造体に代入
				pvVertexBuffer[FCount * 3].Pos = pvCoord[v1 - 1];
				pvVertexBuffer[FCount * 3].normal = pvNormal[vn1 - 1];
				pvVertexBuffer[FCount * 3].geoNormal = pvNormal[vn1 - 1];
				pvVertexBuffer[FCount * 3].tex = pvTexture[vt1 - 1];
				svList[v1 - 1].Push(FCount * 3);

				pvVertexBuffer[FCount * 3 + 1].Pos = pvCoord[v2 - 1];
				pvVertexBuffer[FCount * 3 + 1].normal = pvNormal[vn2 - 1];
				pvVertexBuffer[FCount * 3 + 1].geoNormal = pvNormal[vn2 - 1];
				pvVertexBuffer[FCount * 3 + 1].tex = pvTexture[vt2 - 1];
				svList[v2 - 1].Push(FCount * 3 + 1);

				pvVertexBuffer[FCount * 3 + 2].Pos = pvCoord[v3 - 1];
				pvVertexBuffer[FCount * 3 + 2].normal = pvNormal[vn3 - 1];
				pvVertexBuffer[FCount * 3 + 2].geoNormal = pvNormal[vn3 - 1];
				pvVertexBuffer[FCount * 3 + 2].tex = pvTexture[vt3 - 1];
				svList[v3 - 1].Push(FCount * 3 + 2);

				dwPartFCount++;
				FCount++;
			}
		}

		if (dwPartFCount == 0)//使用されていないマテリアル対策が必要な場合処理追加。Drawにも
		{
			continue;
		}

		const UINT ibByteSize = (UINT)dwPartFCount * 3 * sizeof(UINT);

		D3DCreateBlob(ibByteSize, &Iview[i].IndexBufferCPU);
		CopyMemory(Iview[i].IndexBufferCPU->GetBufferPointer(), &piFaceBuffer[FaceCount * 3 * i], ibByteSize);

		Iview[i].IndexFormat = DXGI_FORMAT_R32_UINT;
		Iview[i].IndexBufferByteSize = ibByteSize;
		Iview[i].IndexCount = dwPartFCount * 3;
	}

	//同一座標頂点の法線統一化(テセレーション用)
	for (int i = 0; i < VCount; i++) {
		int indVB = 0;
		VECTOR3 geo[50];
		int indVb[50];
		int indGeo = 0;
		while (1) {
			indVB = svList[i].Pop();
			if (indVB == -1)break;
			indVb[indGeo] = indVB;
			geo[indGeo++] = pvVertexBuffer[indVB].geoNormal;
		}
		VECTOR3 sum;
		sum.as(0.0f, 0.0f, 0.0f);
		for (int i1 = 0; i1 < indGeo; i1++) {
			sum.x += geo[i1].x;
			sum.y += geo[i1].y;
			sum.z += geo[i1].z;
		}
		VECTOR3 ave;
		ave.x = sum.x / (float)indGeo;
		ave.y = sum.y / (float)indGeo;
		ave.z = sum.z / (float)indGeo;
		for (int i1 = 0; i1 < indGeo; i1++) {
			pvVertexBuffer[indVb[i1]].geoNormal = ave;
		}
	}

	fclose(fp);

	const UINT vbByteSize = (UINT)FCount * 3 * sizeof(VertexM);

	D3DCreateBlob(vbByteSize, &Vview->VertexBufferCPU);
	CopyMemory(Vview->VertexBufferCPU->GetBufferPointer(), pvVertexBuffer, vbByteSize);

	Vview->VertexByteStride = sizeof(VertexM);
	Vview->VertexBufferByteSize = vbByteSize;

	//一時的変数解放
	ARR_DELETE(pvCoord);
	ARR_DELETE(pvNormal);
	ARR_DELETE(pvTexture);
	ARR_DELETE(svList);
}

void MeshData::CreateMesh() {

	GetShaderByteCode(disp);

	CD3DX12_DESCRIPTOR_RANGE texTable, nortexTable;
	texTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	nortexTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);

	CD3DX12_ROOT_PARAMETER slotRootParameter[4];
	slotRootParameter[0].InitAsDescriptorTable(1, &texTable, D3D12_SHADER_VISIBILITY_ALL);
	slotRootParameter[1].InitAsDescriptorTable(1, &nortexTable, D3D12_SHADER_VISIBILITY_ALL);
	slotRootParameter[2].InitAsConstantBufferView(0);
	slotRootParameter[3].InitAsConstantBufferView(1);

	mRootSignature = CreateRs(4, slotRootParameter);

	//パイプラインステートオブジェクト生成
	mPSO = CreatePsoVsPs(vs, ps, mRootSignature.Get(), dx->pVertexLayout_MESH, alpha, blend);
	mPSO_B = CreatePsoVsHsDsPs(vsB, hs, ds, psB, mRootSignature.Get(), dx->pVertexLayout_MESH, alpha, blend);
}

void MeshData::GetTexture() {

	TextureNo *te = new TextureNo[MaterialCount];
	for (int i = 0; i < MaterialCount; i++) {
		te[i].diffuse = pMaterial[i].tex_no;
		te[i].normal = pMaterial[i].nortex_no;
		te[i].movie = m_on;
	}

	mSrvHeap = CreateSrvHeap(MaterialCount, texNum, te);

	for (int i = 0; i < MaterialCount; i++)
		Iview[i].IndexBufferGPU = dx->CreateDefaultBuffer(dx->md3dDevice.Get(),
			mCommandList, &piFaceBuffer[FaceCount * 3 * i], Iview[i].IndexBufferByteSize, Iview[i].IndexBufferUploader);

	ARR_DELETE(te);
	ARR_DELETE(piFaceBuffer);

	Vview->VertexBufferGPU = dx->CreateDefaultBuffer(dx->md3dDevice.Get(),
		mCommandList, pvVertexBuffer, Vview->VertexBufferByteSize, Vview->VertexBufferUploader);
	ARR_DELETE(pvVertexBuffer);
}

void MeshData::InstancedMap(float x, float y, float z, float thetaZ, float thetaY, float thetaX, float size) {
	dx->InstancedMap(&cb[sw], x, y, z, thetaZ, thetaY, thetaX, size);
}

void MeshData::CbSwap() {
	Lock();
	if (!UpOn) {
		upCount++;
		if (upCount > 1)UpOn = TRUE;//cb,2要素初回更新終了
	}
	sw = 1 - sw;//cbスワップ
	insNum = dx->ins_no;
	dx->ins_no = 0;
	Unlock();
	DrawOn = TRUE;
}

void MeshData::InstanceUpdate(float r, float g, float b, float a, float disp) {
	dx->MatrixMap2(&cb[sw], r, g, b, a, disp, 1.0f, 1.0f, 1.0f, 1.0f);
	CbSwap();
}

void MeshData::Update(float x, float y, float z, float r, float g, float b, float a, float thetaZ, float thetaY, float thetaX, float size, float disp) {
	dx->MatrixMap(&cb[sw], x, y, z, r, g, b, a, thetaZ, thetaY, thetaX, size, disp, 1.0f, 1.0f, 1.0f, 1.0f);
	CbSwap();
}

void MeshData::DrawOff() {
	DrawOn = FALSE;
}

void MeshData::Draw() {

	if (!UpOn | !DrawOn)return;

	Lock();
	mObjectCB->CopyData(0, cb[1 - sw]);
	Unlock();

	mCommandList->SetPipelineState(mPSO.Get());

	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(dx->mSwapChainBuffer[dx->mCurrBackBuffer].Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	ID3D12DescriptorHeap* descriptorHeaps[] = { mSrvHeap.Get() };
	mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	mCommandList->SetGraphicsRootSignature(mRootSignature.Get());

	mCommandList->IASetVertexBuffers(0, 1, &Vview->VertexBufferView());

	CD3DX12_GPU_DESCRIPTOR_HANDLE tex(mSrvHeap->GetGPUDescriptorHandleForHeapStart());
	for (int i = 0; i < MaterialCount; i++) {

		mCommandList->IASetIndexBuffer(&Iview[i].IndexBufferView());

		mCommandList->SetGraphicsRootDescriptorTable(0, tex);
		tex.Offset(1, dx->mCbvSrvUavDescriptorSize);//デスクリプタヒープのアドレス位置オフセットで次のテクスチャを読み込ませる
		if (pMaterial[i].nortex_no != -1) {
			mCommandList->IASetPrimitiveTopology(primType_drawB);
			//normalMapが存在する場合diffuseの次に格納されている
			mCommandList->SetGraphicsRootDescriptorTable(1, tex);
			tex.Offset(1, dx->mCbvSrvUavDescriptorSize);
			mCommandList->SetPipelineState(mPSO_B.Get());//normalMap有り無しでPSO切り替え
		}
		else {
			mCommandList->IASetPrimitiveTopology(primType_draw);
			mCommandList->SetPipelineState(mPSO.Get());
		}
		mCommandList->SetGraphicsRootConstantBufferView(2, mObjectCB->Resource()->GetGPUVirtualAddress());
		UINT mElementByteSize = (sizeof(CONSTANT_BUFFER2) + 255) & ~255;
		mCommandList->SetGraphicsRootConstantBufferView(3, mObject_MESHCB->Resource()->GetGPUVirtualAddress() + mElementByteSize * i);

		mCommandList->DrawIndexedInstanced(Iview[i].IndexCount, insNum, 0, 0, 0);
	}

	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(dx->mSwapChainBuffer[dx->mCurrBackBuffer].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
}