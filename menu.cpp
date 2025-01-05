#include "runner.h"
#include "face.h"
#include "led-matrix.h"
#include "controller.cpp"
#include "countdown.cpp"
#include "snake.cpp"
#include "2player-menu.cpp"
#include "pong.cpp"
#include "simon-says.cpp"
#include "dinosaur-game.cpp"
#include "color-menu.cpp"

#include <limits.h>
#include <math.h>
#include <chrono>
#include <cassert>
#include <map>

extern std::chrono::system_clock::time_point* controller1buttons;
extern std::chrono::system_clock::time_point* controller2buttons;

extern int g_red;
extern int g_blue;
extern int g_green;


class MenuFace : public Runner {
public:
  MenuFace(RGBMatrix *m) : Runner(m), matrix_(m) {
    off_screen_canvas_ = m->CreateFrameCanvas();
  }
  void drawSprite(bool** face, int height, int width, int start_x, int start_y, int r, int b, int g) {
    for(int i = 0; i < height; i++)
    {
      for(int j = 0; j < width; j++)
      {
        if(face[i][j] == true)
        {
          //canvas()->SetPixel(j + start_y, i + start_x, r, g, b);
          canvas()->SetPixel(j + start_y, i + start_x, r, g, b);
        }
      }

    }
  }
  void transitionAnimation(bool** face1, bool** face2, bool left, int height, int width, int start_x, int start_y, int r, int b, int g, int shift) {

    if(left)
    {
      for(int k = 0; k < width; k++)
      {
        canvas()->Clear();

        for(int i = 0; i < height; i++)
        {
          for(int j = k; j < width; j++)
          {
            if(face2[i][j] == true)
            {
              //canvas()->SetPixel(j + start_y, i + start_x, r, g, b);
              canvas()->SetPixel(j + start_y - k, i + start_x, r, g, b);
              canvas()->SetPixel(j + start_y - k + shift, i + start_x, r, g, b);
            }
          }

        }

        for(int i = 0; i < height; i++)
        {
          for(int j = 0; j < k; j++)
          {
            if(face1[i][j] == true)
            {
              //canvas()->SetPixel(j + start_y, i + start_x, r, g, b);
              canvas()->SetPixel(j + start_y + (width - k - 1), i + start_x, r, g, b);
              canvas()->SetPixel(j + start_y + (width - k - 1) + shift, i + start_x, r, g, b);
            }
          }

        }

        usleep(10000);

        }
      }
      else
      {
        for(int k = width-1; k >= 0; k--)
        {
          canvas()->Clear();

          for(int i = 0; i < height; i++)
          {
            for(int j = width - 1; j >= k; j--)
            {
              if(face1[i][j] == true)
              {
                //canvas()->SetPixel(j + start_y, i + start_x, r, g, b);
                canvas()->SetPixel(j + start_y - k, i + start_x, r, g, b);
                canvas()->SetPixel(j + start_y - k + shift, i + start_x, r, g, b);
              }
            }

          }

          for(int i = 0; i < height; i++)
          {
            for(int j = k; j >= 0; j--)
            {
              if(face2[i][j] == true)
              {
                //canvas()->SetPixel(j + start_y, i + start_x, r, g, b);
                canvas()->SetPixel(j + start_y + (width - k - 1), i + start_x, r, g, b);
                canvas()->SetPixel(j + start_y + (width - k - 1) + shift, i + start_x, r, g, b);
              }
            }

          }

          usleep(10000);
        }

    }
  }
  void Run() override {
    bool** menu = FileToFace("menu-base", true);
    canvas()->Clear();

    int flowcycle = 1280000; // Integer that is used to divide x in the cosign equation. Higher = slower face floating.
    int flowcounter = 0; // Incremented integer that is used for the cosign function.
    int flowcountercompare = -8; // Used to keep track of the last integer that was used in the cosign function.
    int button; // Button that is pressed.
    bool buttonPressed = false; // Wether or not a button is pressed.
    bool** currentMenu = menu; // Current face
    bool drawNewFace = true;
    bool** prevMenu = menu;

    int menuSpriteW = 48;
    int menuSpriteH = 24;

    std::string menuList[5];
    std::map<int, bool**> MenuScreens;


    int shift = 54;

    bool** colorFace = FileToSprite("menu/color", menuSpriteH, menuSpriteW);
    bool** pongFace = FileToSprite("menu/pong", menuSpriteH, menuSpriteW);
    bool** dinoFace = FileToSprite("menu/dinosaur-game", menuSpriteH, menuSpriteW);
    bool** snakeFace = FileToSprite("menu/snake", menuSpriteH, menuSpriteW);
    bool** simonFace = FileToSprite("menu/simon-says", menuSpriteH, menuSpriteW);
    bool** backFace = FileToSprite("menu/back", menuSpriteH, menuSpriteW);

    bool** rightArrow = FileToSprite("menu/right-arrow", 9, 5);
    bool** leftArrow = FileToSprite("menu/left-arrow", 9, 5);

    bool changedOption = false;

    MenuScreens.insert(std::pair<int, bool**>(0, colorFace));
    MenuScreens.insert(std::pair<int, bool**>(1, pongFace));
    MenuScreens.insert(std::pair<int, bool**>(2, dinoFace));
    MenuScreens.insert(std::pair<int, bool**>(3, snakeFace));
    MenuScreens.insert(std::pair<int, bool**>(4, simonFace));
    MenuScreens.insert(std::pair<int, bool**>(5, backFace));

    int sel = 1;
    int prevsel = sel;
    bool left = true;

    drawSprite(pongFace, menuSpriteH, menuSpriteW, 2, 16, g_red, g_blue, g_green);

    while (!interrupt_received) {
        changedOption = false;
        /* If a button is pressed, maintain the same face that we have been
        drawing. If not, then go back to basic face until a new button is
        pressed */

        flowcounter++;
        drawNewFace = false;


        double cosign = 2 * cos(flowcounter / flowcycle);
       

        if(flowcountercompare != (int)(cosign))
        {
          flowcountercompare = (int)(cosign);
          drawNewFace = true;
        }



	button = current_button_pushed(controller1buttons);


        // FIXME: The number of below if statements sucks. Maybe a dictionary of pointers?

	if(buttonPressed == true)
	{
	  if(button == 0)
	  {
	    buttonPressed = false;
            drawNewFace = true;
	  }
	}
	else
	{
	  if(button != 0)
	  {
	    buttonPressed = true;
            drawNewFace = true;

	    if(button == 19 || button == 8 || button == 12)
            {
              prevsel = sel;

              if(sel == 5)
              {
                sel = 0;
              }
              else
              {
                sel = sel + 1;
              }

              left = true;
              changedOption = true;
	    }
	    else if(button == 18 || button == 7 || button == 11)
	    {
              prevsel = sel;

              if(sel == 0)
              {
                sel = 5;
              }
              else
              {
                sel = sel - 1;
              }

              left = false;
              changedOption = true;
            }
            else if(button == 2 && sel == 3)
            {
              TwoPlayerMenuFace * twoplayer = new TwoPlayerMenuFace(matrix_);
              int option = twoplayer->GetAnswer();

              if(option > 0)
              {
                CountDown * countdown = new CountDown(matrix_);
                countdown->Run();
                Snake * runner = new Snake(matrix_);
                runner->RunSnake(option);
                delete runner;
              }

              delete twoplayer;
              return;
            }
            else if(button == 2 && sel == 0)
            {
              ColorMenu * colorM = new ColorMenu(matrix_);
              colorM->Run();
              delete colorM;
              return;
            }
            else if(button == 2 && sel == 4)
            {
              SimonSays * simon = new SimonSays(matrix_);
              simon->Run();
              delete simon;
              return;
            }
            else if(button == 2 && sel == 2)
            {
              DinosaurGame * dinogame = new DinosaurGame(matrix_);
              dinogame->Run();
              delete dinogame;
              return;
            }
            else if(button == 2 && sel == 1)
            {
              CountDown * countdown = new CountDown(matrix_);
              countdown->Run();
              Pong * runner = new Pong(matrix_);
              runner->Run();
              delete runner;
              return;
            }

            else if(button == 4 || (button == 2 && sel == 5))
            {
              return;
            }

            if(changedOption)
            {
              //drawSprite(MenuScreens[sel], menuSpriteH, menuSpriteW, 4, 20, g_red, g_blue, g_green);
              transitionAnimation(MenuScreens[sel], MenuScreens[prevsel], left, menuSpriteH, menuSpriteW, 2, 13, g_red, g_blue, g_green, shift);
              drawSprite(MenuScreens[sel], menuSpriteH, menuSpriteW, 2, 13, g_red, g_blue, g_green);
              drawSprite(MenuScreens[sel], menuSpriteH, menuSpriteW, 2, 13 + shift, g_red, g_blue, g_green);
            }


	 }

         if(drawNewFace)
         {
           canvas()->Clear();
           drawSprite(leftArrow, 9, 5, 10, 12 - flowcountercompare, g_red, g_blue, g_green);
           drawSprite(rightArrow, 9, 5, 10, 58 + flowcountercompare, g_red, g_blue, g_green);

           drawSprite(leftArrow, 9, 5, 10, 12 - flowcountercompare + shift, g_red, g_blue, g_green);
           drawSprite(rightArrow, 9, 5, 10, 58 + flowcountercompare + shift, g_red, g_blue, g_green);

           drawSprite(MenuScreens[sel], menuSpriteH, menuSpriteW, 2, 13, g_red, g_blue, g_green);
           drawSprite(MenuScreens[sel], menuSpriteH, menuSpriteW, 2, 13 + shift, g_red, g_blue, g_green);
         }

	}
     }

 }
 private:
   RGBMatrix *const matrix_;
   FrameCanvas *off_screen_canvas_;
};
