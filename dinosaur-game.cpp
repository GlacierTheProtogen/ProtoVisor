#include "runner.h"
#include "controller.cpp"
#include "face.h"
#include "dinosaur-game.h"
#include "ground-obstacle.h"
#include "animated-sprite.h"

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

    // Jumping physics.

    bool isJumping = false;
    int startingVelocity = 0;
    int framesButtonHeld = 0;
    bool jumpButtonPressed = false;
    bool longJump = false;
    int prev_y = 23;
    int frames_in_air = 0;
    const double initialVelocity = 1.8;
    const double maxVelocity = 2.3;


    // Sprite work.
    int default_dino_x = 12;
    int default_dino_y = 7;
    int spriteGround = 12;

    int dinoW = 12;
    int dinoH = 20;


    bool** dino1 = FileToSprite("dino1", dinoH, dinoW);
    bool** dino2 = FileToSprite("dino2", dinoH, dinoW);
    bool** dino1Duck = FileToSprite("dino1-ducking1", dinoW, dinoH);
    bool** dino2Duck = FileToSprite("dino2-ducking2", dinoW, dinoH);


    bool** bird1 = FileToSprite("bird1", 30, 30);
    bool** bird2 = FileToSprite("bird2", 30, 30);

    bool** doublecactus1 = FileToSprite("double-cactus-1", 20, 20);
    bool** doublecactus2 = FileToSprite("double-cactus-2", 20, 20);

    bool** cactus1 = FileToSprite("cactus1", 20, 12);
    bool** cactus2 = FileToSprite("cactus2", 20, 12);

    bool** multicactus = FileToSprite("multi-cactus", 36, 20);

    Dinosaur* dinoSprite = new Dinosaur(dino1, dino2, dino1Duck, dino2Duck, 5, default_dino_x, default_dino_y, dinoW, dinoH);
    AnimatedSprite* birdSprite = new AnimatedSprite(bird1, bird2, 3);

    std::deque<groundObstacle*> groundObstacleQueue;

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


      if((frames % 200) == 0)
      {
        //spawn an enemy
        groundObstacle* gobst = new groundObstacle(cactus1, 140, 20, 12);
        groundObstacleQueue.push_back(gobst);
      }

      //Actual drawing

      canvas()->Clear();


      for(int i = 0; i < 128; i++)
      {
        canvas()->SetPixel(i, ground[i], 0, 0, 255);
      }

      button = current_button_pushed(controller1buttons);

      if(button == 2)
      {
        if(jumpButtonPressed == false)
        {
          isJumping = true;
          startingVelocity = 3;
          jumpButtonPressed = true;
        }

        framesButtonHeld++;

      }
      if(button == 0)
      {
        if(jumpButtonPressed == true)
        {
          framesButtonHeld = 0;
          jumpButtonPressed = false;
        }
      }

      double velocity = initialVelocity + (0.1 * framesButtonHeld);

      for(int i = 0; i < groundObstacleQueue.size(); i++)
      {
        drawSprite(groundObstacleQueue[i]->getSpriteReference(), 20, 12, spriteGround, groundObstacleQueue[i]->get_y(), 0, 0, 255);
      }

      if(velocity > maxVelocity)
      {
        velocity = maxVelocity;
      }

      if(isJumping)
      {
        int delta = nextYvalue(frames_in_air, 12, prev_y, velocity);

        if(delta == 12)
        {
          isJumping = false;
          frames_in_air = 0;
        }
        else
        {
          frames_in_air++;
        }

        prev_y = delta;
      }

      //std::cout << prev_y << std::endl;

      drawSprite(dinoSprite->get_current_frame(frames), 20, 12, prev_y, default_dino_y, 0, 0, 255);


      for(int i = 0; i < groundObstacleQueue.size(); i++)
      {
        groundObstacleQueue[i]->set_y(groundObstacleQueue[i]->get_y() - 1);
        if(groundObstacleQueue[i]->get_y() < -20)
        {
          groundObstacleQueue.pop_back();
        }
      }

      usleep(game_speed);

      frames++;

    }
    return;

 }
 private:
   RGBMatrix *const matrix_;
   FrameCanvas *off_screen_canvas_;
};

