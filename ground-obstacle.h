#ifndef GROUND_OBSTACLE_H
#define GROUND_OBSTACLE_H

#include "dinosaur-game.h"

class groundObstacle {
  public:
    groundObstacle(bool** importedSprite, int startingPointX, int startingPointY, int sprite_size_x, int sprite_size_y)
    {
      initial_x = startingPointX;
      initial_y = startingPointY;
      sprite = importedSprite;
      size_x = sprite_size_x;
      size_y = sprite_size_y;
    }
    bool** getSpriteReference()
    {
      return sprite;
    }
    int get_y()
    {
      return initial_y;
    }
    int get_x()
    {
      return initial_x;
    }
    int get_w()
    {
      return size_y;
    }
    int get_h()
    {
      return size_x;
    }
    void set_y(int y)
    {
      initial_y = y;
    }
  private:
    int initial_x;
    int initial_y;
    int size_x;
    int size_y;
    bool** sprite;

};

#endif
