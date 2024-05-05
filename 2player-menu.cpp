#include "runner.h"
#include "face.h"
#include "led-matrix.h"
#include "controller.cpp"
#include "menu.h"

#include <limits.h>
#include <math.h>
#include <chrono>
#include <cassert>
#include <map>

extern std::chrono::system_clock::time_point* controller1buttons;
extern std::chrono::system_clock::time_point* controller2buttons;


class TwoPlayerMenuFace : public Runner {
public:
  TwoPlayerMenuFace(RGBMatrix *m) : Runner(m), matrix_(m) {
    off_screen_canvas_ = m->CreateFrameCanvas();
  }
  void Run() override {
    std::cout << "ERROR. Run function for 2playermenu is GetAnswer()" << std::endl;
  }
  int GetAnswer() {
    uint32_t continuum = 0;

    bool** menu = FileToFace("two-players", true);

    int flowcycle = 1280000; // Integer that is used to divide x in the cosign equation. Higher = slower face floating.
    int flowcounter = 0; // Incremented integer that is used for the cosign function.
    int flowcountercompare = -8; // Used to keep track of the last integer that was used in the cosign function.
    int curButton = -1; // Current button that is pressed. Start off with an arbitrary number that doesn't map to a button.
    int button; // Button that is pressed.
    bool isBlinking = false;
    bool buttonPressed = false; // Wether or not a button is pressed.
    bool MenuSelect; // Decides wether we are drawing a face on the next iteration of the loop
    bool** currentMenu = menu; // Current face
    bool drawNewFace = true;
    bool** prevMenu = menu;

    IntTuple* OneLeftCoords = new IntTuple(2, 4);
    IntTuple* TwoLeftCoords = new IntTuple(2, 14);
    IntTuple* OneRightCoords = new IntTuple(66, 4);
    IntTuple* TwoRightCoords = new IntTuple(66, 14);

    changeOption(currentMenu, OneRightCoords, OneLeftCoords);
    drawFullInput(currentMenu, 0);

    int curselection = 1;

    // Initially, the A button will be still selected from the first menu.
    bool firstrun = false;

    while (!interrupt_received) {

        /* If a button is pressed, maintain the same face that we have been
        drawing. If not, then go back to basic face until a new button is
        pressed */

        drawNewFace = false;


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

	    if(button == 10 || button == 6 || button == 9 || button == 5)
            {
              if(curselection == 1)
              {
                drawNewFace = true;
                curselection = 2;
              }
              else
              {
                drawNewFace = true;
                curselection = 1;
              }
	    }
            else if(button == 2)
            {
              if(firstrun)
              {
                return curselection;
              }
              else
              {
                firstrun = true;
              }
            }

            else if(button == 4)
            {
              return 0;
            }

            if(drawNewFace)
            {
              if(curselection == 2)
              {
                changeOption(currentMenu, OneLeftCoords, TwoLeftCoords);
                changeOption(currentMenu, OneRightCoords, TwoRightCoords);
              }
              else
              {
                changeOption(currentMenu, TwoLeftCoords, OneLeftCoords);
                changeOption(currentMenu, TwoRightCoords, OneRightCoords);
              }
            }
	 }
	}


        if(drawNewFace == true)
        {
          drawFullInput(currentMenu, (int)(0));
        }
     }

 }
 private:
   RGBMatrix *const matrix_;
   FrameCanvas *off_screen_canvas_;
};

