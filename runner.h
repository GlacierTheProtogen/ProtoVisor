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
  void drawFaceInput(bool** face, int floater) {
    canvas()->Clear();
    for(int i=0; i < 64; i++)
    {
      for(int j=0; j < 32; j++)
      {
        if(face[j][i] == true)
        {
          canvas()->SetPixel(i, j-floater, 0, 255, 0);
          canvas()->SetPixel(128-i, j-floater, 0, 255, 0);
        }
      }
    }
  }
  void Run() override {
    uint32_t continuum = 0;

    bool** happy = FileToFace("happy");
    bool** base = FileToFace("baseface");
    bool** heart = FileToFace("heart");
    bool** poker = FileToFace("poker");
    bool** angry = FileToFace("angry");
    bool** sad = FileToFace("sad");
    bool** uwu = FileToFace("uwu");



    int flowcycle = 1280000; // Integer that is used to divide x in the cosign equation. Higher = slower face floating.
    int flowcounter = 0; // Incremented integer that is used for the cosign function.
    int flowcountercompare = -8; // Used to keep track of the last integer that was used in the cosign function.
    int curButton = -1; // Current button that is pressed. Start off with an arbitrary number that doesn't map to a button.
    int button; // Button that is pressed.
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



#endif
