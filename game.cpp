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