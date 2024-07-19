#ifndef VICTORY_H
#define VICTORY_H

#include "runner.h"
#include "face.h"

#include <chrono>
#include <time.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>

class Victory : public Runner {
public:
  Victory(RGBMatrix *m) : Runner(m), matrix_(m) {
    off_screen_canvas_ = m->CreateFrameCanvas();
  }
  void Run() override {
    std::cout << "Run override is victory override" << std::endl;
  }
  void VictorRun(int players) {
    bool** player1 = FileToFace("player-1-victory", true);
    bool** player2 = FileToFace("player-2-victory", true);


    int delaytime = 3000000;

    if(players == 1)
    {
      drawFullInput(player1, 0, 0, 0, 255);
    }
    else
    {
      drawFullInput(player2, 0, 255, 165, 0);
    }

    usleep(delaytime);

    return;

 }
 private:
   RGBMatrix *const matrix_;
   FrameCanvas *off_screen_canvas_;
};

#endif
