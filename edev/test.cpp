#include <libevdev/libevdev.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <thread>
#include <chrono>

// 1,000,000 / 30 = 33333
const int mseconds = 400;
const int frames = 600;


//Arbitrary, just needs to be mapped to /etc/xboxdrv/drv-player1/2.conf
const int X1 = 34;
const int A1 = 20;
const int Y1 = 35;
const int B1 = 21;
const int DPAD_UP_1 = 46;
const int DPAD_DOWN_1 = 47;
const int DPAD_LEFT_1 = 44;
const int DPAD_RIGHT_1 = 45;
const int JOYS_UP_1 = 30;
const int JOYS_DOWN_1 = 31;
const int JOYS_LEFT_1 = 16;
const int JOYS_RIGHT_1 = 17;
const int GUIDE_1 = 2;
const int START_1 = 5;
const int BACK_1 = 4;

const int X2 = 38;
const int A2 = 24;
const int Y2 = 48;
const int B2 = 25;
const int DPAD_UP_2 = 36;
const int DPAD_DOWN_2 = 37;
const int DPAD_LEFT_2 = 22;
const int DPAD_RIGHT_2 = 23;
const int JOYS_UP_2 = 9;
const int JOYS_DOWN_2 = 10;
const int JOYS_LEFT_2 = 49;
const int JOYS_RIGHT_2 = 50;
const int GUIDE_2 = 3;
const int START_2 = 7;
const int BACK_2 = 6;


void process_events(struct libevdev *dev, std::chrono::system_clock::time_point* arr, std::string device) {

	struct input_event ev = {};
	int status = 0;
	auto is_error = [](int v) { return v < 0 && v != -EAGAIN; };
	const auto flags = LIBEVDEV_READ_FLAG_NORMAL | LIBEVDEV_READ_FLAG_BLOCKING;

	int X;
	int A;
	int Y;
	int B;
	int DPAD_UP;
	int DPAD_DOWN;
	int DPAD_LEFT;
	int DPAD_RIGHT;
	int JOYS_UP;
	int JOYS_DOWN;
	int JOYS_LEFT;
	int JOYS_RIGHT;
	int GUIDE;
	int START;
	int BACK;

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
	  JOYS_UP = JOYS_UP_1;
	  JOYS_DOWN = JOYS_DOWN_1;
	  JOYS_LEFT = JOYS_LEFT_1;
	  JOYS_RIGHT = JOYS_RIGHT_1;
	  GUIDE = GUIDE_1;
	  START = START_1;
	  BACK = BACK_1;
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
	  JOYS_UP = JOYS_UP_2;
	  JOYS_DOWN = JOYS_DOWN_2;
	  JOYS_LEFT = JOYS_LEFT_2;
	  JOYS_RIGHT = JOYS_RIGHT_2;
	  GUIDE = GUIDE_2;
	  START = START_2;
	  BACK = BACK_2;
	}

	while(1) {
		//std::cout << "Got input_event";
		//std::cout << " type=" << ev.type;
		//std::cout << " code=" << ev.code;
		//std::cout << " value=" << ev.value << std::endl;


		if(libevdev_has_event_pending(dev))
		{

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
		    else if(ev.code == JOYS_UP){
		      arr[8] = std::chrono::system_clock::now();
		    }
		    else if(ev.code == JOYS_DOWN){
		      arr[9] = std::chrono::system_clock::now();
		    }
		    else if(ev.code == JOYS_LEFT){
		      arr[10] = std::chrono::system_clock::now();
		    }
		    else if(ev.code == JOYS_RIGHT){
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
		  }
		}

		usleep(20);

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


void reader(std::chrono::system_clock::time_point* arr)
{
	while(1) {

	  std::chrono::system_clock::time_point second = std::chrono::system_clock::now();


	  for(std::size_t i = 0; i < 15; i++)
	  {
	    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(second - arr[i]);
	    std::cout << (milliseconds.count() > 40);
	  }

	  std::cout << std::endl;
	  usleep(mseconds);
 	}
}

int main() {

        //int* controller1buttons = new int[15];
	//int* controller2buttons = new int[15];

	std::chrono::system_clock::time_point* controller1buttons = new std::chrono::system_clock::time_point[15];
	std::chrono::system_clock::time_point* controller2buttons = new std::chrono::system_clock::time_point[15];

	std::thread controller1(controller, "event0", controller1buttons);
	std::thread controller2(controller, "event1", controller2buttons);
	std::thread readert(reader, controller1buttons);

	controller1.join();
	controller2.join();
	readert.join();


 	return 0;
}
