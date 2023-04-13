#include"game.hpp"
using namespace std;

void Game::set_win_player(Window* window, Player* player_)
{
	win = window;
	player = player_;
	distance = 0;
	max_score = 0;
}



void Game::make_plat_move()
{
	for (int i = 0; i < jumpables.size(); i++)
        jumpables[i]->move_platform();

}

void Game::find_jumpable()
{
	for(int i=0; i<jumpables.size(); i++)
	{
		if(jumpables[i]->get_type()=="spring")
		  if(player->player_jump(jumpables[i])==2)
		     return;
	}
    for(int i=0; i<jumpables.size(); i++)
	{
		if(jumpables[i]->get_type()!="spring")
		{
			int answer=player->player_jump(jumpables[i]);
		    if(answer == -1)
		       continue;
		    else if(answer == 0)
		    {
			  jumpables[i]->change_is_broken();
			  break;
		    }
		    else
		      break;
		}
	}
}

void Game::hit_enemy(bool& is_end_game)
{
    for(int i=0; i<enemies.size(); i++)
	{
		player->set_enemy(enemies[i]);
		if(player->does_hit_enemy())
		{
			is_end_game = true;
			return;
		}
	}
}
void Game::clear_unwanted()
{
	for(int i=0; i< jumpables.size(); i++)
	  delete jumpables[i];
	for(int i=0; i< enemies.size(); i++)
	  delete enemies[i];
	delete player;
	delete win;
}
void Game::ending_of_game()
{
    while(true)
	{
	  win->clear();
	  win->draw_img(GHOST_BCKGROUND);
	  win->draw_img(DARKNESS,Rectangle(0,110,400,400));
	  win->draw_img(GHOST_LAND,Rectangle(0,510,400,130));
	  win->show_text(LOST, Point(5, 30), WHITE, TEXT_FILE, 60);
	  win->show_text("score:", Point(20, 130), WHITE, TEXT_FILE, 30);
      win->show_text(to_string(max_score), Point(110, 120),RED, TEXT_FILE, 40);
	  win->update_screen();
	  if(win->has_pending_event())
	  {
		Event e= win->poll_for_event();
	    if(e.get_type() == Event::KEY_PRESS )
	      break;
	  }
	}
    clear_unwanted();
}
void Game::playing_game(bool& is_head_right,int& screen_change,bool& is_end_game)
{
	 win->clear();
	 draw_objects(is_head_right, screen_change);
	 hit_enemy(is_end_game);
	 find_jumpable();
	 player->horizonal_move(is_head_right, screen_change);
     player->update_location(is_end_game,screen_change);
	 make_plat_move();
	 if(player->get_y()-PLAYER_HEIGHT>max_score)
	     max_score = player->get_y()-PLAYER_HEIGHT;
	 win->show_text(to_string(max_score), Point(40, 10), BLACK, TEXT_FILE, 30);
	 win->update_screen();
	 delay(20);
}
void Game::begin_game()
{
    Window* window = new Window(WINDOW_WIDTH,WINDOW_HEIGHT,"DOODLE JUMP");
    Player* player_= new Player(PLAYER_HEIGHT,window);
	set_win_player(window,player_);
	bool is_head_right = true;int screen_change = 0;bool is_end_game = false;
	while(!is_end_game)
	   playing_game(is_head_right,screen_change,is_end_game);

}
void Game::changing_screen(int& screen_change)
{
	if((player->get_y()-PLAYER_HEIGHT-screen_change)>(WINDOW_HEIGHT/2))
	{
		distance = player->get_y()-PLAYER_HEIGHT - screen_change - (WINDOW_HEIGHT/2);
		screen_change = screen_change + distance;
	}
}
void Game::draw_objects( bool is_head_right, int& screen_change)
{
	win->draw_img(BACKGROUND);
	changing_screen(screen_change);
	for (int i = 0; i < jumpables.size(); i++)
	{
		if(jumpables[i]->get_y()+(jumpables[i]->get_height())-screen_change < 0 )
		  jumpables.erase(jumpables.begin()+i);
		else
          jumpables[i]->draw_jumpable(win,screen_change);
	}
	for (int i = 0; i < enemies.size(); i++)
	{
		if(enemies[i]->get_y()+(ENEMY_HEIGHT)-screen_change < 0 )
		  enemies.erase(enemies.begin()+i);
		else
          enemies[i]->draw_enemy(win,screen_change);
	}
	player->draw_player(is_head_right,screen_change);
}
void Game::read_input()
{
	int entity_count,xx,yy;
	string type_;
	ifstream input;
	input.open(INPUT_FILE);
	input >> entity_count;
	for (int i = 0; i < entity_count; i++)
	{
		input >> xx >> yy >> type_;
		if(type_=="platform")
		  jumpables.push_back(new Jumpable(((5*xx)/8)-(PLAT_WIDTH/2),(5*yy)/8,"platform",PLAT_WIDTH,PLAT_HEIGHT));
		else if(type_=="mplatform")
		  jumpables.push_back(new Jumpable(((5*xx)/8)-(PLAT_WIDTH/2),(5*yy)/8,"mplatform",PLAT_WIDTH,PLAT_HEIGHT));
        else if(type_=="bplatform")
		  jumpables.push_back(new Jumpable(((5*xx)/8)-(PLAT_WIDTH/2),(5*yy)/8,"bplatform",PLAT_WIDTH,PLAT_HEIGHT));
	    else if(type_=="spring")
		  jumpables.push_back(new Jumpable(((5*xx)/8)-(SPRING_WIDTH/2),(5*yy)/8,"spring",SPRING_WIDTH,SPRING_HEIGHT));
		else if(type_=="enemy")
		  enemies.push_back(new Enemy(((5*xx)/8)-(ENEMY_WIDTH/2),(5*yy)/8));
	}
	input.close();
}
Player::Player(int yy, Window* window)
{
	y=yy;
	vy=INITIAL_SPEED;
	vx=0;
	x=(WINDOW_WIDTH/2)-(PLAYER_WIDTH/2);
    win = window;
}
void Player::draw_player(bool is_head_right,int screen_change)
{
	if(is_head_right)
	   win->draw_img(RIGHT_PLAYER,Rectangle(x,WINDOW_HEIGHT-(y-screen_change ),PLAYER_WIDTH,PLAYER_HEIGHT));
	else
	   win->draw_img(LEFT_PLAYER,Rectangle(x,WINDOW_HEIGHT-(y- screen_change),PLAYER_WIDTH,PLAYER_HEIGHT));
}
void Player::checking_range()
{
	x=x+vx;
	if(x>WINDOW_WIDTH)
	  x=0;
	if(x<0)
	  x=WINDOW_WIDTH;
}
bool Player::does_press_key(Event event)
{
	if(event.get_type() == Event::QUIT)
      exit(0);
    if(event.get_type() == Event::KEY_PRESS )
	   return true;
	if(event.get_type() == Event::KEY_RELEASE)
	{
	  vx=0;
	  return false;
	}
	else
	   return false;
}
void Player::horizonal_move(bool& is_head_right, int screen_change)
{
   while(win->has_pending_event())
    {
	  char pressed_char;Event event= win->poll_for_event();
	  bool is_held = does_press_key(event);
	  if(is_held)
	  {
       pressed_char = event.get_pressed_key();
       if (pressed_char == 'd')
		 change_horizonal_loc(1,is_head_right,screen_change);
       if(pressed_char == 'a')
		change_horizonal_loc(0,is_head_right,screen_change);
	  }
	}
	checking_range();
}
void Player::change_horizonal_loc(bool a,bool& is_head_right,int screen_change)
{
	if(a)
	{
	 is_head_right=true;
	 vx = 7;
	 win->draw_img(RIGHT_PLAYER,Rectangle(x,WINDOW_HEIGHT-(y- screen_change ),PLAYER_WIDTH,PLAYER_HEIGHT));
	}
	else
	{
	  is_head_right=false;
	  vx = -7;
	  win->draw_img(LEFT_PLAYER,Rectangle(x,WINDOW_HEIGHT-(y- screen_change ),PLAYER_WIDTH,PLAYER_HEIGHT));
	}
}
void Player::update_location(bool& is_end_game,int screen_change)
{
	y = y+(vy*0.02);
	vy = vy-(1000*0.02);
	if(y-screen_change<0)
	  is_end_game = true;
}

bool Player::is_on_jumpable()
{
  if(((vy==0) || vy<0)
	&&(x<(jumpable_->get_x())+(jumpable_->get_width())-25)
	&&((x+(PLAYER_WIDTH-25))>(jumpable_->get_x()))
	&&((y-PLAYER_HEIGHT)>jumpable_->get_y())
	&&(((y-PLAYER_HEIGHT)<((jumpable_->get_y()) + (jumpable_->get_height())))))
	return true;
  else
    return false;
}
int Player::player_jump(Jumpable* jumpable)
{
	set_jumpable(jumpable);
	if(is_on_jumpable())
	{
		if(jumpable_->get_type()!="bplatform" && jumpable_->get_type()!="spring")
		{
			vy=INITIAL_SPEED;
			return 1;
		}
		else if(jumpable_->get_type()=="spring")
		{
           vy= 2*INITIAL_SPEED;
		   return 2;
		}
		else
		   return 0;
	}
	else
	  return -1;
}
bool Player::does_hit_enemy()
{
	if(x<(enemy_->get_x())+(ENEMY_WIDTH)
	&&((x+(PLAYER_WIDTH))>(enemy_->get_x()))
	&&(y>enemy_->get_y()+20)
	&&(((y-PLAYER_HEIGHT)<((enemy_->get_y()) + ENEMY_HEIGHT))))
	  return true;
	return false;
}
void Player::set_jumpable(Jumpable* jumpable)
{
	jumpable_= jumpable;
}
void Player::set_enemy(Enemy* enemy)
{
	enemy_ = enemy;
}
int Player::get_y()
{
	return y;
}
int Player::get_x()
{
	 return x;
}
int Player::get_vy()
{
	return vy;
}
