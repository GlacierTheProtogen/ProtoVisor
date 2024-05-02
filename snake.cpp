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

void switchFood(std::deque<IntTuple*> &snake1, std::deque<IntTuple*> &food, int x, int y)
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

    if(x < 0 || x == 32 || y < 0 || y == 128)
    {
      return true;
    }
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

   std::cout << "No of players" << std::endl;
   std::cout << players << std::endl;

    bool** menu = FileToFace("blank-base", true);

    int flowcycle = 1280000; // Integer that is used to divide x in the cosign equation. Higher = slower face floating.
    int flowcounter = 0; // Incremented integer that is used for the cosign function.
    int flowcountercompare = -8; // Used to keep track of the last integer that was used in the cosign function.
    int button; // Button that is pressed.
    bool buttonPressed = false; // Wether or not a button is pressed.
    bool** currentMenu = menu; // Current face

    int direction = 4; // 1: left, 2: down, 3 : up, 4: right

    IntTuple* Player1Start1 = new IntTuple(2, 4);
    IntTuple* Player1Start2 = new IntTuple(2, 6);
    IntTuple* Player1Start3 = new IntTuple(2, 8);
    IntTuple* Player1Start4 = new IntTuple(2, 10);
    IntTuple* Player1Start5 = new IntTuple(2, 12);
    IntTuple* Player1Start6 = new IntTuple(2, 14);

    IntTuple* Player2Start1 = new IntTuple(2, 128);
    IntTuple* Player2Start2 = new IntTuple(2, 126);
    IntTuple* Player2Start3 = new IntTuple(2, 124);
    IntTuple* Player2Start4 = new IntTuple(2, 122);
    IntTuple* Player2Start5 = new IntTuple(2, 120);
    IntTuple* Player2Start6 = new IntTuple(2, 122);

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
  	  button = current_button_pushed(controller1buttons);

        // FIXME: The number of below if statements sucks. Maybe a dictionary of pointers?

	  if(buttonPressed == true)
 	  {
	    if(button == 0)
	    {
	      buttonPressed = false;
	    }
	  }
	  else
	  {
	    if(button != 0)
	    {
	      buttonPressed = true;

	      if((button == 10 || button == 6) && direction != 3)
              {
                direction = 2;
	      }
	      else if((button == 9 || button == 5) && direction != 2)
	      {
                direction = 3;
              }
              else if((button == 12 || button == 8) && direction != 1)
              {
                direction = 4;
              }
              else if((button == 7 || button == 11) && direction != 4)
              {
                direction = 1;
              }

	   }
	  }
          usleep(5000);
        }

        int x_axis = p1snake.back()->get_x();
        int y_axis = p1snake.back()->get_y();

        if(direction == 4)
        {
          y_axis = y_axis + 2;
        }
        else if(direction == 1)
        {
          y_axis = y_axis - 2;
        }
        else if(direction == 3)
        {
          x_axis = x_axis - 2;
        }
        else if(direction == 2)
        {
          x_axis = x_axis + 2;
        }

        bool tailCollide = checkCollision(p1snake, x_axis, y_axis);

        if(checkCollision(p1snake, x_axis, y_axis))
        {
          return;
        }

        bool foodCollide = checkCollision(Food, x_axis, y_axis);


        if(!foodCollide)
        {
          p1snake.pop_front();
        }
        else
        {
          switchFood(p1snake, Food, x_axis, y_axis);
        }

        for(int i = 0; i < Food.size(); i++)
        {
            drawBlip(currentMenu, Food[i], true);
        }

        IntTuple* NewHead = new IntTuple(x_axis, y_axis);
        p1snake.push_back(NewHead);

        IntTuple* Tail = p1snake.front();

        for(int i = 0; i < p1snake.size(); i++)
        {
          drawBlip(currentMenu, p1snake[i], true);
        }

        drawBlip(currentMenu, Tail, false);

        drawFullInput(currentMenu, 0);

     }

 }
 private:
   RGBMatrix *const matrix_;
   FrameCanvas *off_screen_canvas_;
   int players;
};

