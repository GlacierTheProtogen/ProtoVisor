#ifndef RUNNER_H
#define RUNNER_H

#include "led-matrix.h"
using namespace rgb_matrix;


volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
  interrupt_received = true;
}


class Runner {
protected:
  Runner(Canvas *canvas) : canvas_(canvas) {}
  inline Canvas *canvas() { return canvas_; }

public:
  // Half a face mirrored
  void drawFaceInput(bool** face, int floater) {
    canvas()->Clear();
    for(int i=0; i < 64; i++)
    {
      for(int j=0; j < 32; j++)
      {
        if(face[j][i] == true)
        {
          canvas()->SetPixel(i, j-floater, 0, 255, 0);
          canvas()->SetPixel(128-i, j-floater, 0, 255, 0);
        }
      }
    }
  }
  // Full face not mirrored
  void drawMenuInput(bool** face, int floater) {
    canvas()->Clear();
    for(int i = 0; i < 128; i++)
    {
      for(int j = 0; j < 32; j++)
      {
        if(face[j][i] == true)
        {
          canvas()->SetPixel(i, j-floater, 0, 255, 0);
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
