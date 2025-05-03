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
  void faceFlash(bool** currentFace, int simon_r, int simon_g, int simon_b, int sleep_time)
  {
    drawFaceInput(currentFace, 0, simon_r, simon_g, simon_b);
    usleep(sleep_time);
    canvas()->Clear();
    usleep(100000);

  }
  void Run() override {
    //FIXME: The following should be objects.

    bool** base = FileToFace("baseface", false);
    bool** dead = FileToFace("dead", false);
    bool** happy = FileToFace("happy", false);

    int button; // Button that is pressed.
    bool buttonPressed = false; // Wether or not a button is pressed.
    bool** currentFace = base; // Current face

    int colors[4][3];

    // The three color codes for the four colors, red, yellow, green, blue

    // Red
    colors[3][0]=255;
    colors[3][1]=0;
    colors[3][2]=0;

    // Yellow
    colors[2][0]=180;
    colors[2][1]=140;
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
	button = current_button_pushed(controller1buttons);

        //for(int i = 0; i < 4; i++)
        //{
        //  faceFlash(currentFace, colors[i][0], colors[i][1], colors[i][2]);
        //}

        srand (time(NULL));
        int randint = (rand() % 4);
        //std::cout << randint << std::endl;

        colorsOrder.push_back(randint);

        if(colorsOrder.size() > 1)
        {
          for(int j = 0; j < 3; j++)
          {
            int last_color = colorsOrder[colorsOrder.size()-2];
            faceFlash(happy, colors[last_color][0], colors[last_color][1], colors[last_color][2], 300000);
          }
        }

        usleep(900000);

        for(long unsigned int i = 0; i < colorsOrder.size(); i++)
        {
          faceFlash(currentFace, colors[colorsOrder[i]][0], colors[colorsOrder[i]][1], colors[colorsOrder[i]][2], 900000);
        }


        for(long unsigned int i = 0; i < colorsOrder.size(); i++)
        {
          buttonPressed = false;
          while(!buttonPressed)
          {
            button = current_button_pushed(controller1buttons);

            if(button > 0 && button < 5)
            {
              buttonPressed = true;

              if(button - 1 == colorsOrder[i])
              {
                faceFlash(currentFace, colors[colorsOrder[i]][0], colors[colorsOrder[i]][1], colors[colorsOrder[i]][2], 900000);
                continue;
              }
              else
              {
                for(int j = 0; j< 3; j++)
                {
                  faceFlash(dead, colors[colorsOrder[i]][0], colors[colorsOrder[i]][1], colors[colorsOrder[i]][2], 300000);
                }

                //defeat
                return; 
              }
            }
          }

        }
     }


 }
 private:
   RGBMatrix *const matrix_;
   FrameCanvas *off_screen_canvas_;
};

