#include "runner.h"
#include "face.h"
#include "led-matrix.h"
#include "controller.cpp"


#include <limits.h>
#include <math.h>
#include <chrono>
#include <cassert>


extern std::chrono::system_clock::time_point* controller1buttons;
extern std::chrono::system_clock::time_point* controller2buttons;


class MenuFace : public Runner {
public:
  MenuFace(RGBMatrix *m) : Runner(m), matrix_(m) {
    off_screen_canvas_ = m->CreateFrameCanvas();
  }
  void Run() override {
    uint32_t continuum = 0;

    //FIXME: The following should be objects.

    bool** menu = FileToFace("menu-base", true);


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

    drawMenuInput(menu, 0);

    while (!interrupt_received) {

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

        if(prevMenu != currentMenu)
        {
          prevMenu = currentMenu;
          drawNewFace = true;
        }

	button = current_button_pushed(controller1buttons);

        // FIXME: The number of below if statements sucks. Maybe a dictionary of pointers?
        /*

	if(buttonPressed == true)
	{
	  if(button == 0)
	  {
            currentFace = base;
	    buttonPressed = false;
	  }
	}
	else
	{
	  if(button != 0)
	  {
	    buttonPressed = true;

	    if(button == 1)
	    {
              currentFace = sad;
	    }
	    else if(button == 2)
            {
              currentFace = angry;
	    }
	    else if(button == 3)
	    {
              currentFace = happy;
            }
            else if(button == 4)
            {
              currentFace = uwu;
            }
            else if(button == 5)
            {
              currentFace = poker;
            }
            else if(button == 6)
            {
              currentFace = heart;
            }
	 }
	}
        */

        if(drawNewFace == true)
        {
          drawMenuInput(currentMenu, (int)(cosign));
        }
     }

     flowcounter = flowcounter % (10 * flowcycle);



 }
 private:
   RGBMatrix *const matrix_;
   FrameCanvas *off_screen_canvas_;
};

