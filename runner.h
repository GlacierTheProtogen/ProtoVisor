#ifndef RUNNER_H
#define RUNNER_H

#include "led-matrix.h"
using namespace rgb_matrix;

class Runner {
protected:
  Runner(Canvas *canvas) : canvas_(canvas) {}
  inline Canvas *canvas() { return canvas_; }

public:
  virtual ~Runner() {}
  virtual void Run() = 0;

private:
  Canvas *const canvas_;

};


#endif
