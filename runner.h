#ifndef RUNNER_H
#define RUNNER_H

#include "led-matrix.h"
#include <time.h>
#include <chrono>
using namespace rgb_matrix;


volatile bool interrupt_received = false;

static void InterruptHandler(int signo) {
  interrupt_received = true;
}


class IntTuple {
  public:
    IntTuple(int x_input, int y_input)
    {
      coords[0] = x_input;
      coords[1] = y_input;
    }
    int get_x()
    {
      return coords[0];
    }
    int get_y()
    {
      return coords[1];
    }
    void set_x(int x)
    {
      coords[0] = x;
    }
    void set_y(int y)
    {
      coords[1] = y;
    }
  private:
    int coords[2];
};

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

class Runner {
protected:
  Runner(Canvas *canvas) : canvas_(canvas) {}
  inline Canvas *canvas() { return canvas_; }

public:
  // Half a face mirrored
  void drawFaceInput(bool** face, int floater, int r, int b, int g) {
    canvas()->Clear();
    for(int i=0; i < 64; i++)
    {
      for(int j=0; j < 32; j++)
      {
        if(face[j][i] == true)
        {
          canvas()->SetPixel(i, j-floater, r, b, g);
          canvas()->SetPixel(128-i, j-floater, r, b, g);
        }
      }
    }
  }
  void drawPortalColors(bool** face)
  {
    canvas()->Clear();
    for(int i = 0; i < 64; i++)
    {
      for(int j=0; j < 32; j++)
      {
        if(face[j][i] == true)
        {
          canvas()->SetPixel(i, j, 0, 0, 255);
        }
      }
    }
    for(int i = 64; i < 128; i++)
    {
      for(int j=0; j < 32; j++)
      {
        if(face[j][i] == true)
        {
          canvas()->SetPixel(i, j, 255, 165, 0);
        }
      }
    }
  }
  // Full face not mirrored
  void drawFullInput(bool** face, int floater, int r, int b, int g) {
    canvas()->Clear();
    for(int i = 0; i < 128; i++)
    {
      for(int j = 0; j < 32; j++)
      {
        if(face[j][i] == true)
        {
          canvas()->SetPixel(i, j-floater, r, b, g);
        }
      }
    }
  }
  // Only for color menu
  void drawColorOption(int r, int b, int g, int floater, int x, int y, bool selected)
  {

    int squareWidth = 7;
    int displace = 44;

    for(int i = 0; i < squareWidth + 1; i++)
    {
      canvas()->SetPixel(x, y - floater + i, r, b, g);
      canvas()->SetPixel(x + squareWidth, y - floater + i,  r, b, g);
      canvas()->SetPixel(x + i, y - floater, r, b, g);
      canvas()->SetPixel(x + i, y + squareWidth - floater, r, b, g);

      canvas()->SetPixel(x + displace, y - floater + i, r, b, g);
      canvas()->SetPixel(x + + displace + squareWidth, y - floater + i,  r, b, g);
      canvas()->SetPixel(x + i + displace, y - floater, r, b, g);
      canvas()->SetPixel(x + i + displace, y + squareWidth - floater, r, b, g);

    }
    if(selected == true)
    {
      for(int i = x + 1; i < x + squareWidth; i++)
      {
        for(int j = y + 1; j < y + squareWidth; j++)
        {
          canvas()->SetPixel(i, j - floater, r, b, g);
          canvas()->SetPixel(i + displace, j-floater, r, b, g);
        }
      }
    }
    else
    {
      for(int i = x + 1; i < x + squareWidth; i++)
      {
        for(int j = y + 1; j < y + squareWidth; j++)
        {
          canvas()->SetPixel(i, j - floater, 0, 0, 0);
          canvas()->SetPixel(i + displace, j-floater, 0, 0, 0);
        }
      }
    }
  }
  virtual void Run() = 0;

  virtual ~Runner() {}
private:
  Canvas *const canvas_;


};


#endif
