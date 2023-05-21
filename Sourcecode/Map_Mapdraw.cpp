//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Mapクラス                                       **//
//**                                    Mapdraw関数                                      **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include "Map.h"

void Map::MapUpdateObj() {
	if (woodcount > 0)Mapupdate_Wood();
	if (squarecount >= 1)Mapupdate_Wall1();
	if (blockcountA >= 1)poWallA->Update({ 0, 0, 0 }, { 0, 0, 0, 0 }, { 0,0,0 }, { 1.0f,1.0f,1.0f }, 1.0f, 0.1f);
	if (blockcountB >= 1)poWallB->Update({ 0, 0, 0 }, { 0, 0, 0, 0 }, { 0,0,0 }, { 1.0f,1.0f,1.0f }, 0, 0.1f);
	if (blockcountC >= 1)poWallC->Update({ 0, 0, 0 }, { 0, 0, 0, 0 }, { 0,0,0 }, { 1.0f,1.0f,1.0f }, 0, 0.1f);
	if (blockcountD >= 1)poWallD->Update({ 0, 0, 0 }, { 0, 0, 0, 0 }, { 0,0,0 }, { 1.0f,1.0f,1.0f }, 8.0f, 0.1f);
	if (blockcountE >= 1)poWallE->Update({ 0, 0, 0 }, { 0, 0, 0, 0 }, { 0,0,0 }, { 1.0f,1.0f,1.0f }, 8.0f, 0.1f);
}

void Map::MapdrawObj(int comNo) {
	Mapdraw_Wood(comNo);
	Mapdraw_Wall1(comNo);
	if (poWallA)poWallA->Draw(comNo);
	if (poWallB)poWallB->Draw(comNo);
	if (poWallC)poWallC->Draw(comNo);
	if (poWallD)poWallD->Draw(comNo);
	if (poWallE)poWallE->Draw(comNo);
}

bool Map::GetMenuState(int* cnt) {
	static int count = 0;
	static bool menuOn = false;
	if (!menuSt)//menuOFF
		menuOn = false;
	else
		menuOn = true;

	if (menuOn) {//menuON
		if (count < 10) {
			count++;
			*cnt = (int)((float)count * 0.5f);
			return true;
		}
		else {
			*cnt = (int)((float)count * 0.5f);
			return true;
		}
	}
	else {//menuOFF
		if (count > 1) {
			count--;
			*cnt = (int)((float)count * 0.5f);
			return true;
		}
		else {
			*cnt = 1;
			return false;
		}
	}

	return false;
}

Encount Map::MapUpdate(MapState* mapstate, Directionkey direction, Encount encount,
	int battleSwitch, bool menu, bool title, bool ending) {

	//Debug();

	menuSt = menu;

	//ボス撃破後のマップ,テキスト処理
	if (mxy.m[POS_CE] == 51 && boss_killed[map_no] == 1) {
		if (map_no == 3) {
			map_text_f = 200;
			_tcscpy_s(m_tx, L"炎壁の封印が解けた！！");
		}
		mxy.m[POS_CE] = 48;//ボス撃破後ボス出現ポイント消滅
		boss_count = 0;   //ポイント全て表示されないようにする。マップ中に複数使用の場合変更必要
	}

	bool Move_f = FALSE;
	if (*mapstate == NORMAL_MAP && !ending && !title && encount == NOENCOUNT && !menu) {
		encount = Move(mapstate, direction);
		if (direction == UP || direction == DOWN)Move_f = TRUE;
	}
	if (!ending && !title && encount == NOENCOUNT)MovieSoundManager::Dungeon_sound(TRUE, map_no); else MovieSoundManager::Dungeon_soundoff(map_no);
	if (map_no == 1)MovieSoundManager::Rain_sound(TRUE);

	//視点
	float cx = cax1 + cax1 - cax2;
	float cy = cay1 + cay1 - cay2;
	float cz = (float)posz * 100.0f + 40.0f + elevator_step;
	float out_x, out_y;

	GetCamDelayPos(direction, cx, cy, &out_x, &out_y);
	/*cx = 1150;
	cy = 3320;
	cz = 40;
	out_x = 1150;
	out_y = 3320;
	cax2 = 1150;
	cay2 = 3180;*/
	Dx_SwapChain* sw = Dx_SwapChain::GetInstance();
	if (encount == NOENCOUNT)sw->Cameraset({ out_x, out_y, cz - 20.0f }, { cax2, cay2, cz });

	//外の明るさ変化
	bool mainlight = TRUE;
	static float OutCei_r = 0.0f;
	static float OutCei_g = 0.0f;
	static float OutCei_b = 0.0f;
	static bool dirup = TRUE;
	float or , og, ob;
	float pr, pg, pb;
	float fr, fg, fb;
	if (map_no == 1) {
		pr = 0.000015f;
		pg = 0.00002f;
		pb = 0.00002f;
		fr = -0.8f;
		fg = -0.8f;
		fb = -0.8f;
	}
	if (map_no == 1) {
		mainlight = FALSE;
		if (OutCei_r < -0.7f)mainlight = TRUE;//暗くなったらライトオン

		if (dirup == TRUE) {
			if ((OutCei_r -= pr) < fr)OutCei_r = fr;
			if ((OutCei_g -= pg) < fg)OutCei_g = fg;
			if ((OutCei_b -= pb) < fb)OutCei_b = fb;
			if (OutCei_r == fr && OutCei_g == fg && OutCei_b == fb)dirup = FALSE;
		}
		if (dirup == FALSE) {
			if ((OutCei_r += pr) > 0.0f)OutCei_r = 0.0f;
			if ((OutCei_g += pg) > 0.0f)OutCei_g = 0.0f;
			if ((OutCei_b += pb) > 0.0f)OutCei_b = 0.0f;
			if (OutCei_r == 0.0f && OutCei_g == 0.0f && OutCei_b == 0.0f)dirup = TRUE;
		}
		or = OutCei_r;
		og = OutCei_g;
		ob = OutCei_b;
	}
	else {
		or = 0.0f;
		og = 0.0f;
		ob = 0.0f;
	}
	if (map_no == 3 || encount != NOENCOUNT)mainlight = FALSE;

	//フォグoff
	Dx_Light::Fog(1.0f, 1.0f, 1.0f, 2.0f, 0.7f, FALSE);
	/*if (map_no == 4)dx->PointLightPosSet(1, { 1450.0f, 900.0f, 650.0f },
		{ 0.4f, 0.4f, 0.8f, 1.0f },
		true, 2000.0f);*/

	Dx_Light::setGlobalAmbientLight(0.001f, 0.001f, 0.001f);
	switch (map_no) {
	case 0:
		Dx_Light::SetDirectionLight(true);
		Dx_Light::DirectionLight(0.0f, 0.0f, -1.0f, 0.1f, 0.1f, 0.1f);
		//出口光源
		MapupdateWave();
		poGroundM->Update({ 0, 0, 0 }, { 0, 0, 0, 0 }, { 0,0,0 }, { 1.0f,1.0f,1.0f }, 0, 0.1f);
		poCeilingM->Update({ 0, 0, 0 }, { 0, 0, 0, 0 }, { 0,0,0 }, { 1.0f,1.0f,1.0f }, 0, 0.1f);
		poEXIT->Update({ 0, 0, 0 }, { 0, 0, 0, 0 }, { 0,0,0 }, { 1.0f,1.0f,1.0f }, 0, 0.1f);
		poEXIT->setPointLight(0, true, 600.0f, { 0.01f,0.001f,0.01f });

		MapUpdateObj();
		if (mo_count >= 1) {
			Mapupdate_Ds();
		}
		if (r_point_count >= 1)Mapupdate_Recover();
		if (boss_count >= 1 && encount != BOSS)poBoss.Update({ 0, 0, 0 }, { 0, 0, 0, 0 }, { 0,0,0 }, { 1.0f,1.0f,1.0f }, 0, 0.1f);
		break;
	case 1:
		//dx->setGlobalAmbientLight(1.0f, 1.0f, 1.0f);
		Dx_Light::SetDirectionLight(TRUE);
		Dx_Light::DirectionLight(0.3f, 0.3f, -1.0f, 0.1f, 0.1f, 0.1f);
		//dx->Fog(1.0f, 1.0f, 1.0f, 1.0f, 0.7f, false);
		poGroundF->Update({ 1100, 3500, 0 }, { 0, 0, 0, 0 }, { 0, 0,0 }, { 1.0f,1.0f,1.0f }, 0, 0.1f);
		poCeilingF->Update({ 1100, 3500, 0 }, { 0, 0, 0, 0 }, { 0, 0,0 }, { 1.0f,1.0f,1.0f }, 0, 0.1f);
		poGroundE->Update({ 900, 200, 0 }, { 0, 0, 0, 0 }, { 0, 0,0 }, { 1.0f,1.0f,1.0f }, 0, 0.1f);
		poCeilingE->Update({ 900, 200, 0 }, { 0, 0, 0, 0 }, { 0, 0,0 }, { 1.0f,1.0f,1.0f }, 0, 0.1f);
		if (blockcountA >= 1)poWallA->Update({ 0, 0, 0 }, { 0, 0, 0, 0 }, { 0,0,0 }, { 1.0f,1.0f,1.0f }, 0, 0.1f);
		if (blockcountC >= 1)poWallC->Update({ 0, 0, 0 }, { 0, 0, 0, 0 }, { 0,0,0 }, { 1.0f,1.0f,1.0f }, 0, 0.1f);
		//dx->Fog(1.0f, 0.2f, 0.1f, 2.5f, 0.8f, TRUE);
		if (f_wall_count >= 1) {
			poF_Wall->Update({ 0, 0, 0 }, { 0, 0, 0, 0 }, { 0,0,0 }, { 1.0f,1.0f,1.0f }, 6.0f, 0.1f);
		}
		if (mo_count >= 1) {
			Mapupdate_Ds();
		}
		//dx->Fog(1.0f, 1.0f, 1.0f, 2.0f, 0.7f, FALSE);
		if (r_point_count >= 1)Mapupdate_Recover();
		if (boss_count >= 1 && encount != BOSS)poBoss.Update({ 0, 0, 0 }, { 0, 0, 0, 0 }, { 0,0,0 }, { 1.0f,1.0f,1.0f }, 0, 0.1f);
		Dx_Light::DirectionLight(0.3f, 0.3f, -1.0f, 1.0f + or , 1.0f + og, 1.0f + ob);
		Dx_Light::DirectionLight(0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f);
		//dx->Fog(1.0f, 1.0f, 1.0f, 800.0f, 0.17f, TRUE);
		poBackground->Update({ 0, 0, -3000 }, { or , og, ob, 0 }, { 0,0,0 }, { 1.0f,1.0f,1.0f }, 0, 0.1f);
		poDirectionLight->Update({ 0, 0, -3200 }, { or , og, ob, 0 }, { 0,0,0 }, { 1.0f,1.0f,1.0f }, 0, 0.1f);
		//dx->Fog(1.0f, 1.0f, 1.0f, 100.0f, 0.4f, TRUE);
		Mapupdate_Mountain();
		//dx->Fog(1.0f, 1.0f, 1.0f, 3.0f, 0.4f, TRUE);
		poGroundM->Update({ 0, 0, 0 }, { 0, 0, 0, 0 }, { 0,0,0 }, { 1.0f,1.0f,1.0f }, 0, 0.1f);
		if (woodcount > 0)Mapupdate_Wood();
		if (squarecount >= 1)Mapupdate_Wall1();
		if (blockcountB >= 1)poWallB->Update({ 0, 0, 0 }, { 0, 0, 0, 0 }, { 0,0,0 }, { 1.0f,1.0f,1.0f }, 0, 0.1f);
		Mapupdate_Rain();
		break;
	case 2:
		Dx_Light::SetDirectionLight(TRUE);
		Dx_Light::DirectionLight(0.3f, 0.3f, -1.0f, 0.15f, 0.15f, 0.15f);
		poEXIT->Update({ 150.0f, 3930.0f, 0 }, { 0, 0, 0, 0 }, { 0,0,180.0f }, { 1,1,1 }, 0, 0.1f);
		//入口光源
		MapupdateWave();
		poGroundM->Update({ 0, 500, 0 }, { 0, 0, 0, 0 }, { 0,0,0 }, { 1,1,1 }, 0, 0.1f);
		poCeilingM->Update({ 0, 500, 0 }, { 0, 0, 0, 0 }, { 0,0,0 }, { 1,1,1 }, 0, 0.1f);
		poGroundE->Update({ 100, 100, 0 }, { 0, 0, 0, 0 }, { 0,0,0 }, { 1,1,1 }, 4.0f, 0.1f);
		poCeilingE->Update({ 100, 100, 0 }, { 0, 0, 0, 0 }, { 0,0,0 }, { 1,1,1 }, 4.0f, 0.1f);
		MapUpdateObj();
		if (f_wall_count >= 1) {
			poF_Wall->Update({ 0, 0, 0 }, { 0, 0, 0, 0 }, { 0,0,0 }, { 1.0f,1.0f,1.0f }, 20.0f, 0.1f);
		}
		if (mo_count >= 1) {
			Mapupdate_Ds();
		}
		if (r_point_count >= 1)Mapupdate_Recover();
		if (boss_count >= 1 && encount != BOSS)poBoss.Update({ 0, 0, 0 }, { 0, 0, 0, 0 }, { 0,0,0 }, { 1.0f,1.0f,1.0f }, 0, 0.1f);
		break;
	case 3:
		//とりあえずOK後で光源設定する
		Dx_Light::SetDirectionLight(TRUE);
		Dx_Light::DirectionLight(0.3f, 0.3f, -1.0f, 0.15f, 0.15f, 0.15f);
		poGroundF->Update({ 200, 3000, 0 }, { 0, 0, 0, 0 }, { 0,0,0 }, { 1.0f,1.0f,1.0f }, 0, 0.1f);
		poCeilingF->Update({ 200, 2990, 0 }, { 0, 0, 0, 0 }, { 0,0,0 }, { 1.0f,1.0f,1.0f }, 0, 0.1f);
		if (blockcountC >= 1)poWallC->Update({ 0, 0, 0 }, { 0, 0, 0, 0 }, { 0,0,0 }, { 1.0f,1.0f,1.0f }, 0, 0.1f);
		if (mo_count >= 1) {
			Mapupdate_Ds();
		}
		Dx_Light::DirectionLight(0.3f, 0.3f, -1.0f, 1.0f, 1.0f, 1.0f);
		Dx_Light::Fog(1.0f, 0.2f, 0.1f, 2.5f, 0.8f, TRUE);
		poGroundM->Update({ 0, 0, 0 }, { 0, 0, 0, 0 }, { 0,0,0 }, { 1.0f,1.0f,1.0f }, 8.0f, 0.1f);
		poCeilingM->Update({ 0, 0, 0 }, { 0, 0, 0, 0 }, { 0,0,0 }, { 1.0f,1.0f,1.0f }, 8.0f, 0.1f);
		if (blockcountD >= 1)poWallD->Update({ 0, 0, 0 }, { 0, 0, 0, 0 }, { 0,0,0 }, { 1.0f,1.0f,1.0f }, 8.0f, 0.1f);
		if (f_wall_count >= 1) {
			poF_Wall->Update({ 0, 0, 0 }, { 0, 0, 0, 0 }, { 0,0,0 }, { 1.0f,1.0f,1.0f }, 20.0f, 0.1f);
		}
		if (r_point_count >= 1)Mapupdate_Recover();
		if (boss_count >= 1 && encount != BOSS)poBoss.Update({ 0, 0, 0 }, { 0, 0, 0, 0 }, { 0,0,0 }, { 1.0f,1.0f,1.0f }, 0, 0.1f);
		break;
	case 4:
		//入り口に何か細工する予定
		Dx_Light::SetDirectionLight(TRUE);
		Dx_Light::DirectionLight(0.3f, 0.3f, -1.0f, 0.25f, 0.25f, 0.25f);
		Dx_Light::Fog(0.1f, 0.2f, 0.4f, 1.5f, 0.8f, TRUE);
		poGroundM->Update({ 0, 0, 0 }, { 0, 0, 0, 0 }, { 0,0,0 }, { 1.0f,1.0f,1.0f }, 8.0f, 0.1f);
		poCeilingM->Update({ 0, 0, 0 }, { 0, 0, 0, 0 }, { 0,0,0 }, { 1.0f,1.0f,1.0f }, 8.0f, 0.1f);
		MapUpdateObj();
		if (mo_count >= 1) {
			Mapupdate_Ds();
		}
		if (r_point_count >= 1)Mapupdate_Recover();
		if (boss_count >= 1 && encount != BOSS)poBoss.Update({ 0, 0, 0 }, { 0, 0, 0, 0 }, { 0,0,0 }, { 1.0f,1.0f,1.0f }, 0, 0.1f);
		if (Elevator_count >= 1)poElevator.Update({ 0, 0, 4.0f }, { 0, 0, 0, 0 }, { 0,0,0 }, { 1.0f,1.0f,1.0f }, 0, 0.1f);
		break;
	}

	he->TorchSwitch(true/*mainlight*/);
	if (!ending && battleSwitch < 2) {
		HeroUpdate(Move_f);//Mov関数からフラグもらうようにする
		MapHistory.Update(0, 0, 0, 0, 0, 0, 0, 1.0f, 1.0f);//地図
	}
	else {
		MapHistory.DrawOff();
	}

	MapText(m_tx);

	return encount;
}

void Map::SetMovie(int com_no) {
	if (mo_count >= 1) {
		poMo->SetTextureMPixel(com_no, MovieSoundManager::Torch_GetFrame(256, 256));
	}

	if (f_wall_count >= 1) {
		poF_Wall->SetTextureMPixel(com_no, MovieSoundManager::FireWall_GetFrame(256, 256));
	}
}

void Map::MapDraw(int comNo) {

	if (poGroundM)poGroundM->Draw(comNo);
	if (poCeilingM)poCeilingM->Draw(comNo);
	if (poEXIT)poEXIT->Draw(comNo);
	if (poGroundF)poGroundF->Draw(comNo);
	if (poCeilingF)poCeilingF->Draw(comNo);
	if (poGroundE)poGroundE->Draw(comNo);
	if (poCeilingE)poCeilingE->Draw(comNo);
	MapdrawObj(comNo);
	Mapdraw_Mountain(comNo);
	if (0)wav->Draw(comNo);//ブレンドするので最後の方に

	//地図
	//MapHistory.SetTextureMPixel(mapdata1);
	//MapHistory.Draw();

	if (mo_count >= 1) {
		Mapdraw_Ds(comNo);
	}
}

void Map::StreamOutput(int comNo) {
	if (poGroundM)poGroundM->StreamOutput(comNo);
	if (poCeilingM)poCeilingM->StreamOutput(comNo);
	if (poGroundF)poGroundF->StreamOutput(comNo);
	if (poCeilingF)poCeilingF->StreamOutput(comNo);
	if (poGroundE)poGroundE->StreamOutput(comNo);
	if (poCeilingE)poCeilingE->StreamOutput(comNo);
	if (mWood)mWood->StreamOutput(comNo);
	for (int i = 0; i < 3; i++)if (poWall1[i])poWall1[i]->StreamOutput(comNo);
	if (poWallA)poWallA->StreamOutput(comNo);
	if (poWallB)poWallB->StreamOutput(comNo);
	if (poWallC)poWallC->StreamOutput(comNo);
	if (poWallD)poWallD->StreamOutput(comNo);
	if (poWallE)poWallE->StreamOutput(comNo);
	if (mountain)mountain->StreamOutput(comNo);
	if (poEXIT)poEXIT->StreamOutput(comNo);
	if (wav)wav->StreamOutput(comNo);
	if (poDirectionLight)poDirectionLight->StreamOutput(comNo);

	//地図
	//MapHistory.SetTextureMPixel(mapdata1);
	//MapHistory.Draw();

	if (mo_count >= 1) {
		if (poMo)poMo->StreamOutputBillboard(comNo);
	}
}

void Map::StreamOutputAfterDraw() {
	Mapdraw_Recover(0);
	Mapdraw_Rain(0);
	if (poBackground)poBackground->Draw(0);
	poBoss.Draw(0);
	poElevator.Draw(0);
	if (wav)wav->UpdateDxrDivideBuffer();
	if (poWallA)poWallA->UpdateDxrDivideBuffer();
	if (poWallD)poWallD->UpdateDxrDivideBuffer();
	if (f_wall_count >= 1) if (poF_Wall)poF_Wall->Draw(0);
}

ParameterDXR** Map::getParameterDXR(int* numPara) {

	int n = 0;
	pdx = std::make_unique<ParameterDXR* []>(21);

	if (poWallA)pdx[n++] = poWallA->getParameter();
	if (poWallB)pdx[n++] = poWallB->getParameter();
	if (poWallC)pdx[n++] = poWallC->getParameter();
	if (poWallD)pdx[n++] = poWallD->getParameter();
	if (poWallE)pdx[n++] = poWallE->getParameter();

	if (poWall1[0])pdx[n++] = poWall1[0]->getParameter();
	if (poWall1[1])pdx[n++] = poWall1[1]->getParameter();
	if (poWall1[2])pdx[n++] = poWall1[2]->getParameter();

	if (poGroundF)pdx[n++] = poGroundF->getParameter();
	if (poCeilingF)pdx[n++] = poCeilingF->getParameter();
	if (poGroundM)pdx[n++] = poGroundM->getParameter();
	if (poCeilingM)pdx[n++] = poCeilingM->getParameter();
	if (poGroundE)pdx[n++] = poGroundE->getParameter();
	if (poCeilingE)pdx[n++] = poCeilingE->getParameter();
	if (wav)pdx[n++] = wav->getParameter();
	if (poDirectionLight)pdx[n++] = poDirectionLight->getParameter();

	if (mWood)pdx[n++] = mWood->getParameter();
	if (mountain)pdx[n++] = mountain->getParameter();
	if (poMo) {
		pdx[n++] = poMo->getParameter();
	}
	if (poEXIT) {
		pdx[n++] = poEXIT->getParameter();
	}
	*numPara = n;
	return pdx.get();
}

void Map::HeroUpdate(bool mf) {
	if (he)he->OBJWalkUpdate(cax1, cay1, (float)posz * 100.0f + elevator_step, 0, 0, 0, src_theta, mf);
}

Position::E_Pos *Map::Getposition(int p) {

	if (src_theta == 0 || src_theta == 360) {
		e_pos[0].theta = 0.0f;
		e_pos[0].element = TRUE;
		e_pos[0].x = cax1 + (int)(sin(0.0f * 3.14f / 180.0f) * 100.0f);
		e_pos[0].y = cay1 - (int)(cos(0.0f * 3.14f / 180.0f) * 100.0f);
		e_pos[0].z = posz * 100.0f;
		if (p > 0 && posy - 2 >= 0 && posx - 1 >= 0 && mxy.m[posz * mxy.y * mxy.x + (posy - 2) * mxy.x + (posx - 1)] == 48) {
			e_pos[1].theta = 338.0f;
			e_pos[1].element = TRUE;
			e_pos[1].x = cax1 + (int)(sin(338.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[1].y = cay1 - (int)(cos(338.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[1].z = posz * 100.0f;
		}
		else e_pos[1].element = FALSE;
		if (p > 1 && posy - 2 >= 0 && mxy.m[posz * mxy.y * mxy.x + (posy - 2) * mxy.x + posx] == 48) {
			e_pos[2].theta = 0.0f;
			e_pos[2].element = TRUE;
			e_pos[2].x = cax1 + (int)(sin(0.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[2].y = cay1 - (int)(cos(0.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[2].z = posz * 100.0f;
		}
		else e_pos[2].element = FALSE;
		if (p > 2 && posy - 2 >= 0 && posx + 1 <= 29 && mxy.m[posz * mxy.y * mxy.x + (posy - 2) * mxy.x + (posx + 1)] == 48) {
			e_pos[3].theta = 22.0f;
			e_pos[3].element = TRUE;
			e_pos[3].x = cax1 + (int)(sin(22.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[3].y = cay1 - (int)(cos(22.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[3].z = posz * 100.0f;
		}
		else e_pos[3].element = FALSE;
	}

	if (src_theta == 90) {
		e_pos[0].theta = 90.0f;
		e_pos[0].element = TRUE;
		e_pos[0].x = cax1 + (int)(sin(90.0f * 3.14f / 180.0f) * 100.0f);
		e_pos[0].y = cay1 - (int)(cos(90.0f * 3.14f / 180.0f) * 100.0f);
		e_pos[0].z = posz * 100.0f;
		if (p > 0 && posy - 1 >= 0 && posx + 2 <= 29 && mxy.m[posz * mxy.y * mxy.x + (posy - 1) * mxy.x + (posx + 2)] == 48) {
			e_pos[1].theta = 68.0f;
			e_pos[1].element = TRUE;
			e_pos[1].x = cax1 + (int)(sin(68.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[1].y = cay1 - (int)(cos(68.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[1].z = posz * 100.0f;
		}
		else e_pos[1].element = FALSE;
		if (p > 1 && posx + 2 <= 29 && mxy.m[posz * mxy.y * mxy.x + posy * mxy.x + (posx + 2)] == 48) {
			e_pos[2].theta = 90.0f;
			e_pos[2].element = TRUE;
			e_pos[2].x = cax1 + (int)(sin(90.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[2].y = cay1 - (int)(cos(90.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[2].z = posz * 100.0f;
		}
		else e_pos[2].element = FALSE;
		if (p > 2 && posy + 1 <= 29 && posx + 2 <= 29 && mxy.m[posz * mxy.y * mxy.x + (posy + 1) * mxy.x + (posx + 2)] == 48) {
			e_pos[3].theta = 112.0f;
			e_pos[3].element = TRUE;
			e_pos[3].x = cax1 + (int)(sin(112.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[3].y = cay1 - (int)(cos(112.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[3].z = posz * 100.0f;
		}
		else e_pos[3].element = FALSE;
	}

	if (src_theta == 180) {
		e_pos[0].theta = 180.0f;
		e_pos[0].element = TRUE;
		e_pos[0].x = cax1 + (int)(sin(180.0f * 3.14f / 180.0f) * 100.0f);
		e_pos[0].y = cay1 - (int)(cos(180.0f * 3.14f / 180.0f) * 100.0f);
		e_pos[0].z = posz * 100.0f;
		if (p > 0 && posy + 2 <= 29 && posx + 1 <= 29 && mxy.m[posz * mxy.y * mxy.x + (posy + 2) * mxy.x + (posx + 1)] == 48) {
			e_pos[1].theta = 158.0f;
			e_pos[1].element = TRUE;
			e_pos[1].x = cax1 + (int)(sin(158.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[1].y = cay1 - (int)(cos(158.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[1].z = posz * 100.0f;
		}
		else e_pos[1].element = FALSE;
		if (p > 1 && posy + 2 <= 29 && mxy.m[posz * mxy.y * mxy.x + (posy + 2) * mxy.x + posx] == 48) {
			e_pos[2].theta = 180.0f;
			e_pos[2].element = TRUE;
			e_pos[2].x = cax1 + (int)(sin(180.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[2].y = cay1 - (int)(cos(180.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[2].z = posz * 100.0f;
		}
		else e_pos[2].element = FALSE;
		if (p > 2 && posy + 2 <= 29 && posx - 1 >= 0 && mxy.m[posz * mxy.y * mxy.x + (posy + 2) * mxy.x + (posx - 1)] == 48) {
			e_pos[3].theta = 202.0f;
			e_pos[3].element = TRUE;
			e_pos[3].x = cax1 + (int)(sin(202.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[3].y = cay1 - (int)(cos(202.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[3].z = posz * 100.0f;
		}
		else e_pos[3].element = FALSE;
	}

	if (src_theta == 270) {
		e_pos[0].theta = 270.0f;
		e_pos[0].element = TRUE;
		e_pos[0].x = cax1 + (int)(sin(270.0f * 3.14f / 180.0f) * 100.0f);
		e_pos[0].y = cay1 - (int)(cos(270.0f * 3.14f / 180.0f) * 100.0f);
		e_pos[0].z = posz * 100.0f;
		if (p > 0 && posy + 1 <= 29 && posx - 2 >= 0 && mxy.m[posz * mxy.y * mxy.x + (posy + 1) * mxy.x + (posx - 2)] == 48) {
			e_pos[1].theta = 248.0f;
			e_pos[1].element = TRUE;
			e_pos[1].x = cax1 + (int)(sin(248.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[1].y = cay1 - (int)(cos(248.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[1].z = posz * 100.0f;
		}
		else e_pos[1].element = FALSE;
		if (p > 1 && posx - 2 >= 0 && mxy.m[posz * mxy.y * mxy.x + posy * mxy.x + (posx - 2)] == 48) {
			e_pos[2].theta = 270.0f;
			e_pos[2].element = TRUE;
			e_pos[2].x = cax1 + (int)(sin(270.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[2].y = cay1 - (int)(cos(270.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[2].z = posz * 100.0f;
		}
		else e_pos[2].element = FALSE;
		if (p > 2 && posy - 1 >= 0 && posx - 2 >= 0 && mxy.m[posz * mxy.y * mxy.x + (posy - 1) * mxy.x + (posx - 2)] == 48) {
			e_pos[3].theta = 292.0f;
			e_pos[3].element = TRUE;
			e_pos[3].x = cax1 + (int)(sin(292.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[3].y = cay1 - (int)(cos(292.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[3].z = posz * 100.0f;
		}
		else e_pos[3].element = FALSE;
	}
	return e_pos;
}

Position::H_Pos *Map::Getposition() {

	h_pos.cx1 = cax1;
	h_pos.cx2 = cax2;
	h_pos.cy1 = cay1;
	h_pos.cy2 = cay2;
	h_pos.px = posx;
	h_pos.py = posy;
	h_pos.pz = posz;
	h_pos.theta = src_theta;
	h_pos.cx = cax1 + cax1 - cax2;
	h_pos.cy = cay1 + cay1 - cay2;
	h_pos.cz = posz * 100.0f + 40.0f;

	return &h_pos;
}

void Map::Setposition(Position::H_Pos *pos) {
	cax1 = pos->cx1;
	cax2 = pos->cx2;
	cay1 = pos->cy1;
	cay2 = pos->cy2;
	posx = pos->px;
	posy = pos->py;
	posz = pos->pz;
	src_theta = pos->theta;
}

Position::mapxy *Map::Getmap() {
	return &mxy;
}

void Map::Debug() {//デバック用
	text->UpDateText(L"theta", 100.0f, 10.0f, 15.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateValue((int)(src_theta * 100.0f), 200.0f, 10.0f, 15.0f, 5, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateText(L"posx", 100.0f, 25.0f, 15.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateValue(posx, 200.0f, 25.0f, 15.0f, 5, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateText(L"posy", 100.0f, 40.0f, 15.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateValue(posy, 200.0f, 40.0f, 15.0, 5, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateText(L"posz", 100.0f, 55.0f, 15.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateValue(posz, 200.0f, 55.0f, 15.0f, 5, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateText(L"cax1", 100.0f, 70.0f, 15.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateValue((int)(cax1 * 100.0f), 200.0f, 70.0f, 15.0f, 5, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateText(L"cax2", 100.0f, 85.0f, 15.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateValue((int)(cax2 * 100.0f), 200.0f, 85.0f, 15.0f, 5, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateText(L"cay1", 100.0f, 100.0f, 15.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateValue((int)(cay1 * 100.0f), 200.0f, 100.0f, 15.0f, 5, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateText(L"cay2", 100.0f, 115.0f, 15.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->UpDateValue((int)(cay2 * 100.0f), 200.0f, 115.0f, 15.0f, 5, { 1.0f, 1.0f, 1.0f, 1.0f });
}
