#include "runner.h"
#include "face.h"
#include "controller.cpp"


#include <limits.h>
#include <math.h>
#include <chrono>
#include <cassert>
#include <map>
#include <deque>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

extern std::chrono::system_clock::time_point* controller1buttons;
extern std::chrono::system_clock::time_point* controller2buttons;


int getRandInt()
{
  /* Return a random integer, based off of millisec instead of
  second.
  */
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  srand((time_t)ts.tv_nsec);

  return rand();
}

void singleP_switchFood(std::deque<IntTuple*> &snake1, std::deque<IntTuple*> &food, int x, int y)
{
  /* Reassign one of the food blips, make sure that it is not
  already true on the board.
  */

  for(int i = 0; i < food.size(); i++)
  {
    if(x == food[i]->get_x() && y == food[i]->get_y())
    {

      int new_x;
      int new_y;

      bool repeat = false;
      while(!repeat)
      {

        repeat = true;

        int xrand = getRandInt() % 31;
        int yrand = getRandInt() % 127;

        xrand = xrand - (xrand % 2);
        yrand = yrand - (yrand % 2);

        for(int i = 0; i < snake1.size(); i++)
        {
          if(snake1[i]->get_x() == xrand && snake1[i]->get_y() == yrand)
          {
            repeat = false;
          }
        }

        for(int i = 0; i < food.size(); i++)
        {
          if(food[i]->get_x() == xrand && food[i]->get_y() == yrand)
          {
            repeat = false;
          }
        }

        new_x = xrand;
        new_y = yrand;

      }

      food[i]->set_x(new_x);
      food[i]->set_y(new_y);

    }
  }

  return;

}


void multiP_switchFood(std::deque<IntTuple*> &snake1, std::deque<IntTuple*> &snake2, std::deque<IntTuple*> &food, int p1x, int p1y, int p2x, int p2y)
{
  /* Reassign one of the food blips, make sure that it is not
  already true on the board. Accounts for 2 snakes.
  */

  for(int i = 0; i < food.size(); i++)
  {
    if((p1x == food[i]->get_x() && p1y == food[i]->get_y()) || (p2x == food[i]->get_x() && p2y == food[i]->get_y()))
    {

      int new_x;
      int new_y;

      bool repeat = false;
      while(!repeat)
      {

        repeat = true;

        int xrand = getRandInt() % 31;
        int yrand = getRandInt() % 127;

        xrand = xrand - (xrand % 2);
        yrand = yrand - (yrand % 2);

        for(int i = 0; i < snake1.size(); i++)
        {
          if((snake1[i]->get_x() == xrand && snake1[i]->get_y() == yrand) || (snake2[i]->get_x() == xrand && snake2[i]->get_y() == yrand))
          {
            repeat = false;
          }
        }

        for(int i = 0; i < food.size(); i++)
        {
          if(food[i]->get_x() == xrand && food[i]->get_y() == yrand)
          {
            repeat = false;
          }
        }

        new_x = xrand;
        new_y = yrand;

      }

      food[i]->set_x(new_x);
      food[i]->set_y(new_y);

    }

  }

  return;

}



void prepareFood(std::deque<IntTuple*> &food)
{
  /* 6 random coordinates that are initialized. Make sure that
  no two pieces of food are in the same spot, or on the starting
  position of the snake.
  */

  for(int i = 0; i < 6; i++)
  {
    int xrand = (getRandInt() % 31);
    int yrand = (getRandInt() % 127);

    xrand = xrand - (xrand % 2);
    yrand = yrand - (yrand % 2);

    bool repeat = false;

    if(xrand == 2)
    {
      xrand = 6;
    }

    for(int i = 0; i < food.size(); i++)
    {
      if(xrand == food[i]->get_x() && yrand == food[i]->get_y())
      {
        repeat = true;
      }
    }

    if(repeat == false)
    {
      IntTuple* FoodCoords = new IntTuple(xrand, yrand);
      food.push_back(FoodCoords);
    }
    else
    {
      i--;
    }
  }

}

bool checkCollision(std::deque<IntTuple*> &p1snake, int x, int y)
{

  for(int i = 0; i < p1snake.size(); i++)
  {
    if(p1snake[i]->get_x() == x && p1snake[i]->get_y() == y)
    {
      return true;
    }
  }

  if(x < 0 || x == 32 || y < 0 || y == 128)
  {
    return true;
  }


  return false;
}

//given a point, draw a 2X2 square from the top left.
void drawBlip(bool** chart, IntTuple* coords, bool value)
{
  for(int i = coords->get_x(); i < coords->get_x()+2; i++)
  {
    for(int j = coords->get_y(); j < coords->get_y()+2; j++)
    {
      chart[i][j] = value;
    }
  }
}

class Snake : public Runner {
public:
  Snake(RGBMatrix *m) : Runner(m), matrix_(m) {
    off_screen_canvas_ = m->CreateFrameCanvas();
  }
  virtual void Run() override {
    std::cout << "Run function for snake is RunSnake()" << std::endl;
  }
  void RunSnake(int players) {
    uint32_t continuum = 0;

    bool** menu = FileToFace("blank-base", true);

    int flowcycle = 1280000; // Integer that is used to divide x in the cosign equation. Higher = slower face floating.
    int flowcounter = 0; // Incremented integer that is used for the cosign function.
    int flowcountercompare = -8; // Used to keep track of the last integer that was used in the cosign function.
    int p1button; // Button that is pressed.
    int p2button;
    bool p1buttonPressed = false; // Wether or not a button is pressed.
    bool p2buttonPressed = false;
    bool** currentMenu = menu; // Current face

    int p1direction = 4; // 1: left, 2: down, 3 : up, 4: right
    int p2direction = 1;


    IntTuple* Player1Start1 = new IntTuple(2, 4);
    IntTuple* Player1Start2 = new IntTuple(2, 6);
    IntTuple* Player1Start3 = new IntTuple(2, 8);
    IntTuple* Player1Start4 = new IntTuple(2, 10);
    IntTuple* Player1Start5 = new IntTuple(2, 12);
    IntTuple* Player1Start6 = new IntTuple(2, 14);

    IntTuple* Player2Start1 = new IntTuple(2, 124);
    IntTuple* Player2Start2 = new IntTuple(2, 122);
    IntTuple* Player2Start3 = new IntTuple(2, 120);
    IntTuple* Player2Start4 = new IntTuple(2, 118);
    IntTuple* Player2Start5 = new IntTuple(2, 116);
    IntTuple* Player2Start6 = new IntTuple(2, 114);

    std::deque<IntTuple*> p1snake;
    std::deque<IntTuple*> p2snake;

    p1snake.push_back(Player1Start1);
    p1snake.push_back(Player1Start2);
    p1snake.push_back(Player1Start3);
    p1snake.push_back(Player1Start4);
    p1snake.push_back(Player1Start5);
    p1snake.push_back(Player1Start6);

    p2snake.push_back(Player2Start1);
    p2snake.push_back(Player2Start2);
    p2snake.push_back(Player2Start3);
    p2snake.push_back(Player2Start4);
    p2snake.push_back(Player2Start5);
    p2snake.push_back(Player2Start6);

    drawFullInput(currentMenu, 0);

    std::deque<IntTuple*> Food;
    prepareFood(Food);

    for(int i = 0; i < Food.size(); i++)
    {
      drawBlip(currentMenu, Food[i], true);
    }

    while (!interrupt_received) {

        /* If a button is pressed, maintain the same face that we have been
        drawing. If not, then go back to basic face until a new button is
        pressed */

        flowcounter++;

        for(int i = 0; i < 60; i++)
        {
  	  p1button = current_button_pushed(controller1buttons);
	  p2button = current_button_pushed(controller2buttons);

          // FIXME: The number of below if statements sucks. Maybe a dictionary of pointers?

	  if(p1buttonPressed == true)
 	  {
	    if(p1button == 0)
	    {
	      p1buttonPressed = false;
	    }
	  }
	  else
	  {
	    if(p1button != 0)
	    {
	      p1buttonPressed = true;

	      if((p1button == 10 || p1button == 6) && p1direction != 3)
              {
                p1direction = 2;
	      }
	      else if((p1button == 9 || p1button == 5) && p1direction != 2)
	      {
                p1direction = 3;
              }
              else if((p1button == 12 || p1button == 8) && p1direction != 1)
              {
                p1direction = 4;
              }
              else if((p1button == 7 || p1button == 11) && p1direction != 4)
              {
                p1direction = 1;
              }
            }
	   }


           if(players == 2)
           {
	      if(p2buttonPressed == true)
	      {
                if(p2button == 0)
                {
                  p2buttonPressed = false;
                }
	      }
	      else
              {
                if(p2button != 0)
                {
                  p2buttonPressed = true;

                  if((p2button == 10 || p2button == 6) && p2direction != 3)
                  {
                    p2direction = 2;
                  }
                  else if((p2button == 9 || p2button == 5) && p2direction != 2)
                  {
                    p2direction = 3;
                  }
                  else if((p2button == 12 || p2button == 8) && p2direction != 1)
                  {
                    p2direction = 4;
                  }
                  else if((p2button == 7 || p2button == 11) && p2direction != 4)
                  {
                    p2direction = 1;
                  }
                }
             }
          }
          usleep(5000);

        }

        int p1x_axis = p1snake.back()->get_x();
        int p1y_axis = p1snake.back()->get_y();

        int p2x_axis = p2snake.back()->get_x();
        int p2y_axis = p2snake.back()->get_y();

        if(p1direction == 4)
        {
          p1y_axis = p1y_axis + 2;
        }
        else if(p1direction == 1)
        {
          p1y_axis = p1y_axis - 2;
        }
        else if(p1direction == 3)
        {
          p1x_axis = p1x_axis - 2;
        }
        else if(p1direction == 2)
        {
          p1x_axis = p1x_axis + 2;
        }


        if(players == 2)
        {
          if(p2direction == 4)
          {
            p2y_axis = p2y_axis + 2;
          }
          else if(p2direction == 1)
          {
            p2y_axis = p2y_axis - 2;
          }
          else if(p2direction == 3)
          {
            p2x_axis = p2x_axis - 2;
          }
          else if(p2direction == 2)
          {
            p2x_axis = p2x_axis + 2;
          }

        }


        if(checkCollision(p1snake, p1x_axis, p1y_axis))
        {
          // Player 2 victory
          return;
        }


        if(players == 2)
        {
          if(checkCollision(p2snake, p2x_axis, p2y_axis))
          {
            // Player 1 victory
            return;
          }
        }

        bool p1foodCollide = checkCollision(Food, p1x_axis, p1y_axis);
        bool p2foodCollide = checkCollision(Food, p2x_axis, p2y_axis);

        //if(!foodCollide)
        //{
        //  p1snake.pop_front();
        //}
        //else
        //{
        //  switchFood(p1snake, Food, x_axis, y_axis);
        //}

        if(p1foodCollide && p2foodCollide)
        {
          multiP_switchFood(p1snake, p2snake, Food, p1x_axis, p1y_axis, p2x_axis, p2y_axis);
        }
        else if(p1foodCollide)
	{
	  if(players == 2)
          {
	    multiP_switchFood(p1snake, p2snake, Food, p1x_axis, p1y_axis, p2x_axis, p2y_axis);
	    p2snake.pop_front();
          }
          else
	  {
	    singleP_switchFood(p1snake, Food, p1x_axis, p1y_axis);
	  }

	}
	else if(p2foodCollide)
	{
	  multiP_switchFood(p1snake, p2snake, Food, p1x_axis, p1y_axis, p2x_axis, p2y_axis);
	}
	else
	{
	  p1snake.pop_front();
	  if(players == 2)
	  {
	    p2snake.pop_front();
	  }
	}

        for(int i = 0; i < Food.size(); i++)
        {
            drawBlip(currentMenu, Food[i], true);
        }

        IntTuple* P1NewHead = new IntTuple(p1x_axis, p1y_axis);
        p1snake.push_back(P1NewHead);

        IntTuple* P1Tail = p1snake.front();

        for(int i = 0; i < p1snake.size(); i++)
        {
          drawBlip(currentMenu, p1snake[i], true);
        }

        drawBlip(currentMenu, P1Tail, false);

        if(players == 2)
        {
          IntTuple* P2NewHead = new IntTuple(p2x_axis, p2y_axis);
          p2snake.push_back(P2NewHead);

          IntTuple* P2Tail = p2snake.front();

          for(int i = 0; i < p2snake.size(); i++)
          {
            drawBlip(currentMenu, p2snake[i], true);
          }

          drawBlip(currentMenu, P2Tail, false);

        }

        drawFullInput(currentMenu, 0);

     }

 }
 private:
   RGBMatrix *const matrix_;
   FrameCanvas *off_screen_canvas_;
   int players;
};

