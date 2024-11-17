#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H


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
