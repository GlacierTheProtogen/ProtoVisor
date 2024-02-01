#include "runner.h"
#include "face.h"
#include "led-matrix.h"
#include "controller.cpp"


#include <limits.h>
#include <math.h>
#include <chrono>


extern std::chrono::system_clock::time_point* controller1buttons;
extern std::chrono::system_clock::time_point* controller2buttons;

volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
  interrupt_received = true;
}


class ProtoFace : public Runner {
public:
  ProtoFace(RGBMatrix *m) : Runner(m), matrix_(m) {
    off_screen_canvas_ = m->CreateFrameCanvas();
  }
  void Run() override {
    uint32_t continuum = 0;

    //FIXME: The following should be objects.

    bool** happy = FileToFace("happy");
    bool** base = FileToFace("baseface");
    bool** heart = FileToFace("heart");
    bool** poker = FileToFace("poker");
    bool** angry = FileToFace("angry");
    bool** sad = FileToFace("sad");
    bool** uwu = FileToFace("uwu");

    bool** happyblink = FileToFace("happy-blink");
    bool** baseblink = FileToFace("baseface-blink");
    bool** heartblink = FileToFace("heart-blink");
    bool** pokerblink = FileToFace("poker-blink");
    bool** angryblink = FileToFace("angry-blink");
    bool** sadblink = FileToFace("sad-blink");


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

    drawFaceInput(base, 0);

    while (!interrupt_received) {

        /* If a button is pressed, maintain the same face that we have been
        drawing. If not, then go back to basic face until a new button is
        pressed */

        flowcounter++;
        drawNewFace = false;

        if((flowcounter % 10560000) < 556000 && isBlinking == false)
        {
            isBlinking = true;
            drawNewFace = true;
        }
        else if((flowcounter % 10560000) >= 556000 && isBlinking == true)
        {
            isBlinking = false;
            drawNewFace = true;
        }

        double cosign = 2 * cos(flowcounter / flowcycle);

        if(flowcountercompare != (int)(cosign))
        {
          //std::cout << flowcountercompare << std::endl;
          //std::cout << (int)(cosign) << std::endl << std::endl;

          flowcountercompare = (int)(cosign);
          drawNewFace = true;
        }

        if(prevFace != currentFace)
        {
          prevFace = currentFace;
          drawNewFace = true;
        }

	button = is_button_pushed(controller1buttons);

        // FIXME: The number of below if statements sucks. Maybe a dictionary of pointers?

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
          drawFaceInput(currentFace, (int)(cosign));
        }
     }

     flowcounter = flowcounter % (10 * flowcycle);



 }
 private:
   RGBMatrix *const matrix_;
   FrameCanvas *off_screen_canvas_;
};

