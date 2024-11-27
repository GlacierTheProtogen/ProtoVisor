#ifndef BIRD_OBSTACLE_H
#define BIRD_OBSTACLE_H

#include "dinosaur-game.h"

class birdObstacle {
  public:
    birdObstacle(bool** frame1_input, bool** frame2_input, int startingPointX, int startingPointY, int sprite_size_x, int sprite_size_y, int frames_input)
    {
      initial_x = startingPointX;
      initial_y = startingPointY;
      frame1 = frame1_input;
      frame2 = frame2_input;
      frames = frames_input;
      sprite_size_x = sprite_size_x;
      sprite_size_y = sprite_size_y;
    }
    bool** getSpriteReference(int totalFrames)
    {
      if((totalFrames % (frames * 2)) > frames)
      {
        return frame1;
      }
      return frame2;
    }
    int get_y()
    {
      return initial_y;
    }
    int get_x()
    {
      return initial_x;
    }
    void set_y(int y)
    {
      initial_y = y;
    }
  private:
    int initial_x;
    int initial_y;
    int frames;
    int size_x;
    int size_y;
    bool** frame1;
    bool** frame2;

};

#endif
