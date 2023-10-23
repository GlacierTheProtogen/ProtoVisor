#include <libevdev/libevdev.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>


void process_events(struct libevdev *dev) {

	struct input_event ev = {};
	int status = 0;
	auto is_error = [](int v) { return v < 0 && v != -EAGAIN; };
	auto has_next_event = [](int v) { return v >= 0; };
	const auto flags = LIBEVDEV_READ_FLAG_NORMAL | LIBEVDEV_READ_FLAG_BLOCKING;

	while (status = libevdev_next_event(dev, flags, &ev), !is_error(status)) {
		if (!has_next_event(status)) continue;

		std::cout << "Got input_event";
		std::cout << " type=" << ev.type;
		std::cout << " code=" << ev.code;
		std::cout << " value=" << ev.value << std::endl;
	}
}


int main() {
	struct libevdev *dev = nullptr;

	std::string path = "/dev/input/event0";
	int fd = 0;

	while(1) {
		fd = open(path.c_str(), O_RDWR|O_CLOEXEC);
    		if (libevdev_new_from_fd(fd, &dev) == 0) {
			libevdev_grab(dev, LIBEVDEV_GRAB);
			process_events(dev);
			libevdev_free(dev);
		}
		close(fd);
	}

	return 0;
}
