#include "runner.h"
#include "face.h"
#include "controller.cpp"
#include "victory.h"

#include <limits.h>
#include <math.h>
#include <cassert>
#include <map>
#include <deque>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

extern std::chrono::system_clock::time_point* controller1buttons;
extern std::chrono::system_clock::time_point* controller2buttons;


void prepareFood(std::deque<IntTuple*> &food)
{
  /* 6 random coordinates that are initialized. Make sure that
  no two pieces of food are in the same spot, or on the starting
  position of the snake.
  */

  for(int i = 0; i < 6; i++)
  {
    int xrand = (getRandInt() % 31);
    int yrand = (getRandInt() % 106);

    if(yrand < 22)
    {
      yrand = yrand + 22;
    }

    xrand = xrand - (xrand % 2);
    yrand = yrand - (yrand % 2);

    bool repeat = false;

    if(xrand == 2)
    {
      xrand = 6;
    }

    for(long unsigned int i = 0; i < food.size(); i++)
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

  for(long unsigned int i = 0; i < p1snake.size(); i++)
  {
    if(p1snake[i]->get_x() == x && p1snake[i]->get_y() == y)
    {
      return true;
    }
  }

  if(x < 0 || x == 32 || y < 20 || y == 108)
  {
    return true;
  }


  return false;
}

bool checkVictory(std::deque<IntTuple*> &snake1, std::deque<IntTuple*> &snake2)
{
  for(long unsigned int i = 0; i < snake1.size(); i++)
  {
    if(snake1[i]->get_x() == snake2.back()->get_x() && snake1[i]->get_y() == snake2.back()->get_y())
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
  void drawBoundaries()
  {
    for(int i = 0; i < 32; i++)
    {
      canvas()->SetPixel(19, i, 255, 255, 255);
      canvas()->SetPixel(108, i, 255, 255, 255);
    }
  }
  void drawBlipArray(std::deque<IntTuple*> &array, int r, int g, int b)
  {
    for(long unsigned int i = 0; i < array.size(); i++)
    {
      for(int x = array[i]->get_x(); x < array[i]->get_x() + 2; x++)
      {
        for(int y = array[i]->get_y(); y < array[i]->get_y() + 2; y++)
        {
          canvas()->SetPixel(y, x, r, g, b);
        }
      }
    }
  }
  void singleP_switchFood(bool** face, std::deque<IntTuple*> &food, int x, int y)
  {
    /* Reassign one of the food blips, make sure that it is not
    already true on the board.
    */

    for(long unsigned int i = 0; i < food.size(); i++)
    {
      if(x == food[i]->get_x() && y == food[i]->get_y())
      {
        std::vector<int> free_x_values;
        std::vector<int> free_y_values;

        for(int i = 0; i < 32; i +=2)
        {
          for(int j = 22; j < 108; j +=2)
          {
            if(face[i][j] == false)
            {
              free_x_values.push_back(i);
              free_y_values.push_back(j);
            }
          }
        }

        int index = getRandInt() % free_x_values.size() - 1;

        if(index == -1)
        {
          index = 0;
        }

        food[i]->set_x(free_x_values[index]);
        food[i]->set_y(free_y_values[index]);

      }
    }

    return;

  }

  void multiP_switchFood(bool** face, std::deque<IntTuple*> &food, int p1x, int p1y, int p2x, int p2y)
  {
    /* Reassign one of the food blips, make sure that it is not
    already true on the board. Accounts for 2 snakes.
    */

    for(long unsigned int i = 0; i < food.size(); i++)
    {
      if((p1x == food[i]->get_x() && p1y == food[i]->get_y()) || (p2x == food[i]->get_x() && p2y == food[i]->get_y()))
      {

        std::vector<int> free_x_values;
        std::vector<int> free_y_values;

        for(int i = 0; i < 32; i +=2)
        {
          for(int j = 22; j < 108; j +=2)
          {
            if(face[i][j] == false)
            {
              free_x_values.push_back(i);
              free_y_values.push_back(j);
            }
          }
        }

        int index = getRandInt() % free_x_values.size() - 1;

        if(index == -1)
        {
          index = 0;
        }

        food[i]->set_x(free_x_values[index]);
        food[i]->set_y(free_y_values[index]);

      }

    }

    return;

  }

  void freeSnakeMem(std::deque<IntTuple*> &snake)
  {
    for(int i = 0; i < snake.size(); i++)
    {
      delete(snake[i]);
    }
  }

  void RunSnake(int players) {
    bool** menu = FileToFace("blank-base", true);

    int p1button; // Button that is pressed.
    int p2button;
    bool p1buttonPressed = false; // Wether or not a button is pressed.
    bool p2buttonPressed = false;
    bool** currentMenu = menu; // Current face

    int p1direction = 4; // 1: left, 2: down, 3 : up, 4: right
    int p2direction = 1;


    IntTuple* Player1Start1 = new IntTuple(2, 24);
    IntTuple* Player1Start2 = new IntTuple(2, 26);
    IntTuple* Player1Start3 = new IntTuple(2, 28);
    IntTuple* Player1Start4 = new IntTuple(2, 30);
    IntTuple* Player1Start5 = new IntTuple(2, 32);
    IntTuple* Player1Start6 = new IntTuple(2, 34);

    IntTuple* Player2Start1 = new IntTuple(2, 104);
    IntTuple* Player2Start2 = new IntTuple(2, 102);
    IntTuple* Player2Start3 = new IntTuple(2, 100);
    IntTuple* Player2Start4 = new IntTuple(2, 98);
    IntTuple* Player2Start5 = new IntTuple(2, 96);
    IntTuple* Player2Start6 = new IntTuple(2, 94);

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

    drawFullInput(currentMenu, 0, 0, 0, 255);

    std::deque<IntTuple*> Food;
    prepareFood(Food);

    for(long unsigned int i = 0; i < Food.size(); i++)
    {
      drawBlip(currentMenu, Food[i], true);
    }

    while (!interrupt_received) {

        /* If a button is pressed, maintain the same face that we have been
        drawing. If not, then go back to basic face until a new button is
        pressed */


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

	      if((p1button == 10 || p1button == 6 || p1button == 17) && p1direction != 3)
              {
                p1direction = 2;
	      }
	      else if((p1button == 9 || p1button == 5 || p1button == 16) && p1direction != 2)
	      {
                p1direction = 3;
              }
              else if((p1button == 12 || p1button == 8 || p1button == 19) && p1direction != 1)
              {
                p1direction = 4;
              }
              else if((p1button == 7 || p1button == 11 || p1button == 18) && p1direction != 4)
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

                  if((p2button == 10 || p2button == 6 || p2button == 17) && p2direction != 3)
                  {
                    p2direction = 2;
                  }
                  else if((p2button == 9 || p2button == 5 || p2button == 16) && p2direction != 2)
                  {
                    p2direction = 3;
                  }
                  else if((p2button == 12 || p2button == 8 || p2button == 19) && p2direction != 1)
                  {
                    p2direction = 4;
                  }
                  else if((p2button == 7 || p2button == 11 || p2button == 18) && p2direction != 4)
                  {
                    p2direction = 1;
                  }
                }
             }
          }
          usleep(3000);

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

        if(players == 2)
        {
          if(checkCollision(p2snake, p2x_axis, p2y_axis))
          {
            // Player 1 victory
            Victory* victory = new Victory(matrix_);
            victory->VictorRun(1);
            delete victory;
            freeSnakeMem(p1snake);
            freeSnakeMem(p2snake);
            freeSnakeMem(Food);
            return;
          }
          else if(checkCollision(p1snake, p1x_axis, p1y_axis))
          {
            // Player 2 victory
            Victory* victory = new Victory(matrix_);
            victory->VictorRun(2);
            delete victory;
            freeSnakeMem(p1snake);
            freeSnakeMem(p2snake);
            freeSnakeMem(Food);
            return;
          }
        }
        else {
          //Just return if it's one player and they collide
          if(checkCollision(p1snake, p1x_axis, p1y_axis))
          {
            freeSnakeMem(p1snake);
            freeSnakeMem(Food);
            return;
          }
        }

        bool p1foodCollide = checkCollision(Food, p1x_axis, p1y_axis);
        bool p2foodCollide = checkCollision(Food, p2x_axis, p2y_axis);

        if(p1foodCollide && p2foodCollide)
        {
          multiP_switchFood(currentMenu, Food, p1x_axis, p1y_axis, p2x_axis, p2y_axis);
        }
        else if(p1foodCollide)
	{

	  if(players == 2)
          {
            p2snake.pop_front();
	    multiP_switchFood(currentMenu, Food, p1x_axis, p1y_axis, p2x_axis, p2y_axis);
          }
          else
	  {
	    singleP_switchFood(currentMenu, Food, p1x_axis, p1y_axis);
	  }

	}
	else if(p2foodCollide)
	{
          p1snake.pop_front();
	  multiP_switchFood(currentMenu, Food, p1x_axis, p1y_axis, p2x_axis, p2y_axis);
	}
	else
	{
	  p1snake.pop_front();
	  if(players == 2)
	  {
	    p2snake.pop_front();
	  }
	}

        for(long unsigned int i = 0; i < Food.size(); i++)
        {
            drawBlip(currentMenu, Food[i], true);
        }

        IntTuple* P1NewHead = new IntTuple(p1x_axis, p1y_axis);
        p1snake.push_back(P1NewHead);

        IntTuple* P1Tail = p1snake.front();

        for(long unsigned int i = 0; i < p1snake.size(); i++)
        {
          drawBlip(currentMenu, p1snake[i], true);
        }

        drawBlip(currentMenu, P1Tail, false);

        if(players == 2)
        {
          IntTuple* P2NewHead = new IntTuple(p2x_axis, p2y_axis);
          p2snake.push_back(P2NewHead);

          IntTuple* P2Tail = p2snake.front();

          for(long unsigned int i = 0; i < p2snake.size(); i++)
          {
            drawBlip(currentMenu, p2snake[i], true);
          }

          drawBlip(currentMenu, P2Tail, false);

        }

        if(players == 2)
        {
          bool check1victory = checkVictory(p1snake, p2snake);

          if(check1victory)
          {
            Victory * victory = new Victory(matrix_);
            victory->VictorRun(1);
            return;
          }

          bool check2victory = checkVictory(p2snake, p1snake);

          if(check2victory)
          {
            Victory * victory = new Victory(matrix_);
            victory->VictorRun(2);
            return;
          }

        }

        canvas()->Clear();

        drawBoundaries();

        drawBlipArray(Food, 255, 255, 255);
        drawBlipArray(p1snake, 0, 0, 255);

        if(players == 2)
        {
          drawBlipArray(p2snake, 255, 165, 0);
        }

     }

 }
 private:
   RGBMatrix *const matrix_;
   FrameCanvas *off_screen_canvas_;
   int players;
};

