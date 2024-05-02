#include "runner.h"
#include "face.h"
#include "controller.cpp"
#include "menu.cpp"

#include <limits.h>
#include <math.h>
#include <chrono>
#include <cassert>


extern std::chrono::system_clock::time_point* controller1buttons;
extern std::chrono::system_clock::time_point* controller2buttons;

class ProtoFace : public Runner {
public:
  ProtoFace(RGBMatrix *m) : Runner(m), matrix_(m) {
    off_screen_canvas_ = m->CreateFrameCanvas();
  }
  void Run() override {
    //FIXME: The following should be objects.

    bool** happy = FileToFace("happy", false);
    bool** base = FileToFace("baseface", false);
    bool** heart = FileToFace("heart", false);
    bool** poker = FileToFace("poker", false);
    bool** angry = FileToFace("angry", false);
    bool** sad = FileToFace("sad", false);
    bool** uwu = FileToFace("uwu", false);

    bool** happyblink = FileToFace("happy-blink", false);
    bool** baseblink = FileToFace("baseface-blink", false);
    bool** heartblink = FileToFace("heart-blink", false);
    bool** pokerblink = FileToFace("poker-blink", false);
    bool** angryblink = FileToFace("angry-blink", false);
    bool** sadblink = FileToFace("sad-blink", false);


    int flowcycle = 1280000; // Integer that is used to divide x in the cosign equation. Higher = slower face floating.
    int flowcounter = 0; // Incremented integer that is used for the cosign function.
    int flowcountercompare = -8; // Used to keep track of the last integer that was used in the cosign function.
    int curButton = -1; // Current button that is pressed. Start off with an arbitrary number that doesn't map to a button.
    int button; // Button that is pressed.
    bool isBlinking = false;
    bool buttonPressed = false; // Wether or not a button is pressed.
    bool drawNewFace; // Decides wether we are drawing a face on the next iteration of the loop
    bool** currentFace = base; // Current face
    bool** prevFace = base; // Face to keep track of what the previous face was
    int home_button_counter1 = 0; // Keeps track of how long the home button on controller1 has been pushed down
    int home_button_counter2 = 0; // Keeps track of how long the home button on controller2 has been pushed down
    int home_time = 3556000; // Time to keep track of the home button being pushed down before going to menu
    int eye_open_counter =  10560000; // Time to keep eyes open before blinking
    int eye_closed_counter = eye_open_counter / 20; // Time eyes are kept closed for blinking

    int rave_r = 0;
    int rave_g = 0;
    int rave_b = 0;
    bool rave_mode = false;
    int rave_switch = 0;
    int prev_rave_switch = 0;
    int rave_loop = 200000;

    drawFaceInput(base, 0, 0, 0, 255);

    while (!interrupt_received) {

        /* If a button is pressed, maintain the same face that we have been
        drawing. If not, then go back to basic face until a new button is
        pressed */

        flowcounter++;
        drawNewFace = false;

        rave_switch = (flowcounter - (flowcounter % rave_loop)) % 765;


        if((prev_rave_switch != rave_switch) && rave_mode == true)
        {
          if(rave_switch <=255) {
            int c = rave_switch;
            rave_b = 255 - c;
            rave_r = c;
          }
          else if(rave_switch > 255 && rave_switch <= 511) {
            int c = rave_switch - 256;
            rave_r = 255 - c;
            rave_g = c;
          }
          else {
            int c = rave_switch - 512;
            rave_g = 255 - c;
            rave_b = c;
          }

          drawNewFace = true;
        }

        prev_rave_switch = rave_switch;

	if(is_button_pushed(controller1buttons, 12))
        {
          home_button_counter1++;
        }
        else
        {
	  home_button_counter1 = 0;
        }

        if(is_button_pushed(controller2buttons, 12))
        {
          home_button_counter2++;
        }

        else
        {
          home_button_counter2 = 0;
        }

        if(home_button_counter1 > home_time || home_button_counter2 > home_time)
        {
          Runner *runner = new MenuFace(matrix_);
          runner->Run();

          delete runner;

        }

        if((flowcounter % eye_open_counter) < eye_closed_counter && isBlinking == false)
        {
          isBlinking = true;
          drawNewFace = true;
        }
        else if((flowcounter % eye_open_counter) >= eye_closed_counter && isBlinking == true)
        {
          isBlinking = false;
          drawNewFace = true;
        }

        double cosign = 2 * cos(flowcounter / flowcycle);

        if(flowcountercompare != (int)(cosign))
        {
          flowcountercompare = (int)(cosign);
          drawNewFace = true;
        }

        if(prevFace != currentFace)
        {
          prevFace = currentFace;
          drawNewFace = true;
        }

	button = current_button_pushed(controller1buttons);

        // FIXME: The number of below if statements sucks. Maybe a dictionary of pointers?

	if(buttonPressed == true)
	{
	  if(button == 0)
	  {
            currentFace = base;
	    buttonPressed = false;
            rave_mode = false;
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
              rave_mode = false;
	    }
	    else if(button == 2)
            {
              currentFace = angry;
              rave_mode = false;
	    }
	    else if(button == 3)
	    {
              currentFace = happy;
              rave_mode = false;
            }
            else if(button == 4)
            {
              currentFace = uwu;
              rave_mode = false;
            }
            else if(button == 5)
            {
              currentFace = poker;
              rave_mode = false;
            }
            else if(button == 6)
            {
              currentFace = heart;
              rave_mode = false;
            }
            else if(button == 7)
            {
              rave_mode = true;
              currentFace = happy;
            }
	 }
	}


        if(isBlinking == true)
        {
          if(currentFace == angry)
          {
            currentFace = angryblink;
          }
          else if(currentFace == sad)
          {
            currentFace = sadblink;
          }
          else if(currentFace == poker)
          {
            currentFace = pokerblink;
          }
          else if(currentFace == heart)
          {
            currentFace = heartblink;
          }
          else if(currentFace == base)
          {
            currentFace = baseblink;
          }

        }

        if(isBlinking == false)
        {
         if(currentFace == angryblink)
         {
           currentFace = angry;
         }
         else if(currentFace == sadblink)
         {
           currentFace = sad;
         }
         else if(currentFace == pokerblink)
         {
           currentFace = poker;
         }
         else if(currentFace == heartblink)
         {
           currentFace = heart;
         }
         else if(currentFace == baseblink)
         {
           currentFace = base;
         }
        }


        if(drawNewFace == true)
        {
          if(rave_mode)
          {
            drawFaceInput(currentFace, (int)(cosign), rave_r, rave_b, rave_g);
          }
          else
          {
            drawFaceInput(currentFace, (int)(cosign), 0, 0, 255);
          }
        }

     }


 }
 private:
   RGBMatrix *const matrix_;
   FrameCanvas *off_screen_canvas_;
};

