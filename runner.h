#ifndef RUNNER_H
#define RUNNER_H


extern std::chrono::system_clock::time_point* controller1buttons;
extern std::chrono::system_clock::time_point* controller2buttons;

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



#endif
