//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Battleクラス                                    **//
//**                                    Menucreate関数                                   **//
//*****************************************************************************************//

#include <new>     //placement new
#include <time.h>
#include "Enemy.h"
#include "EnemySide.h"
#include "EnemyBoss.h"
#include "Battle.h"
#include "Hero.h"
#include <memory.h>

void Battle::SetParameter(Hero *he, Position::E_Pos *e_po, Position::H_Pos *h_po, Encount encount, int no, int e_nu) {
	he_para = he;
	e_po_para = e_po;
	h_po_para = h_po;
	encount_para = encount;
	no_para = no;
	e_nu_para = e_nu;
}

void Battle::Init() {

	dx = Dx12Process::GetInstance();
	text = DxText::GetInstance();
	comNo = 0;
	e_num = e_nu_para;//敵出現数
	memcpy(e_pos, e_po_para, sizeof(Position::E_Pos) * 4);//ポジションアドレス
	memcpy(&h_pos, h_po_para, sizeof(Position::H_Pos));//ポジションアドレス
	b_pos = GetBtPos(&h_pos);//アドレスで渡す
	command.GetVBarray2D(1);
	comDraw = TRUE;//全敵HP0時コマンド表示をオフにする
	h_select.GetVBarray2D(1);
	Escape_f = 0;
	Escape_s = FALSE;
	E_select.GetVBarray(SQUARE,1);
	CreateFin = FALSE;
	battlefirst = FALSE;
	CamActOn = FALSE;
	CamActInd = -1;

	srand((unsigned)time(NULL));

	int en_bgm;
	if (encount_para == SIDE)en_bgm = 0;
	if (encount_para == BOSS) {
		if (no_para <= 2)en_bgm = 1;
		if (no_para == 3)en_bgm = 2;
		if (no_para == 4)en_bgm = 3;
	}

	MovieSoundManager::ObjCreate_battle(en_bgm);
	MovieSoundManager::Enemy_sound(FALSE);
	MovieSoundManager::Enemy_sound(TRUE);
	if (encount_para == SIDE) {
		//通常の敵の生成
		enemyside = new EnemySide[e_num];
		int rnd;
		//アップキャスト前に初期化
		for (int i = 0; i < e_num; i++) {
			rnd = (rand() % 4) + no_para * 4;
			new(enemyside + i) EnemySide(rnd, i, &h_pos, e_pos);// 配列をplacement newを使って初期化する
		}

		//アップキャスト
		enemy = enemyside;
	}
	if (encount_para == BOSS) {
		//ボス生成
		enemyboss = new EnemyBoss[e_num];

		//アップキャスト前に初期化
		for (int i = 0; i < e_num; i++) {
			new(enemyboss + i) EnemyBoss(no_para, i, &h_pos, e_pos);// 配列をplacement newを使って初期化する
		}

		//アップキャスト
		enemy = enemyboss;
	}

	command_run_first_flg = FALSE;
	time_stop_flg = FALSE;
	com_select = NOSELECT;
	E_com_select = NOSELECT;
	select_obj = 0;
	E_select_obj = 0;
	MAG_select = NOSEL;
	E_MAG_select = NOSEL;
	e_draw = new Draw[e_num];

	for (int i = 0; i < e_num; i++) {
		e_draw[i].AGmeter = 0.0f;
		e_draw[i].action = RECOVER;
		e_draw[i].RCVdrawY = 0;
		e_draw[i].RCVdata = -1;
		e_draw[i].Recov_f = FALSE;
		e_draw[i].DMdrawY = 0;
		e_draw[i].DMdata = -1;
		e_draw[i].command_run = FALSE;
		e_draw[i].LOST_fin = FALSE;
		if (e_pos[i].element == FALSE) {
			e_draw[i].LOST_fin = TRUE; enemy[i].Dieflg(TRUE);
		}
		E_drawPos(i);
	}

	for (int i = 0; i < 4; i++) {
		h_draw[i].AGmeter = 0.0f;
		h_draw[i].action = NORMAL;
		h_draw[i].RCVdrawY = 0;
		h_draw[i].RCVdata = -1;
		h_draw[i].Recov_f = FALSE;
		h_draw[i].DMdrawY = 0;
		h_draw[i].DMdata = -1;
		h_draw[i].command_run = FALSE;
		h_draw[i].LOST_fin = FALSE;
		if (he_para[i].Dieflg() == TRUE)h_draw[i].LOST_fin = TRUE;
		//↓コマンド入力パラメタ
		h_draw[i].manu = MAIN_M;
		h_draw[i].M_select = 0;
		h_draw[i].A_select = 0;
		h_draw[i].MA_select = 0;
		h_draw[i].R_select = 0;
		H_drawPos(i);
	}
}

void Battle::SetVertex() {
	VertexBC se[4] = {
		//カーソル左上
		-25.0f, -25.0f, 10.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		//カーソル右上
		25.0f, -25.0f, 10.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		//カーソル左下
		-25.0f, 25.0f, 10.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		//カーソル右下
		25.0f, 25.0f, 10.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	UINT ind[6] = { 0, 1, 2, 2, 1, 3 };
	E_select.setVertex(se, 4, ind, 6);

	for (int i = 0; i < e_num; i++) {
		enemy[i].SetVertex();
	}
}

void Battle::SetCommandList(int com_no) {
	comNo = com_no;
	command.SetCommandList(comNo);
	h_select.SetCommandList(comNo);
	E_select.SetCommandList(comNo);
	for (int i = 0; i < e_num; i++) {
		enemy[i].SetCommandList(comNo);
	}
}

void Battle::CreateBattle() {
	Menucreate();
	h_select.CreateBox(0.0f, 0.0f, 0.0f, 0.1f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, TRUE, TRUE);
	E_select.Create(FALSE, -1, FALSE, FALSE);
	for (int i = 0; i < e_num; i++) {
		enemy[i].CreateEnemy();
	}
	CreateFin = TRUE;
}

bool Battle::CreateB_Fin() {
	return CreateFin;
}

void Battle::Menucreate() {
	command.CreateBox(5.0f, 0.0f, 0.9f, 135.0f, 200.0f, 0.6f, 0.6f, 0.6f, 0.7f, TRUE, TRUE);
}

void Battle::Cursor_h(int no) {

	static bool clr_f = TRUE;
	static float r = 1.0f;

	float m = tfloat.Add(0.002f);
	if (clr_f) {
		if ((r -= m) <= 0)clr_f = FALSE;
	}
	else {
		if ((r += m) >= 1.0f)clr_f = TRUE;
	}

	float x;
	if (no == 0)x = 10.0f;
	if (no == 1)x = 180.0f;
	if (no == 2)x = 360.0f;
	if (no == 3)x = 530.0f;

	//回復対象カーソル左
	h_select.InstancedSetConstBf(x - 10.0f, 440.0f, r, r, 0.7f, 1.0f, 5.0f, 120.0f);

	//回復対象カーソル右
	h_select.InstancedSetConstBf(x + 125.0f, 440.0f, r, r, 0.7f, 1.0f, 5.0f, 120.0f);

	//回復対象カーソル上
	h_select.InstancedSetConstBf(x - 5.0f, 440.0f, r, r, 0.7f, 1.0f, 130.0f, 5.0f);

	//回復対象カーソル下
	h_select.InstancedSetConstBf(x - 5.0f, 555.0f, r, r, 0.7f, 1.0f, 130.0f, 5.0f);
}

void Battle::Cursor_e(int select, float* pr, float* pb) {

	static float theta = 0.0f;
	static float r = 1.0f;
	static float b = 0.0f;
	static bool clr_f = TRUE;
	float m = tfloat.Add(0.02f);
	if (clr_f) {
		b += m;
		if ((r -= m) <= 0.0f)clr_f = FALSE;
	}
	else {
		b -= m;
		if ((r += m) >= 1.0f)clr_f = TRUE;
	}

	m = tfloat.Add(0.2f);
	if ((theta += m) > 360.0f)theta = 0.0f;
	*pr = r;
	*pb = b;
	E_select.Instancing({ e_pos[select].x, e_pos[select].y, e_pos[select].z }, { 0, 0,theta }, { 1,1,1 });
}

void Battle::SelectPermissionMove(Hero *hero) {

	if (h_draw[0].command_run == TRUE) {
		h_draw[0].command_run = FALSE;
		if (hero[1].Dieflg() == FALSE && h_draw[1].AGmeter > METER_MAX)h_draw[1].command_run = TRUE;
		else if (hero[2].Dieflg() == FALSE && h_draw[2].AGmeter > METER_MAX)h_draw[2].command_run = TRUE;
		else if (hero[3].Dieflg() == FALSE && h_draw[3].AGmeter > METER_MAX)h_draw[3].command_run = TRUE;
		else if (hero[0].Dieflg() == FALSE && h_draw[0].AGmeter > METER_MAX)h_draw[0].command_run = TRUE;
		else command_run_first_flg = FALSE;//誰も選択権無し
		return;
	}
	if (h_draw[1].command_run == TRUE) {
		h_draw[1].command_run = FALSE;
		if (hero[2].Dieflg() == FALSE && h_draw[2].AGmeter > METER_MAX)h_draw[2].command_run = TRUE;
		else if (hero[3].Dieflg() == FALSE && h_draw[3].AGmeter > METER_MAX)h_draw[3].command_run = TRUE;
		else if (hero[0].Dieflg() == FALSE && h_draw[0].AGmeter > METER_MAX)h_draw[0].command_run = TRUE;
		else if (hero[1].Dieflg() == FALSE && h_draw[1].AGmeter > METER_MAX)h_draw[1].command_run = TRUE;
		else command_run_first_flg = FALSE;
		return;
	}
	if (h_draw[2].command_run == TRUE) {
		h_draw[2].command_run = FALSE;
		if (hero[3].Dieflg() == FALSE && h_draw[3].AGmeter > METER_MAX)h_draw[3].command_run = TRUE;
		else if (hero[0].Dieflg() == FALSE && h_draw[0].AGmeter > METER_MAX)h_draw[0].command_run = TRUE;
		else if (hero[1].Dieflg() == FALSE && h_draw[1].AGmeter > METER_MAX)h_draw[1].command_run = TRUE;
		else if (hero[2].Dieflg() == FALSE && h_draw[2].AGmeter > METER_MAX)h_draw[2].command_run = TRUE;
		else command_run_first_flg = FALSE;
		return;
	}
	if (h_draw[3].command_run == TRUE) {
		h_draw[3].command_run = FALSE;
		if (hero[0].Dieflg() == FALSE && h_draw[0].AGmeter > METER_MAX)h_draw[0].command_run = TRUE;
		else if (hero[1].Dieflg() == FALSE && h_draw[1].AGmeter > METER_MAX)h_draw[1].command_run = TRUE;
		else if (hero[2].Dieflg() == FALSE && h_draw[2].AGmeter > METER_MAX)h_draw[2].command_run = TRUE;
		else if (hero[3].Dieflg() == FALSE && h_draw[3].AGmeter > METER_MAX)h_draw[3].command_run = TRUE;
		else command_run_first_flg = FALSE;
		return;
	}
}

Battle::~Battle() {
	dx->WaitFence();
	MovieSoundManager::ObjDelete_battle();
	ARR_DELETE(enemy);
	ARR_DELETE(e_draw);
}