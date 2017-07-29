//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　          SkinMeshクラス(FBX_SDK)                           **//
//**                                                                                     **//
//*****************************************************************************************//

//Smoothie3Dでモデル→Blenderでアニメーションの場合,-Z前方,Y上で出力する
//makehumanでモデル→Blenderでアニメーション(カーネギーメロン大学でGETしたモーションキャプチャデータ使用)
//の場合,Y前方,Z上で出力する

#define _CRT_SECURE_NO_WARNINGS
#define FBX_PCS 5
#include "Dx12Process.h"
#include <string.h>

using namespace std;

mutex SkinMesh::mtx;

volatile bool SkinMesh::stInitFBX_ON = FALSE;
volatile bool SkinMesh::stSetNewPose_ON = FALSE;

FbxManager *SkinMesh::m_pSdkManager = NULL;
FbxImporter *SkinMesh::m_pImporter = NULL;

SkinMesh_sub::SkinMesh_sub() {
	current_frame = 0.0f;
	centering = TRUE;
	offset = FALSE;
	cx = cy = cz = 0.0f;
	connect_step = 3000.0f;
}

SkinMesh_sub::~SkinMesh_sub() {
	if (m_pmyScene) m_pmyScene->Destroy();
}

bool SkinMesh_sub::Create(CHAR *szFileName) {
	int iFormat = -1;
	SkinMesh::m_pImporter->Initialize((const char*)szFileName, iFormat);
	m_pmyScene = FbxScene::Create(SkinMesh::m_pSdkManager, "my scene");
	return SkinMesh::m_pImporter->Import(m_pmyScene);
}

void SkinMesh::CreateManager() {
	m_pSdkManager = FbxManager::Create();
	m_pImporter = FbxImporter::Create(m_pSdkManager, "my importer");
}

void SkinMesh::DeleteManager() {
	if (m_pSdkManager) m_pSdkManager->Destroy();
}

SkinMesh::SkinMesh() {
	ZeroMemory(this, sizeof(SkinMesh));
	dx = Dx12Process::GetInstance();
	mCommandList = dx->dx_sub[0].mCommandList.Get();
	fbx = new SkinMesh_sub[FBX_PCS];
	m_ppSubAnimationBone = NULL;
	m_pClusterName = NULL;
	AnimLastInd = -1;
	BoneConnect = -1.0f;
	pvVB_delete_f = TRUE;
	pvVB = NULL;
}

SkinMesh::~SkinMesh() {
	ARR_DELETE(pvVB);
	ARR_DELETE(m_pClusterName);
	ARR_DELETE(m_ppSubAnimationBone);
	ARR_DELETE(m_pLastBoneMatrix);
	ARR_DELETE(m_pdwNumVert);
	ARR_DELETE(m_ppNodeArray);
	ARR_DELETE(m_BoneArray);
	ARR_DELETE(m_ppCluster);
	ARR_DELETE(m_pMaterial);

	S_DELETE(mObjectCB0);
	S_DELETE(mObjectCB1);
	S_DELETE(mObject_BONES);

	DestroyFBX();
}

void SkinMesh::SetCommandList(int no) {
	com_no = no;
	mCommandList = dx->dx_sub[com_no].mCommandList.Get();
}

void SkinMesh::SetState(bool al, bool bl) {
	alpha = al;
	blend = bl;
}

void SkinMesh::ObjCentering(bool f, int ind) {
	fbx[ind].centering = f;
	fbx[ind].offset = FALSE;
	fbx[ind].cx = fbx[ind].cy = fbx[ind].cz = 0.0f;
}

void SkinMesh::CreateRotMatrix(float thetaZ, float thetaY, float thetaX, int ind) {
	MATRIX rotZ, rotY, rotX, rotZY;
	MatrixIdentity(&fbx[ind].rotZYX);
	MatrixRotationZ(&rotZ, thetaZ);
	MatrixRotationY(&rotY, thetaY);
	MatrixRotationX(&rotX, thetaX);
	MatrixMultiply(&rotZY, &rotZ, &rotY);
	MatrixMultiply(&fbx[ind].rotZYX, &rotZY, &rotX);
}

void SkinMesh::ObjCentering(float x, float y, float z, float thetaZ, float thetaY, float thetaX, int ind) {
	fbx[ind].centering = TRUE;
	fbx[ind].offset = FALSE;
	fbx[ind].cx = x;
	fbx[ind].cy = y;
	fbx[ind].cz = z;
	CreateRotMatrix(thetaZ, thetaY, thetaX, ind);
}

void SkinMesh::ObjOffset(float x, float y, float z, float thetaZ, float thetaY, float thetaX, int ind) {
	fbx[ind].centering = TRUE;
	fbx[ind].offset = TRUE;
	fbx[ind].cx = x;
	fbx[ind].cy = y;
	fbx[ind].cz = z;
	CreateRotMatrix(thetaZ, thetaY, thetaX, ind);
}

int SkinMesh::SearchNodeCount(FbxNode *pnode, FbxNodeAttribute::EType SearchType) {
	static int matchNodeCount = 0;//スレッドセーフにする場合はクラス変数に変更する

	if (!pnode) { matchNodeCount = 0; return 0; }//pNodeにNULLを与えるとリセット

	FbxNodeAttribute* pAttr = pnode->GetNodeAttribute();//ノードタイプ取得(rootNodeの場合NULLになる)
	if (pAttr && pAttr->GetAttributeType() == SearchType && strncmp("GZM_", pnode->GetName(), 4) != 0) matchNodeCount++;//タイプ一致の場合カウント

	int childNodeCount = pnode->GetChildCount();
	if (childNodeCount == 0)return matchNodeCount;//ここで探索終了の可能性が有るのでカウント数を返す

	for (int i = 0; i < childNodeCount; i++) {
		FbxNode *pChild = pnode->GetChild(i);  //子Nodeを取得
		SearchNodeCount(pChild, SearchType);
	}

	return matchNodeCount;//ここで探索終了の可能性が有るのでカウント数を返す
}

FbxNode *SkinMesh::SearchNode(FbxNode *pnode, FbxNodeAttribute::EType SearchType, int Ind) {
	static int matchNodeCount = 0;

	if (!pnode) { matchNodeCount = 0; return NULL; }//リセット

	FbxNodeAttribute* pAttr = pnode->GetNodeAttribute();//ノードタイプ取得(rootNodeの場合NULLになる)
	if (pAttr && pAttr->GetAttributeType() == SearchType && strncmp("GZM_", pnode->GetName(), 4) != 0) { //探索Nodeタイプが一致したか
		if (matchNodeCount == Ind) {//探索Nodeが一致したか
			return pnode;//探索完了
		}
		matchNodeCount++;//一致しない場合カウント
	}

	//rootNodeまたは探索未完の場合子ノード確認, 無い場合NULLを返す
	int childNodeCount = pnode->GetChildCount();
	if (childNodeCount == 0)return NULL;

	//子Node探索
	for (int i = 0; i < childNodeCount; i++) {
		FbxNode *pChild = pnode->GetChild(i);  //子Nodeを取得
		FbxNode *returnNode = SearchNode(pChild, SearchType, Ind);
		if (returnNode)return returnNode;//NULL以外が返った場合探索完了になるのでNodeを返す
		//NULLの場合子Node探索を継続
	}

	//子ノード全てNULLの場合
	return NULL;
}

HRESULT SkinMesh::InitFBX(CHAR *szFileName, int p) {

	static bool f = FALSE;

	stInitFBX_ON = TRUE;
	while (stSetNewPose_ON);//アニメーション中の場合終了まで待つ
	f = fbx[p].Create(szFileName);
	stInitFBX_ON = FALSE;

	if (f)return S_OK;
	return E_FAIL;
}

FbxScene *SkinMesh::GetScene(int p) {
	return fbx[p].m_pmyScene;
}

void SkinMesh::DestroyFBX() {
	stInitFBX_ON = TRUE;
	while (stSetNewPose_ON);//アニメーション中の場合終了まで待つ
	ARR_DELETE(fbx);
	stInitFBX_ON = FALSE;
}

HRESULT SkinMesh::ReadSkinInfo(MY_VERTEX_S *pvVB) {

	FbxMesh *pmesh = m_ppNodeArray[0]->GetMesh();
	FbxDeformer *pDeformer = pmesh->GetDeformer(0);
	FbxSkin *pSkinInfo = static_cast<FbxSkin*>(pDeformer);

	for (int i = 0; i < m_iNumBone; i++) {
		m_ppCluster[i] = pSkinInfo->GetCluster(i);//行列関係はどのメッシュからでも同じ物を得られるようだ
		const char *name = m_ppCluster[i]->GetName();
		strcpy(&m_pClusterName[i * 255], name);//ボーンの名前保持
	}

	int VertexStart = 0;
	for (int m = 0; m < NodeArraypcs; m++) {

		FbxMesh *pmesh = m_ppNodeArray[m]->GetMesh();
		FbxDeformer *pDeformer = pmesh->GetDeformer(0);
		FbxSkin *pSkinInfo = static_cast<FbxSkin*>(pDeformer);

		//各Boneのウエイト,インデックスを調べ頂点配列に加える
		for (int i = 0; i < m_iNumBone; i++) {
			FbxCluster *cl = pSkinInfo->GetCluster(i);
			int iNumIndex = cl->GetControlPointIndicesCount();//このボーンに影響を受ける頂点インデックス数
			int *piIndex = cl->GetControlPointIndices();     //このボーンに影響を受ける頂点のインデックス配列
			double *pdWeight = cl->GetControlPointWeights();//このボーンに影響を受ける頂点のウエイト配列

			for (int k = 0; k < iNumIndex; k++) {
				int index = piIndex[k];      //影響を受ける頂点
				double weight = pdWeight[k];//ウエイト
				for (int m = 0; m < 4; m++) {
					//各Bone毎に影響を受ける頂点のウエイトを一番大きい数値に更新していく
					if (weight > pvVB[index + VertexStart].bBoneWeight[m]) {//調べたウエイトの方が大きい
						pvVB[index + VertexStart].bBoneIndex[m] = i;//Boneインデックス登録
						pvVB[index + VertexStart].bBoneWeight[m] = (float)weight;//ウエイト登録
						break;
					}
				}
			}
		}
		VertexStart += m_pdwNumVert[m];
	}

	//ウエイト正規化
	for (DWORD i = 0; i < VerAllpcs; i++) {
		float we = 0;
		for (int j = 0; j < 4; j++) {
			we += pvVB[i].bBoneWeight[j];
		}
		float we1 = 1.0f / we;
		for (int j = 0; j < 4; j++) {
			pvVB[i].bBoneWeight[j] *= we1;
		}
	}

	//ボーン設定
	FbxAMatrix mat;

	for (int i = 0; i < m_iNumBone; i++) {

		m_ppCluster[i]->GetTransformLinkMatrix(mat);

		for (int x = 0; x < 4; x++) {
			for (int y = 0; y < 4; y++) {
				m_BoneArray[i].mBindPose.m[y][x] = (float)mat.Get(y, x);
			}
		}
	}

	return S_OK;
}

void SkinMesh::SetConnectStep(int ind, float step) {
	fbx[ind].connect_step = step;
}

void SkinMesh::Vertex_hold() {
	pvVB_delete_f = FALSE;
}

HRESULT SkinMesh::GetFbx(CHAR* szFileName) {
	//FBXローダーを初期化
	if (FAILED(InitFBX(szFileName, 0)))
	{
		MessageBox(0, L"FBXローダー初期化失敗", NULL, MB_OK);
		return E_FAIL;
	}
	return S_OK;
}

void SkinMesh::GetBuffer(float end_frame) {

	mObjectCB0 = new UploadBuffer<CONSTANT_BUFFER>(dx->md3dDevice.Get(), 1, true);
	mObject_BONES = new UploadBuffer<SHADER_GLOBAL_BONES>(dx->md3dDevice.Get(), 1, true);

	fbx[0].end_frame = end_frame;
	FbxScene *pScene = GetScene(0);//シーン取得
	FbxNode *pNodeRoot = pScene->GetRootNode();//ルートノード取得

	NodeArraypcs = SearchNodeCount(pNodeRoot, FbxNodeAttribute::eMesh);//eMesh数カウント
	SearchNodeCount(NULL, FbxNodeAttribute::eMesh);//リセット

	//各メッシュへのポインタ配列取得
	m_ppNodeArray = new FbxNode*[NodeArraypcs];
	for (int i = 0; i < NodeArraypcs; i++) {
		m_ppNodeArray[i] = SearchNode(pNodeRoot, FbxNodeAttribute::eMesh, i);
		SearchNode(NULL, FbxNodeAttribute::eMesh, 0);//リセット
	}

	//事前にメッシュ毎頂点数、ポリゴン数マテリアル数等を調べる
	m_pdwNumVert = new DWORD[NodeArraypcs];
	MateAllpcs = 0;//マテリアル全数
	m_pMaterialCount = new DWORD[NodeArraypcs];
	VerAllpcs = 0;//頂点全数
	pdwNumFace = new DWORD[NodeArraypcs];

	//カウント
	for (int i = 0; i < NodeArraypcs; i++) {
		FbxMesh *pFbxMesh = m_ppNodeArray[i]->GetMesh();
		m_pdwNumVert[i] = pFbxMesh->GetControlPointsCount();//メッシュ毎頂点数
		m_pMaterialCount[i] = m_ppNodeArray[i]->GetMaterialCount();//メッシュ毎マテリアル数
		VerAllpcs += m_pdwNumVert[i];                 //頂点全数カウント
		MateAllpcs += m_pMaterialCount[i];           //マテリアル全数カウント
		pdwNumFace[i] = pFbxMesh->GetPolygonCount();//メッシュ毎ポリゴン数
	}

	//メッシュ毎4頂点分割前インデックス配列生成
	IndexCount34Me = new int[NodeArraypcs];
	//マテリアル毎4頂点分割後インデックス配列生成
	IndexCount3M = new int[MateAllpcs];

	//マテリアル配列生成
	m_pMaterial = new MY_MATERIAL_S[MateAllpcs];

	//マテリアルコンスタントバッファ
	mObjectCB1 = new UploadBuffer<SHADER_GLOBAL1>(dx->md3dDevice.Get(), MateAllpcs, true);

	//VBO
	Vview = std::make_unique<VertexView>();
	//IBO マテリアル毎に生成する
	Iview = std::make_unique<IndexView[]>(MateAllpcs);

	pvVB = new MY_VERTEX_S[VerAllpcs];//頂点バッファはそのままの数で確保,不要な頂点が存在してもIndexでアクセスするので問題無し

	//インデックス数計算(ポリゴン1個に付き頂点 2個～5個が存在することが有るが
	//fbxからIndex配列コピー時に全てコピーするのでそのままの個数をカウントする)
	for (int k = 0; k < NodeArraypcs; k++) {
		IndexCount34Me[k] = 0;
		FbxMesh *pFbxMesh = m_ppNodeArray[k]->GetMesh();
		for (DWORD i = 0; i < pdwNumFace[k]; i++) {
			IndexCount34Me[k] += pFbxMesh->GetPolygonSize(i);//メッシュ毎の分割前頂点インデックス数
		}
	}

	int mInd = 0;
	for (int k = 0; k < NodeArraypcs; k++) {
		FbxMesh *pFbxMesh = m_ppNodeArray[k]->GetMesh();
		FbxLayerElementMaterial *mat = pFbxMesh->GetLayer(0)->GetMaterials();//レイヤー0のマテリアル取得
		for (DWORD j = 0; j < m_pMaterialCount[k]; j++) {
			IndexCount3M[mInd] = 0;
			for (DWORD i = 0; i < pdwNumFace[k]; i++) {
				int matId = mat->GetIndexArray().GetAt(i);//ポリゴンiの関連するマテリアル番号取得
				if (matId == j) {//現在処理中のマテリアルと一致した場合以下処理実行(1Meshに2以上のマテリアルが有る場合の処理)
					int pcs = pFbxMesh->GetPolygonSize(i);//各ポリゴンの頂点インデックス数
					switch (pcs) {
					case 3:
						IndexCount3M[mInd] += 3;
						break;
					case 4:
						IndexCount3M[mInd] += 6;
						break;
					}
				}
			}
			mInd++;
		}
	}

	piFaceBuffer = new int*[NodeArraypcs];
	for (int i = 0; i < NodeArraypcs; i++) {
		piFaceBuffer[i] = new int[IndexCount34Me[i]];
	}

	pIndex = new int*[MateAllpcs];
	for (int i = 0; i < MateAllpcs; i++) {
		pIndex[i] = new int[IndexCount3M[i]];
	}

	FbxMesh *pmesh = m_ppNodeArray[0]->GetMesh();
	FbxDeformer *pDeformer = pmesh->GetDeformer(0);
	FbxSkin *pSkinInfo = static_cast<FbxSkin*>(pDeformer);
	m_iNumBone = pSkinInfo->GetClusterCount();//どのメッシュからでも同じボーン数が得られているようだ
	m_ppCluster = new  FbxCluster*[m_iNumBone];//ボーン情報配列
	m_pClusterName = new char[m_iNumBone * 255];

	//ボーンを生成
	m_BoneArray = new BONE[m_iNumBone];
	m_pLastBoneMatrix = new MATRIX[m_iNumBone];
}

void SkinMesh::SetVertex() {

	//メッシュ毎に配列格納処理
	int mInd = 0;//マテリアル内カウント
	DWORD VerArrStart = 0;
	for (int m = 0; m < NodeArraypcs; m++) {
		FbxMesh *pFbxMesh = m_ppNodeArray[m]->GetMesh();
		FbxNode *pNode = pFbxMesh->GetNode();

		FbxVector4 *pCoord = pFbxMesh->GetControlPoints();//FbxMeshから頂点ローカル座標配列取得, Directxに対してZが逆

														  //頂点配列をfbxからコピー
		for (DWORD i = 0; i < m_pdwNumVert[m]; i++) {
			pvVB[i + VerArrStart].vPos.x = (float)-pCoord[i][0];//FBXは右手座標系なのでxあるいはｚを反転
			pvVB[i + VerArrStart].vPos.y = (float)pCoord[i][1];
			pvVB[i + VerArrStart].vPos.z = (float)pCoord[i][2];
		}

		int *piIndex = pFbxMesh->GetPolygonVertices();//fbxから頂点インデックス配列取得
		memcpy(piFaceBuffer[m], piIndex, sizeof(int) * IndexCount34Me[m]);//コピー

																		  //法線読み込み
		FbxVector4 Normal;
		for (DWORD i = 0; i < pdwNumFace[m]; i++) {
			int iStartIndex = pFbxMesh->GetPolygonVertexIndex(i);//ポリゴンを構成する最初のインデックス取得
			int pcs = pFbxMesh->GetPolygonSize(i);//各ポリゴンの頂点数
			if (pcs != 3 && pcs != 4)continue;//頂点数3個,4個以外はスキップ
			int iVert0Index, iVert1Index, iVert2Index, iVert3Index;

			//以下の法線読み込みの際GetPolygonVertexNormalでインデックス順で読み込むので
			//頂点バッファもインデックス順で参照する為にインデックスを取得する
			iVert0Index = piFaceBuffer[m][iStartIndex] + VerArrStart;
			iVert1Index = piFaceBuffer[m][iStartIndex + 1] + VerArrStart;
			iVert2Index = piFaceBuffer[m][iStartIndex + 2] + VerArrStart;
			if (pcs == 4)iVert3Index = piFaceBuffer[m][iStartIndex + 3] + VerArrStart;

			if (iVert0Index < 0) continue;
			pFbxMesh->GetPolygonVertexNormal(i, 0, Normal);//(polyInd, verInd, FbxVector4)
			pvVB[iVert0Index].vNorm.x = (float)-Normal[0];//FBXは右手座標系なのでxあるいはｚを反転
			pvVB[iVert0Index].vNorm.y = (float)Normal[1];
			pvVB[iVert0Index].vNorm.z = (float)Normal[2];

			pFbxMesh->GetPolygonVertexNormal(i, 1, Normal);
			pvVB[iVert1Index].vNorm.x = (float)-Normal[0];
			pvVB[iVert1Index].vNorm.y = (float)Normal[1];
			pvVB[iVert1Index].vNorm.z = (float)Normal[2];

			pFbxMesh->GetPolygonVertexNormal(i, 2, Normal);
			pvVB[iVert2Index].vNorm.x = (float)-Normal[0];
			pvVB[iVert2Index].vNorm.y = (float)Normal[1];
			pvVB[iVert2Index].vNorm.z = (float)Normal[2];

			if (pcs == 4) {
				pFbxMesh->GetPolygonVertexNormal(i, 3, Normal);
				pvVB[iVert3Index].vNorm.x = (float)-Normal[0];
				pvVB[iVert3Index].vNorm.y = (float)Normal[1];
				pvVB[iVert3Index].vNorm.z = (float)Normal[2];
			}
		}

		//テクスチャー座標読み込み
		FbxVector2 UV;
		FbxLayerElementUV *pUV = 0;
		pUV = pFbxMesh->GetLayer(0)->GetUVs();
		const char *uvname = pUV->GetName();
		FbxLayerElement::EMappingMode mappingMode = pUV->GetMappingMode();
		bool UnMap = TRUE;
		if (mappingMode == FbxLayerElement::eByPolygonVertex)UnMap = FALSE;

		for (DWORD i = 0; i < pdwNumFace[m]; i++) {
			int iStartIndex = pFbxMesh->GetPolygonVertexIndex(i);//ポリゴンを構成する最初のインデックス取得
			int pcs = pFbxMesh->GetPolygonSize(i);//各ポリゴンの頂点数
			if (pcs != 3 && pcs != 4)continue;//頂点数3個,4個以外はスキップ
			int iVert0Index, iVert1Index, iVert2Index, iVert3Index;

			iVert0Index = piFaceBuffer[m][iStartIndex] + VerArrStart;
			iVert1Index = piFaceBuffer[m][iStartIndex + 1] + VerArrStart;
			iVert2Index = piFaceBuffer[m][iStartIndex + 2] + VerArrStart;
			if (pcs == 4)iVert3Index = piFaceBuffer[m][iStartIndex + 3] + VerArrStart;

			if (iVert0Index < 0) continue;
			pFbxMesh->GetPolygonVertexUV(i, 0, uvname, UV, UnMap);//(polyInd, verInd, UV_Name, FbxVector2_UV, bool_UnMap)
			pvVB[iVert0Index].vTex.x = (float)UV[0];
			pvVB[iVert0Index].vTex.y = 1.0f - (float)UV[1];//(1.0f-UV)

			pFbxMesh->GetPolygonVertexUV(i, 1, uvname, UV, UnMap);
			pvVB[iVert1Index].vTex.x = (float)UV[0];
			pvVB[iVert1Index].vTex.y = 1.0f - (float)UV[1];

			pFbxMesh->GetPolygonVertexUV(i, 2, uvname, UV, UnMap);
			pvVB[iVert2Index].vTex.x = (float)UV[0];
			pvVB[iVert2Index].vTex.y = 1.0f - (float)UV[1];

			if (pcs == 4) {
				pFbxMesh->GetPolygonVertexUV(i, 3, uvname, UV, UnMap);
				pvVB[iVert3Index].vTex.x = (float)UV[0];
				pvVB[iVert3Index].vTex.y = 1.0f - (float)UV[1];
			}
		}

		for (DWORD i = 0; i < m_pMaterialCount[m]; i++) {
			//フォンモデルを想定
			FbxSurfaceMaterial *pMaterial = pNode->GetMaterial(i);
			FbxSurfacePhong *pPhong = (FbxSurfacePhong*)pMaterial;

			typedef FbxPropertyT<FbxDouble3> FbxPropertyDouble3;

			//拡散反射光
			FbxPropertyDouble3 d3Diffuse = pPhong->Diffuse;
			m_pMaterial[mInd].Kd.x = (float)d3Diffuse.Get()[0];
			m_pMaterial[mInd].Kd.y = (float)d3Diffuse.Get()[1];
			m_pMaterial[mInd].Kd.z = (float)d3Diffuse.Get()[2];
			m_pMaterial[mInd].Kd.w = 1.0f;//透けさせたい場合ここをどうにかする

										  //テクスチャー
			FbxProperty lProperty;
			lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
			FbxTexture *texture = FbxCast<FbxTexture>(lProperty.GetSrcObject<FbxTexture>(0));
			if (texture) {
				FbxFileTexture *fileTexture = FbxCast<FbxFileTexture>(texture);
				strcpy_s(m_pMaterial[mInd].szTextureName, fileTexture->GetFileName());
				//ファイル名を元に既にデコード済みのテクスチャ番号を読み込む
				m_pMaterial[mInd].tex_no = dx->GetTexNumber(m_pMaterial[mInd].szTextureName);
			}
			else {
				strcpy_s(m_pMaterial[mInd].szTextureName, pMaterial->GetName());//テクスチャ名が無い場合マテリアル名から
																				//ファイル名を元に既にデコード済みのテクスチャ番号を読み込む
				m_pMaterial[mInd].tex_no = dx->GetTexNumber(m_pMaterial[mInd].szTextureName);
			}

			int iCount = 0;
			int polygon_cnt = 0;
			for (DWORD k = 0; k < pdwNumFace[m]; k++) {
				FbxLayerElementMaterial* mat = pFbxMesh->GetLayer(0)->GetMaterials();//レイヤー0のマテリアル取得
				int matId = mat->GetIndexArray().GetAt(k);//ポリゴンkの関連するマテリアル番号取得
				int pcs = pFbxMesh->GetPolygonSize(k);//各ポリゴンの頂点数
				if (pcs != 3 && pcs != 4)continue;//頂点数3個,4個以外はスキップ
				if (matId == i)//現在処理中のマテリアルと一致した場合以下処理実行(1Meshに2以上のマテリアルが有る場合の処理)
				{
					pIndex[mInd][iCount] = pFbxMesh->GetPolygonVertex(k, 0) + VerArrStart;//ポリゴンkを構成する0番目の頂点のインデックス番号
					pIndex[mInd][iCount + 1] = pFbxMesh->GetPolygonVertex(k, 1) + VerArrStart;
					pIndex[mInd][iCount + 2] = pFbxMesh->GetPolygonVertex(k, 2) + VerArrStart;
					iCount += 3;

					//4頂点は{ 0, 1, 2 }, { 0, 2, 3 }で分割するよう処理
					if (pcs == 4) {
						pIndex[mInd][iCount] = pFbxMesh->GetPolygonVertex(k, 0) + VerArrStart;
						pIndex[mInd][iCount + 1] = pFbxMesh->GetPolygonVertex(k, 2) + VerArrStart;
						pIndex[mInd][iCount + 2] = pFbxMesh->GetPolygonVertex(k, 3) + VerArrStart;
						iCount += 3;//4頂点を分割後はインデックスは3頂点の倍
					}
					polygon_cnt++;
				}
			}

			//インデックスバッファ生成1段回目
			if (iCount > 0) CreateIndexBuffer(iCount, pIndex[mInd], mInd);
			m_pMaterial[mInd].dwNumFace = polygon_cnt;//そのマテリアル内のポリゴン数	
			mInd++;
		}
		VerArrStart += m_pdwNumVert[m];
	}

	//各一時格納用配列解放
	for (int i = 0; i < NodeArraypcs; i++) {
		ARR_DELETE(piFaceBuffer[i]);
	}
	ARR_DELETE(piFaceBuffer);

	ARR_DELETE(m_pMaterialCount);
	ARR_DELETE(IndexCount34Me);
	ARR_DELETE(IndexCount3M);
	ARR_DELETE(pdwNumFace);

	for (int i = 0; i < MateAllpcs; i++) {
		SHADER_GLOBAL1 sg;
		sg.vDiffuse = m_pMaterial[i].Kd;//ディフューズカラーをシェーダーに渡す
		mObjectCB1->CopyData(i, sg);
	}

	//スキン情報(ジョイント, ウェイト)
	ReadSkinInfo(pvVB);
}

void SkinMesh::CreateFromFBX() {

	//インデックスバッファ生成2段回目, 一時格納配列解放
	for (int i = 0; i < MateAllpcs; i++) {
		CreateIndexBuffer2(pIndex[i], i);
		ARR_DELETE(pIndex[i]);
	}
	ARR_DELETE(pIndex);

	//バーテックスバッファー作成
	const UINT vbByteSize = (UINT)VerAllpcs * sizeof(MY_VERTEX_S);

	D3DCreateBlob(vbByteSize, &Vview->VertexBufferCPU);
	CopyMemory(Vview->VertexBufferCPU->GetBufferPointer(), pvVB, vbByteSize);
	Vview->VertexBufferGPU = dx->CreateDefaultBuffer(dx->md3dDevice.Get(),
		mCommandList, pvVB, vbByteSize, Vview->VertexBufferUploader);
	Vview->VertexByteStride = sizeof(MY_VERTEX_S);
	Vview->VertexBufferByteSize = vbByteSize;

	if (pvVB_delete_f)ARR_DELETE(pvVB);//使わない場合解放

	vs = dx->pVertexShader_SKIN.Get();
	ps = dx->pPixelShader_SKIN.Get();

	CD3DX12_DESCRIPTOR_RANGE texTable;
	texTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	CD3DX12_ROOT_PARAMETER slotRootParameter[4];
	slotRootParameter[0].InitAsDescriptorTable(1, &texTable, D3D12_SHADER_VISIBILITY_ALL);
	slotRootParameter[1].InitAsConstantBufferView(0);
	slotRootParameter[2].InitAsConstantBufferView(1);
	slotRootParameter[3].InitAsConstantBufferView(2);

	auto staticSamplers = dx->GetStaticSamplers();

	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(4, slotRootParameter,
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
	//RootSignature生成
	dx->md3dDevice->CreateRootSignature(
		0,
		serializedRootSig->GetBufferPointer(),
		serializedRootSig->GetBufferSize(),
		IID_PPV_ARGS(mRootSignature.GetAddressOf()));

	//マテリアルの数だけDescriptorを用意する
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = MateAllpcs;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	dx->md3dDevice->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&mSrvHeap));

	//パイプラインステートオブジェクト生成
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	psoDesc.InputLayout = { dx->pVertexLayout_SKIN.data(), (UINT)dx->pVertexLayout_SKIN.size() };
	psoDesc.pRootSignature = mRootSignature.Get();
	psoDesc.VS =
	{
		reinterpret_cast<BYTE*>(vs->GetBufferPointer()),
		vs->GetBufferSize()
	};
	psoDesc.PS =
	{
		reinterpret_cast<BYTE*>(ps->GetBufferPointer()),
		ps->GetBufferSize()
	};
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.BlendState.IndependentBlendEnable = FALSE;
	psoDesc.BlendState.AlphaToCoverageEnable = alpha;//アルファテストon/off
	psoDesc.BlendState.RenderTarget[0].BlendEnable = blend;//ブレンドon/off
	psoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	psoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = dx->mBackBufferFormat;
	psoDesc.SampleDesc.Count = dx->m4xMsaaState ? 4 : 1;
	psoDesc.SampleDesc.Quality = dx->m4xMsaaState ? (dx->m4xMsaaQuality - 1) : 0;
	psoDesc.DSVFormat = dx->mDepthStencilFormat;
	dx->md3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPSO));

	GetTexture();
}

HRESULT SkinMesh::GetFbxSub(CHAR* szFileName, int ind) {
	if (ind <= 0) {
		MessageBox(0, L"FBXローダー初期化失敗", NULL, MB_OK);
		return E_FAIL;
	}

	if (FAILED(InitFBX(szFileName, ind))) {
		MessageBox(0, L"FBXローダー初期化失敗", NULL, MB_OK);
		return E_FAIL;
	}
	return S_OK;
}

HRESULT SkinMesh::GetBuffer_Sub(int ind, float end_frame) {

	FbxScene *pScene = GetScene(ind);//シーン取得
	FbxNode *pNodeRoot = pScene->GetRootNode();//ルートノード取得
	fbx[ind].end_frame = end_frame;

	int BoneNum = SearchNodeCount(pNodeRoot, FbxNodeAttribute::eSkeleton);
	if (BoneNum == 0) {
		MessageBox(0, L"FBXローダー初期化失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SearchNodeCount(NULL, FbxNodeAttribute::eSkeleton);//リセット

	if (!m_ppSubAnimationBone) {
		m_ppSubAnimationBone = new FbxNode*[(FBX_PCS - 1) * m_iNumBone];
	}
	return S_OK;
}

void SkinMesh::CreateFromFBX_SubAnimation(int ind) {

	FbxScene *pScene = GetScene(ind);//シーン取得
	FbxNode *pNodeRoot = pScene->GetRootNode();//ルートノード取得

	int loopind = 0;
	int searchCount = 0;
	while (loopind < m_iNumBone) {
		int sa_ind = (ind - 1) * m_iNumBone + loopind;
		m_ppSubAnimationBone[sa_ind] = SearchNode(pNodeRoot, FbxNodeAttribute::eSkeleton, searchCount + 1);
		searchCount++;
		SearchNode(NULL, FbxNodeAttribute::eSkeleton, 0);//リセット
		const char *name = m_ppSubAnimationBone[sa_ind]->GetName();
		char *name2 = &m_pClusterName[loopind * 255];//各Bone名の先頭アドレスを渡す
		while (*name != '\0')name++;//終端文字までポインタを進める
		while (*name2 != '\0')name2++;
		while (*(--name) == *(--name2) && *name2 != ' ');
		if (*name2 != ' ')continue;
		loopind++;
	}
}

void SkinMesh::CreateIndexBuffer(int cnt, int *pIndex, int IviewInd) {

	const UINT ibByteSize = (UINT)cnt * sizeof(UINT);

	D3DCreateBlob(ibByteSize, &Iview[IviewInd].IndexBufferCPU);
	CopyMemory(Iview[IviewInd].IndexBufferCPU->GetBufferPointer(), pIndex, ibByteSize);

	Iview[IviewInd].IndexFormat = DXGI_FORMAT_R32_UINT;
	Iview[IviewInd].IndexBufferByteSize = ibByteSize;
	Iview[IviewInd].IndexCount = cnt;
}

void SkinMesh::CreateIndexBuffer2(int *pIndex, int IviewInd) {

	Iview[IviewInd].IndexBufferGPU = dx->CreateDefaultBuffer(dx->md3dDevice.Get(),
		mCommandList, pIndex, Iview[IviewInd].IndexBufferByteSize, Iview[IviewInd].IndexBufferUploader);
}

//ボーンを次のポーズ位置にセットする
bool SkinMesh::SetNewPoseMatrices(float ti, int ind) {

	stSetNewPose_ON = TRUE;
	if (stInitFBX_ON) { stSetNewPose_ON = FALSE; return FALSE; }//FBX初期化中はアニメーションしない
	if (AnimLastInd == -1)AnimLastInd = ind;//最初に描画するアニメーション番号で初期化
	bool ind_change = FALSE;
	if (AnimLastInd != ind) {//アニメーションが切り替わった
		ind_change = TRUE; AnimLastInd = ind;
		fbx[ind].current_frame = 0.0f;//アニメーションが切り替わってるのでMatrix更新前にフレームを0に初期化
	}
	bool frame_end = FALSE;
	fbx[ind].current_frame += ti;
	if (fbx[ind].end_frame <= fbx[ind].current_frame) frame_end = TRUE;

	if (frame_end || ind_change) {
		for (int i = 0; i < m_iNumBone; i++) {
			for (int x = 0; x < 4; x++) {
				for (int y = 0; y < 4; y++) {
					m_pLastBoneMatrix[i].m[y][x] = m_BoneArray[i].mNewPose.m[y][x];
				}
			}
		}
		BoneConnect = 0.1f;
	}

	if (BoneConnect != -1.0f)fbx[ind].current_frame = 0.0f;

	int frame = (int)fbx[ind].current_frame;
	FbxTime time;
	time.SetTime(0, 0, 0, frame / 10, 0, FbxTime::eFrames60);

	bool subanm = TRUE;
	if (ind <= 0 || ind > FBX_PCS - 1)subanm = FALSE;

	FbxMatrix matf0;
	if (!subanm) {
		matf0 = m_ppCluster[0]->GetLink()->EvaluateGlobalTransform(time);
	}
	else {
		matf0 = m_ppSubAnimationBone[(ind - 1) * m_iNumBone]->EvaluateGlobalTransform(time);
	}

	MATRIX mat0_mov;
	MatrixIdentity(&mat0_mov);
	if (fbx[ind].offset) {
		MatrixTranslation(&mat0_mov, fbx[ind].cx, fbx[ind].cy, fbx[ind].cz);
	}
	else {
		MatrixTranslation(&mat0_mov, (float)-matf0.Get(3, 0) + fbx[ind].cx,
			(float)-matf0.Get(3, 1) + fbx[ind].cy,
			(float)-matf0.Get(3, 2) + fbx[ind].cz);
	}

	MATRIX pose;
	for (int i = 0; i < m_iNumBone; i++) {
		FbxMatrix mat;
		if (!subanm) {
			mat = m_ppCluster[i]->GetLink()->EvaluateGlobalTransform(time);
		}
		else {
			mat = m_ppSubAnimationBone[(ind - 1) * m_iNumBone + i]->EvaluateGlobalTransform(time);
		}

		for (int x = 0; x < 4; x++) {
			for (int y = 0; y < 4; y++) {
				if (fbx[ind].centering) {
					pose.m[y][x] = (float)mat.Get(y, x);
				}
				else {
					m_BoneArray[i].mNewPose.m[y][x] = (float)mat.Get(y, x);
				}
			}
		}
		if (fbx[ind].centering) {
			MATRIX tmp;
			MatrixMultiply(&tmp, &fbx[ind].rotZYX, &mat0_mov);
			MatrixMultiply(&m_BoneArray[i].mNewPose, &pose, &tmp);
		}
	}

	if (frame_end)fbx[ind].current_frame = 0.0f;

	if (BoneConnect != -1.0f) {
		if (fbx[ind].connect_step <= 0.0f || BoneConnect > 1.0f)BoneConnect = -1.0f;
		else {
			for (int i = 0; i < m_iNumBone; i++) {
				StraightLinear(&m_BoneArray[i].mNewPose, &m_pLastBoneMatrix[i], &m_BoneArray[i].mNewPose, BoneConnect += (ti / fbx[ind].connect_step));
			}
		}
	}
	stSetNewPose_ON = FALSE;
	return frame_end;
}

//次の（現在の）ポーズ行列を返す
MATRIX SkinMesh::GetCurrentPoseMatrix(int index) {
	MATRIX inv;
	MatrixIdentity(&inv);
	MatrixInverse(&inv, &m_BoneArray[index].mBindPose);//FBXのバインドポーズは初期姿勢（絶対座標）
	MATRIX ret;
	MatrixIdentity(&ret);
	MatrixMultiply(&ret, &inv, &m_BoneArray[index].mNewPose);//バインドポーズの逆行列とフレーム姿勢行列をかける

	return ret;
}

VECTOR3 SkinMesh::GetVertexPosition(int verNum, float adjustZ, float adjustY, float adjustX, float thetaZ, float thetaY, float thetaX, float scal) {

	//頂点にボーン行列を掛け出力
	VECTOR3 ret, pos;
	MATRIX rotZ, rotY, rotX, rotZY, rotZYX;
	MATRIX scale, scaro;
	MatrixScaling(&scale, scal, scal, scal);
	MatrixRotationZ(&rotZ, thetaZ);
	MatrixRotationY(&rotY, thetaY);
	MatrixRotationX(&rotX, thetaX);
	MatrixMultiply(&rotZY, &rotZ, &rotY);
	MatrixMultiply(&rotZYX, &rotZY, &rotX);
	MatrixMultiply(&scaro, &scale, &rotZYX);
	ret.x = ret.y = ret.z = 0.0f;

	for (int i = 0; i < 4; i++) {
		pos.x = pvVB[verNum].vPos.x;
		pos.y = pvVB[verNum].vPos.y;
		pos.z = pvVB[verNum].vPos.z;
		MATRIX m = GetCurrentPoseMatrix(pvVB[verNum].bBoneIndex[i]);
		VectorMatrixMultiply(&pos, &m);
		VectorMultiply(&pos, pvVB[verNum].bBoneWeight[i]);
		VectorAddition(&ret, &ret, &pos);
	}
	ret.x += adjustX;
	ret.y += adjustY;
	ret.z += adjustZ;
	VectorMatrixMultiply(&ret, &scaro);
	return ret;
}

void SkinMesh::MatrixMap_Bone(SHADER_GLOBAL_BONES *sgb) {

	for (int i = 0; i < m_iNumBone; i++)
	{
		MATRIX mat = GetCurrentPoseMatrix(i);
		MatrixTranspose(&mat);
		sgb->mBone[i] = mat;
	}
}

void SkinMesh::GetTexture() {
	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(mSrvHeap->GetCPUDescriptorHandleForHeapStart());
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	for (int i = 0; i < MateAllpcs; i++) {
		srvDesc.Format = dx->texture[m_pMaterial[i].tex_no]->GetDesc().Format;
		srvDesc.Texture2D.MipLevels = dx->texture[m_pMaterial[i].tex_no]->GetDesc().MipLevels;
		dx->md3dDevice->CreateShaderResourceView(dx->texture[m_pMaterial[i].tex_no], &srvDesc, hDescriptor);
		hDescriptor.Offset(1, dx->mCbvSrvUavDescriptorSize);
	}
}

void SkinMesh::CbSwap() {
	Lock();
	if (!UpOn) {
		upCount++;
		if (upCount > 1)UpOn = TRUE;//cb,2要素初回更新終了
	}
	sw = 1 - sw;//cbスワップ
	dx->ins_no = 0;
	Unlock();
	DrawOn = TRUE;
}

bool SkinMesh::Update(float time, float x, float y, float z, float r, float g, float b, float thetaZ, float thetaY, float thetaX, float size) {
	return Update(0, time, x, y, z, r, g, b, thetaZ, thetaY, thetaX, size);
}

bool SkinMesh::Update(int ind, float ti, float x, float y, float z, float r, float g, float b, float thetaZ, float thetaY, float thetaX, float size) {

	bool frame_end = FALSE;
	dx->MatrixMap(&cb[sw], x, y, z, r, g, b, thetaZ, thetaY, thetaX, size, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	if (ti != -1.0f)frame_end = SetNewPoseMatrices(ti, ind);
	MatrixMap_Bone(&sgb[sw]);
	CbSwap();
	return frame_end;
}

void SkinMesh::DrawOff() {
	DrawOn = FALSE;
}

void SkinMesh::Draw() {

	if (!UpOn | !DrawOn)return;

	Lock();
	mObjectCB0->CopyData(0, cb[1 - sw]);
	mObject_BONES->CopyData(0, sgb[1 - sw]);
	Unlock();

	mCommandList->SetPipelineState(mPSO.Get());
	mCommandList->RSSetViewports(1, &dx->mScreenViewport);
	mCommandList->RSSetScissorRects(1, &dx->mScissorRect);

	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(dx->mSwapChainBuffer[dx->mCurrBackBuffer].Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	//レンダーターゲットのセット
	mCommandList->OMSetRenderTargets(1, &CD3DX12_CPU_DESCRIPTOR_HANDLE(
		dx->mRtvHeap->GetCPUDescriptorHandleForHeapStart(),
		dx->mCurrBackBuffer,
		dx->mRtvDescriptorSize), true, &dx->mDsvHeap->GetCPUDescriptorHandleForHeapStart());

	ID3D12DescriptorHeap* descriptorHeaps[] = { mSrvHeap.Get() };
	mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	mCommandList->SetGraphicsRootSignature(mRootSignature.Get());

	mCommandList->IASetVertexBuffers(0, 1, &Vview->VertexBufferView());
	mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	CD3DX12_GPU_DESCRIPTOR_HANDLE tex(mSrvHeap->GetGPUDescriptorHandleForHeapStart());
	for (int i = 0; i < MateAllpcs; i++) {
		//使用されていないマテリアル対策
		if (m_pMaterial[i].dwNumFace == 0)
		{
			continue;
		}
		mCommandList->IASetIndexBuffer(&Iview[i].IndexBufferView());

		mCommandList->SetGraphicsRootDescriptorTable(0, tex);
		tex.Offset(1, dx->mCbvSrvUavDescriptorSize);//デスクリプタヒープのアドレス位置オフセットで次のテクスチャを読み込ませる

		mCommandList->SetGraphicsRootConstantBufferView(1, mObjectCB0->Resource()->GetGPUVirtualAddress());
		UINT mElementByteSize = (sizeof(SHADER_GLOBAL1) + 255) & ~255;
		mCommandList->SetGraphicsRootConstantBufferView(2, mObjectCB1->Resource()->GetGPUVirtualAddress() + mElementByteSize * i);
		mCommandList->SetGraphicsRootConstantBufferView(3, mObject_BONES->Resource()->GetGPUVirtualAddress());

		mCommandList->DrawIndexedInstanced(Iview[i].IndexCount, 1, 0, 0, 0);
	}

	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(dx->mSwapChainBuffer[dx->mCurrBackBuffer].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
}


