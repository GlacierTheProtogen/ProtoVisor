#ifndef DINOSAUR_GAME_H
#define DINOSAUR_GAME_H

#include <cmath>

// Return what the dino's next y value should be based on how many frames he's been in the air, where he is now, and the ground.

int nextYvalue(int frames, int ground, int prev_y)
{
  double velocity = 3 - (0.3 * frames);

  double new_y = prev_y - velocity;
  int rounded_y = round(new_y);

  if(rounded_y > ground)
  {
    rounded_y = ground;
  }

  return rounded_y;

}


//Sprites with 2 frames
class AnimatedSprite {
  public:
    AnimatedSprite(bool** frame1_input, bool** frame2_input, int MovementFrames)
    {
      frame1 = frame1_input;
      frame2 = frame2_input;
      frames = MovementFrames;
    }
    bool** get_current_frame(int totalFrames)
    {
      if((totalFrames % (frames * 2)) > frames)
      {
        return frame1;
      }
      else
      {
        return frame2;
      }
    }
  private:
    bool** frame1;
    bool** frame2;
    int frames;
};


#endif
