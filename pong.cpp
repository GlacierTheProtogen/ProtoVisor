#include "runner.h"
#include "face.h"
#include "controller.cpp"
#include "victory.h"

#include <limits.h>
#include <math.h>
#include <chrono>
#include <cassert>
#include <map>
#include <deque>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <thread>

extern std::chrono::system_clock::time_point* controller1buttons;
extern std::chrono::system_clock::time_point* controller2buttons;

extern int g_red;
extern int g_blue;
extern int g_green;

bool checkPaddleHit(std::deque<IntTuple*> &paddle, int x, int y)
{

  for(long unsigned int i = 0; i < paddle.size(); i++)
  {
    if(paddle[i]->get_x() == x && paddle[i]->get_y() == y)
    {
      return true;
    }
  }

  return false;
}

int checkPongVictory(std::deque<IntTuple*> &snake1, std::deque<IntTuple*> &snake2)
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

void movePaddle(std::deque<IntTuple*> &paddle, bool direction)
{
  if(direction == false)
  {
    IntTuple* PaddleTail = paddle.back();

    if(PaddleTail->get_x() + 2 < 32)
    {
      IntTuple* NewHead = new IntTuple(PaddleTail->get_x() + 2, PaddleTail->get_y());
      paddle.push_back(NewHead);
      paddle.pop_front();
    }

  }
  else
  {
    IntTuple* PaddleTail = paddle.front();

    if(PaddleTail->get_x() - 2 > -2)
    {
      IntTuple* NewHead = new IntTuple(PaddleTail->get_x() - 2, PaddleTail->get_y());
      paddle.push_front(NewHead);
      paddle.pop_back();
    }

  }
  return;
}

void ball(std::deque<IntTuple*> &p1paddle, std::deque<IntTuple*> &p2paddle, IntTuple* ball_coords, IntTuple* visual_ball, bool* ballBreak)
{
  double angle = 45;
  double hypotenuese = 5;

  double pi = M_PI;

  double next_x = 100.0;
  double next_y = 400.0;
  int bounce_timer = 0;


  int cycle_time = 17000;

  double y_direction = 1;

  while(*ballBreak)
  {

    if(bounce_timer > 0)
    {
      bounce_timer--;
    }

    double x_movement = hypotenuese * sin(angle * (pi/180));
    double y_movement = (hypotenuese * cos(angle * (pi/180)) * y_direction);

    next_x = next_x + x_movement;
    next_y = next_y + y_movement;

    double angle_change = -45;

    for(unsigned long int i = 0; i < p1paddle.size(); i++)
    {
      angle_change = angle_change + 15;

      int paddle_x = (p1paddle[i]->get_x())*10;
      int paddle_y = (p1paddle[i]->get_y())*10;

      // Fix me: Duplicate code, should only be its own function
      if((next_x >= paddle_x && next_x <= paddle_x + 20) && (next_y >= paddle_y && next_y <= paddle_y + 20) && bounce_timer == 0)
      {
        y_direction = -y_direction;
        bounce_timer = 30;


        double new_x_movement = x_movement + (hypotenuese * sin(angle * (pi/180)));
        double new_y_movement = (y_movement + (hypotenuese * cos(angle * (pi/180)) * y_direction));

        bool angle_viable = (angle + angle_change >= -75 && angle + angle_change <= 75);

        if(angle_viable)
        {
          angle = angle + angle_change;
          x_movement = new_x_movement;
          y_movement = new_y_movement;
        }

        if(cycle_time > 1000)
        {
          cycle_time = cycle_time - 1000;
        }
        else if(cycle_time > 100)
        {
          cycle_time = cycle_time - 10;
        }

        break;
      }

      paddle_x = (p2paddle[i]->get_x())*10;
      paddle_y = (p2paddle[i]->get_y())*10;

      if((next_x >= paddle_x  && next_x <= paddle_x + 20) && (next_y >= paddle_y - 20 && next_y <= paddle_y) && bounce_timer == 0)
      {
        y_direction = -y_direction;
        bounce_timer = 30;


        double new_x_movement = x_movement + (hypotenuese * sin(angle * (pi/180)));
        double new_y_movement = (y_movement + (hypotenuese * cos(angle * (pi/180)) * y_direction));

        bool angle_viable = (angle_change >= -75 && angle_change <= 75);

        if(angle_viable)
        {
          angle = angle + angle_change;
          x_movement = new_x_movement;
          y_movement = new_y_movement;
        }

        //cycle_time = cycle_time - 1000;

        if(cycle_time > 1000)
        {
	  cycle_time = cycle_time - 1000;
        }
        else if(cycle_time > 100)
        {
          cycle_time = cycle_time - 10;
        }

        break;
      }
    }

    if(next_x >= 300 || next_x <= 0)
    {
      angle = -angle;
    }

    if(next_y > 630)
    {
      visual_ball->set_y(62);
    }
    else
    {
      visual_ball->set_y(64);
    }
    visual_ball->set_x(std::round(next_x/10));


    ball_coords->set_x(std::round(next_x/10));
    ball_coords->set_y(std::round(next_y/10));

    usleep(cycle_time);
  }

  return;
}



class Pong : public Runner {
public:
  Pong(RGBMatrix *m) : Runner(m), matrix_(m) {
    off_screen_canvas_ = m->CreateFrameCanvas();
  }
  void drawBlipArray(std::deque<IntTuple*> &array, int r, int g, int b)
  {
    for(unsigned long int i = 0; i < array.size(); i++)
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
  void drawBall(IntTuple* ball, int r, int g, int b)
  {
    for(int x = ball->get_x(); x < ball->get_x() + 2; x++)
    {
      for(int y = ball->get_y(); y < ball->get_y() + 2; y++)
      {
        canvas()->SetPixel(y, x, r, g, b);
      }
    }
  }

  void Run() override {
    bool** menu = FileToFace("blank-base", true);

    bool p1direction = false;
    bool p1move = false;
    bool p2direction = false;
    bool p2move = false;

    int p1button; // Button that is pressed.
    int p2button;

    bool p1buttonPressed = false; // Wether or not a button is pressed.
    bool p2buttonPressed = false;
    bool** currentMenu = menu; // Current face

    IntTuple* Player1Start1 = new IntTuple(12, 16);
    IntTuple* Player1Start2 = new IntTuple(14, 16);
    IntTuple* Player1Start3 = new IntTuple(16, 16);
    IntTuple* Player1Start4 = new IntTuple(18, 16);
    IntTuple* Player1Start5 = new IntTuple(20, 16);

    IntTuple* Player2Start1 = new IntTuple(12, 112);
    IntTuple* Player2Start2 = new IntTuple(14, 112);
    IntTuple* Player2Start3 = new IntTuple(16, 112);
    IntTuple* Player2Start4 = new IntTuple(18, 112);
    IntTuple* Player2Start5 = new IntTuple(20, 112);

    std::deque<IntTuple*> p1paddle;
    std::deque<IntTuple*> p2paddle;

    p1paddle.push_back(Player1Start1);
    p1paddle.push_back(Player1Start2);
    p1paddle.push_back(Player1Start3);
    p1paddle.push_back(Player1Start4);
    p1paddle.push_back(Player1Start5);

    p2paddle.push_back(Player2Start1);
    p2paddle.push_back(Player2Start2);
    p2paddle.push_back(Player2Start3);
    p2paddle.push_back(Player2Start4);
    p2paddle.push_back(Player2Start5);

    drawFullInput(currentMenu, 0, 0, 0, 255);

    IntTuple ball_coords(20, 60);
    IntTuple visual_ball(20, 62);

    bool ballBreak = true;
    std::thread ballThread(ball, std::ref(p1paddle), std::ref(p2paddle), &ball_coords, &visual_ball, &ballBreak);

    while (!interrupt_received) {

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

	  if(p1button == 10 || p1button == 6 || p1button == 17)
          {
            p1direction = false;
            p1move = true;
	  }
	  else if(p1button == 9 || p1button == 5 || p1button == 16)
	  {
            p1direction = true;
            p1move = true;
          }
        }
      }

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

          if(p2button == 10 || p2button == 6 || p2button == 17)
          {
            p2direction = false;
            p2move = true;
          }
          else if(p2button == 9 || p2button == 5 || p2button == 16)
          {
            p2direction = true;
            p2move = true;
          }
        }
      }

      usleep(200);

      if(p1move == true)
      {
        movePaddle(p1paddle, p1direction);
        p1move = false;
      }

      if(p2move == true)
      {
        movePaddle(p2paddle, p2direction);
        p2move = false;
      }

     canvas()->Clear();

     if(ball_coords.get_y() >= 128)
     {
       ballBreak = false;
       ballThread.join();
       Victory* victory = new Victory(matrix_);
       victory->VictorRun(1);
       delete victory;

       for(int i = 0; i < p1paddle.size(); i++)
       {
         delete p1paddle[i];
         delete p2paddle[i];
       }

       return;
     }

     if(ball_coords.get_y() <= 0)
     {
       ballBreak = false;
       ballThread.join();
       Victory* victory = new Victory(matrix_);
       victory->VictorRun(2);
       delete victory;

       for(int i = 0; i < p1paddle.size(); i++)
       {
         delete p1paddle[i];
         delete p2paddle[i];
       }


       return;
     }


     drawBlipArray(p1paddle, 0, 0, 255);
     drawBlipArray(p2paddle, 255, 165, 0);
     drawBall(&ball_coords, 255, 255, 255);
     drawBall(&visual_ball, 255, 255, 255);

    }
   }
 }
 private:
   RGBMatrix *const matrix_;
   FrameCanvas *off_screen_canvas_;
   int players;
};
