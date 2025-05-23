#ifndef DINOSAUR_GAME_H
#define DINOSAUR_GAME_H

#include <cmath>
#include <unistd.h>
#include <map>

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


// Returning a sprite based off of the digits from an input int

bool** intToSprite(int score, std::map<int, bool**> digitMap)
{

  int numDigits = 10;
  int width = numDigits * 4 - 1;


  std::string converted = std::to_string(score);
  std::string final;

  int scoreLength = converted.length();

  for(int i = 0; i < numDigits; i++)
  {
    if(i > scoreLength -1)
    {
      final = "0" + final;
    }
    else
    {
      final = final + converted[i];
    }
  }


  bool** scoreSprite = new bool*[5];
  //scoreSprite=new bool*[5];

  for(int i = 0; i < 5; i++)
  {
    scoreSprite[i] = new bool[width]{false};

    int iter = 0;

    for(int j = 0; j < numDigits; j++)
    {

      int digIter = (int)(final[j]) - 48;

      if (digitMap.find(digIter) == digitMap.end()) continue;

      for(int k = 0; k < 3; k++)
      {


        if(iter < width)
        {
          scoreSprite[i][iter] = digitMap[digIter][i][k];
          iter++;
        }

      /**
        if(digitMap[digIter][i][k])
        {
          scoreSprite[i][iter]=true;
          iter++;
        }
        else
        {
          scoreSprite[i][iter]=false;
          iter++;
        }
      **/
      }

      if (iter < width)
      {
        scoreSprite[i][iter]=false;
        iter++;
      }

    }

  }

  /***

  std::cout << "printing the score:" << std::endl;
  for(int i = 0; i < ; i++)
  {
    for(int j = 0; j < 39; j++)
    {
      std::cout << scoreSprite[i][j];
    }

    std::cout << std::endl;
  }
  ***/

  return scoreSprite;
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
      isDucking = false;
      duck_difference = abs(width - height);

    }
    bool detectCollission(int ref_height, int ref_width, int start_ref_x, int start_ref_y, int frames)
    {
      // Compare the two radii of the given sprite and the dino. Not perfect, but not n ^ 4 operations.

      int local_width;
      int local_height;

      int collissionDistance = 10;

      if(isDucking)
      {
        local_width = frame_width;
        local_height = frame_height;
      }
      else
      {
        local_height = frame_width;
        local_width = frame_height;
      }

      //int dino_rad_x = getInitX() + (local_width/2);
      //int dino_rad_y = getInitY() + (local_height/2);

      //int ref_rad_x = start_ref_x + (ref_width/2);
      //int ref_rad_y = start_ref_y + (ref_height/2);

      //int x_difference = ref_rad_y - dino_rad_x;
      //int y_difference = ref_rad_x - dino_rad_y;

      int dino_rad_x = getInitX() + (local_height/2);
      int dino_rad_y = getInitY() + (local_width/2);

      int ref_rad_x = start_ref_x + (ref_height/2);
      int ref_rad_y = start_ref_y + (ref_width/2);

      int x_difference = ref_rad_y - dino_rad_x;
      int y_difference = ref_rad_x - dino_rad_y;

      double distance = sqrt((x_difference * x_difference) + (y_difference * y_difference));

      if(distance < collissionDistance)
      {
        return true;
      }

      return false;
    }
    bool** get_current_frame(int totalFrames)
    {
      if((totalFrames % (frames * 2)) > frames)
      {
        if(isDucking) {
          return duck_frame1;
        }
        return frame1;
      }
      else
      {
        if(isDucking) {
          return duck_frame2;
        } else {
          return frame2;
        }
      }
    }
    int getInitX()
    {
      return initial_x;
    }
    int getInitY()
    {
      if(isDucking)
      {
        return initial_y + duck_difference;
      }
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
      if(isDucking) {
        return frame_height;
      } else {
        return frame_width;
      }
    }
    int getH()
    {
      if(isDucking) {
        return frame_width;
      } else {
        return frame_height;
      }
    }
    void setDucking(bool ducking)
    {
      isDucking = ducking;
    }
    bool getDucking()
    {
      return isDucking;
    }
    int getDuckDifference()
    {
      return duck_difference;
    }

  private:
    bool** frame1;
    bool** frame2;
    bool** duck_frame1;
    bool** duck_frame2;
    int duck_difference;
    int frames;
    int initial_x;
    int initial_y;
    int frame_width;
    int frame_height;
    bool isDucking;
};


#endif
