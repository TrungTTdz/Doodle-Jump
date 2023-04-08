#ifndef GAME
#define GAME

#include"player.hpp"
#include<vector>
#include<fstream>

#define BACKGROUND "background.png"
#define GHOST_LAND "ghost_land.png"
#define GHOST_BCKGROUND "ghost_bck.png"
#define DARKNESS "darkness.png"
#define LOST "GAME OVER :("
#define TEXT_FILE "OpenSans.ttf"
#define INPUT_FILE "map.txt"

class Game
{
  public:
    void set_win_player(Window* window,Player* player_);
    void begin_game();
	void read_input();
	void draw_objects(bool is_head_right, int& screen_change);
	void find_jumpable();
	void make_plat_move();
	void ending_of_game();
	void hit_enemy(bool& is_end_game);
	void playing_game(bool& is_head_right,int& screen_change,bool& is_end_game);
	void changing_screen(int& screen_change);
	void clear_unwanted();
  private:
    Window* win;
	Player* player;
	std::vector<Jumpable*> jumpables;
	std::vector<Enemy*> enemies;
	int distance;
	int max_score;
};
#endif