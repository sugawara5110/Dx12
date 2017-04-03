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
int Map::MapComCurr = MAP_COM_1;

int Map::GetMapNo(){
	return map_no_s;
}

void Map::SetMapNo(int no){
	map_no_s = no;
}

void Map::SetBossKilled(int i, int f){

	if (i == -1){
		for (int i1 = 0; i1 < 5; i1++){
			boss_killed[i1] = 0;
		}
		return;
	}
	boss_killed[i] = f;
}

int *Map::GetBossKilled(){
	return boss_killed;
}

int Map::GetBossKilled(int map_no){
	return boss_killed[map_no];
}

Map::Map(Position::H_Pos *h_p, Hero *hero) {
	Map_Com = MapComCurr;
	if (MapComCurr == MAP_COM_1)MapComCurr = MAP_COM_2; else MapComCurr = MAP_COM_1;
	map_no = map_no_s;
	dx = Dx12Process::GetInstance();
	text = DxText::GetInstance();
	he = hero;
	map_text_f = 0;
	recover_p_f = FALSE;
	boss_p_f = FALSE;
	elevator_UP = FALSE;
	elevator_DOWN = FALSE;
	elevator_step = 0.0f;
	wood = NULL;
	wall1 = NULL;

	dx->Bigin(Map_Com);

	switch (map_no) {
	case 0:
		//出口
		poEXIT.SetCommandList(Map_Com);
		poEXIT.GetVBarray(SQUARE, 1);
		Mapcreate_EXIT(400.0f, 0.5f, 100.0f, 100.0f);
		poEXIT.Create(FALSE, -1, FALSE, FALSE);
		//地面メイン
		poGroundM.SetCommandList(Map_Com);
		poGroundM.GetVBarray(CONTROL_POINT, 1050);
		Mapcreate_Ground(&poGroundM, 30, 35, 0.0f, 1.0f);
		poGroundM.Create(TRUE, 1, TRUE, FALSE);
		//空メイン
		poCeilingM.SetCommandList(Map_Com);
		poCeilingM.GetVBarray(CONTROL_POINT, 1050);
		Mapcreate_Ceiling(&poCeilingM, 30, 35, 100.0f, 1.0f);
		poCeilingM.Create(TRUE, 2, TRUE, FALSE);
		break;
	case 1:
		//山
		mountain.SetCommandList(Map_Com);
		mountain.SetState(TRUE, TRUE, FALSE);
		mountain.GetVBarray("./dat/mesh/mountain.obj");
		mountain.GetTexture();
		//地面入り口
		poGroundF.SetCommandList(Map_Com);
		poGroundF.GetVBarray(CONTROL_POINT, 6);
		Mapcreate_Ground(&poGroundF, 2, 3, 2.0f, 1.0f);
		poGroundF.Create(TRUE, 1, TRUE, FALSE);
		//空入り口
		poCeilingF.SetCommandList(Map_Com);
		poCeilingF.GetVBarray(CONTROL_POINT, 6);
		Mapcreate_Ceiling(&poCeilingF, 2, 3, 100.0f, 1.0f);
		poCeilingF.Create(TRUE, 2, TRUE, FALSE);
		//地面メイン
		poGroundM.SetCommandList(Map_Com);
		poGroundM.GetVBarray(CONTROL_POINT, 1600);
		Mapcreate_Ground(&poGroundM, 40, 40, 0.0f, 1.0f);
		poGroundM.Create(TRUE, 5, TRUE, FALSE);
		//空メイン
		poBackground.SetCommandList(Map_Com);
		poBackground.GetVBarray(SQUARE, 5);
		Mapcreate_Background(-3500.0f, 7500.0f);
		poBackground.Create(FALSE, 6, TRUE, FALSE);
		//雨
		poRain.SetCommandList(Map_Com);
		poRain.GetVBarray(LINE_L, 1);
		poRain.SetVertex(0, 0,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			0.0f, 0.0f);

		poRain.SetVertex(1, 1,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			0.0f, 0.0f);
		poRain.Create(FALSE, -1, FALSE, FALSE);
		//地面出口
		poGroundE.SetCommandList(Map_Com);
		poGroundE.GetVBarray(CONTROL_POINT, 6);
		Mapcreate_Ground(&poGroundE, 2, 3, 2.0f, 1.0f);
		poGroundE.Create(TRUE, 12, TRUE, FALSE);
		//空出口
		poCeilingE.SetCommandList(Map_Com);
		poCeilingE.GetVBarray(CONTROL_POINT, 6);
		Mapcreate_Ceiling(&poCeilingE, 2, 3, 100.0f, 1.0f);
		poCeilingE.Create(TRUE, 11, TRUE, FALSE);
		break;
	case 2:
		//入口
		poEXIT.SetCommandList(Map_Com);
		poEXIT.GetVBarray(SQUARE, 1);
		Mapcreate_EXIT(-50.0f, -50.0f, 100.0f, 100.0f);
		poEXIT.Create(FALSE, -1, FALSE, FALSE);
		//地面メイン
		poGroundM.SetCommandList(Map_Com);
		poGroundM.GetVBarray(CONTROL_POINT, 1050);
		Mapcreate_Ground(&poGroundM, 30, 35, 0.0f, 1.0f);
		poGroundM.Create(TRUE, 12, TRUE, FALSE);
		//空メイン
		poCeilingM.SetCommandList(Map_Com);
		poCeilingM.GetVBarray(CONTROL_POINT, 1050);
		Mapcreate_Ceiling(&poCeilingM, 30, 35, 200.0f, 1.0f);
		poCeilingM.Create(TRUE, 11, TRUE, FALSE);
		//地面出口
		poGroundE.SetCommandList(Map_Com);
		poGroundE.GetVBarray(CONTROL_POINT, 8);
		Mapcreate_Ground(&poGroundE, 2, 4, 0.0f, 1.0f);
		poGroundE.Create(TRUE, 15, TRUE, FALSE);
		//空出口
		poCeilingE.SetCommandList(Map_Com);
		poCeilingE.GetVBarray(CONTROL_POINT, 8);
		Mapcreate_Ceiling(&poCeilingE, 2, 4, 200.0f, 1.0f);
		poCeilingE.Create(TRUE, 15, TRUE, FALSE);
		break;
	case 3:
		//地面入り口
		poGroundF.SetCommandList(Map_Com);
		poGroundF.GetVBarray(CONTROL_POINT, 6);
		Mapcreate_Ground(&poGroundF, 2, 3, 0.0f, 1.0f);
		poGroundF.Create(TRUE, 12, TRUE, FALSE);
		//空入り口
		poCeilingF.SetCommandList(Map_Com);
		poCeilingF.GetVBarray(CONTROL_POINT, 6);
		Mapcreate_Ceiling(&poCeilingF, 2, 3, 100.0f, 1.0f);
		poCeilingF.Create(TRUE, 11, TRUE, FALSE);
		//地面メイン
		poGroundM.SetCommandList(Map_Com);
		poGroundM.GetVBarray(CONTROL_POINT, 900);
		Mapcreate_Ground(&poGroundM, 30, 30, 0.0f, 1.0f);
		poGroundM.Create(TRUE, 15, TRUE, FALSE);
		//空メイン
		poCeilingM.SetCommandList(Map_Com);
		poCeilingM.GetVBarray(CONTROL_POINT, 900);
		Mapcreate_Ceiling(&poCeilingM, 30, 30, 200.0f, 1.0f);
		poCeilingM.Create(TRUE, 15, TRUE, FALSE);
		break;
	case 4:
		//地面メイン
		poGroundM.SetCommandList(Map_Com);
		poGroundM.GetVBarray(CONTROL_POINT, 900);
		Mapcreate_Ground(&poGroundM, 30, 30, 0.0f, 1.0f);
		poGroundM.Create(TRUE, 27, TRUE, FALSE);
		//空メイン
		poCeilingM.SetCommandList(Map_Com);
		poCeilingM.GetVBarray(CONTROL_POINT, 900);
		Mapcreate_Ceiling(&poCeilingM, 30, 30, 800.0f, 1.0f);
		poCeilingM.Create(TRUE, 28, TRUE, FALSE);
		break;
	}

	Mapfilter(h_p);

	//配置位置ずらし用
	wood = new OBJPosRandomValue[woodcount];
	wall1 = new OBJPosRandomValue[squarecount];

	//木
	if (woodcount > 0) {
		mWood.SetCommandList(Map_Com);
		mWood.SetState(TRUE, TRUE, FALSE);
		mWood.GetVBarray("./dat/mesh/tree.obj");
		mWood.GetTexture();
	}

	//壁(板)
	if (squarecount >= 1) {
		for (int i = 0; i < 3; i++) {
			poWall1[i].SetCommandList(Map_Com);
			poWall1[i].GetVBarray(SQUARE, 1);
			Mapcreate_Wall1(i);
			poWall1[i].Create(TRUE, 4, TRUE, TRUE);
		}
	}

	//壁(ブロック)
	if (blockcountA >= 1) {
		poWallA.SetCommandList(Map_Com);
		poWallA.GetVBarray(CONTROL_POINT, blockcountA);
		Mapcreate_Wall(&poWallA, 49, 52, 100.0f, -1.0f, 1.0f);
		poWallA.Create(TRUE, 0, TRUE, FALSE);
	}
	if (blockcountB >= 1) {
		poWallB.SetCommandList(Map_Com);
		poWallB.GetVBarray(CONTROL_POINT, blockcountB);
		Mapcreate_Wall(&poWallB, 74, 74, 100.0f, -1.0f, 1.0f);
		poWallB.Create(TRUE, 8, TRUE, FALSE);
	}
	if (blockcountC >= 1) {
		poWallC.SetCommandList(Map_Com);
		poWallC.GetVBarray(CONTROL_POINT, blockcountC);
		Mapcreate_Wall(&poWallC, 75, 79, 100.0f, -1.0f, 1.0f);
		poWallC.Create(TRUE, 11, TRUE, FALSE);
	}
	if (blockcountD >= 1) {
		poWallD.SetCommandList(Map_Com);
		poWallD.GetVBarray(CONTROL_POINT, blockcountD);
		Mapcreate_Wall(&poWallD, 76, 76, 100.0f, -2.0f, 2.0f);
		poWallD.Create(TRUE, 15, TRUE, FALSE);
	}
	if (blockcountE >= 1) {
		poWallE.SetCommandList(Map_Com);
		poWallE.GetVBarray(CONTROL_POINT, blockcountE);
		Mapcreate_Wall(&poWallE, 77, 78, 100.0f, -2.0f, 2.0f);
		poWallE.Create(TRUE, 26, TRUE, FALSE);
	}

	//リカバーポイント
	if (r_point_count >= 1) {
		poRecover.SetCommandList(Map_Com);
		poRecover.GetVBarray(SQUARE, r_point_count);
		for (int i = 0; i < 12; i++) {
			poRecoverLine[i].SetCommandList(Map_Com);
			poRecoverLine[i].GetVBarray(LINE_L, r_point_count);
		}
		Mapcreate_Recover();
		poRecover.Create(FALSE, 70, TRUE, TRUE);
		for (int i = 0; i < 12; i++)
			poRecoverLine[i].Create(FALSE, -1, FALSE, FALSE);
	}

	//動画テクスチャ松明
	if (mo_count >= 1) {
		poMo.SetCommandList(Map_Com);
		poMo.GetVBarray(lightcount);
		poMo.TextureInit(128, 128);
		Mapcreate_Ds();
		poMo.CreateBillboard();
	}

	//動画テクスチャ炎壁
	if (f_wall_count >= 1) {
		poF_Wall.SetCommandList(Map_Com);
		poF_Wall.GetVBarray(CONTROL_POINT, f_wall_count);
		poF_Wall.TextureInit(256, 256);
		Mapcreate_Wall(&poF_Wall, 73, 73, 200.0f, -6.0f, 6.0f);
		poF_Wall.Create(FALSE, -1, TRUE, TRUE);
	}

	//ボス出現ポイント
	if (boss_count >= 1) {
		poBoss.SetCommandList(Map_Com);
		poBoss.GetVBarray(SQUARE, boss_count);
		Mapcreate_BossPoint();
		poBoss.Create(FALSE, 61, TRUE, TRUE);
	}

	//エレベーター
	if (Elevator_count >= 1) {
		poElevator.SetCommandList(Map_Com);
		poElevator.GetVBarray(SQUARE, Elevator_count);
		Mapcreate_Elevator();
		poElevator.Create(FALSE, 70, TRUE, TRUE);
	}

	dx->End(Map_Com);
	dx->WaitForInit();

	//ライトポジション構造体確保
	light = (LightPos*)malloc(sizeof(LightPos) * lightcount);
	//ポイントライトリセット
	dx->ResetPointLight();
}

void Map::Mapdraw_Wood() {

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
	mWood.InstanceDraw(0, 0, 0, 0.3f);
}

void Map::Mapdraw_Mountain() {
	mountain.InstancedMap(-1500.0f, 2000.0f, 0, 0, 0, 0, 500.0f);
	mountain.Draw(5500.0f, 2000.0f, 0, 0, 0, 0, 0, 0, 0, 500.0f, 0.3f);
}

void Map::Mapcreate_Wall1(int i){
	//左前
	poWall1[i].SetVertex(0, 0,
		-50.0f, 0.0f, 100.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f);

	//右前
	poWall1[i].SetVertex(1, 4, 1,
		50.0f, 0.0f, 100.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f);

	//左奥
	poWall1[i].SetVertex(2, 3, 2,
		-50.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f);

	//右奥
	poWall1[i].SetVertex(5, 3,
		50.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f);
}

void Map::Mapdraw_Wall1() {

	int p = 0;
	for (int k3 = 0; k3 < mxy.z; k3++) {
		for (int j = 0; j < mxy.y; j++) {
			for (int i = 0; i < mxy.x; i++) {
				if (mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != 53)continue;
				float x = (float)i * 100.0f + 50.0f + wall1[p].x;
				float y = (float)j * 100.0f + 50.0f + wall1[p++].y;
				float z = (float)k3 * 100.0f;
				poWall1[i % 3].InstancedMap(x, y, z, src_theta);
			}
		}
	}
	for (int i = 0; i < 3; i++)poWall1[i].InstanceDraw(0, 0, 0, 0);
}

void Map::Mapcreate_Wall(PolygonData *pd, int no1, int no2, float height, float adjust, float adjust2){

	int k = 0;
	for (int k3 = 0; k3 < mxy.z; k3++){
		for (int j = 0; j < mxy.y; j++){
			for (int i = 0; i < mxy.x; i++){

				if (mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != no1 && mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != no2)continue;
				//壁ブロック頂点設定
				//正面検査
				if (k3 < mxy.z - 1 && mxy.m[(k3 + 1) * mxy.y * mxy.x + j * mxy.x + i] != no1 &&
					mxy.m[(k3 + 1) * mxy.y * mxy.x + j * mxy.x + i] != no2){
					//正面左上
					pd->SetVertex(k, k,
						(float)i * 100.0f - adjust2, (float)j * 100.0f - adjust2, (float)k3 * 100.0f + height + adjust,
						0.0f, 0.0f, 1.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						0.0f, 0.0f);

					//正面左下
					pd->SetVertex(k + 1, k + 1,
						(float)i * 100.0f - adjust2, (float)j * 100.0f + 100.0f + adjust2, (float)k3 * 100.0f + height + adjust,
						0.0f, 0.0f, 1.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						0.0f, 1.0f);

					//正面右下
					pd->SetVertex(k + 2, k + 2,
						(float)i * 100.0f + 100.0f + adjust2, (float)j * 100.0f + 100.0f + adjust2, (float)k3 * 100.0f + height + adjust,
						0.0f, 0.0f, 1.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						1.0f, 1.0f);

					//正面右上
					pd->SetVertex(k + 3, k + 3,
						(float)i * 100.0f + 100.0f + adjust2, (float)j * 100.0f - adjust2, (float)k3 * 100.0f + height + adjust,
						0.0f, 0.0f, 1.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						1.0f, 0.0f);
					k += 4;
				}

				//左面検査
				if (i > 0 && mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + (i - 1)] != no1 &&
					mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + (i - 1)] != no2){
					//左面上前
					pd->SetVertex(k, k,
						(float)i * 100.0f - adjust, (float)j * 100.0f - adjust2, (float)k3 * 100.0f + height + adjust2,
						-1.0f, 0.0f, 0.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						0.0f, 0.0f);

					//左面上後
					pd->SetVertex(k + 1, k + 1,
						(float)i * 100.0f - adjust, (float)j * 100.0f - adjust2, (float)k3 * 100.0f - adjust2,
						-1.0f, 0.0f, 0.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						0.0f, 1.0f);

					//左面下後
					pd->SetVertex(k + 2, k + 2,
						(float)i * 100.0f - adjust, (float)j * 100.0f + 100.0f + adjust2, (float)k3 * 100.0f - adjust2,
						-1.0f, 0.0f, 0.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						1.0f, 1.0f);

					//左面下前
					pd->SetVertex(k + 3, k + 3,
						(float)i * 100.0f - adjust, (float)j * 100.0f + 100.0f + adjust2, (float)k3 * 100.0f + height + adjust2,
						-1.0f, 0.0f, 0.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						1.0f, 0.0f);
					k += 4;
				}

				//右面検査
				if (i < mxy.x - 1 && mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + (i + 1)] != no1 &&
					mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + (i + 1)] != no2){
					//右面下前
					pd->SetVertex(k, k,
						(float)i * 100.0f + 100.0f + adjust, (float)j * 100.0f + 100.0f + adjust2, (float)k3 * 100.0f + height + adjust2,
						1.0f, 0.0f, 0.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						0.0f, 0.0f);

					//右面下後
					pd->SetVertex(k + 1, k + 1,
						(float)i * 100.0f + 100.0f + adjust, (float)j * 100.0f + 100.0f + adjust2, (float)k3 * 100.0f - adjust2,
						1.0f, 0.0f, 0.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						0.0f, 1.0f);

					//右面上後
					pd->SetVertex(k + 2, k + 2,
						(float)i * 100.0f + 100.0f + adjust, (float)j * 100.0f - adjust2, (float)k3 * 100.0f - adjust2,
						1.0f, 0.0f, 0.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						1.0f, 1.0f);

					//右面上前
					pd->SetVertex(k + 3, k + 3,
						(float)i * 100.0f + 100.0f + adjust, (float)j * 100.0f - adjust2, (float)k3 * 100.0f + height + adjust2,
						1.0f, 0.0f, 0.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						1.0f, 0.0f);
					k += 4;
				}

				//上面検査
				if (j > 0 && mxy.m[k3 * mxy.y * mxy.x + (j - 1) * mxy.x + i] != no1 &&
					mxy.m[k3 * mxy.y * mxy.x + (j - 1) * mxy.x + i] != no2){
					//上面右前
					pd->SetVertex(k, k,
						(float)i * 100.0f + 100.0f + adjust2, (float)j * 100.0f - adjust, (float)k3 * 100.0f + height + adjust2,
						0.0f, -1.0f, 0.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						0.0f, 0.0f);

					//上面右後
					pd->SetVertex(k + 1, k + 1,
						(float)i * 100.0f + 100.0f + adjust2, (float)j * 100.0f - adjust, (float)k3 * 100.0f - adjust2,
						0.0f, -1.0f, 0.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						0.0f, 1.0f);

					//上面左後
					pd->SetVertex(k + 2, k + 2,
						(float)i * 100.0f - adjust2, (float)j * 100.0f - adjust, (float)k3 * 100.0f - adjust2,
						0.0f, -1.0f, 0.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						1.0f, 1.0f);

					//上面左前
					pd->SetVertex(k + 3, k + 3,
						(float)i * 100.0f - adjust2, (float)j * 100.0f - adjust, (float)k3 * 100.0f + height + adjust2,
						0.0f, -1.0f, 0.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						1.0f, 0.0f);
					k += 4;
				}

				//裏面検査
				if (k3 > 0 && mxy.m[(k3 - 1) * mxy.y * mxy.x + j * mxy.x + i] != no1 &&
					mxy.m[(k3 - 1) * mxy.y * mxy.x + j * mxy.x + i] != no2){
					//裏面右上
					pd->SetVertex(k, k,
						(float)i * 100.0f + 100.0f + adjust2, (float)j * 100.0f - adjust2, (float)k3 * 100.0f - adjust,
						0.0f, 0.0f, -1.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						0.0f, 0.0f);

					//裏面右下
					pd->SetVertex(k + 1, k + 1,
						(float)i * 100.0f + 100.0f + adjust2, (float)j * 100.0f + 100.0f + adjust2, (float)k3 * 100.0f - adjust,
						0.0f, 0.0f, -1.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						0.0f, 1.0f);

					//裏面左下
					pd->SetVertex(k + 2, k + 2,
						(float)i * 100.0f - adjust2, (float)j * 100.0f + 100.0f + adjust2, (float)k3 * 100.0f - adjust,
						0.0f, 0.0f, -1.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						1.0f, 1.0f);

					//裏面左上
					pd->SetVertex(k + 3, k + 3,
						(float)i * 100.0f - adjust2, (float)j * 100.0f - adjust2, (float)k3 * 100.0f - adjust,
						0.0f, 0.0f, -1.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						1.0f, 0.0f);
					k += 4;
				}

				//底面検査
				if (j < mxy.y - 1 && mxy.m[k3 * mxy.y * mxy.x + (j + 1) * mxy.x + i] != no1 &&
					mxy.m[k3 * mxy.y * mxy.x + (j + 1) * mxy.x + i] != no2){
					//底面左前
					pd->SetVertex(k, k,
						(float)i * 100.0f - adjust2, (float)j * 100.0f + 100.0f + adjust, (float)k3 * 100.0f + height + adjust2,
						0.0f, 1.0f, 0.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						0.0f, 0.0f);

					//底面左後
					pd->SetVertex(k + 1, k + 1,
						(float)i * 100.0f - adjust2, (float)j * 100.0f + 100.0f + adjust, (float)k3 * 100.0f - adjust2,
						0.0f, 1.0f, 0.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						0.0f, 1.0f);

					//底面右後
					pd->SetVertex(k + 2, k + 2,
						(float)i * 100.0f + 100.0f + adjust2, (float)j * 100.0f + 100.0f + adjust, (float)k3 * 100.0f - adjust2,
						0.0f, 1.0f, 0.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						1.0f, 1.0f);

					//底面右前
					pd->SetVertex(k + 3, k + 3,
						(float)i * 100.0f + 100.0f + adjust2, (float)j * 100.0f + 100.0f + adjust, (float)k3 * 100.0f + height + adjust2,
						0.0f, 1.0f, 0.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						1.0f, 0.0f);
					k += 4;
				}
			}
		}
	}
}

void Map::Mapcreate_Ground(PolygonData *pd, int pcsx, int pcsy, float height, float adjust){

	//地面頂点
	int k = 0;
	float size = 100.0f;//1マスの大きさ
	for (int j = 0; j < pcsy; j++){
		for (int i = 0; i < pcsx; i++){
			float x = size * i;
			float y = size * j;
			//地面左上
			pd->SetVertex(k, k,
				x - adjust, y - adjust, height,
				0.0f, 0.0f, 1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				0.0f, 0.0f);

			//地面左下
			pd->SetVertex(k + 1, k + 1,
				x - adjust, y + size + adjust, height,
				0.0f, 0.0f, 1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				0.0f, 1.0f);

			//地面右下
			pd->SetVertex(k + 2, k + 2,
				x + size + adjust, y + size + adjust, height,
				0.0f, 0.0f, 1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				1.0f, 1.0f);

			//地面右上
			pd->SetVertex(k + 3, k + 3,
				x + size + adjust, y - adjust, height,
				0.0f, 0.0f, 1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				1.0f, 0.0f);

			k += 4;
		}
	}
}

void Map::Mapcreate_Ceiling(PolygonData *pd, int pcsx, int pcsy, float height, float adjust){

	//天井頂点
	int k = 0;
	float size = 100.0f;//1マスの大きさ
	for (int j = 0; j < pcsy; j++){
		for (int i = 0; i < pcsx; i++){
			float x = size * i;
			float y = size * j;
			//天井左上
			pd->SetVertex(k, k,
				x - adjust, y - adjust, height,
				0.0f, 0.0f, -1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				0.0f, 0.0f);

			//天井右上
			pd->SetVertex(k + 1, k + 1,
				x + size + adjust, y - adjust, height,
				0.0f, 0.0f, -1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				1.0f, 0.0f);

			//天井右下
			pd->SetVertex(k + 2, k + 2,
				x + size + adjust, y + size + adjust, height,
				0.0f, 0.0f, -1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				1.0f, 1.0f);

			//天井左下
			pd->SetVertex(k + 3, k + 3,
				x - adjust, y + size + adjust, height,
				0.0f, 0.0f, -1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				0.0f, 1.0f);

			k += 4;
		}
	}
}

void Map::Mapcreate_Background(float st, float end){

	float height = end - st;
	//正面
	//右上
	poBackground.SetVertex(0, 0,
		end, st, height,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f);

	//左上
	poBackground.SetVertex(1, 4, 1,
		st, st, height,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		5.0f, 0.0f);

	//右下
	poBackground.SetVertex(2, 3, 2,
		end, end, height,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 5.0f);

	//左下
	poBackground.SetVertex(5, 3,
		st, end, height,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		5.0f, 5.0f);

	//上
	//左前
	poBackground.SetVertex(6, 4,
		st, st, height,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		5.0f, 5.0f);

	//右前
	poBackground.SetVertex(7, 10, 5,
		end, st, height,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 5.0f);

	//左奥
	poBackground.SetVertex(8, 9, 6,
		st, st, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		5.0f, 0.0f);

	//右奥
	poBackground.SetVertex(11, 7,
		end, st, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f);

	//下
	//右前
	poBackground.SetVertex(12, 8,
		end, end, height,
		0.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f);

	//左前
	poBackground.SetVertex(13, 16, 9,
		st, end, height,
		0.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		5.0f, 0.0f);

	//右奥
	poBackground.SetVertex(14, 15, 10,
		end, end, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 5.0f);

	//左奥
	poBackground.SetVertex(17, 11,
		st, end, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		5.0f, 5.0f);

	//右
	//上前
	poBackground.SetVertex(18, 12,
		end, st, height,
		-1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		5.0f, 0.0f);

	//下前
	poBackground.SetVertex(19, 22, 13,
		end, end, height,
		-1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		5.0f, 5.0f);

	//上奥
	poBackground.SetVertex(20, 21, 14,
		end, st, 0.0f,
		-1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f);

	//下奥
	poBackground.SetVertex(23, 15,
		end, end, 0.0f,
		-1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 5.0f);

	//左
	//下前
	poBackground.SetVertex(24, 16,
		st, end, height,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 5.0f);

	//上前
	poBackground.SetVertex(25, 28, 17,
		st, st, height,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f);

	//下奥
	poBackground.SetVertex(26, 27, 18,
		st, end, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		5.0f, 5.0f);

	//上奥
	poBackground.SetVertex(29, 19,
		st, st, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		5.0f, 0.0f);
}

void Map::Mapdraw_Rain() {

	int x, y;
	for (int i = 0; i < 148; i++) {
		x = rand() % 500;
		y = rand() % 500;
		poRain.InstancedMap(cax1 - 250.0f + x, cay1 - 250.0f + y, 0.0f, 0.0f, (float)(rand() % 300));
	}
	poRain.InstanceDraw(0.0f, 0.0f, 0.0f, 0.0f);
}

void Map::Mapcreate_Recover() {

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
				poRecover.SetVertex(k1, k,
					(float)i * 100.0f, (float)j * 100.0f, (float)k3 * 100.0f + 2.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					0.0f, 0.0f);

				//回復ポイント右上
				poRecover.SetVertex(k1 + 1, k1 + 4, k + 1,
					(float)i * 100.0f + 100.0f, (float)j * 100.0f, (float)k3 * 100.0f + 2.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 0.0f);

				//回復ポイント左下
				poRecover.SetVertex(k1 + 2, k1 + 3, k + 2,
					(float)i * 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f + 2.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					0.0f, 1.0f);

				//回復ポイント右下
				poRecover.SetVertex(k1 + 5, k + 3,
					(float)i * 100.0f + 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f + 2.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f);

				//line
				for (int l = 0; l < 12; l++) {
					poRecoverLine[l].SetVertex(l1, l1,
						(float)i * 100.0f + 50.0f, (float)j * 100.0f + 50.0f, (float)k3 * 100.0f + 1.0f,
						0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 1.0f,
						0.0f, 0.0f);

					poRecoverLine[l].SetVertex(l1 + 1, l1 + 1,
						(float)i * 100.0f + 50.0f, (float)j * 100.0f + 50.0f, (float)k3 * 100.0f,
						0.0f, 0.0f, 0.0f,
						0.0f, 0.4f, 1.0f, 1.0f,
						0.0f, 0.0f);
				}
				k += 4;
				k1 += 6;
				l1 += 2;
			}
		}
	}
}

void Map::Mapdraw_Recover() {

	for (float j = 0.0f; j < 12.0f; j++) {
		for (float i1 = 0.0f; i1 < 120.0f; i1++) {
			float i = j * 30.0f + i1 * 0.25f;
			float line_y = -cos(i * 3.14f / 180.0f) * 50.0f;
			float line_x = sin(i * 3.14f / 180.0f) * 50.0f;
			int rnd = rand() % 20;
			poRecoverLine[(int)j].InstancedMapSize3(line_x, line_y, 0.0f, 0.0f, 1.0f, 1.0f, (float)rnd);
		}
		poRecoverLine[(int)j].InstanceDraw(0.0f, 0.0f, 0.0f, 0.0f);
	}
	dx->PointLightPosSet(7, recovPosX, recovPosY, 2.0f, 0.2f, 0.8f, 0.2f, 1.0f, 100.0f, 10.0f, 2.0f, TRUE);
	poRecover.Draw(0, 0, 4.0f, 0, 0, 0, 0, 0);
}

void Map::Mapcreate_Ds() {

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

void Map::Mapdraw_Ds() {

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
						light[licnt].range = 80.0f;
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
						light[licnt].range = 80.0f;
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
						light[licnt].range = 80.0f;
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
						light[licnt].range = 80.0f;
						light[licnt].brightness = 0.4f + ((float)(rand() % 4) * 0.1f);
						light[licnt].attenuation = 2.0f;
						light[licnt].on_off = TRUE;
						licnt++;
					}
				}
			}
		}
	}

	int loopcount = LIGHT_PCS_init;//ライトのインデックス(0:手持ち松明用, 1:ラスボス用, 2:出入口用, 3,4,5,6:戦闘用は固定, 7:リカバーポイント)
	//各ライト設定
	for (int i = 0; i < licnt && loopcount < LIGHT_PCS; i++) {
		dx->PointLightPosSet(loopcount, light[i].x, light[i].y, light[i].z, light[i].r, light[i].g, light[i].b, light[i].a, light[i].range, light[i].brightness, light[i].attenuation, light[i].on_off);
		loopcount++;
	}

	poMo.DrawBillboard(20.0f);
}

void Map::Mapcreate_BossPoint(){

	int k = 0;
	int k1 = 0;
	for (int k3 = 0; k3 < mxy.z; k3++){
		for (int j = 0; j < mxy.y; j++){
			for (int i = 0; i < mxy.x; i++){
				if (mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != 51)continue;
				//ボスポイント左前
				poBoss.SetVertex(k1, k,
					(float)i * 100.0f, (float)j * 100.0f + 50.0f, (float)k3 * 100.0f + 100.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					0.0f, 0.0f);

				//ボスポイント右前
				poBoss.SetVertex(k1 + 1, k1 + 4, k + 1,
					(float)i * 100.0f + 100.0f, (float)j * 100.0f + 50.0f, (float)k3 * 100.0f + 100.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 0.0f);

				//ボスポイント左奥
				poBoss.SetVertex(k1 + 2, k1 + 3, k + 2,
					(float)i * 100.0f, (float)j * 100.0f + 50.0f, (float)k3 * 100.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					0.0f, 1.0f);

				//ボスポイント右奥
				poBoss.SetVertex(k1 + 5, k + 3,
					(float)i * 100.0f + 100.0f, (float)j * 100.0f + 50.0f, (float)k3 * 100.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f);

				k += 4;
				k1 += 6;
			}
		}
	}
}

void Map::Mapcreate_Elevator(){

	int k = 0;
	int k1 = 0;
	for (int k3 = 0; k3 < mxy.z; k3++){
		for (int j = 0; j < mxy.y; j++){
			for (int i = 0; i < mxy.x; i++){
				if (mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != 65 &&
					mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != 66)continue;
				//エレベータ左上
				poElevator.SetVertex(k1, k,
					(float)i * 100.0f, (float)j * 100.0f, (float)k3 * 100.0f + 2.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					0.0f, 0.0f);

				//エレベータ右上
				poElevator.SetVertex(k1 + 1, k1 + 4, k + 1,
					(float)i * 100.0f + 100.0f, (float)j * 100.0f, (float)k3 * 100.0f + 2.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 0.0f);

				//エレベータ左下
				poElevator.SetVertex(k1 + 2, k1 + 3, k + 2,
					(float)i * 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f + 2.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					0.0f, 1.0f);

				//エレベータ右下
				poElevator.SetVertex(k1 + 5, k + 3,
					(float)i * 100.0f + 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f + 2.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f);

				k += 4;
				k1 += 6;
			}
		}
	}
}

void Map::Mapcreate_EXIT(float x, float y, float z, float xsize){

	//左前
	poEXIT.SetVertex(0, 0,
		x, y, z,
		255.0f, 255.0f, 255.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f);

	//右前
	poEXIT.SetVertex(1, 4, 1,
		x + xsize, y, z,
		255.0f, 255.0f, 255.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f);

	//左奥
	poEXIT.SetVertex(2, 3, 2,
		x, y, 0.0f,
		255.0f, 255.0f, 255.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f);

	//右奥
	poEXIT.SetVertex(5, 3,
		x + xsize, y, 0.0f,
		255.0f, 255.0f, 255.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f);
}

Map::~Map(){
	dx->WaitFenceCurrent();
	dx->PointLightPosSet(2, 450.0f, 0.0f, 50.0f, 1.0f, 1.0f, 1.0f, 1.0f, 250.0f, 300.0f, 2.0f, FALSE);//出口ライト消す
	ARR_DELETE(wood);
	ARR_DELETE(wall1);
	free(light);
	light = NULL;
	free(mxy.m);
	mxy.m = NULL;
}

bool Map::ViewCulling(float obj_x, float obj_y, float obj_z){
	//対象オブジェクトまでの方角計算
	int theta;
	float radian;
	float cx = cax1 + cax1 - cax2;//視点
	float cy = cay1 + cay1 - cay2;//視点
	float dist_x = obj_x - cx;
	float dist_y = cy - obj_y;

	//オブジェクトと現在位置のxyが等しい場合(2フロア以上の場合発生する)
	if (dist_x == 0 && dist_y == 0)return FALSE;

	if (dist_x == 0){
		if (cy > obj_y)theta = 0;
		if (cy < obj_y)theta = 180;
	}
	if (dist_y == 0){
		if (cx < obj_x)theta = 90;
		if (cx > obj_x)theta = 270;
	}
	if (cx != obj_x && cy != obj_y){
		radian = atan(dist_x / dist_y);
		theta = (int)(180.0 * radian / 3.14159265359);
		if (cx < obj_x && cy < obj_y)theta += 180;
		if (cx > obj_x){
			if (cy < obj_y)theta += 180;
			if (cy > obj_y)theta += 360;
		}
	}

	//XY視野内判定
	int viewleft = (int)(src_theta - dx->GetViewY_theta() / 1.5);
	int viewright = (int)(src_theta + dx->GetViewY_theta() / 1.5);
	viewleft = (viewleft + 360) % 360;
	viewright = viewright % 360;
	bool ret = FALSE;
	if (viewleft < viewright){
		if (viewleft < theta && viewright > theta)ret = TRUE;
	}
	if (viewleft > viewright){
		if (viewleft < theta || viewright > theta)ret = TRUE;
	}

	//XYZ視野内判定
	if (ret == TRUE){
		float dist_z = fabsf(posz * 100.0f - obj_z);
		if (dist_z == 0)return TRUE;
		float dist_xy = sqrt(dist_x * dist_x + dist_y * dist_y);
		radian = atan(dist_z / dist_xy);
		theta = (int)(180.0 * radian / 3.14159265359);
		if (theta < dx->GetViewY_theta() / 1.5)return TRUE;
	}

	return FALSE;
}