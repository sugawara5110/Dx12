//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　    MovieSoundManagerクラス                                 **//
//**                                ObjCreate関数                                        **//
//*****************************************************************************************//

#include "MovieSoundManager.h"

MovieSoundManager::Movie *MovieSoundManager::mo;
MovieSoundManager::Movie *MovieSoundManager::f_wall;
MovieSoundManager::Sound_ *MovieSoundManager::dungeon_so[5];
MovieSoundManager::Sound_ *MovieSoundManager::rain_so;
MovieSoundManager::Sound_ *MovieSoundManager::enemy_so[2];
MovieSoundManager::Sound_ *MovieSoundManager::title_so;
MovieSoundManager::Sound_ *MovieSoundManager::die_so;
MovieSoundManager::Sound_ *MovieSoundManager::att_so;
MovieSoundManager::Sound_ *MovieSoundManager::flame_so;
MovieSoundManager::Sound_ *MovieSoundManager::heal_so;
MovieSoundManager::Sound_ *MovieSoundManager::magic_so;
MovieSoundManager::Sound_ *MovieSoundManager::select_so;
MovieSoundManager::Sound_ *MovieSoundManager::enter_so;
MovieSoundManager::Sound_ *MovieSoundManager::ending_so;
MovieSoundManager::Sound_ *MovieSoundManager::bosslost_so;
int MovieSoundManager::map_n;

MovieSoundManager::MovieSoundManager(){}

void MovieSoundManager::ObjInit(){

	mo = new Movie(0);
	f_wall = new Movie(1);
	for (int i = 0; i < 5; i++)dungeon_so[i] = NULL;
	rain_so = NULL;
	enemy_so[0] = new Sound_(31);//雑魚敵はずっと保持しっぱなし
	enemy_so[1] = NULL;
	bosslost_so = NULL;
	title_so = NULL;
	die_so = NULL;
	ending_so = NULL;
	att_so = new Sound_(0);
	flame_so = new Sound_(1);
	heal_so = new Sound_(2);
	magic_so = new Sound_(3);
	select_so = new Sound_(4);
	enter_so = new Sound_(5);
	map_n = 0;
}

void MovieSoundManager::ObjCreate_title(){

	if (title_so == NULL)title_so = new Sound_(20);
}

void MovieSoundManager::ObjDelete_title(){
	S_DELETE(title_so)
}

void MovieSoundManager::ObjCreate_map(int map_no){
	map_n = map_no;
	switch (map_no){
	case 0:
		if (dungeon_so[0] == NULL)dungeon_so[0] = new Sound_(21);
		break;
	case 1:
		if (dungeon_so[1] == NULL)dungeon_so[1] = new Sound_(22);
		if (rain_so == NULL)rain_so = new Sound_(23);
		break;
	case 2:
		if (dungeon_so[2] == NULL)dungeon_so[2] = new Sound_(24);
		break;
	case 3:
		if (dungeon_so[3] == NULL)dungeon_so[3] = new Sound_(25);
		break;
	case 4:
		if (dungeon_so[4] == NULL)dungeon_so[4] = new Sound_(26);
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

void MovieSoundManager::ObjCreate_battle(int n){

	if (die_so == NULL)die_so = new Sound_(30);
	if (n == 1 && enemy_so[1] == NULL)enemy_so[1] = new Sound_(32);
	if (n == 2 && enemy_so[1] == NULL)enemy_so[1] = new Sound_(33);
	if (n == 3 && enemy_so[1] == NULL)enemy_so[1] = new Sound_(34);
	if (n > 0 && bosslost_so == NULL)bosslost_so = new Sound_(36);
}

void MovieSoundManager::ObjDelete_battle(){
	Enemy_soundoff();
	S_DELETE(die_so);
	S_DELETE(enemy_so[1]);
	S_DELETE(bosslost_so);
}

void MovieSoundManager::ObjCreate_ending(){

	if (ending_so == NULL)ending_so = new Sound_(35);
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
