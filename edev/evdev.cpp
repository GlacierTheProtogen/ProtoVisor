#include <libevdev-1.0/libevdev/libevdev.h>
#include <cstddef>
#include <string>
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <fcntl.h>

// https://suricrasia.online/blog/turning-a-keyboard-into/

int main()
{

 struct libevdev *dev = NULL;

 std::string filename = "/dev/input/event0";

 int fd;
 int rc = 1;

 fd = open(filename.c_str(), O_RDONLY);

 if (fd < 0) {
    perror("Failed to open device");
    return 0;
 }

 if (libevdev_new_from_fd(fd, &dev) == 0) {
	std::string phys = libevdev_get_phys(dev);
	std::string name = libevdev_get_name(dev);

	std::cout << filename << std::endl;
	std::cout << "- phys: " << phys << std::endl;
	std::cout << "- name: " << name << std::endl;
	std::cout << std::endl;

	libevdev_free(dev);
 }

 close(fd);

 return 0;
}
