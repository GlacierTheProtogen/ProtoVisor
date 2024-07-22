#include "runner.h"
#include "controller.cpp"
#include "face.h"

#include <chrono>
#include <vector>

extern std::chrono::system_clock::time_point* controller1buttons;
extern std::chrono::system_clock::time_point* controller2buttons;



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
          canvas()->SetPixel(j, i, r, b, g);
        }
      }
    }
  }
  void Run() override {
    //FIXME: The following should be objects.

    int button; // Button that is pressed.
    bool buttonPressed = false; // Wether or not a button is pressed.

    canvas()->Clear();

    bool** dino1 = FileToSprite("dino1", 20, 12);

    drawSprite(dino1, 20, 12, 1, 1, 0, 0, 255);


    while (!interrupt_received){
      button = current_button_pushed(controller1buttons);
      usleep(5900000);

    }
    return;

 }
 private:
   RGBMatrix *const matrix_;
   FrameCanvas *off_screen_canvas_;
};

