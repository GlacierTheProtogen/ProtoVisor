#ifndef DINOSAUR_GAME_H
#define DINOSAUR_GAME_H

#include <cmath>

// Return what the dino's next y value should be based on how many frames he's been in the air, where he is now, and the ground.

int nextYvalue(int frames, int ground, int prev_y, double initial_velocity)
{
  double velocity = initial_velocity - (0.1 * frames);

  double new_y = prev_y - velocity;
  int rounded_y = round(new_y);

  if(rounded_y > ground)
  {
    rounded_y = ground;
  }

  return rounded_y;

}


//Dinosaur Object. Assumes that the ducking sprite has the height and width inverted from the regular sprite.
class Dinosaur {
  public:
    Dinosaur(bool** frame1_input, bool** frame2_input, bool** duck_frame1_input, bool** duck_frame2_input, int MovementFrames, int initial_x_input, int initial_y_input, int width, int height)
    {
      frame1 = frame1_input;
      frame2 = frame2_input;
      frames = MovementFrames;
      initial_x = initial_x_input;
      initial_y = initial_y_input;
      frame_width = width;
      frame_height = height;
      duck_frame1 = duck_frame1_input;
      duck_frame2 = duck_frame2_input;
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
    int getInitX()
    {
       return initial_x;
    }
    int getInitY()
    {
       return initial_y;
    }
    void setInitY(int y)
    {
      initial_y = y;
    }
    void setInitX(int x)
    {
      initial_x = x;
    }
    int getW()
    {
      return frame_width;
    }
    int getH()
    {
      return frame_height;
    }
  private:
    bool** frame1;
    bool** frame2;
    bool** duck_frame1;
    bool** duck_frame2;
    int frames;
    int initial_x;
    int initial_y;
    int frame_width;
    int frame_height;
};


#endif
