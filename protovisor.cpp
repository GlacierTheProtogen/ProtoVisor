#include <iostream>
#include "led-matrix.h"

using namespace rgb_matrix;

#include <libevdev-1.0/libevdev/libevdev.h>
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
  void Run() override {
    uint32_t continuum = 0;
    bool** face = FileToFace("happy");

      while (!interrupt_received) {
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
}
private:
  RGBMatrix *const matrix_;
  FrameCanvas *off_screen_canvas_;
};

int main(int argc, char *argv[])
{

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

