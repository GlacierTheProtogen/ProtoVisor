#ifndef CONTROLLER_H
#define CONTROLLER_H


#include <iostream>
#include "led-matrix.h"

using namespace rgb_matrix;

#include <libevdev/libevdev.h>

#include <unistd.h>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <fcntl.h>

#include "face.h"
#include "runner.h"


using std::min;
using std::max;


// Only needed for reader function
const int mseconds = 400;

// Microseconds for how long a button stays pushed down
int frames = 50;

//Arbitrary, just needs to be mapped to /etc/xboxdrv/drv-player1/2.conf
//FIXME: Replace below with enums.
const int X1 = 34;
const int A1 = 20;
const int Y1 = 35;
const int B1 = 21;
const int DPAD_UP_1 = 46;
const int DPAD_DOWN_1 = 47;
const int DPAD_LEFT_1 = 44;
const int DPAD_RIGHT_1 = 45;
const int X_JOYS_UP_1 = 30;
const int X_JOYS_DOWN_1 = 31;
const int X_JOYS_LEFT_1 = 16;
const int X_JOYS_RIGHT_1 = 17;
const int GUIDE_1 = 2;
const int START_1 = 5;
const int BACK_1 = 4;
const int Y_JOYS_UP_1 = 18;
const int Y_JOYS_DOWN_1 = 19;
const int Y_JOYS_LEFT_1 = 32;
const int Y_JOYS_RIGHT_1 = 33;
const int L_BUMPER_1 = 273;
const int R_BUMPER_1 = 57;
const int L_TRIGGER_1 = 183;
const int R_TRIGGER_1 = 184;


const int X2 = 38;
const int A2 = 24;
const int Y2 = 48;
const int B2 = 25;
const int DPAD_UP_2 = 36;
const int DPAD_DOWN_2 = 37;
const int DPAD_LEFT_2 = 22;
const int DPAD_RIGHT_2 = 23;
const int X_JOYS_UP_2 = 9;
const int X_JOYS_DOWN_2 = 10;
const int X_JOYS_LEFT_2 = 49;
const int X_JOYS_RIGHT_2 = 50;
const int GUIDE_2 = 3;
const int START_2 = 7;
const int BACK_2 = 6;
const int Y_JOYS_UP_2 = 189;
const int Y_JOYS_DOWN_2 = 190;
const int Y_JOYS_LEFT_2 = 187;
const int Y_JOYS_RIGHT_2 = 188;
const int L_BUMPER_2 = 185;
const int R_BUMPER_2 = 186;
const int L_TRIGGER_2 = 44;
const int R_TRIGGER_2 = 272;




/*
 The controller is initiated by:
 1. Create an arry of timestamps
 2. Create an infinite loop that detects
 key presses from /dev/input/* using libevdev
 and xboxdrv library
 3. If we want to see if a button is pressed,
 compare the timestamp with the current time

 We use this method instead of a more streamlined
 and built in "isPressed" button because xboxdrv
 is not able to detect wether or not a button
 is being held down. A bit convoluted, but works
 for our needs.


*/


void process_events(struct libevdev *dev, std::chrono::system_clock::time_point* arr, std::string device) {
	struct input_event ev = {};
	int status = 0;
	auto is_error = [](int v) { return v < 0 && v != -EAGAIN; };
	const auto flags = LIBEVDEV_READ_FLAG_NORMAL | LIBEVDEV_READ_FLAG_BLOCKING;


        // FIXME: Replace the below with enums.

	int X;
	int A;
	int Y;
	int B;
	int DPAD_UP;
	int DPAD_DOWN;
	int DPAD_LEFT;
	int DPAD_RIGHT;
	int X_JOYS_UP;
	int X_JOYS_DOWN;
	int X_JOYS_LEFT;
	int X_JOYS_RIGHT;
	int GUIDE;
	int START;
	int BACK;
        int Y_JOYS_UP;
        int Y_JOYS_DOWN;
        int Y_JOYS_LEFT;
        int Y_JOYS_RIGHT;
        int L_BUMPER;
        int R_BUMPER;
        int L_TRIGGER;
        int R_TRIGGER;


	if(device == "event0")
	{
	  X = X1;
	  A = A1;
	  Y = Y1;
	  B = B1;
	  DPAD_UP = DPAD_UP_1;
	  DPAD_DOWN = DPAD_DOWN_1;
	  DPAD_LEFT = DPAD_LEFT_1;
	  DPAD_RIGHT = DPAD_RIGHT_1;
	  X_JOYS_UP = X_JOYS_UP_1;
	  X_JOYS_DOWN = X_JOYS_DOWN_1;
	  X_JOYS_LEFT = X_JOYS_LEFT_1;
	  X_JOYS_RIGHT = X_JOYS_RIGHT_1;
	  GUIDE = GUIDE_1;
	  START = START_1;
	  BACK = BACK_1;
          Y_JOYS_UP = Y_JOYS_UP_1;
          Y_JOYS_DOWN = Y_JOYS_DOWN_1;
          Y_JOYS_LEFT = Y_JOYS_LEFT_1;
          Y_JOYS_RIGHT = Y_JOYS_RIGHT_1;
          L_BUMPER = L_BUMPER_1;
          R_BUMPER = R_BUMPER_1;
          L_TRIGGER = L_TRIGGER_1;
          R_TRIGGER = R_TRIGGER_1;
	}
	else
	{
	  X = X2;
	  A = A2;
	  Y = Y2;
	  B = B2;
	  DPAD_UP = DPAD_UP_2;
	  DPAD_DOWN = DPAD_DOWN_2;
	  DPAD_LEFT = DPAD_LEFT_2;
	  DPAD_RIGHT = DPAD_RIGHT_2;
	  X_JOYS_UP = X_JOYS_UP_2;
	  X_JOYS_DOWN = X_JOYS_DOWN_2;
	  X_JOYS_LEFT = X_JOYS_LEFT_2;
	  X_JOYS_RIGHT = X_JOYS_RIGHT_2;
	  GUIDE = GUIDE_2;
	  START = START_2;
	  BACK = BACK_2;
          Y_JOYS_UP = Y_JOYS_UP_2;
          Y_JOYS_DOWN = Y_JOYS_DOWN_2;
          Y_JOYS_LEFT = Y_JOYS_LEFT_2;
          Y_JOYS_RIGHT = Y_JOYS_RIGHT_2;
          L_BUMPER = L_BUMPER_2;
          R_BUMPER = R_BUMPER_2;
          L_TRIGGER = L_TRIGGER_2;
          R_TRIGGER = R_TRIGGER_2;
	}

	while(1) {

		if(libevdev_has_event_pending(dev))
		{
                  //std::cout << "Got input_event";
                  //std::cout << " type=" << ev.type;
                  //std::cout << " code=" << ev.code;
                  //std::cout << " value=" << ev.value << std::endl;


                  // Can't be a switch statement because of variable comparison
		  status = libevdev_next_event(dev, flags, &ev);
		  if(!is_error(status))
	 	  {
		    if(ev.code == X){
		      arr[0] = std::chrono::system_clock::now();
		    }
		    else if(ev.code == A){
		      arr[1] = std::chrono::system_clock::now();
		    }
		    else if(ev.code == Y){
		      arr[2] = std::chrono::system_clock::now();
		    }
		    else if(ev.code == B){
		      arr[3] = std::chrono::system_clock::now();
		    }
		    else if(ev.code == DPAD_UP){
		      arr[4] = std::chrono::system_clock::now();
		    }
		    else if(ev.code == DPAD_DOWN){
		      arr[5] = std::chrono::system_clock::now();
		    }
		    else if(ev.code == DPAD_LEFT){
		      arr[6] = std::chrono::system_clock::now();
		    }
		    else if(ev.code == DPAD_RIGHT){
		      arr[7] = std::chrono::system_clock::now();
		    }
		    else if(ev.code == X_JOYS_UP){
		      arr[8] = std::chrono::system_clock::now();
		    }
		    else if(ev.code == X_JOYS_DOWN){
		      arr[9] = std::chrono::system_clock::now();
		    }
		    else if(ev.code == X_JOYS_LEFT){
		      arr[10] = std::chrono::system_clock::now();
		    }
		    else if(ev.code == X_JOYS_RIGHT){
		      arr[11] = std::chrono::system_clock::now();
		    }
		    else if(ev.code == GUIDE){
		      arr[12] = std::chrono::system_clock::now();
		    }
		    else if(ev.code == START){
		      arr[13] = std::chrono::system_clock::now();
		    }
		    else if(ev.code == BACK){
		      arr[14] = std::chrono::system_clock::now();
		    }
                    else if(ev.code == Y_JOYS_UP){
                      arr[15] = std::chrono::system_clock::now();
                    }
                    else if(ev.code == Y_JOYS_DOWN){
                      arr[16] = std::chrono::system_clock::now();
                    }
                    else if(ev.code == Y_JOYS_LEFT){
                      arr[17] = std::chrono::system_clock::now();
                    }
                    else if(ev.code == Y_JOYS_RIGHT){
                      arr[18] = std::chrono::system_clock::now();
                    }
                    else if(ev.code == L_BUMPER){
                      arr[19] = std::chrono::system_clock::now();
                    }
                    else if(ev.code == R_BUMPER){
                      arr[20] = std::chrono::system_clock::now();
                    }
                    else if(ev.code == L_TRIGGER){
                      arr[21] = std::chrono::system_clock::now();
                    }
                    else if(ev.code == R_TRIGGER){
                      arr[22] = std::chrono::system_clock::now();
                    }

		  }
		}

	}
}


void controller(std::string device, std::chrono::system_clock::time_point* arr) {

	struct libevdev *dev = nullptr;

        std::string path = "/dev/input/" + device;
        int fd = 0;

	while(1) {
		fd = open(path.c_str(), O_RDWR|O_CLOEXEC);
		if (libevdev_new_from_fd(fd, &dev) == 0) {
			libevdev_grab(dev, LIBEVDEV_GRAB);
			process_events(dev, arr, device);
			libevdev_free(dev);
		}
		close(fd);
	}

}



//Testing purposes only
void reader(std::chrono::system_clock::time_point* arr)
{
	while(1) {

	  std::chrono::system_clock::time_point second = std::chrono::system_clock::now();

	  for(std::size_t i = 0; i < 23; i++)
	  {
	    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(second - arr[i]);
	    std::cout << (milliseconds.count() < frames);
	  }

	  std::cout << std::endl;
	  usleep(mseconds);
 	}
}


int current_button_pushed(std::chrono::system_clock::time_point* arr)
{
	//return the array value of a button pushed, otherwise return 0 if nothing is pushed.
	std::chrono::system_clock::time_point ref = std::chrono::system_clock::now();
	int iter = 1;

	for(std::size_t i = 0; i < 23; i++)
	{
	  auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(ref - arr[i]);
	  if(milliseconds.count() < frames)
	  {
	    return iter;
	  }
	  iter++;
	}

	return 0;
}

bool is_button_pushed(std::chrono::system_clock::time_point* arr, int i)
{
	//return wether or not a button is pushed within a controller array.
        std::chrono::system_clock::time_point ref = std::chrono::system_clock::now();

        //FIXME: Error checking to see wether or not the index is within range.

        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(ref - arr[i]);

	if(milliseconds.count() < frames)
        {
	  return true;
	}

	return false;

}


#endif
