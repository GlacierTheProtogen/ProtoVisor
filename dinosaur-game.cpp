#include "runner.h"
#include "controller.cpp"
#include "face.h"
#include "dinosaur-game.h"

#include <chrono>
#include <deque>

extern std::chrono::system_clock::time_point* controller1buttons;
extern std::chrono::system_clock::time_point* controller2buttons;


class DinosaurGame : public Runner {
public:
  DinosaurGame(RGBMatrix *m) : Runner(m), matrix_(m) {
    off_screen_canvas_ = m->CreateFrameCanvas();
  }
  void drawSprite(bool** face, int height, int width, int start_x, int start_y, int r, int b, int g) {
    for(int i = 0; i < height; i++)
    {
      for(int j = 0; j < width; j++)
      {
        if(face[i][j] == true)
        {
          canvas()->SetPixel(j + start_y, i + start_x, r, b, g);
        }
      }
    }
  }
  void Run() override {
    //FIXME: The following should be objects.

    int button; // Button that is pressed.
    bool buttonPressed = false; // Wether or not a button is pressed.

    int game_speed = 25000;

    canvas()->Clear();

    int frames = 0;

    // Continuously draw the ground. have divets every once in a while.
    int lumps [5] = { 0, 1, 1, 0, 0 };
    int ground_lump_iter = 4;
    bool lump_direction = false;


    std::deque<int> ground;

    for(int i = 0; i < 128; i++)
    {
      ground.push_back(23);
    }

    int default_dino_x = 12;
    int default_dino_y = 7;

    bool** dino1 = FileToSprite("dino1", 20, 12);
    bool** dino2 = FileToSprite("dino2", 20, 12);

    bool** bird1 = FileToSprite("bird1", 30, 30);
    bool** bird2 = FileToSprite("bird2", 30, 30);

    bool** doublecactus1 = FileToSprite("double-cactus-1", 20, 20);
    bool** doublecactus2 = FileToSprite("double-cactus-2", 20, 20);

    bool** cactus1 = FileToSprite("cactus1", 12, 20);
    bool** cactus2 = FileToSprite("cactus2", 12, 20);

    bool** multicactus = FileToSprite("multi-cactus", 36, 20);

    AnimatedSprite* dinoSprite = new AnimatedSprite(dino1, dino2, 5);
    AnimatedSprite* birdSprite = new AnimatedSprite(bird1, bird2, 3);


    while (!interrupt_received){

      for(int i = 0; i < 128; i++)
      {
        canvas()->SetPixel(i, ground[i], 0, 0, 255);
      }

      int lumpchance = (getRandInt() % 64);
      int lumpdir = (getRandInt() % 2);

      if(ground_lump_iter != 4)
      {
        ground_lump_iter++;
      }

      if(lumpchance == 1)
      {
        ground_lump_iter = 0;
      }

      ground.pop_front();

      if(lumpdir)
      {
        ground.push_back(23 + lumps[ground_lump_iter]);
      }
      else
      {
        ground.push_back(23 - lumps[ground_lump_iter]);
      }



      //Actual drawing

      canvas()->Clear();

      drawSprite(dinoSprite->get_current_frame(frames), 20, 12, default_dino_x, default_dino_y, 0, 0, 255);

      for(int i = 0; i < 128; i++)
      {
        canvas()->SetPixel(i, ground[i], 0, 0, 255);
      }

      button = current_button_pushed(controller1buttons);
      usleep(game_speed);

      frames++;

    }
    return;

 }
 private:
   RGBMatrix *const matrix_;
   FrameCanvas *off_screen_canvas_;
};

