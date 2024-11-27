#include "runner.h"
#include "controller.cpp"
#include "face.h"
#include "dinosaur-game.h"
#include "ground-obstacle.h"
#include "animated-sprite.h"
#include "bird-obstacle.h"


#include <chrono>
#include <deque>

extern std::chrono::system_clock::time_point* controller1buttons;
extern std::chrono::system_clock::time_point* controller2buttons;

extern int g_red;
extern int g_blue;
extern int g_green;


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
    bool duckButton = false;
    bool collission = false;

    // Sprite work.
    int default_dino_x = 11;
    int default_dino_y = 7;
    int spriteGround = 11;

    bool crash = false;

    int birdW = 15;
    int birdH = 15;

    int dinoW = 12;
    int dinoH = 20;

    int doubleCactusW = 20;
    int doubleCactusH = 20;

    int cactusW = 20;
    int cactusH = 12;

    int multiCactusW = 36;
    int multiCactusH = 20;

    int dinoFrames = 5; // How many frames pass before switching to the other leg with running animation.
    int birdFrames = 3;

    bool** dino1 = FileToSprite("dino1", dinoH, dinoW);
    bool** dino2 = FileToSprite("dino2", dinoH, dinoW);

    bool** dino1Duck = FileToSprite("dino-ducking1", dinoW, dinoH);
    bool** dino2Duck = FileToSprite("dino-ducking2", dinoW, dinoH);


    bool** bird1 = FileToSprite("bird1", birdW, birdH);
    bool** bird2 = FileToSprite("bird2", birdW, birdH);

    bool** doublecactus1 = FileToSprite("double-cactus-1", doubleCactusW, doubleCactusH);
    bool** doublecactus2 = FileToSprite("double-cactus-2", doubleCactusW, doubleCactusH);

    bool** cactus1 = FileToSprite("cactus1", cactusW, cactusH);
    bool** cactus2 = FileToSprite("cactus2", cactusW, cactusH);

    bool** multicactus = FileToSprite("multi-cactus", multiCactusW, multiCactusH);


    Dinosaur* dinoSprite = new Dinosaur(dino1, dino2, dino1Duck, dino2Duck, dinoFrames, default_dino_x, default_dino_y, dinoW, dinoH);
    AnimatedSprite* birdSprite = new AnimatedSprite(bird1, bird2, birdFrames);

    std::deque<groundObstacle*> groundObstacleQueue;
    std::deque<birdObstacle*> birdObstacleQueue;


    while (!interrupt_received){

      for(int i = 0; i < 128; i++)
      {
        canvas()->SetPixel(i, ground[i], g_red, g_green, g_blue);
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
        groundObstacle* gobst = new groundObstacle(cactus1, spriteGround, 140, 20, 12);
        groundObstacleQueue.push_back(gobst);
      }

      if((frames % 200) == 0)
      {
        //spawn a bird
        birdObstacle* bobst = new birdObstacle(bird1, bird2, 10, 140, 15, 15, 15);
        birdObstacleQueue.push_back(bobst);
      }

      //Actual drawing
      canvas()->Clear();


      for(int i = 0; i < 128; i++)
      {
        canvas()->SetPixel(i, ground[i], g_red, g_green, g_blue);
      }

      //button = current_button_pushed(controller1buttons);
      //std::cout << "Button pushed:" << std::endl;
      //std::cout << button << std::endl;

      if(jump_button_pushed(controller1buttons))
      {
        if(jumpButtonPressed == false)
        {
          isJumping = true;
          startingVelocity = 3;
          jumpButtonPressed = true;

        }

        framesButtonHeld++;

      }
      else
      {
        if(jumpButtonPressed == true)
        {
          framesButtonHeld = 0;
          jumpButtonPressed = false;
        }
      }
      if(duck_button_pushed(controller1buttons))
      {
        dinoSprite->setDucking(true);
      }
      else
      {
        dinoSprite->setDucking(false);
      }

      double velocity = initialVelocity + (0.1 * framesButtonHeld);

      for(int i = 0; i < groundObstacleQueue.size(); i++)
      {
        drawSprite(groundObstacleQueue[i]->getSpriteReference(), 20, 12, spriteGround, groundObstacleQueue[i]->get_y(), g_red, g_green, g_blue);
        if(dinoSprite->detectCollission(20, 12, groundObstacleQueue[i]->get_x(), groundObstacleQueue[i]->get_y(), frames))
        {
          crash = true;
        }
      }

      for(int i = 0; i < birdObstacleQueue.size(); i++)
      {
        drawSprite(birdObstacleQueue[i]->getSpriteReference(frames), 15, 15, birdObstacleQueue[i]->get_x(), birdObstacleQueue[i]->get_y(), g_red, g_green, g_blue);
        if(dinoSprite->detectCollission(15, 15, birdObstacleQueue[i]->get_x(), birdObstacleQueue[i]->get_y(), frames))
        {
          crash = true;
        }
      }

      if(velocity > maxVelocity)
      {
        velocity = maxVelocity;
      }


      if(isJumping)
      {
        int delta = nextYvalue(frames_in_air, 11, prev_y, velocity);

        if(delta == 11)
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

      dinoSprite->setInitY(prev_y);
      drawSprite(dinoSprite->get_current_frame(frames), dinoSprite->getH(), dinoSprite->getW(), dinoSprite->getInitY(), dinoSprite->getInitX(), g_red, g_green, g_blue);


      for(int i = 0; i < groundObstacleQueue.size(); i++)
      {
        groundObstacleQueue[i]->set_y(groundObstacleQueue[i]->get_y() - 1);
        if(groundObstacleQueue[i]->get_y() < -20)
        {
          groundObstacleQueue.pop_back();
        }
      }

      for(int i = 0; i < birdObstacleQueue.size(); i++)
      {
        birdObstacleQueue[i]->set_y(birdObstacleQueue[i]->get_y() - 1);
        if(birdObstacleQueue[i]->get_y() < -20)
        {
          birdObstacleQueue.pop_back();
        }
      }

      usleep(game_speed);

      frames++;

      if(crash)
      {
        //game over
        return;
      }

    }
    return;

 }
 private:
   RGBMatrix *const matrix_;
   FrameCanvas *off_screen_canvas_;
};

