#ifndef PLAYER
#define PLAYER

#include"jumpable.hpp"
#include"enemy.hpp"

#define PLAYER_WIDTH 77
#define PLAYER_HEIGHT 75
#define INITIAL_SPEED 500
#define LEFT_PLAYER "left.png"
#define RIGHT_PLAYER "right.png"

class Player
{
  public:
     Player(int yy,Window* window);
	 void update_location(bool& is_end_game,int screen_change);
	 int get_y();
	 int get_x();
	 int get_vy();
	 void change_horizonal_loc(bool a,bool& is_head_right,int screen_change);
	 void change_x();
	 void checking_range();
	 void horizonal_move( bool& is_head_right, int screen_change);
	 void set_jumpable(Jumpable* jumpable);
	 void set_enemy(Enemy* enemy);
	 int player_jump(Jumpable* jumpable);
	 bool is_on_jumpable();
	 bool does_hit_enemy();
	 void draw_player(bool is_head_right,int screen_change);
	 bool does_press_key(Event event);

  private:
     int y;
     int vx;
	 int vy;
	 int x;
	 Window* win;
	 Jumpable* jumpable_;
	 Enemy* enemy_;	
};
#endif