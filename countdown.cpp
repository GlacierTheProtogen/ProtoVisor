#include "runner.h"
#include "face.h"

#include <chrono>
#include <time.h>

class CountDown : public Runner {
public:
  CountDown(RGBMatrix *m) : Runner(m), matrix_(m) {
    off_screen_canvas_ = m->CreateFrameCanvas();
  }
  void Run() override {
    bool** three = FileToFace("three", true);
    bool** two = FileToFace("two", true);
    bool** one = FileToFace("one", true);
    int delaytime = 1000000;

    drawFaceInput(three, 0, 0, 0, 255);
    usleep(delaytime);
    drawFaceInput(two, 0, 0, 0, 255);
    usleep(delaytime);
    drawFaceInput(one, 0, 0, 0, 255);
    usleep(delaytime);

    return;

 }
 private:
   RGBMatrix *const matrix_;
   FrameCanvas *off_screen_canvas_;
};

