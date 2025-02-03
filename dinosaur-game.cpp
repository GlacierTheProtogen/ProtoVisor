#include "runner.h"
#include "controller.cpp"
#include "face.h"
#include "dinosaur-game.h"
#include "ground-obstacle.h"
#include "animated-sprite.h"
#include "bird-obstacle.h"
#include <map>

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

    int game_speed = 22000;
    int speed_iter = 0;


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

    std::map<int, bool**> digits;

    int digitH = 5;
    int digitW = 3;


    int dinoFrames = 5; // How many frames pass before switching to the other leg with running animation.
    int birdFrames = 3;

    bool** dino1 = FileToSprite("dinosaur-game/dino1", dinoH, dinoW);
    bool** dino2 = FileToSprite("dinosaur-game/dino2", dinoH, dinoW);

    bool** one = FileToSprite("numbers/one", digitH, digitW);
    bool** two = FileToSprite("numbers/two", digitH, digitW);
    bool** three = FileToSprite("numbers/three", digitH, digitW);
    bool** four = FileToSprite("numbers/four", digitH, digitW);
    bool** five = FileToSprite("numbers/five", digitH, digitW);
    bool** six = FileToSprite("numbers/six", digitH, digitW);
    bool** seven = FileToSprite("numbers/seven", digitH, digitW);
    bool** eight = FileToSprite("numbers/eight", digitH, digitW);
    bool** nine = FileToSprite("numbers/nine", digitH, digitW);
    bool** zero = FileToSprite("numbers/zero", digitH, digitW);

    digits.insert(std::pair<int, bool**>(0, zero));
    digits.insert(std::pair<int, bool**>(1, one));
    digits.insert(std::pair<int, bool**>(2, two));
    digits.insert(std::pair<int, bool**>(3, three));
    digits.insert(std::pair<int, bool**>(4, four));
    digits.insert(std::pair<int, bool**>(5, five));
    digits.insert(std::pair<int, bool**>(6, six));
    digits.insert(std::pair<int, bool**>(7, seven));
    digits.insert(std::pair<int, bool**>(8, eight));
    digits.insert(std::pair<int, bool**>(9, nine));


    bool** dino1Duck = FileToSprite("dinosaur-game/dino-ducking1", dinoW, dinoH);
    bool** dino2Duck = FileToSprite("dinosaur-game/dino-ducking2", dinoW, dinoH);


    bool** bird1 = FileToSprite("dinosaur-game/bird1", birdW, birdH);
    bool** bird2 = FileToSprite("dinosaur-game/bird2", birdW, birdH);

    bool** doublecactus1 = FileToSprite("dinosaur-game/double-cactus-1", doubleCactusW, doubleCactusH);
    bool** doublecactus2 = FileToSprite("dinosaur-game/double-cactus-2", doubleCactusW, doubleCactusH);

    bool** cactus1 = FileToSprite("dinosaur-game/cactus1", cactusW, cactusH);
    bool** cactus2 = FileToSprite("dinosaur-game/cactus2", cactusW, cactusH);

    bool** multicactus = FileToSprite("dinosaur-game/multi-cactus", multiCactusW, multiCactusH);


    Dinosaur* dinoSprite = new Dinosaur(dino1, dino2, dino1Duck, dino2Duck, dinoFrames, default_dino_x, default_dino_y, dinoW, dinoH);
    AnimatedSprite* birdSprite = new AnimatedSprite(bird1, bird2, birdFrames);

    std::deque<groundObstacle*> groundObstacleQueue;
    std::deque<birdObstacle*> birdObstacleQueue;


    while (!interrupt_received){

      speed_iter++;

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

      // If there are no more than two cactuses or birds on theplaying field at the same time, randomly spawn another one.
      // Birds get a random height and a random cactus gets spawned.

      bool birdJustSpawned = false;
      bool groundJustSpawned = false;


      for(int i = 0; i < birdObstacleQueue.size(); i++)
      {
        if(birdObstacleQueue[i]->get_y() > 80)
        {
          birdJustSpawned = true;
        }
      }

      for(int i = 0; i < groundObstacleQueue.size(); i++)
      {
        if(groundObstacleQueue[i]->get_y() > 80)
        {
          groundJustSpawned = true;
        }
      }

      int spawnChance = 600;

      int spawnGround = getRandInt() % spawnChance;
      int spawnBird = getRandInt() % spawnChance;

      if(spawnGround > spawnChance - 10  && groundObstacleQueue.size() < 3 && !birdJustSpawned && !groundJustSpawned && frames > 60)
      {
        //spawn an enemy
        bool** enemySprite;

        int enemyHeight;
        int enemyWidth;

        if(spawnGround >= spawnChance - 2)
        {
          enemySprite = cactus1;
          enemyHeight = cactusH;
          enemyWidth = cactusW;
        }
        else if(spawnGround >= spawnChance - 4)
        {
          enemySprite = cactus2;
          enemyHeight = cactusH;
          enemyWidth = cactusW;
        }
        else if(spawnGround >= spawnChance - 6)
        {
          enemySprite = multicactus;
          enemyHeight = multiCactusH;
          enemyWidth = multiCactusW;
        }
        else if(spawnGround >= spawnChance - 8)
        {
          enemySprite = doublecactus1;
          enemyHeight = doubleCactusH;
          enemyWidth = doubleCactusW;
        }
        else
        {
          enemySprite = doublecactus2;
          enemyHeight = doubleCactusH;
          enemyWidth = doubleCactusW;
        }

        groundObstacle* gobst = new groundObstacle(enemySprite, spriteGround + 2, 140, enemyWidth, enemyHeight);
        groundObstacleQueue.push_back(gobst);
      }

      if(spawnBird > spawnChance - 10 && birdObstacleQueue.size() < 3 && !groundJustSpawned)
      {
        //spawn a bird

        int birdBuffer = spawnBird - spawnChance - 10;

        birdObstacle* bobst = new birdObstacle(bird1, bird2, 12 + birdBuffer, 140, 15, 15, 15);
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
        drawSprite(groundObstacleQueue[i]->getSpriteReference(), groundObstacleQueue[i]->get_h(), groundObstacleQueue[i]->get_w(), spriteGround + 2, groundObstacleQueue[i]->get_y(), g_red, g_green, g_blue);
        if(dinoSprite->detectCollission(groundObstacleQueue[i]->get_h(), groundObstacleQueue[i]->get_w(), groundObstacleQueue[i]->get_x(), groundObstacleQueue[i]->get_y(), frames))
        {
          crash = true;
        }
      }

      for(int i = 0; i < birdObstacleQueue.size(); i++)
      {
        drawSprite(birdObstacleQueue[i]->getSpriteReference(frames), birdW, birdH, birdObstacleQueue[i]->get_x(), birdObstacleQueue[i]->get_y(), g_red, g_green, g_blue);
        if(dinoSprite->detectCollission(birdW, birdH, birdObstacleQueue[i]->get_x(), birdObstacleQueue[i]->get_y(), frames))
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
          groundObstacleQueue.erase(groundObstacleQueue.begin());
        }
      }

      for(int i = 0; i < birdObstacleQueue.size(); i++)
      {
        birdObstacleQueue[i]->set_y(birdObstacleQueue[i]->get_y() - 1);
        if(birdObstacleQueue[i]->get_y() < -20)
        {
          birdObstacleQueue.erase(birdObstacleQueue.begin());
        }
      }


      int score = frames;

      bool** scoreBoard = intToSprite(score, digits);

      drawSprite(scoreBoard, 5, 39, 0, 0, g_red, g_green, g_blue);

      for (int i = 0; i < 5; i++)
      {
        delete[] scoreBoard[i];
      }

      delete[] scoreBoard;

      usleep(game_speed);

      game_speed = game_speed - 2;

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

