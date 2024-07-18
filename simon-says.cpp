#include "runner.h"
#include "face.h"
#include "controller.cpp"


#include <chrono>
#include <vector>

extern std::chrono::system_clock::time_point* controller1buttons;
extern std::chrono::system_clock::time_point* controller2buttons;



class SimonSays : public Runner {
public:
  SimonSays(RGBMatrix *m) : Runner(m), matrix_(m) {
    off_screen_canvas_ = m->CreateFrameCanvas();
  }
  void faceFlash(bool** currentFace, int simon_r, int simon_g, int simon_b)
  {
    drawFaceInput(currentFace, 0, simon_r, simon_g, simon_b);
    usleep(900000);
    canvas()->Clear();
    usleep(100000);

  }
  void Run() override {
    //FIXME: The following should be objects.

    bool** base = FileToFace("baseface", false);

    int curButton = -1; // Current button that is pressed. Start off with an arbitrary number that doesn't map to a button.
    int button; // Button that is pressed.
    bool isBlinking = false;
    bool buttonPressed = false; // Wether or not a button is pressed.
    bool drawNewFace; // Decides wether we are drawing a face on the next iteration of the loop
    bool** currentFace = base; // Current face
    bool** prevFace = base; // Face to keep track of what the previous face was
    int home_time = 3556000; // Time to keep track of the home button being pushed down before going to menu
    int eye_open_counter =  10560000; // Time to keep eyes open before blinking
    int eye_closed_counter = eye_open_counter / 20; // Time eyes are kept closed for blinking

    int simon_r = 0;
    int simon_g = 0;
    int simon_b = 255;

    int colors[4][3];

    // The three color codes for the four colors, red, yellow, green, blue

    // Red
    colors[3][0]=255;
    colors[3][1]=0;
    colors[3][2]=0;

    // Yellow
    colors[2][0]=255;
    colors[2][1]=255;
    colors[2][2]=0;

    // Green
    colors[1][0]=0;
    colors[1][1]=255;
    colors[1][2]=0;

    // Blue
    colors[0][0]=0;
    colors[0][1]=0;
    colors[0][2]=255;

    std::vector<int> colorsOrder;

    while (!interrupt_received){
        drawNewFace = false;

	button = current_button_pushed(controller1buttons);

        //for(int i = 0; i < 4; i++)
        //{
        //  faceFlash(currentFace, colors[i][0], colors[i][1], colors[i][2]);
        //}

        srand (time(NULL));
        int randint = (rand() % 4);
        //std::cout << randint << std::endl;

        colorsOrder.push_back(randint);

        usleep(900000);

        for(int i = 0; i < colorsOrder.size(); i++)
        {
          faceFlash(currentFace, colors[colorsOrder[i]][0], colors[colorsOrder[i]][1], colors[colorsOrder[i]][2]);
        }


        for(int i = 0; i < colorsOrder.size(); i++)
        {
          buttonPressed = false;
          while(!buttonPressed)
          {
            button = current_button_pushed(controller1buttons);

            if(button > 0 && button < 5)
            {
              buttonPressed = true;

              std::cout << "The correct answer was " << colorsOrder[i] << std::endl;
              std::cout << "Your answer: " << button - 1 << std::endl;

              if(button - 1 == colorsOrder[i])
              {
                faceFlash(currentFace, colors[colorsOrder[i]][0], colors[colorsOrder[i]][1], colors[colorsOrder[i]][2]);
                continue;
              }
              else
              {
                //defeat
                return; 
              }
            }
          }

//          usleep(900000);
        }


	/** if(buttonPressed == true)
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

	    if(button == 2)
	    {
              currentFace = base;
	    }
	 }
	}

        if(drawNewFace == true)
        {
            drawFaceInput(currentFace, 0, simon_r, simon_g, simon_b);
        }
        **/
     }


 }
 private:
   RGBMatrix *const matrix_;
   FrameCanvas *off_screen_canvas_;
};

