CFLAGS=-Wall -g -O3 -Wextra `pkg-config --cflags --libs libevdev`
CXXFLAGS=$(CFLAGS)
OBJECTS=protovisor.o runner.o controller.o menu.o snake.o
BINARIES=protovisor

RGB_LIB_DISTRIBUTION=/home/dietpi/workspace/rpi-rgb-led-matrix
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a
LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread `pkg-config --cflags --libs libevdev`

MAGICK_CXXFLAGS?=$(shell GraphicsMagick++-config --cppflags --cxxflags)
MAGICK_LDFLAGS=$(shell GraphicsMagick++-config --ldflags --libs)

all : $(BINARIES)


$(RGB_LIBRARY): FORCE
	$(MAKE) -C $(RGB_LIBDIR)

protovisor : protovisor.o runner.o controller.o menu.o 2player-menu.o snake.o $(RGB_LIBRARY)
	$(CXX) $< -o $@ $(LDFLAGS)

protovisor.o : protovisor.cpp
	$(CXX) -I $(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

runner.o : runner.cpp runner.h
	$(CXX) -I $(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

controller.o : controller.cpp
	$(CXX) -I $(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

menu.o : menu.cpp menu.h
	$(CXX) -I $(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

2player-menu.o : 2player-menu.cpp
	$(CXX) -I $(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

snake.o : snake.cpp
	$(CXX) -I $(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(BINARIES)

FORCE:
.PHONY: FORCE
