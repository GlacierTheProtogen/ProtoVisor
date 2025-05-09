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
  void Run() override {
    bool** menu = FileToFace("menu-base", true);

    int flowcycle = 1280000; // Integer that is used to divide x in the cosign equation. Higher = slower face floating.
    int flowcounter = 0; // Incremented integer that is used for the cosign function.
    int flowcountercompare = -8; // Used to keep track of the last integer that was used in the cosign function.
    int button; // Button that is pressed.
    bool buttonPressed = false; // Wether or not a button is pressed.
    bool** currentMenu = menu; // Current face
    bool drawNewFace = true;
    bool** prevMenu = menu;

    IntTuple* SettingsCoords = new IntTuple(2, 4);
    IntTuple* PongCoords = new IntTuple(2, 14);
    IntTuple* DinosaurGameCoords = new IntTuple(2, 24);
    IntTuple* SnakeCoords = new IntTuple(66, 4);
    IntTuple* SimonSaysCoords = new IntTuple(66, 14);
    IntTuple* BackCoords = new IntTuple(66, 24);

    std::map<int, IntTuple*> MenuItems;
    MenuItems.insert(std::make_pair(0, SettingsCoords));
    MenuItems.insert(std::make_pair(1, PongCoords));
    MenuItems.insert(std::make_pair(2, DinosaurGameCoords));
    MenuItems.insert(std::make_pair(3, SnakeCoords));
    MenuItems.insert(std::make_pair(4, SimonSaysCoords));
    MenuItems.insert(std::make_pair(5, BackCoords));

    changeOption(currentMenu, SettingsCoords, SettingsCoords);
    drawFullInput(currentMenu, 0, 0, 0, 255);

    // Treat the menu items as an array. Manipulate this number based on what
    // Input the user presses on the controller. Finally, map out this
    // Selection on the menu.
    int sel = 0;
    int prevsel = 0;

    while (!interrupt_received) {

        /* If a button is pressed, maintain the same face that we have been
        drawing. If not, then go back to basic face until a new button is
        pressed */

        flowcounter++;
        drawNewFace = false;

        prevsel = sel;

        double cosign = 2 * cos(flowcounter / flowcycle);

        if(flowcountercompare != (int)(cosign))
        {
          flowcountercompare = (int)(cosign);
          drawNewFace = true;
        }

        if(prevMenu != currentMenu)
        {
          prevMenu = currentMenu;
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

	    if(button == 10 || button == 6 || button == 17)
            {
              if(sel == 5)
              {
                sel = 0;
              }
              else
              {
                sel = sel + 1;
              }
	    }
	    else if(button == 9 || button == 5 || button == 16)
	    {
              if(sel == 0)
              {
                sel = 5;
              }
              else
              {
                sel = sel - 1;
              }
            }
            else if(button == 12 || button == 8 || button == 7 || button == 11 || button == 18 || button == 19)
            {
              sel = sel + 3;
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
            }
            else if(button == 2 && sel == 0)
            {
              ColorMenu * colorM = new ColorMenu(matrix_);
              colorM->Run();
              delete colorM;
            }
            else if(button == 2 && sel == 4)
            {
              SimonSays * simon = new SimonSays(matrix_);
              simon->Run();
              delete simon;
            }
            else if(button == 2 && sel == 2)
            {
              DinosaurGame * dinogame = new DinosaurGame(matrix_);
              dinogame->Run();
              delete dinogame;
            }
            else if(button == 2 && sel == 1)
            {
              CountDown * countdown = new CountDown(matrix_);
              countdown->Run();
              Pong * runner = new Pong(matrix_);
              runner->Run();
              delete runner;
            }

            else if(button == 4 || (button == 2 && sel == 5))
            {
              return;
            }

            //sel = abs(sel % 6);

            sel = abs(sel) % 6;
            changeOption(currentMenu, MenuItems[prevsel], MenuItems[sel]);

	 }
	}


        if(drawNewFace == true)
        {
          drawFullInput(currentMenu, (int)(cosign), g_red, g_green, g_blue);
        }
     }

 }
 private:
   RGBMatrix *const matrix_;
   FrameCanvas *off_screen_canvas_;
};
