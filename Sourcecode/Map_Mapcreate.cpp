//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Mapクラス                                       **//
//**                                    Mapcreate関数                                    **//
//*****************************************************************************************//

#include "Map.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

int Map::map_no_s;
MapStPos Map::MPos = POS_ST;
int Map::boss_killed[5];

int Map::GetMapNo() {
	return map_no_s;
}

void Map::SetMapNo(int no) {
	map_no_s = no;
}

void Map::SetBossKilled(int i, int f) {

	if (i == -1) {
		for (int i1 = 0; i1 < 5; i1++) {
			boss_killed[i1] = 0;
		}
		return;
	}
	boss_killed[i] = f;
}

int *Map::GetBossKilled() {
	return boss_killed;
}

int Map::GetBossKilled(int map_no) {
	return boss_killed[map_no];
}

void Map::SetMapHistory(MapHistoryData *maphi) {
	maphis = *maphi;
}

MapHistoryData *Map::GetMapHistory() {
	return &maphis;
}

Map::Map(Position::H_Pos* h_p, Hero* hero) {

	map_no = map_no_s;
	dx = Dx12Process::GetInstance();
	text = DxText::GetInstance();
	he = hero;
	map_text_f = 0;
	comNo = 0;
	recover_p_f = FALSE;
	boss_p_f = FALSE;
	elevator_UP = FALSE;
	elevator_DOWN = FALSE;
	elevator_step = 0.0f;
	wood = NULL;
	wall1 = NULL;

	switch (map_no) {
	case 0:
		//出口
		poEXIT.GetVBarray(SQUARE);
		//地面メイン
		poGroundM.GetVBarray(CONTROL_POINT);
		//空メイン
		poCeilingM.GetVBarray(CONTROL_POINT);
		break;
	case 1:
		//山
		mountain.SetState(TRUE, TRUE, FALSE, 0.8f, 0.8f);//3角形化必要
		mountain.GetBuffer("./dat/mesh/mountain.obj");
		//地面入り口
		poGroundF.GetVBarray(CONTROL_POINT);
		//空入り口
		poCeilingF.GetVBarray(CONTROL_POINT);
		//地面メイン
		poGroundM.GetVBarray(CONTROL_POINT);
		//空メイン
		poBackground.GetVBarray(SQUARE);
		//雨
		poRain.GetVBarray(LINE_L);
		//地面出口
		poGroundE.GetVBarray(SQUARE);
		//空出口
		poCeilingE.GetVBarray(CONTROL_POINT);
		break;
	case 2:
		//波
		wav.GetVBarray(4);
		//入口
		poEXIT.GetVBarray(SQUARE);
		//地面メイン
		poGroundM.GetVBarray(CONTROL_POINT);
		//空メイン
		poCeilingM.GetVBarray(CONTROL_POINT);
		//地面出口
		poGroundE.GetVBarray(CONTROL_POINT);
		//空出口
		poCeilingE.GetVBarray(CONTROL_POINT);
		break;
	case 3:
		//地面入り口
		poGroundF.GetVBarray(CONTROL_POINT);
		//空入り口
		poCeilingF.GetVBarray(CONTROL_POINT);
		//地面メイン
		poGroundM.GetVBarray(CONTROL_POINT);
		//空メイン
		poCeilingM.GetVBarray(CONTROL_POINT);
		break;
	case 4:
		//地面メイン
		poGroundM.GetVBarray(CONTROL_POINT);
		//空メイン
		poCeilingM.GetVBarray(CONTROL_POINT);
		break;
	}

	Mapfilter(h_p);

	//配置位置ずらし用
	wood = new OBJPosRandomValue[woodcount];
	wall1 = new OBJPosRandomValue[squarecount];

	//木
	if (woodcount > 0) {
		mWood.SetState(TRUE, TRUE, FALSE);
		mWood.GetBuffer("./dat/mesh/tree.obj");
	}

	//壁(板)
	if (squarecount >= 1) {
		for (int i = 0; i < 3; i++) {
			poWall1[i].GetVBarray(SQUARE);
		}
	}

	//壁(ブロック)
	if (blockcountA >= 1) {
		poWallA.GetVBarray(CONTROL_POINT);
	}
	if (blockcountB >= 1) {
		poWallB.GetVBarray(CONTROL_POINT);
	}
	if (blockcountC >= 1) {
		poWallC.GetVBarray(CONTROL_POINT);
	}
	if (blockcountD >= 1) {
		poWallD.GetVBarray(CONTROL_POINT);
	}
	if (blockcountE >= 1) {
		poWallE.GetVBarray(CONTROL_POINT);
	}

	//リカバーポイント
	if (r_point_count >= 1) {
		poRecover.GetVBarray(SQUARE);
		for (int i = 0; i < 12; i++) {
			poRecoverLine[i].GetVBarray(LINE_L);
		}
	}

	//動画テクスチャ松明
	if (mo_count >= 1) {
		poMo.GetBufferBill(lightcount);
	}

	//動画テクスチャ炎壁
	if (f_wall_count >= 1) {
		poF_Wall.GetVBarray(CONTROL_POINT);
	}

	//ボス出現ポイント
	if (boss_count >= 1) {
		poBoss.GetVBarray(SQUARE);
	}

	//エレベーター
	if (Elevator_count >= 1) {
		poElevator.GetVBarray(SQUARE);
	}

	//地図
	MapHistory.GetVBarray2D(1);

	//ライトポジション構造体確保
	light = (LightPos*)malloc(sizeof(LightPos) * lightcount);
	//ポイントライトリセット
	dx->ResetPointLight();
}

void Map::SetVertex() {
	//雨
	VertexBC rv[2] = {
		0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f ,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f };
	UINT ri[2] = { 0,1 };
	switch (map_no) {
	case 0:
		//出口
		Mapcreate_EXIT(400.0f, 0.5f, 100.0f, 100.0f);
		//地面メイン
		Mapcreate_Ground(&poGroundM, 30, 35, 0.0f, 1.0f);
		//空メイン
		Mapcreate_Ceiling(&poCeilingM, 30, 35, 100.0f, 1.0f);
		break;
	case 1:
		//山
		mountain.SetVertex();
		//地面入り口
		Mapcreate_Ground(&poGroundF, 2, 3, 2.0f, 1.0f);
		//空入り口
		Mapcreate_Ceiling(&poCeilingF, 2, 3, 100.0f, 1.0f);
		//地面メイン
		Mapcreate_Ground(&poGroundM, 40, 40, 0.0f, 1.0f);
		//空メイン
		Mapcreate_Background(-3500.0f, 7500.0f);
		//雨
		poRain.setVertex(rv, 2, ri, 2);
		//地面出口
		Mapcreate_Ground(&poGroundE, 2, 3, 2.0f, 1.0f);
		//空出口
		Mapcreate_Ceiling(&poCeilingE, 2, 3, 100.0f, 1.0f);
		break;
	case 2:
		//波
		wav.SetVertex(0, -50.0f, -50.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
		wav.SetVertex(1, -50.0f, 50.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
		wav.SetVertex(2, 50.0f, 50.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
		wav.SetVertex(3, 50.0f, -50.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
		//入口
		Mapcreate_EXIT(-50.0f, -50.0f, 100.0f, 100.0f);
		//地面メイン
		Mapcreate_Ground(&poGroundM, 30, 35, 0.0f, 1.0f);
		//空メイン
		Mapcreate_Ceiling(&poCeilingM, 30, 35, 200.0f, 1.0f);
		//地面出口
		Mapcreate_Ground(&poGroundE, 2, 4, 0.0f, 1.0f);
		//空出口
		Mapcreate_Ceiling(&poCeilingE, 2, 4, 200.0f, 1.0f);
		break;
	case 3:
		//地面入り口
		Mapcreate_Ground(&poGroundF, 2, 3, 0.0f, 1.0f);
		//空入り口
		Mapcreate_Ceiling(&poCeilingF, 2, 3, 100.0f, 1.0f);
		//地面メイン
		Mapcreate_Ground(&poGroundM, 30, 30, 0.0f, 1.0f);
		//空メイン
		Mapcreate_Ceiling(&poCeilingM, 30, 30, 200.0f, 1.0f);
		break;
	case 4:
		//地面メイン
		Mapcreate_Ground(&poGroundM, 30, 30, 0.0f, 1.0f);
		//空メイン
		Mapcreate_Ceiling(&poCeilingM, 30, 30, 800.0f, 1.0f);
		break;
	}

	//木
	if (woodcount > 0) {
		mWood.SetVertex();
	}

	//壁(板)
	if (squarecount >= 1) {
		for (int i = 0; i < 3; i++) {
			Mapcreate_Wall1(i);
		}
	}

	//壁(ブロック)
	if (blockcountA >= 1) {
		Mapcreate_Wall(blockcountA, &poWallA, 49, 52, 100.0f, -1.0f, 1.0f);
	}
	if (blockcountB >= 1) {
		Mapcreate_Wall(blockcountB, &poWallB, 74, 74, 100.0f, -1.0f, 1.0f);
	}
	if (blockcountC >= 1) {
		Mapcreate_Wall(blockcountC, &poWallC, 75, 79, 100.0f, -1.0f, 1.0f);
	}
	if (blockcountD >= 1) {
		Mapcreate_Wall(blockcountD, &poWallD, 76, 76, 100.0f, -2.0f, 2.0f);
	}
	if (blockcountE >= 1) {
		Mapcreate_Wall(blockcountE, &poWallE, 77, 78, 100.0f, -2.0f, 2.0f);
	}

	//リカバーポイント
	if (r_point_count >= 1) {
		Mapcreate_Recover(r_point_count);
	}

	//動画テクスチャ松明
	if (mo_count >= 1) {
		Mapcreate_Ds(mo_count);
	}

	//動画テクスチャ炎壁
	if (f_wall_count >= 1) {
		Mapcreate_Wall(f_wall_count, &poF_Wall, 73, 73, 200.0f, -6.0f, 6.0f);
	}

	//ボス出現ポイント
	if (boss_count >= 1) {
		Mapcreate_BossPoint(boss_count);
	}

	//エレベーター
	if (Elevator_count >= 1) {
		Mapcreate_Elevator(Elevator_count);
	}
}

void Map::SetCommandList(int com_no) {
	comNo = com_no;
	wav.SetCommandList(comNo);
	mWood.SetCommandList(comNo);
	mountain.SetCommandList(comNo);
	poWallA.SetCommandList(comNo);
	poWallB.SetCommandList(comNo);
	poWallC.SetCommandList(comNo);
	poWallD.SetCommandList(comNo);
	poWallE.SetCommandList(comNo);
	for (int i = 0; i < 3; i++)
		poWall1[i].SetCommandList(comNo);
	poF_Wall.SetCommandList(comNo);
	poGroundF.SetCommandList(comNo);
	poCeilingF.SetCommandList(comNo);
	poGroundM.SetCommandList(comNo);
	poCeilingM.SetCommandList(comNo);
	poGroundE.SetCommandList(comNo);
	poCeilingE.SetCommandList(comNo);
	poBackground.SetCommandList(comNo);
	poRain.SetCommandList(comNo);
	poRecover.SetCommandList(comNo);
	for (int i = 0; i < 12; i++)
		poRecoverLine[i].SetCommandList(comNo);
	poBoss.SetCommandList(comNo);
	poElevator.SetCommandList(comNo);
	poEXIT.SetCommandList(comNo);
	poMo.SetCommandList(comNo);
	MapHistory.SetCommandList(comNo);
}

void Map::CreateMap() {

	switch (map_no) {
	case 0:
		//出口
		poEXIT.Create(FALSE, -1, FALSE, FALSE);
		//地面メイン
		poGroundM.Create(TRUE, dx->GetTexNumber("./dat/texture/map/ground1.da"),
			dx->GetTexNumber("./dat/texture/map/ground1Nor.da"), -1, TRUE, FALSE);
		//空メイン
		poCeilingM.Create(TRUE, dx->GetTexNumber("./dat/texture/map/ceiling1.da"),
			dx->GetTexNumber("./dat/texture/map/ceiling1Nor.da"), -1, TRUE, FALSE);
		break;
	case 1:
		//山
		mountain.CreateMesh();
		//地面入り口
		poGroundF.Create(TRUE, dx->GetTexNumber("./dat/texture/map/ground1.da"),
			dx->GetTexNumber("./dat/texture/map/ground1Nor.da"), -1, TRUE, FALSE);
		//空入り口
		poCeilingF.Create(TRUE, dx->GetTexNumber("./dat/texture/map/ceiling1.da"),
			dx->GetTexNumber("./dat/texture/map/ceiling1Nor.da"), -1, TRUE, FALSE);
		//地面メイン
		poGroundM.Create(TRUE, dx->GetTexNumber("./dat/texture/map/ground2.da"),
			dx->GetTexNumber("./dat/texture/map/ground2Nor.da"), -1, TRUE, FALSE);
		//空メイン
		poBackground.Create(FALSE, dx->GetTexNumber("./dat/texture/map/ceiling2.da"), TRUE, FALSE);
		//雨
		poRain.Create(FALSE, -1, FALSE, FALSE);
		//地面出口
		poGroundE.Create(TRUE, dx->GetTexNumber("./dat/texture/map/ground3.da"),
			dx->GetTexNumber("./dat/texture/map/ground3Nor.da"), -1, TRUE, FALSE);
		//空出口
		poCeilingE.Create(TRUE, dx->GetTexNumber("./dat/texture/map/ceiling3_wall3.da"),
			dx->GetTexNumber("./dat/texture/map/ceiling3_wall3Nor.da"), -1, TRUE, FALSE);
		break;
	case 2:
		//波
		wav.Create(dx->GetTexNumber("./dat/texture/map/wave.da"),
			-1/*dx->GetTexNumber("./dat/texture/map/waveNor.da")*/, TRUE, TRUE, 1.0f, 64.0f);
		//入口
		poEXIT.Create(FALSE, -1, FALSE, FALSE);
		//地面メイン
		poGroundM.Create(TRUE, dx->GetTexNumber("./dat/texture/map/ground3.da"),
			dx->GetTexNumber("./dat/texture/map/ground3Nor.da"), -1, TRUE, FALSE);
		//空メイン
		poCeilingM.Create(TRUE, dx->GetTexNumber("./dat/texture/map/ceiling3_wall3.da"),
			dx->GetTexNumber("./dat/texture/map/ceiling3_wall3Nor.da"), -1, TRUE, FALSE);
		//地面出口
		poGroundE.Create(TRUE, dx->GetTexNumber("./dat/texture/map/ceiling4_ground4.da"),
			dx->GetTexNumber("./dat/texture/map/ceiling4_ground4Nor.da"), -1, TRUE, FALSE);
		//空出口
		poCeilingE.Create(TRUE, dx->GetTexNumber("./dat/texture/map/ceiling4_ground4.da"),
			dx->GetTexNumber("./dat/texture/map/ceiling4_ground4Nor.da"), -1, TRUE, FALSE);
		break;
	case 3:
		//地面入り口
		poGroundF.Create(TRUE, dx->GetTexNumber("./dat/texture/map/ground3.da"),
			dx->GetTexNumber("./dat/texture/map/ground3Nor.da"), -1, TRUE, FALSE);
		//空入り口
		poCeilingF.Create(TRUE, dx->GetTexNumber("./dat/texture/map/ceiling3_wall3.da"),
			dx->GetTexNumber("./dat/texture/map/ceiling3_wall3Nor.da"), -1, TRUE, FALSE);
		//地面メイン
		poGroundM.Create(TRUE, dx->GetTexNumber("./dat/texture/map/ceiling4_ground4.da"),
			dx->GetTexNumber("./dat/texture/map/ceiling4_ground4Nor.da"), -1, TRUE, FALSE);
		//空メイン
		poCeilingM.Create(TRUE, dx->GetTexNumber("./dat/texture/map/ceiling4_ground4.da"),
			dx->GetTexNumber("./dat/texture/map/ceiling4_ground4Nor.da"), -1, TRUE, FALSE);
		break;
	case 4:
		//地面メイン
		poGroundM.Create(TRUE, dx->GetTexNumber("./dat/texture/map/ground5.da"),
			dx->GetTexNumber("./dat/texture/map/ground5Nor.da"), -1, TRUE, FALSE);
		//空メイン
		poCeilingM.Create(TRUE, dx->GetTexNumber("./dat/texture/map/ceiling5.da"),
			dx->GetTexNumber("./dat/texture/map/ceiling5Nor.da"), -1, TRUE, FALSE);
		break;
	}

	//木
	if (woodcount > 0) {
		mWood.CreateMesh();
	}

	//壁(板)
	if (squarecount >= 1) {
		for (int i = 0; i < 3; i++) {
			poWall1[i].Create(TRUE, dx->GetTexNumber("./dat/texture/map/wall2.da"), TRUE, TRUE);
		}
	}

	//壁(ブロック)
	if (blockcountA >= 1) {
		poWallA.SetCol(0.6f, 0.6f, 0.6f, 0.1f, 0.1f, 0.1f, 0, 0, 0);
		poWallA.Create(TRUE, dx->GetTexNumber("./dat/texture/map/wall1.da"),
			dx->GetTexNumber("./dat/texture/map/wall1Nor.da"), -1, TRUE, FALSE);
	}
	if (blockcountB >= 1) {
		poWallB.Create(TRUE, dx->GetTexNumber("./dat/texture/map/wall2-1.da"),
			dx->GetTexNumber("./dat/texture/map/wall2Nor.da"), -1, TRUE, FALSE);
	}
	if (blockcountC >= 1) {
		poWallC.Create(TRUE, dx->GetTexNumber("./dat/texture/map/ceiling3_wall3.da"),
			dx->GetTexNumber("./dat/texture/map/ceiling3_wall3Nor.da"), -1, TRUE, FALSE);
	}
	if (blockcountD >= 1) {
		poWallD.Create(TRUE, dx->GetTexNumber("./dat/texture/map/ceiling4_ground4.da"),
			dx->GetTexNumber("./dat/texture/map/ceiling4_ground4Nor.da"), -1, TRUE, FALSE);
	}
	if (blockcountE >= 1) {
		poWallE.Create(TRUE, dx->GetTexNumber("./dat/texture/map/wall5.da"),
			dx->GetTexNumber("./dat/texture/map/wall5Nor.da"), -1, TRUE, FALSE);
	}

	//リカバーポイント
	if (r_point_count >= 1) {
		poRecover.Create(FALSE, dx->GetTexNumber("recover.jpg"), TRUE, TRUE);
		for (int i = 0; i < 12; i++)
			poRecoverLine[i].Create(FALSE, -1, FALSE, FALSE);
	}

	//動画テクスチャ松明
	if (mo_count >= 1) {
		poMo.TextureInit(128, 128);
		poMo.CreateBillboard();
	}

	//動画テクスチャ炎壁
	if (f_wall_count >= 1) {
		poF_Wall.TextureInit(256, 256);
		poF_Wall.Create(FALSE, -1, TRUE, TRUE);
	}

	//ボス出現ポイント
	if (boss_count >= 1) {
		poBoss.Create(FALSE, dx->GetTexNumber("boss_magic.jpg"), TRUE, TRUE);
	}

	//エレベーター
	if (Elevator_count >= 1) {
		poElevator.Create(FALSE, dx->GetTexNumber("recover.jpg"), TRUE, TRUE);
	}

	MapHistory.TexOn();
	MapHistory.TextureInit(128, 128);
	MapHistory.CreateBox(600.0f, 100.0f, 0.1f, 100.0f, 100.0f, 1.0f, 1.0f, 1.0f, 1.0f, TRUE, TRUE);
}

void Map::Mapupdate_Wood() {

	int p = 0;
	for (int k3 = 0; k3 < mxy.z; k3++) {
		for (int j = 0; j < mxy.y; j++) {
			for (int i = 0; i < mxy.x; i++) {
				if (mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != 87)continue;
				float x = (float)i * 100.0f + 50.0f + wood[p].x;
				float y = (float)j * 100.0f + 50.0f + wood[p++].y;
				float z = (float)k3 * 100.0f;
				float xx = cax1 - x;
				float yy = cay1 - y;
				float zz = (float)posz * 100.0f - z;
				if (sqrt(xx * xx + yy * yy + zz * zz) > 600.0f)continue;
				mWood.InstancedMap(x, y, z, 0, 0, 0, 10.0f);
			}
		}
	}
	mWood.InstanceUpdate(0, 0, 0, 0, 0.2f);
}

void Map::Mapdraw_Wood() {
	mWood.Draw();
}

void Map::Mapupdate_Mountain() {
	mountain.InstancedMap(-1500.0f, 2000.0f, 0, 0, 0, 0, 500.0f);
	mountain.Update(5500.0f, 2000.0f, 0, 0, 0, 0, 0, 0, 0, 0, 500.0f, 0);
}

void Map::Mapdraw_Mountain() {
	mountain.Draw();
}

void Map::Mapcreate_Wall1(int i) {
	Vertex wa[4] = {
		//左前
		-50.0f, 0.0f, 100.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f,
		//右前
		50.0f, 0.0f, 100.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f,
		//左奥
		-50.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f,
		//右奥
		50.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f
	};
	UINT ind[6] = { 0, 1, 2, 2, 1, 3 };
	poWall1[i].setVertex(wa, 4, ind, 6);
}

void Map::Mapupdate_Wall1() {

	int p = 0;
	for (int k3 = 0; k3 < mxy.z; k3++) {
		for (int j = 0; j < mxy.y; j++) {
			for (int i = 0; i < mxy.x; i++) {
				if (mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != 53)continue;
				float x = (float)i * 100.0f + 50.0f + wall1[p].x;
				float y = (float)j * 100.0f + 50.0f + wall1[p++].y;
				float z = (float)k3 * 100.0f;
				poWall1[i % 3].InstancedMap(x, y, z, src_theta, 0, 0);
			}
		}
	}
	for (int i = 0; i < 3; i++)poWall1[i].InstanceUpdate(0, 0, 0, 0, 0);
}

void Map::Mapdraw_Wall1() {
	for (int i = 0; i < 3; i++)poWall1[i].Draw();
}

void Map::Mapcreate_Wall(int numB, PolygonData* pd, int no1, int no2, float height, float adjust, float adjust2) {
	Vertex* bl = new Vertex[numB * 6 * 4];
	UINT* ind = new UINT[numB * 6 * 6];
	int k = 0;
	int kI = 0;
	for (int k3 = 0; k3 < mxy.z; k3++) {
		for (int j = 0; j < mxy.y; j++) {
			for (int i = 0; i < mxy.x; i++) {

				if (mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != no1 && mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != no2)continue;
				//壁ブロック頂点設定
				//正面検査
				if (k3 < mxy.z - 1 && mxy.m[(k3 + 1) * mxy.y * mxy.x + j * mxy.x + i] != no1 &&
					mxy.m[(k3 + 1) * mxy.y * mxy.x + j * mxy.x + i] != no2) {
					//正面左上
					bl[k].Pos.as((float)i * 100.0f - adjust2, (float)j * 100.0f - adjust2, (float)k3 * 100.0f + height + adjust);
					bl[k].normal.as(0.0f, 0.0f, 1.0f);
					bl[k].tex.as(0.0f, 0.0f);
					ind[kI] = k;
					//正面右上
					bl[k + 1].Pos.as((float)i * 100.0f + 100.0f + adjust2, (float)j * 100.0f - adjust2, (float)k3 * 100.0f + height + adjust);
					bl[k + 1].normal.as(0.0f, 0.0f, 1.0f);
					bl[k + 1].tex.as(1.0f, 0.0f);
					ind[kI + 1] = k + 1;
					ind[kI + 4] = k + 1;
					//正面左下
					bl[k + 2].Pos.as((float)i * 100.0f - adjust2, (float)j * 100.0f + 100.0f + adjust2, (float)k3 * 100.0f + height + adjust);
					bl[k + 2].normal.as(0.0f, 0.0f, 1.0f);
					bl[k + 2].tex.as(0.0f, 1.0f);
					ind[kI + 2] = k + 2;
					ind[kI + 3] = k + 2;
					//正面右下
					bl[k + 3].Pos.as((float)i * 100.0f + 100.0f + adjust2, (float)j * 100.0f + 100.0f + adjust2, (float)k3 * 100.0f + height + adjust);
					bl[k + 3].normal.as(0.0f, 0.0f, 1.0f);
					bl[k + 3].tex.as(1.0f, 1.0f);
					ind[kI + 5] = k + 3;
					k += 4;
					kI += 6;
				}

				//左面検査
				if (i > 0 && mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + (i - 1)] != no1 &&
					mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + (i - 1)] != no2) {
					//左面上前
					bl[k].Pos.as((float)i * 100.0f - adjust, (float)j * 100.0f - adjust2, (float)k3 * 100.0f + height + adjust2);
					bl[k].normal.as(-1.0f, 0.0f, 0.0f);
					bl[k].tex.as(0.0f, 0.0f);
					ind[kI] = k;
					//左面下前
					bl[k + 1].Pos.as((float)i * 100.0f - adjust, (float)j * 100.0f + 100.0f + adjust2, (float)k3 * 100.0f + height + adjust2);
					bl[k + 1].normal.as(-1.0f, 0.0f, 0.0f);
					bl[k + 1].tex.as(1.0f, 0.0f);
					ind[kI + 1] = k + 1;
					ind[kI + 4] = k + 1;
					//左面上後
					bl[k + 2].Pos.as((float)i * 100.0f - adjust, (float)j * 100.0f - adjust2, (float)k3 * 100.0f - adjust2);
					bl[k + 2].normal.as(-1.0f, 0.0f, 0.0f);
					bl[k + 2].tex.as(0.0f, 1.0f);
					ind[kI + 2] = k + 2;
					ind[kI + 3] = k + 2;
					//左面下後
					bl[k + 3].Pos.as((float)i * 100.0f - adjust, (float)j * 100.0f + 100.0f + adjust2, (float)k3 * 100.0f - adjust2);
					bl[k + 3].normal.as(-1.0f, 0.0f, 0.0f);
					bl[k + 3].tex.as(1.0f, 1.0f);
					ind[kI + 5] = k + 3;
					k += 4;
					kI += 6;
				}

				//右面検査
				if (i < mxy.x - 1 && mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + (i + 1)] != no1 &&
					mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + (i + 1)] != no2) {
					//右面下前
					bl[k].Pos.as((float)i * 100.0f + 100.0f + adjust, (float)j * 100.0f + 100.0f + adjust2, (float)k3 * 100.0f + height + adjust2);
					bl[k].normal.as(1.0f, 0.0f, 0.0f);
					bl[k].tex.as(0.0f, 0.0f);
					ind[kI] = k;
					//右面上前
					bl[k + 1].Pos.as((float)i * 100.0f + 100.0f + adjust, (float)j * 100.0f - adjust2, (float)k3 * 100.0f + height + adjust2);
					bl[k + 1].normal.as(1.0f, 0.0f, 0.0f);
					bl[k + 1].tex.as(1.0f, 0.0f);
					ind[kI + 1] = k + 1;
					ind[kI + 4] = k + 1;
					//右面下後
					bl[k + 2].Pos.as((float)i * 100.0f + 100.0f + adjust, (float)j * 100.0f + 100.0f + adjust2, (float)k3 * 100.0f - adjust2);
					bl[k + 2].normal.as(1.0f, 0.0f, 0.0f);
					bl[k + 2].tex.as(0.0f, 1.0f);
					ind[kI + 2] = k + 2;
					ind[kI + 3] = k + 2;
					//右面上後
					bl[k + 3].Pos.as((float)i * 100.0f + 100.0f + adjust, (float)j * 100.0f - adjust2, (float)k3 * 100.0f - adjust2);
					bl[k + 3].normal.as(1.0f, 0.0f, 0.0f);
					bl[k + 3].tex.as(1.0f, 1.0f);
					ind[kI + 5] = k + 3;
					k += 4;
					kI += 6;
				}

				//上面検査
				if (j > 0 && mxy.m[k3 * mxy.y * mxy.x + (j - 1) * mxy.x + i] != no1 &&
					mxy.m[k3 * mxy.y * mxy.x + (j - 1) * mxy.x + i] != no2) {
					//上面右前
					bl[k].Pos.as((float)i * 100.0f + 100.0f + adjust2, (float)j * 100.0f - adjust, (float)k3 * 100.0f + height + adjust2);
					bl[k].normal.as(0.0f, -1.0f, 0.0f);
					bl[k].tex.as(0.0f, 0.0f);
					ind[kI] = k;
					//上面左前
					bl[k + 1].Pos.as((float)i * 100.0f - adjust2, (float)j * 100.0f - adjust, (float)k3 * 100.0f + height + adjust2);
					bl[k + 1].normal.as(0.0f, -1.0f, 0.0f);
					bl[k + 1].tex.as(1.0f, 0.0f);
					ind[kI + 1] = k + 1;
					ind[kI + 4] = k + 1;
					//上面右後
					bl[k + 2].Pos.as((float)i * 100.0f + 100.0f + adjust2, (float)j * 100.0f - adjust, (float)k3 * 100.0f - adjust2);
					bl[k + 2].normal.as(0.0f, -1.0f, 0.0f);
					bl[k + 2].tex.as(0.0f, 1.0f);
					ind[kI + 2] = k + 2;
					ind[kI + 3] = k + 2;
					//上面左後
					bl[k + 3].Pos.as((float)i * 100.0f - adjust2, (float)j * 100.0f - adjust, (float)k3 * 100.0f - adjust2);
					bl[k + 3].normal.as(0.0f, -1.0f, 0.0f);
					bl[k + 3].tex.as(1.0f, 1.0f);
					ind[kI + 5] = k + 3;
					k += 4;
					kI += 6;
				}

				//裏面検査
				if (k3 > 0 && mxy.m[(k3 - 1) * mxy.y * mxy.x + j * mxy.x + i] != no1 &&
					mxy.m[(k3 - 1) * mxy.y * mxy.x + j * mxy.x + i] != no2) {
					//裏面右上
					bl[k].Pos.as((float)i * 100.0f + 100.0f + adjust2, (float)j * 100.0f - adjust2, (float)k3 * 100.0f - adjust);
					bl[k].normal.as(0.0f, 0.0f, -1.0f);
					bl[k].tex.as(0.0f, 0.0f);
					ind[kI] = k;
					//裏面左上
					bl[k + 1].Pos.as((float)i * 100.0f - adjust2, (float)j * 100.0f - adjust2, (float)k3 * 100.0f - adjust);
					bl[k + 1].normal.as(0.0f, 0.0f, -1.0f);
					bl[k + 1].tex.as(1.0f, 0.0f);
					ind[kI + 1] = k + 1;
					ind[kI + 4] = k + 1;
					//裏面右下
					bl[k + 2].Pos.as((float)i * 100.0f + 100.0f + adjust2, (float)j * 100.0f + 100.0f + adjust2, (float)k3 * 100.0f - adjust);
					bl[k + 2].normal.as(0.0f, 0.0f, -1.0f);
					bl[k + 2].tex.as(0.0f, 1.0f);
					ind[kI + 2] = k + 2;
					ind[kI + 3] = k + 2;
					//裏面左下
					bl[k + 3].Pos.as((float)i * 100.0f - adjust2, (float)j * 100.0f + 100.0f + adjust2, (float)k3 * 100.0f - adjust);
					bl[k + 3].normal.as(0.0f, 0.0f, -1.0f);
					bl[k + 3].tex.as(1.0f, 1.0f);
					ind[kI + 5] = k + 3;
					k += 4;
					kI += 6;
				}

				//底面検査
				if (j < mxy.y - 1 && mxy.m[k3 * mxy.y * mxy.x + (j + 1) * mxy.x + i] != no1 &&
					mxy.m[k3 * mxy.y * mxy.x + (j + 1) * mxy.x + i] != no2) {
					//底面左前
					bl[k].Pos.as((float)i * 100.0f - adjust2, (float)j * 100.0f + 100.0f + adjust, (float)k3 * 100.0f + height + adjust2);
					bl[k].normal.as(0.0f, 1.0f, 0.0f);
					bl[k].tex.as(0.0f, 0.0f);
					ind[kI] = k;
					//底面右前
					bl[k + 1].Pos.as((float)i * 100.0f + 100.0f + adjust2, (float)j * 100.0f + 100.0f + adjust, (float)k3 * 100.0f + height + adjust2);
					bl[k + 1].normal.as(0.0f, 1.0f, 0.0f);
					bl[k + 1].tex.as(1.0f, 0.0f);
					ind[kI + 1] = k + 1;
					ind[kI + 4] = k + 1;
					//底面左後
					bl[k + 2].Pos.as((float)i * 100.0f - adjust2, (float)j * 100.0f + 100.0f + adjust, (float)k3 * 100.0f - adjust2);
					bl[k + 2].normal.as(0.0f, 1.0f, 0.0f);
					bl[k + 2].tex.as(0.0f, 1.0f);
					ind[kI + 2] = k + 2;
					ind[kI + 3] = k + 2;
					//底面右後
					bl[k + 3].Pos.as((float)i * 100.0f + 100.0f + adjust2, (float)j * 100.0f + 100.0f + adjust, (float)k3 * 100.0f - adjust2);
					bl[k + 3].normal.as(0.0f, 1.0f, 0.0f);
					bl[k + 3].tex.as(1.0f, 1.0f);
					ind[kI + 5] = k + 3;
					k += 4;
					kI += 6;
				}
			}
		}
	}
	pd->setVertex(bl, numB * 6 * 4, ind, numB * 6 * 6);
	ARR_DELETE(bl);
	ARR_DELETE(ind);
}

void Map::Mapcreate_Ground(PolygonData* pd, int pcsx, int pcsy, float height, float adjust) {
	Vertex* bl = new Vertex[pcsx * pcsy * 4];
	UINT* ind = new UINT[pcsx * pcsy * 6];
	//地面頂点
	int k = 0;
	int kI = 0;
	float size = 100.0f;//1マスの大きさ
	for (int j = 0; j < pcsy; j++) {
		for (int i = 0; i < pcsx; i++) {
			float x = size * i;
			float y = size * j;
			//地面左上
			bl[k].Pos.as(x - adjust, y - adjust, height);
			bl[k].normal.as(0.0f, 0.0f, 1.0f);
			bl[k].tex.as(0.0f, 0.0f);
			ind[kI] = k;
			//地面右上
			bl[k + 1].Pos.as(x + size + adjust, y - adjust, height);
			bl[k + 1].normal.as(0.0f, 0.0f, 1.0f);
			bl[k + 1].tex.as(1.0f, 0.0f);
			ind[kI + 1] = k + 1;
			ind[kI + 4] = k + 1;
			//地面左下
			bl[k + 2].Pos.as(x - adjust, y + size + adjust, height);
			bl[k + 2].normal.as(0.0f, 0.0f, 1.0f);
			bl[k + 2].tex.as(0.0f, 1.0f);
			ind[kI + 2] = k + 2;
			ind[kI + 3] = k + 2;
			//地面右下
			bl[k + 3].Pos.as(x + size + adjust, y + size + adjust, height);
			bl[k + 3].normal.as(0.0f, 0.0f, 1.0f);
			bl[k + 3].tex.as(1.0f, 1.0f);
			ind[kI + 5] = k + 3;
			k += 4;
			kI += 6;
		}
	}
	pd->setVertex(bl, pcsx * pcsy * 4, ind, pcsx * pcsy * 6);
	ARR_DELETE(bl);
	ARR_DELETE(ind);
}

void Map::Mapcreate_Ceiling(PolygonData* pd, int pcsx, int pcsy, float height, float adjust) {
	Vertex* bl = new Vertex[pcsx * pcsy * 4];
	UINT* ind = new UINT[pcsx * pcsy * 6];
	//天井頂点
	int k = 0;
	int kI = 0;
	float size = 100.0f;//1マスの大きさ
	for (int j = 0; j < pcsy; j++) {
		for (int i = 0; i < pcsx; i++) {
			float x = size * i;
			float y = size * j;
			//天井左上
			bl[k].Pos.as(x - adjust, y - adjust, height);
			bl[k].normal.as(0.0f, 0.0f, -1.0f);
			bl[k].tex.as(0.0f, 0.0f);
			ind[kI] = k;
			//天井左下
			bl[k + 1].Pos.as(x - adjust, y + size + adjust, height);
			bl[k + 1].normal.as(0.0f, 0.0f, -1.0f);
			bl[k + 1].tex.as(0.0f, 1.0f);
			ind[kI + 1] = k + 1;
			ind[kI + 4] = k + 1;
			//天井右上
			bl[k + 2].Pos.as(x + size + adjust, y - adjust, height);
			bl[k + 2].normal.as(0.0f, 0.0f, -1.0f);
			bl[k + 2].tex.as(1.0f, 0.0f);
			ind[kI + 2] = k + 2;
			ind[kI + 3] = k + 2;
			//天井右下
			bl[k + 3].Pos.as(x + size + adjust, y + size + adjust, height);
			bl[k + 3].normal.as(0.0f, 0.0f, -1.0f);
			bl[k + 3].tex.as(1.0f, 1.0f);
			ind[kI + 5] = k + 3;
			k += 4;
			kI += 6;
		}
	}
	pd->setVertex(bl, pcsx * pcsy * 4, ind, pcsx * pcsy * 6);
	ARR_DELETE(bl);
	ARR_DELETE(ind);
}

void Map::Mapcreate_Background(float st, float end) {
	float height = end - st;

	Vertex bl[20] = {
		//正面
		//右上
		end, st, height,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f,
		//左上
		st, st, height,
		0.0f, 1.0f, 0.0f,
		5.0f, 0.0f,
		//右下
		end, end, height,
		0.0f, 1.0f, 0.0f,
		0.0f, 5.0f,
		//左下
		st, end, height,
		0.0f, 1.0f, 0.0f,
		5.0f, 5.0f,
		//上
		//左前
		st, st, height,
		0.0f, 1.0f, 0.0f,
		5.0f, 5.0f,
		//右前
		end, st, height,
		0.0f, 1.0f, 0.0f,
		0.0f, 5.0f,
		//左奥
		st, st, 0.0f,
		0.0f, 1.0f, 0.0f,
		5.0f, 0.0f,
		//右奥
		end, st, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f,
		//下
		//右前
		end, end, height,
		0.0f, -1.0f, 0.0f,
		0.0f, 0.0f,
		//左前
		st, end, height,
		0.0f, -1.0f, 0.0f,
		5.0f, 0.0f,
		//右奥
		end, end, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, 5.0f,
		//左奥
		st, end, 0.0f,
		0.0f, -1.0f, 0.0f,
		5.0f, 5.0f,
		//右
		//上前
		end, st, height,
		-1.0f, 0.0f, 0.0f,
		5.0f, 0.0f,
		//下前
		end, end, height,
		-1.0f, 0.0f, 0.0f,
		5.0f, 5.0f,
		//上奥
		end, st, 0.0f,
		-1.0f, 0.0f, 0.0f,
		0.0f, 0.0f,
		//下奥
		end, end, 0.0f,
		-1.0f, 0.0f, 0.0f,
		0.0f, 5.0f,
		//左
		//下前
		st, end, height,
		1.0f, 0.0f, 0.0f,
		0.0f, 5.0f,
		//上前
		st, st, height,
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f,
		//下奥
		st, end, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f, 5.0f,
		//上奥
		st, st, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f, 0.0f
	};

	UINT ind[30] = {
	0,1,2,2,1,3,
	4,5,6,6,5,7,
	8,9,10,10,9,11,
	12,13,14,14,13,15,
	16,17,18,18,17,19
	};

	poBackground.setVertex(bl, 20, ind, 30);
}

void Map::Mapupdate_Rain() {

	int x, y;
	for (int i = 0; i < 148; i++) {
		x = rand() % 500;
		y = rand() % 500;
		poRain.InstancedMap(cax1 - 250.0f + x, cay1 - 250.0f + y, 0.0f,
			0.0f, 0.0f, 0.0f, (float)(rand() % 300));
	}
	poRain.InstanceUpdate(0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
}

void Map::Mapdraw_Rain() {
	poRain.Draw();
}

void Map::Mapcreate_Recover(int num) {
	Vertex* bl = new Vertex[num * 4];
	UINT* ind = new UINT[num * 6];
	VertexBC* bc[12];
	UINT* bci[12];
	for (int i = 0; i < 12; i++) {
		bc[i] = new VertexBC[num * 2];
		bci[i] = new UINT[num * 2];
	}
	int k = 0;
	int k1 = 0;
	int l1 = 0;
	for (int k3 = 0; k3 < mxy.z; k3++) {
		for (int j = 0; j < mxy.y; j++) {
			for (int i = 0; i < mxy.x; i++) {
				if (mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != 50)continue;//2のアスキーコード50
				//光源位置登録
				recovPosX = i * 100.0f + 50.0f;
				recovPosY = j * 100.0f + 50.0f;
				//回復ポイント左上
				bl[k].Pos.as((float)i * 100.0f, (float)j * 100.0f, (float)k3 * 100.0f + 2.0f);
				bl[k].normal.as(0.0f, 0.0f, 0.0f);
				bl[k].tex.as(0.0f, 0.0f);
				ind[k1] = k;
				//回復ポイント右上
				bl[k + 1].Pos.as((float)i * 100.0f + 100.0f, (float)j * 100.0f, (float)k3 * 100.0f + 2.0f);
				bl[k + 1].normal.as(0.0f, 0.0f, 0.0f);
				bl[k + 1].tex.as(1.0f, 0.0f);
				ind[k1 + 1] = k + 1;
				ind[k1 + 4] = k + 1;
				//回復ポイント左下
				bl[k + 2].Pos.as((float)i * 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f + 2.0f);
				bl[k + 2].normal.as(0.0f, 0.0f, 0.0f);
				bl[k + 2].tex.as(0.0f, 1.0f);
				ind[k1 + 2] = k + 2;
				ind[k1 + 3] = k + 2;
				//回復ポイント右下
				bl[k + 3].Pos.as((float)i * 100.0f + 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f + 2.0f);
				bl[k + 3].normal.as(0.0f, 0.0f, 0.0f);
				bl[k + 3].tex.as(1.0f, 1.0f);
				ind[k1 + 5] = k + 3;
				//line
				for (int l = 0; l < 12; l++) {
					bc[l][l1].Pos.as((float)i * 100.0f + 50.0f, (float)j * 100.0f + 50.0f, (float)k3 * 100.0f + 1.0f);
					bc[l][l1].color.as(0.0f, 1.0f, 0.0f, 1.0f);
					bci[l][l1] = l1;

					bc[l][l1 + 1].Pos.as((float)i * 100.0f + 50.0f, (float)j * 100.0f + 50.0f, (float)k3 * 100.0f);
					bc[l][l1 + 1].color.as(0.0f, 0.4f, 1.0f, 1.0f);
					bci[l][l1 + 1] = l1 + 1;
				}
				k += 4;
				k1 += 6;
				l1 += 2;
			}
		}
	}
	poRecover.setVertex(bl, num * 4, ind, num * 6);
	ARR_DELETE(bl);
	ARR_DELETE(ind);
	for (int i = 0; i < 12; i++) {
		poRecoverLine[i].setVertex(bc[i], num * 2, bci[i], num * 2);
		ARR_DELETE(bc[i]);
		ARR_DELETE(bci[i]);
	}
}

void Map::Mapupdate_Recover() {

	for (float j = 0.0f; j < 12.0f; j++) {
		for (float i1 = 0.0f; i1 < 120.0f; i1++) {
			float i = j * 30.0f + i1 * 0.25f;
			float line_y = -cos(i * 3.14f / 180.0f) * 50.0f;
			float line_x = sin(i * 3.14f / 180.0f) * 50.0f;
			int rnd = rand() % 20;
			poRecoverLine[(int)j].InstancedMap(line_x, line_y, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f, 1.0f, (float)rnd);
		}
		poRecoverLine[(int)j].InstanceUpdate(0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	}
	dx->PointLightPosSet(7, recovPosX, recovPosY, 2.0f,
		0.2f, 0.8f, 0.2f, 1.0f,
		true, 500.0f);
	poRecover.Update(0, 0, 4.0f, 0, 0, 0, 0, 0, 0);
}

void Map::Mapdraw_Recover() {
	for (float j = 0.0f; j < 12.0f; j++) {
		poRecoverLine[(int)j].Draw();
	}
	poRecover.Draw();
}

void Map::Mapcreate_Ds(int num) {
	int ind = 0;
	for (int k3 = 0; k3 < mxy.z; k3++) {
		for (int j = 0; j < mxy.y; j++) {
			for (int i = 0; i < mxy.x; i++) {

				//松明有壁以外スキップ
				if (mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != 52 &&
					mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != 78 &&
					mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != 79)continue;
				poMo.SetVertex(ind++,
					(float)i * 100.0f - 10.0f, (float)j * 100.0f + 50.0f, (float)k3 * 100.0f + 65.0f,
					1.0f, 1.0f, 1.0f, 0.8f);
				poMo.SetVertex(ind++,
					(float)i * 100.0f + 110.0f, (float)j * 100.0f + 50.0f, (float)k3 * 100.0f + 65.0f,
					1.0f, 1.0f, 1.0f, 0.8f);
				poMo.SetVertex(ind++,
					(float)i * 100.0f + 50.0f, (float)j * 100.0f - 10.0f, (float)k3 * 100.0f + 65.0f,
					1.0f, 1.0f, 1.0f, 0.8f);
				poMo.SetVertex(ind++,
					(float)i * 100.0f + 50.0f, (float)j * 100.0f + 110.0f, (float)k3 * 100.0f + 65.0f,
					1.0f, 1.0f, 1.0f, 0.8f);
			}
		}
	}
}

void Map::Mapupdate_Ds() {

	int licnt = 0;
	for (int k3 = 0; k3 < mxy.z; k3++) {
		for (int j = 0; j < mxy.y; j++) {
			for (int i = 0; i < mxy.x; i++) {

				//松明有壁以外スキップ
				if (mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != 52 &&
					mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != 78 &&
					mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != 79)continue;

				//左(マップ端に配置する場合アクセス違反防止を追加すること)
				if (mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i - 1] == 48) {
					//視野外スキップ
					if (ViewCulling((float)i * 100.0f - 10.0f, (float)j * 100.0f + 50.0f, (float)k3 * 100.0f + 65.0f) == TRUE) {
						light[licnt].x = i * 100.0f - 10.0f;
						light[licnt].y = j * 100.0f + 50.0f;
						light[licnt].z = k3 * 100.0f + 65.0f;
						light[licnt].r = 1.0f;
						light[licnt].g = 0.4f;
						light[licnt].b = 0.4f;
						light[licnt].a = 1.0f;
						light[licnt].range = 800.0f;
						light[licnt].brightness = 0.4f + ((float)(rand() % 4) * 0.1f);
						light[licnt].attenuation = 2.0f;
						light[licnt].on_off = TRUE;
						licnt++;
					}
				}
				//右
				if (mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i + 1] == 48) {
					//視野外スキップ
					if (ViewCulling((float)i * 100.0f + 110.0f, (float)j * 100.0f + 50.0f, (float)k3 * 100.0f + 65.0f) == TRUE) {
						light[licnt].x = i * 100.0f + 110.0f;
						light[licnt].y = j * 100.0f + 50.0f;
						light[licnt].z = k3 * 100.0f + 65.0f;
						light[licnt].r = 1.0f;
						light[licnt].g = 0.4f;
						light[licnt].b = 0.4f;
						light[licnt].a = 1.0f;
						light[licnt].range = 800.0f;
						light[licnt].brightness = 0.4f + ((float)(rand() % 4) * 0.1f);
						light[licnt].attenuation = 2.0f;
						light[licnt].on_off = TRUE;
						licnt++;
					}
				}
				//上
				if (mxy.m[k3 * mxy.y * mxy.x + (j - 1) * mxy.x + i] == 48) {
					//視野外スキップ
					if (ViewCulling((float)i * 100.0f + 50.0f, (float)j * 100.0f - 10.0f, (float)k3 * 100.0f + 65.0f) == TRUE) {
						light[licnt].x = i * 100.0f + 50.0f;
						light[licnt].y = j * 100.0f - 10.0f;
						light[licnt].z = k3 * 100.0f + 65.0f;
						light[licnt].r = 1.0f;
						light[licnt].g = 0.4f;
						light[licnt].b = 0.4f;
						light[licnt].a = 1.0f;
						light[licnt].range = 800.0f;
						light[licnt].brightness = 0.4f + ((float)(rand() % 4) * 0.1f);
						light[licnt].attenuation = 2.0f;
						light[licnt].on_off = TRUE;
						licnt++;
					}
				}
				//下
				if (mxy.m[k3 * mxy.y * mxy.x + (j + 1) * mxy.x + i] == 48) {
					//視野外スキップ
					if (ViewCulling((float)i * 100.0f + 50.0f, (float)j * 100.0f + 110.0f, (float)k3 * 100.0f + 65.0f) == TRUE) {
						light[licnt].x = i * 100.0f + 50.0f;
						light[licnt].y = j * 100.0f + 110.0f;
						light[licnt].z = k3 * 100.0f + 65.0f;
						light[licnt].r = 1.0f;
						light[licnt].g = 0.4f;
						light[licnt].b = 0.4f;
						light[licnt].a = 1.0f;
						light[licnt].range = 800.0f;
						light[licnt].brightness = 0.4f + ((float)(rand() % 4) * 0.1f);
						light[licnt].attenuation = 2.0f;
						light[licnt].on_off = TRUE;
						licnt++;
					}
				}
			}
		}
	}

	int loopcount = 8;//ライトのインデックス(0:手持ち松明用, 1:ラスボス用, 2:出入口用, 3,4,5,6:戦闘用は固定, 7:リカバーポイント)
	//各ライト設定
	for (int i = 0; i < licnt && loopcount < LIGHT_PCS; i++) {
		dx->PointLightPosSet(loopcount, light[i].x, light[i].y, light[i].z,
			light[i].r, light[i].g, light[i].b, light[i].a,
			light[i].on_off, light[i].range);
		loopcount++;
	}

	poMo.Update(20.0f);
}

void Map::Mapdraw_Ds() {
	poMo.DrawBillboard();
}

void Map::Mapcreate_BossPoint(int num) {
	Vertex* bl = new Vertex[num * 4];
	UINT* ind = new UINT[num * 6];
	int k = 0;
	int k1 = 0;
	for (int k3 = 0; k3 < mxy.z; k3++) {
		for (int j = 0; j < mxy.y; j++) {
			for (int i = 0; i < mxy.x; i++) {
				if (mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != 51)continue;
				//ボスポイント左前
				bl[k].Pos.as((float)i * 100.0f, (float)j * 100.0f + 50.0f, (float)k3 * 100.0f + 100.0f);
				bl[k].normal.as(0.0f, 0.0f, 0.0f);
				bl[k].tex.as(0.0f, 0.0f);
				ind[k1] = k;
				//ボスポイント右前
				bl[k + 1].Pos.as((float)i * 100.0f + 100.0f, (float)j * 100.0f + 50.0f, (float)k3 * 100.0f + 100.0f);
				bl[k + 1].normal.as(0.0f, 0.0f, 0.0f);
				bl[k + 1].tex.as(1.0f, 0.0f);
				ind[k1 + 1] = k + 1;
				ind[k1 + 4] = k + 1;
				//ボスポイント左奥
				bl[k + 2].Pos.as((float)i * 100.0f, (float)j * 100.0f + 50.0f, (float)k3 * 100.0f);
				bl[k + 2].normal.as(0.0f, 0.0f, 0.0f);
				bl[k + 2].tex.as(0.0f, 1.0f);
				ind[k1 + 2] = k + 2;
				ind[k1 + 3] = k + 2;
				//ボスポイント右奥
				bl[k + 3].Pos.as((float)i * 100.0f + 100.0f, (float)j * 100.0f + 50.0f, (float)k3 * 100.0f);
				bl[k + 3].normal.as(0.0f, 0.0f, 0.0f);
				bl[k + 3].tex.as(1.0f, 1.0f);
				ind[k1 + 5] = k + 3;
				k += 4;
				k1 += 6;
			}
		}
	}
	poBoss.setVertex(bl, num * 4, ind, num * 6);
	ARR_DELETE(bl);
	ARR_DELETE(ind);
}

void Map::Mapcreate_Elevator(int num) {
	Vertex* bl = new Vertex[num * 4];
	UINT* ind = new UINT[num * 6];
	int k = 0;
	int k1 = 0;
	for (int k3 = 0; k3 < mxy.z; k3++) {
		for (int j = 0; j < mxy.y; j++) {
			for (int i = 0; i < mxy.x; i++) {
				if (mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != 65 &&
					mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != 66)continue;
				//エレベータ左上
				bl[k].Pos.as((float)i * 100.0f, (float)j * 100.0f, (float)k3 * 100.0f + 2.0f);
				bl[k].normal.as(0.0f, 0.0f, 0.0f);
				bl[k].tex.as(0.0f, 0.0f);
				ind[k1] = k;
				//エレベータ右上
				bl[k + 1].Pos.as((float)i * 100.0f + 100.0f, (float)j * 100.0f, (float)k3 * 100.0f + 2.0f);
				bl[k + 1].normal.as(0.0f, 0.0f, 0.0f);
				bl[k + 1].tex.as(1.0f, 0.0f);
				ind[k1 + 1] = k + 1;
				ind[k1 + 4] = k + 1;
				//エレベータ左下
				bl[k + 2].Pos.as((float)i * 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f + 2.0f);
				bl[k + 2].normal.as(0.0f, 0.0f, 0.0f);
				bl[k + 2].tex.as(0.0f, 1.0f);
				ind[k1 + 2] = k + 2;
				ind[k1 + 3] = k + 2;
				//エレベータ右下
				bl[k + 3].Pos.as((float)i * 100.0f + 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f + 2.0f);
				bl[k + 3].normal.as(0.0f, 0.0f, 0.0f);
				bl[k + 3].tex.as(1.0f, 1.0f);
				ind[k1 + 5] = k + 3;
				k += 4;
				k1 += 6;
			}
		}
	}
	poElevator.setVertex(bl, num * 4, ind, num * 6);
	ARR_DELETE(bl);
	ARR_DELETE(ind);
}

void Map::Mapcreate_EXIT(float x, float y, float z, float xsize) {
	VertexBC ex[4] = {
		//左前
		x, y, z,
		1.0f, 1.0f, 1.0f, 1.0f,
		//右前
		x + xsize, y, z,
		1.0f, 1.0f, 1.0f, 1.0f,
		//左奥
		x, y, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		//右奥
		x + xsize, y, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	};
	UINT ind[6] = { 0, 1, 2, 2, 1, 3 };
	poEXIT.setVertex(ex, 4, ind, 6);
}

Map::~Map() {
	dx->WaitFenceCurrent();
	dx->ResetPointLight();
	ARR_DELETE(wood);
	ARR_DELETE(wall1);
	free(light);
	light = NULL;
	free(mxy.m);
	mxy.m = NULL;
	for (int i = 0; i < 128; i++) {
		free(mapdata[i]);
		mapdata[i] = NULL;
	}
	free(mapdata);
	mapdata = NULL;
}

bool Map::ViewCulling(float obj_x, float obj_y, float obj_z) {
	//対象オブジェクトまでの方角計算
	int theta;
	float radian;
	float cx = cax1 + cax1 - cax2;//視点
	float cy = cay1 + cay1 - cay2;//視点
	float dist_x = obj_x - cx;
	float dist_y = cy - obj_y;

	//オブジェクトと現在位置のxyが等しい場合(2フロア以上の場合発生する)
	if (dist_x == 0 && dist_y == 0)return FALSE;

	if (dist_x == 0) {
		if (cy > obj_y)theta = 0;
		if (cy < obj_y)theta = 180;
	}
	if (dist_y == 0) {
		if (cx < obj_x)theta = 90;
		if (cx > obj_x)theta = 270;
	}
	if (cx != obj_x && cy != obj_y) {
		radian = atan(dist_x / dist_y);
		theta = (int)(180.0 * radian / 3.14159265359);
		if (cx < obj_x && cy < obj_y)theta += 180;
		if (cx > obj_x) {
			if (cy < obj_y)theta += 180;
			if (cy > obj_y)theta += 360;
		}
	}

	//XY視野内判定
	int viewleft = (int)(src_theta - dx->GetViewY_theta() / 2.5f);
	int viewright = (int)(src_theta + dx->GetViewY_theta() / 2.5f);
	viewleft = (viewleft + 360) % 360;
	viewright = viewright % 360;
	bool ret = FALSE;
	if (viewleft < viewright) {
		if (viewleft < theta && viewright > theta)ret = TRUE;
	}
	if (viewleft > viewright) {
		if (viewleft < theta || viewright > theta)ret = TRUE;
	}

	//XYZ視野内判定
	if (ret == TRUE) {
		float dist_z = fabsf(posz * 100.0f - obj_z);
		if (dist_z == 0)return TRUE;
		float dist_xy = sqrt(dist_x * dist_x + dist_y * dist_y);
		radian = atan(dist_z / dist_xy);
		theta = (int)(180.0 * radian / 3.14159265359);
		if (theta < dx->GetViewY_theta() / 1.5)return TRUE;
	}

	return FALSE;
}

void Map::MapupdateWave() {
	int stX = posx - 6;
	if (stX < 0)stX = 0;
	int enX = posx + 6;
	if (enX > 29)enX = 29;
	int stY = posy - 6;
	if (stY < 0)stY = 0;
	int enY = posy + 6;
	if (enY > 39)enY = 39;
	for (int y = stY; y <= enY; y++) {
		for (int x = stX; x <= enX; x++) {
			if (mxy.m[mxy.x * y + x] == 48 || mxy.m[mxy.x * y + x] == 51 ||
				mxy.m[mxy.x * y + x] == 50 || mxy.m[mxy.x * y + x] == 54 || mxy.m[mxy.x * y + x] == 55) {
				wav.InstancedMap(x * 100.0f + 50.0f, y * 100.0f + 50.0f, 5.0f, 0.0f, 1.3f);
			}
		}
	}
	wav.InstanceUpdate(0, 0, 0, -0.4f, 0.4f);
}