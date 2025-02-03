#include "runner.h"
#include "face.h"
#include "unistd.h"

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

    drawPortalColors(three);
    usleep(delaytime);
    drawPortalColors(two);
    usleep(delaytime);
    drawPortalColors(one);
    usleep(delaytime);

    freeFace(three, 32);
    freeFace(two, 32);
    freeFace(one, 32);

    return;

 }
 private:
   RGBMatrix *const matrix_;
   FrameCanvas *off_screen_canvas_;
};

