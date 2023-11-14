#include <iostream>
#include "led-matrix.h"

using namespace rgb_matrix;

#include <libevdev/libevdev.h>

#include <assert.h>
#include <getopt.h>
#include <limits.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <thread>
#include <fcntl.h>


#include "face.h"
#include "controller.h"



using std::min;
using std::max;

#define TERM_ERR  "\033[1;31m"
#define TERM_NORM "\033[0m"

using namespace rgb_matrix;


std::chrono::system_clock::time_point* controller1buttons = new std::chrono::system_clock::time_point[15];
std::chrono::system_clock::time_point* controller2buttons = new std::chrono::system_clock::time_point[15];


volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
  interrupt_received = true;
}


static int usage(const char *progname) {
  fprintf(stderr, "usage: %s <options> -D <demo-nr> [optional parameter]\n",
          progname);
  fprintf(stderr, "Options:\n");
  fprintf(stderr,
          "\t-D <demo-nr>              : Always needs to be set\n"
          );


  rgb_matrix::PrintMatrixFlags(stderr);

  return 1;
}


class Runner {
protected:
  Runner(Canvas *canvas) : canvas_(canvas) {}
  inline Canvas *canvas() { return canvas_; }

public:
  virtual ~Runner() {}
  virtual void Run() = 0;

private:
  Canvas *const canvas_;

};



class ProtoFace : public Runner {
public:
  ProtoFace(RGBMatrix *m) : Runner(m), matrix_(m) {
    off_screen_canvas_ = m->CreateFrameCanvas();
  }
  void drawFaceInput(bool** face) {
    for(int i=0; i < 64; i++)
    {
      for(int j=0; j < 32; j++)
      {
        if(face[j][i] == true)
        {
          canvas()->SetPixel(i, j, 0, 255, 0);
          canvas()->SetPixel(128-i, j, 0, 255, 0);
        }
      }
    }
  }
  void Run() override {
    uint32_t continuum = 0;
    bool buttonPressed = false;

    bool** happy = FileToFace("happy");
    bool** base = FileToFace("baseface");
    bool** heart = FileToFace("heart");
    bool** poker = FileToFace("poker");
    bool** angry = FileToFace("angry");
    bool** sad = FileToFace("sad");
    bool** uwu = FileToFace("uwu");


    int button;
    //int blinktimer = 0;

    bool button_pressed = false;


    drawFaceInput(base);

    while (!interrupt_received) {

        /* If a button is pressed, maintain the same face that we have been
        drawing. If not, then go back to basic face until a new button is
        pressed */

	button = is_button_pushed(controller1buttons);

	if(button_pressed == true)
	{
	  if(button == 0)
	  {
	    button_pressed = false;
            canvas()->Clear();
            drawFaceInput(base);
	  }
	}
	else
	{
	  if(button != 0)
	  {
	    button_pressed = true;
	    canvas()->Clear();

	    if(button == 1)
	    {
              drawFaceInput(sad);
	    }
	    else if(button == 2)
            {
              drawFaceInput(angry);
	    }
	    else if(button == 3)
	    {
              drawFaceInput(happy);
            }
            else if(button == 4)
            {
              drawFaceInput(uwu);
            }
            else if(button == 5)
            {
              drawFaceInput(poker);
            }
            else if(button == 6)
            {
              drawFaceInput(heart);
            }
	  }
	}
     }

 }
 private:
   RGBMatrix *const matrix_;
   FrameCanvas *off_screen_canvas_;
};

int main(int argc, char *argv[])
{

  // Set up the controllers.


  std::thread controller1(controller, "event0", controller1buttons);
  std::thread controller2(controller, "event1", controller2buttons);
  //std::thread readert(reader, controller1buttons);

  int demo = -1;
  int scroll_ms = 30;
  const char *demo_parameter = NULL;


  RGBMatrix::Options matrix_options;
  rgb_matrix::RuntimeOptions runtime_opt;

  // These are the defaults when no command-line flags are given.
  matrix_options.rows = 32;
  matrix_options.cols = 128;
  matrix_options.chain_length = 1;
  matrix_options.parallel = 1;

  // First things first: extract the command line flags that contain
  // relevant matrix options.
  if (!ParseOptionsFromFlags(&argc, &argv, &matrix_options, &runtime_opt)) {
    return usage(argv[0]);
  }

  int opt;
  while ((opt = getopt(argc, argv, "dD:r:P:c:p:b:m:LR:")) != -1) {
    switch (opt) {
    case 'D':
      demo = atoi(optarg);
      break;

    case 'm':
      scroll_ms = atoi(optarg);
      break;

    default: /* '?' */
      return usage(argv[0]);
    }
  }

  if (optind < argc) {
    demo_parameter = argv[optind];
  }


  RGBMatrix *matrix = RGBMatrix::CreateFromOptions(matrix_options, runtime_opt);
  if (matrix == NULL)
    return 1;

	printf("Size: %dx%d. Hardware gpio mapping: %s\n",
	matrix->width(), matrix->height(), matrix_options.hardware_mapping);

	Canvas *canvas = matrix;

	Runner *runner = new ProtoFace(matrix);

	runner->Run();

	delete runner;
	delete canvas;

	printf("Recieved CTRL-C. Exiting.\n");
	return 0;
}

