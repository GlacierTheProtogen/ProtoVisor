#ifndef RUNNER_H
#define RUNNER_H

#include "led-matrix.h"
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
  virtual ~Runner() {}
  virtual void Run() = 0;

private:
  Canvas *const canvas_;

};


#endif
