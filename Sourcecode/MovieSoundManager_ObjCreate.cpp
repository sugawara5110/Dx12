//*****************************************************************************************//
//**                                                                                     **//
//**                   Å@Å@Å@    MovieSoundManagerÉNÉâÉX                                 **//
//**                                ObjCreateä÷êî                                        **//
//*****************************************************************************************//

#include "MovieSoundManager.h"

Movie *MovieSoundManager::mo;
Movie *MovieSoundManager::f_wall;
Sound_ *MovieSoundManager::dungeon_so[5];
Sound_ *MovieSoundManager::rain_so;
Sound_ *MovieSoundManager::enemy_so[2];
Sound_ *MovieSoundManager::title_so;
Sound_ *MovieSoundManager::die_so;
Sound_ *MovieSoundManager::att_so;
Sound_ *MovieSoundManager::flame_so;
Sound_ *MovieSoundManager::heal_so;
Sound_ *MovieSoundManager::magic_so;
Sound_ *MovieSoundManager::select_so;
Sound_ *MovieSoundManager::enter_so;
Sound_ *MovieSoundManager::ending_so;
Sound_ *MovieSoundManager::bosslost_so;
int MovieSoundManager::map_n;

void MovieSoundManager::ObjInit() {

	Dx12Process::Lock();
	mo = new Movie();
	f_wall = new Movie();
	enemy_so[0] = new Sound_();//éGãõìGÇÕÇ∏Ç¡Ç∆ï€éùÇµÇ¡ÇœÇ»Çµ
	att_so = new Sound_();
	flame_so = new Sound_();
	heal_so = new Sound_();
	magic_so = new Sound_();
	select_so = new Sound_();
	enter_so = new Sound_();
	Dx12Process::Unlock();

	new(mo) Movie("./dat/movie/torch.da");
	new(f_wall) Movie("./dat/movie/f_wall.da");
	for (int i = 0; i < 5; i++)dungeon_so[i] = NULL;
	rain_so = NULL;
	new(enemy_so[0]) Sound_("./dat/movie/side_enemy.da");//éGãõìGÇÕÇ∏Ç¡Ç∆ï€éùÇµÇ¡ÇœÇ»Çµ
	enemy_so[1] = NULL;
	bosslost_so = NULL;
	title_so = NULL;
	die_so = NULL;
	ending_so = NULL;
	new(att_so) Sound_("./dat/movie/att.da");
	new(flame_so) Sound_("./dat/movie/flame.da");
	new(heal_so) Sound_("./dat/movie/heal.da");
	new(magic_so) Sound_("./dat/movie/magic.da");
	new(select_so) Sound_("./dat/movie/select.da");
	new(enter_so) Sound_("./dat/movie/enter.da");
	map_n = 0;
}

void MovieSoundManager::ObjCreate_title() {

	if (title_so == NULL) {
		Dx12Process::Lock();
		title_so = new Sound_();
		Dx12Process::Unlock();
		new(title_so) Sound_("./dat/movie/title.da");
	}
}

void MovieSoundManager::ObjDelete_title(){
	S_DELETE(title_so)
}

void MovieSoundManager::ObjCreate_map(int map_no) {
	map_n = map_no;
	switch (map_no) {
	case 0:
		if (dungeon_so[0] == NULL) {
			Dx12Process::Lock();
			dungeon_so[0] = new Sound_();
			Dx12Process::Unlock();
			new(dungeon_so[0]) Sound_("./dat/movie/dungeon0.da");
		}
		break;
	case 1:
		if (dungeon_so[1] == NULL) {
			Dx12Process::Lock();
			dungeon_so[1] = new Sound_();
			Dx12Process::Unlock();
			new(dungeon_so[1]) Sound_("./dat/movie/dungeon1.da");
		}
		if (rain_so == NULL) {
			Dx12Process::Lock();
			rain_so = new Sound_();
			Dx12Process::Unlock();
			new(rain_so) Sound_("./dat/movie/rain.da");
		}
		break;
	case 2:
		if (dungeon_so[2] == NULL) {
			Dx12Process::Lock();
			dungeon_so[2] = new Sound_();
			Dx12Process::Unlock();
			new(dungeon_so[2]) Sound_("./dat/movie/dungeon2.da");
		}
		break;
	case 3:
		if (dungeon_so[3] == NULL) {
			Dx12Process::Lock();
			dungeon_so[3] = new Sound_();
			Dx12Process::Unlock();
			new(dungeon_so[3]) Sound_("./dat/movie/dungeon3.da");
		}
		break;
	case 4:
		if (dungeon_so[4] == NULL) {
			Dx12Process::Lock();
			dungeon_so[4] = new Sound_();
			Dx12Process::Unlock();
			new(dungeon_so[4]) Sound_("./dat/movie/dungeon4.da");
		}
		break;
	}
}

void MovieSoundManager::ObjChange_map(){
	for (int i = 0; i < 5; i++){
		if (map_n != i)S_DELETE(dungeon_so[i]);
	}
	if (map_n != 1 && rain_so != NULL)S_DELETE(rain_so);
}

void MovieSoundManager::ObjDelete_map(){
	for (int i = 0; i < 5; i++){
		S_DELETE(dungeon_so[i]);
	}
	S_DELETE(rain_so);
}

void MovieSoundManager::ObjCreate_battle(int n) {

	if (die_so == NULL) {
		Dx12Process::Lock();
		die_so = new Sound_();
		Dx12Process::Unlock();
		new(die_so) Sound_("./dat/movie/die.da");
	}
	if (n == 1 && enemy_so[1] == NULL) {
		Dx12Process::Lock();
		enemy_so[1] = new Sound_();
		Dx12Process::Unlock();
		new(enemy_so[1]) Sound_("./dat/movie/boss_enemy.da");
	}
	if (n == 2 && enemy_so[1] == NULL) {
		Dx12Process::Lock();
		enemy_so[1] = new Sound_();
		Dx12Process::Unlock();
		new(enemy_so[1]) Sound_("./dat/movie/boss_enemy2.da");
	}
	if (n == 3 && enemy_so[1] == NULL) {
		Dx12Process::Lock();
		enemy_so[1] = new Sound_();
		Dx12Process::Unlock();
		new(enemy_so[1]) Sound_("./dat/movie/lastboss.da");
	}
	if (n > 0 && bosslost_so == NULL) {
		Dx12Process::Lock();
		bosslost_so = new Sound_();
		Dx12Process::Unlock();
		new(bosslost_so) Sound_("./dat/movie/bosslost.da");
	}
}

void MovieSoundManager::ObjDelete_battle(){
	Enemy_soundoff();
	S_DELETE(die_so);
	S_DELETE(enemy_so[1]);
	S_DELETE(bosslost_so);
}

void MovieSoundManager::ObjCreate_ending() {

	if (ending_so == NULL) {
		Dx12Process::Lock();
		ending_so = new Sound_();
		Dx12Process::Unlock();
		new(ending_so) Sound_("./dat/movie/ending.da");
	}
}

void MovieSoundManager::ObjDelete_ending(){
	S_DELETE(ending_so)
}

void MovieSoundManager::ObjDelete(){

	ObjDelete_title();
	ObjDelete_map();
	ObjDelete_battle();
	ObjDelete_ending();

	S_DELETE(enemy_so[0]);
	S_DELETE(mo);
	S_DELETE(f_wall);
	S_DELETE(att_so);
	S_DELETE(flame_so);
	S_DELETE(heal_so);
	S_DELETE(magic_so);
	S_DELETE(select_so);
	S_DELETE(enter_so);
}

int **MovieSoundManager::Torch_GetFrame(int width, int height){
	return mo->GetFrame(width, height);
}

int **MovieSoundManager::FireWall_GetFrame(int width, int height){
	return f_wall->GetFrame(width, height);
}

void MovieSoundManager::Dungeon_sound(bool repeat, int map){
	dungeon_so[map]->sound(repeat, -1000);
}

void MovieSoundManager::Dungeon_soundoff(int map){
	dungeon_so[map]->soundoff();
}

void MovieSoundManager::Rain_sound(bool repeat){
	rain_so->sound(repeat, -1200);
}

void MovieSoundManager::Rain_soundoff(){
	rain_so->soundoff();
}

void MovieSoundManager::Enemy_sound(bool repeat){
	if (enemy_so[1] != NULL)enemy_so[1]->sound(repeat, -1000);
	if (enemy_so[1] == NULL && enemy_so[0] != NULL)enemy_so[0]->sound(repeat, -1000);
}

void MovieSoundManager::Enemy_soundoff(){
	if (enemy_so[1] != NULL)enemy_so[1]->soundoff();
	if (enemy_so[1] == NULL && enemy_so[0] != NULL)enemy_so[0]->soundoff();
}

void MovieSoundManager::Bosslost_sound(bool repeat){
	bosslost_so->soundloop(repeat, -700, 5, 30);
}

void MovieSoundManager::Title_sound(bool repeat){
	title_so->sound(repeat, -1000);
}

void MovieSoundManager::Title_soundoff(){
	title_so->soundoff();
}

void MovieSoundManager::Ending_sound(bool repeat){
	ending_so->sound(repeat, -1000);
}

void MovieSoundManager::Die_sound(bool repeat){
	die_so->sound(repeat, -1000);
}

void MovieSoundManager::Die_soundoff(){
	die_so->soundoff();
}

void MovieSoundManager::Att_sound(bool repeat){
	att_so->sound(repeat, -700);
}

void MovieSoundManager::Flame_sound(bool repeat){
	flame_so->sound(repeat, -700);
}

void MovieSoundManager::Heal_sound(bool repeat){
	heal_so->sound(repeat, -700);
}

void MovieSoundManager::Magic_sound(bool repeat){
	magic_so->sound(repeat, -500);
}

void MovieSoundManager::Select_sound(bool repeat){
	select_so->sound(repeat, -500);
}

void MovieSoundManager::Enter_sound(bool repeat){
	enter_so->sound(repeat, -500);
}
