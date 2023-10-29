#include <libevdev/libevdev.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <thread>
#include <unistd.h>

// 1,000,000 / 30 = 33333
int mseconds = 33333;


void counter(int* arr)
{
	while(1) {
	  for(int i = 0; i < 8; i++)
	  {
	    if(arr[i] != 0)
	    {
	      arr[i] = arr[i] - 1;
	    }
	  }
	  usleep(mseconds);
	}
}


void process_events(struct libevdev *dev, int* arr) {

	struct input_event ev = {};
	int status = 0;
	auto is_error = [](int v) { return v < 0 && v != -EAGAIN; };
	auto has_next_event = [](int v) { return v >= 0; };
	const auto flags = LIBEVDEV_READ_FLAG_NORMAL | LIBEVDEV_READ_FLAG_BLOCKING;
	//const auto flags = LIBEVDEV_READ_FLAG_NORMAL;

	//std::thread countert(counter, arr);
	//countert.join();

	//status = libevdev_next_event(dev, flags, &ev);

	while(1) {
	//while (status = libevdev_next_event(dev, flags, &ev), !is_error(status)) {
	//	if (!has_next_event(status)) {
	//	  continue;
	//	}
		//std::cout << "Got input_event";
		//std::cout << " type=" << ev.type;
		//std::cout << " code=" << ev.code;
		//std::cout << " value=" << ev.value << std::endl;


		//status = libevdev_next_event(dev, flags, &ev);

		if(libevdev_has_event_pending(dev))
		{

		  std::cout << "Event detected" << std::endl;

		  status = libevdev_next_event(dev, flags, &ev);
		  if(!is_error(status))
	 	  {

		    if(has_next_event(status)) {
		      if(ev.code == 20)
		      {
		        arr[1] = 40;
		      }
		    }
		  }
		}

		std::cout << "None" << std::endl;

		if(ev.code == 20)
		{
		  arr[1] = 40;
		}

		if(arr[1] != 0)
		{
		  arr[1] = arr[1] - 1;
		}

		usleep(mseconds/3);
	}
}


void controller(std::string device, int* arr) {

	struct libevdev *dev = nullptr;

        std::string path = "/dev/input/" + device;
        int fd = 0;

	while(1) {
		fd = open(path.c_str(), O_RDWR|O_CLOEXEC);
		if (libevdev_new_from_fd(fd, &dev) == 0) {
			libevdev_grab(dev, LIBEVDEV_GRAB);
			process_events(dev, arr);
			libevdev_free(dev);
		}
		close(fd);
	}


}


void reader(int *arr)
{
	while(1){
	  //for(int i = 0; i < 8; i++)
	  //{
	  //  std::cout << arr[i];
	  //}
	  std::cout << arr[1];
	  std::cout << std::endl;
	  usleep(mseconds);

	}
}




int main() {

        int *arr = new int[8];

	for(std::size_t i = 0; i < 8; i++){
		arr[i] = false;
	}

	std::thread controller1(controller, "event0", arr);
	//std::thread repeater1(reader, arr);
	//std::thread controller2(controller, "event1", arr);

	controller1.join();
	//repeater1.join();
	//controller2.join();


 	return 0;
}
