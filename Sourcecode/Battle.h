//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@	         Battle�N���X                                    **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Battle_Header
#define Class_Battle_Header

#include "MovieSoundManager.h"
#include "Position.h"
#include "../../Common/Direct3DWrapper/DxText.h"
#include "../../Common/Window/Control.h"
#include "Enum.h"

#define METER_MAX 25000
#define DrawYMAX -160
#define DrawYMIN -180

//�O���錾
class Map;
class Parameter;
class Hero;
class Enemy;
class EnemySide;
class EnemyBoss;
//�O���錾

class Battle {

private:
	//�����p�����[�^�[
	Hero* he_para;
	Position::E_Pos* e_po_para;
	Position::H_Pos* h_po_para;
	Encount encount_para;
	int no_para, e_nu_para;

	Dx12Process* dx;
	DxText* text;
	T_float tfloat;
	int comNo;
	PolygonData2D command, h_select;//�R�}���h�I���E�C���h�E, �񕜑I���J�[�\��
	PolygonData E_select;//�G�I���J�[�\��
	Enemy* enemy;
	EnemySide* enemyside;
	EnemyBoss* enemyboss;
	int e_num;            //�G�o����
	bool command_run_first_flg;//�R�}���h�I�������̏�� == FALSE�A�R�}���h�I����TRUE�ƂȂ�B1�̂���
	bool time_stop_flg;       //���ԃX�g�b�v�t���O, 1�A�̃A�N�V�����`HPMP������1�̂������Ȃ̂Ńt���O��1��ok
	float Escape_f;          //�G�X�P�[�v�\���t���O,���W
	bool Escape_s;          //�G�X�P�[�v����
	bool battlefirst;      //�J�n����̃J�����ړ������t���O
	bool CamActOn;        //
	int CamActInd;
	bool CreateFin;

	CommandSelect com_select;   //�R�}���h����(1���[�v�ɕt��1�̂��Ȃ̂�1��ok�������l)
	bool          comDraw;
	CommandSelect E_com_select;//�G�p(�e���v���[�g�֐��g�p�̈׈���������)
	MagicSelect MAG_select;   //�I�������}�W�b�N
	MagicSelect E_MAG_select;//�G�p
	int select_obj;           //�I��Ώ۔ԍ��G,�v���C���[����,�G�t�F�N�g�I��(4==�S��)
	int E_select_obj;        //�G�p(�G�t�F�N�g�I��p)(4==�S��)

	typedef struct {
		float AGmeter;        //���[�^�[
		Action action;
		MagicSelect Magrun;//�I���}�W�b�N
		float RCVdrawY;     //�񕜕\���t���O,�\�����W
		int RCVdata;       //�񕜐��ۊ�
		bool Recov_f;      //���J�o���[�J�n�t���O�I����FALSE
		float DMdrawY;    //�_���[�W�\���t���O,�\�����WY�̏㉺��
		int DMdata;      //�_���[�W���ۊ�
		float draw_x;   //�����\���ʒu
		float draw_y;   //�����\���ʒu
		bool command_run;  //�R�}���h�A�N�Z�X�����,�U���X�^�[�g�t���O
		bool LOST_fin;    //LOST�A�N�V�����I���t���O
		//���R�}���h����
		MenuSelect manu;//���I�����j���[(�ȉ��R�}���h�I���֘A)
		int M_select;    //���C�����j���[�J�[�\���ʒu
		int A_select;   //�U���ΏۃJ�[�\���ʒu
		int MA_select; //�}�W�b�N�ΏۃJ�[�\���ʒu
		int R_select; //�񕜑ΏۃJ�[�\���ʒu 
	}Draw;
	Draw h_draw[4];
	Draw* e_draw;
	Position::E_Pos e_pos[4];
	Position::H_Pos h_pos;
	Position::Bt_H_Pos* b_pos;

	void Debug(Enemy* enemy);//�f�o�b�N�p
	void Menucreate();
	void Cursor_h(int no);
	void Cursor_e(int select, float* r, float* b);
	void SelectPermissionMove(Hero* hero);
	CommandSelect E_AT_select(Hero* hero);
	CommandSelect Menu_main(Hero* hero, int i, Directionkey direction);
	CommandSelect Menu_AT(int i, Directionkey direction);
	CommandSelect Menu_MAG_AT(int i, Directionkey direction);
	CommandSelect Menu_RCV(Hero* hero, int i, Directionkey direction);
	CommandSelect H_AT_select(Hero* hero, int i, Directionkey direction);
	bool Escapedraw();
	void E_drawPos(int i);
	void H_drawPos(int i);

	template<typename T_rcv>
	void ValueDraw(T_rcv* rcv, Draw* dm, Draw* rc, int dmI, int rcI) {
		//���U���Ώۖ��͉񕜑Ώۂ̃A�N�V����,�f�[�^�����J�n
		bool draw_flg = FALSE;
		for (int i1 = 0; i1 < dmI; i1++) {
			if (dm[i1].DMdata >= 0) {
				dm[i1].action = DAMAGE;//�Ώۂ��_���[�W�󂯂��̂ł��̌�_���[�W����
				dm[i1].DMdrawY = DrawYMIN;//�p�����[�^�[�ԈႦ��Ɛ����\������Ȃ��̂Œ���,�t���O�I��
				draw_flg = TRUE;
			}
		}
		for (int i1 = 0; i1 < rcI; i1++) {
			if (rc[i1].RCVdata >= 0) {
				if (rc[i1].Recov_f == TRUE) {
					rcv[i1].Dieflg(FALSE);
					rc[i1].action = RECOVER;//�Ώۂ����J�o���[���󂯂��̂ł��̌㏈��
					rc[i1].LOST_fin = FALSE;
				}
				else {
					rc[i1].action = rcv[i1].Normal_act_get();//�Ώۂ��񕜎󂯂��̂ł��̌�񕜏���(�񕜗pact�͖���)
				}
				rc[i1].RCVdrawY = DrawYMIN;//�p�����[�^�[�ԈႦ��Ɛ����\������Ȃ��̂Œ���,�t���O�I��
				rc[i1].Recov_f = FALSE;//action��RECOVER�����������_�ł����p����,����������ł��Ȃ��ꍇ��HEAL�ɒǉ��K�v
				draw_flg = TRUE;
			}
		}
		if (draw_flg == FALSE)time_stop_flg = FALSE;//�����s�������Ȃ̂ŃX�g�b�v����(MP����Ȃ��ꍇ)
	}

	template<typename T_dm, typename T_att>
	void ATprocess(T_dm* dm, T_att* att, Draw* d, Draw* at) {
		if (dm->Dieflg() == FALSE) {
			at->action = ATTACK;
			d->DMdata = att->GetAttack();
			time_stop_flg = TRUE;
		}
		at->AGmeter = 0;
	}

	template<typename T_dm, typename T_att>
	void MAGprocess(T_dm* p_dm, T_att* p_att, T_att* att, Draw* at, Draw* p_at, Draw* p_d, int* select_ob, MagicSelect* select_M, TemplateType type) {
		int att_n;
		int dm_n;
		if (type == E_ATT) {
			att_n = e_num;
			dm_n = 4;
		}
		if (type == H_ATT) {
			att_n = 4;
			dm_n = e_num;
		}
		int cnt = 0;
		at->action = MAGIC;
		//MP����
		if (att->DownMP(*select_M) == FALSE) {
			//MP����Ȃ��ꍇ�̏���
			at->AGmeter = 0;
			at->Magrun = NOSEL;
			time_stop_flg = TRUE;
			return;
		}
		//�}�W�b�N�U������
		switch (*select_M) {
		case FLAME:
			att->s_Fp(1);
			//�S�̍U��
			if (*select_ob == 4) {
				for (int i = 0; i < dm_n; i++) {
					if (p_dm[i].Dieflg() == FALSE)cnt++;//�Ώېl���Ń_���[�W�ω�
				}
				for (int i = 0; i < dm_n; i++) {
					if (p_dm[i].Dieflg() == TRUE)continue;
					p_d[i].DMdata = att->GetMagic(FLAME, cnt);
				}
			}
			else {
				//�P�̍U��
				if (p_dm[*select_ob].Dieflg() == TRUE) {
					//�I����U�����O�ɑΏ�LOST�̏ꍇ�̏���
					at->action = NORMAL; at->AGmeter = 0; return;
				}
				p_d[*select_ob].DMdata = att->GetMagic(FLAME, 1);
			}
			break;
		case HEAL:
			att->s_Hp(1);
			//�S�̍U��
			if (*select_ob == 4) {
				for (int i = 0; i < att_n; i++) {
					if (p_att[i].Dieflg() == FALSE)cnt++;
				}
				for (int i = 0; i < att_n; i++) {
					if (p_att[i].Dieflg() == TRUE)continue;
					p_at[i].RCVdata = att->GetMagic(HEAL, cnt);
				}
			}
			else {
				//�P�̍U��
				if (p_att[*select_ob].Dieflg() == TRUE) {
					//�I����񕜒��O�ɑΏ�LOST�̏ꍇ�̏���
					at->action = NORMAL;  at->AGmeter = 0; return;
				}
				p_at[*select_ob].RCVdata = att->GetMagic(HEAL, 1);
			}
			break;
		case RECOV:
			att->s_Rp(1);
			//�퓬�s�\�ґI��:����  ����ȊO�I��:����
			p_at[*select_ob].RCVdata = att->GetMagic(RECOV, 1);
			if (p_att[*select_ob].Dieflg() == TRUE) {
				p_at[*select_ob].Recov_f = TRUE;//���J�o���[�t���O
			}
			break;
		}
		at->AGmeter = 0;
		time_stop_flg = TRUE;
	}

	template<typename T_rcv>
	void RCVdraw(T_rcv* rcv, Draw* at, float adjustX, float adjustY) {
		if (at->RCVdrawY != 0) {
			if ((at->RCVdrawY += tfloat.Add(0.1f)) < DrawYMAX) {
				text->UpDateValue(at->RCVdata, at->draw_x + adjustX, at->draw_y + at->RCVdrawY + adjustY, 30.0f, 5, { 0.3f, 1.0f, 0.3f, 1.0f });
			}
			else {
				if (at->RCVdrawY >= DrawYMAX && at->RCVdrawY < DrawYMAX + 20.0f) {
					text->UpDateValue(at->RCVdata, at->draw_x + adjustX, at->draw_y + DrawYMAX + adjustY, 30.0f, 5, { 0.3f, 1.0f, 0.3f, 1.0f });
				}
				else {
					rcv->UpHP(at->RCVdata);
					at->RCVdrawY = 0;//�����ŏ��������Ȃ��Ɛ��l�\��������ςȂ�
					time_stop_flg = FALSE;
				}
			}
		}
	}

	template<typename T_dm>
	void DMdraw(T_dm* dm, Draw* d, float adjustX, float adjustY) {
		if (d->DMdrawY != 0) {
			if ((d->DMdrawY += tfloat.Add(0.1f)) < DrawYMAX) {
				text->UpDateValue(d->DMdata, d->draw_x + adjustX, d->draw_y + d->DMdrawY + adjustY, 30.0f, 5, { 1.0f, 0.3f, 0.3f, 1.0f });
			}
			else {
				if (d->DMdrawY >= DrawYMAX && d->DMdrawY < DrawYMAX + 20.0f) {
					text->UpDateValue(d->DMdata, d->draw_x + adjustX, d->draw_y + DrawYMAX + adjustY, 30.0f, 5, { 1.0f, 0.3f, 0.3f, 1.0f });
				}
				else {
					dm->DownHP(d->DMdata);
					d->DMdrawY = 0;//�����ŏ��������Ȃ��Ɛ��l�\��������ςȂ�
					time_stop_flg = FALSE;
				}
			}
		}
	}

public:
	Battle() {}
	void SetParameter(Hero* he, Position::E_Pos* e_pos, Position::H_Pos* h_pos, Encount encount, int no, int e_nu);
	void Init();
	void SetVertex();
	void SetCommandList(int com_no);
	void CreateBattle();
	bool CreateB_Fin();
	Result FightUpdate(Hero* he, Directionkey direction, Result result);
	void FightDraw(Encount enc);
	void StreamOutput(Encount enc);
	void StreamOutputAfterDraw(Encount enc);
	void Draw2D(Encount enc);
	Position::Bt_H_Pos* GetBtPos(Position::H_Pos* h_p);
	bool GetH_DM(int element);
	bool GetH_RCV(int element);
	bool GetE_DM(int element);
	bool GetE_RCV(int element);
	bool GetBossEffectState(float* blur);
	~Battle();

	ParameterDXR** getParameterDXR(int* numPara);
	void setPointLightNo();
};

#endif