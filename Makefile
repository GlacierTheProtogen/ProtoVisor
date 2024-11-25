CFLAGS=-Wall -g -O3 -Wextra `pkg-config --cflags --libs libevdev`
CXXFLAGS=$(CFLAGS)
OBJECTS=protovisor.o runner.o controller.o menu.o 2player-menu.o countdown.o snake.o victory.o pong.o simon-says.o dinosaur-game.o color-menu.o
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

protovisor : protovisor.o runner.o controller.o menu.o 2player-menu.o countdown.o snake.o victory.o pong.o simon-says.o dinosaur-game.o color-menu.o $(RGB_LIBRARY)
	$(CXX) $< -o $@ $(LDFLAGS)

protovisor.o : protovisor.cpp
	$(CXX) -I $(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

runner.o : runner.cpp runner.h
	$(CXX) -I $(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

controller.o : controller.cpp
	$(CXX) -I $(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

menu.o : menu.cpp menu.h
	$(CXX) -I $(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

color-menu.o : color-menu.cpp
	$(CXX) -I $(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

2player-menu.o : 2player-menu.cpp
	$(CXX) -I $(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

countdown.o : countdown.cpp runner.h
	$(CXX) -I $(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

snake.o : snake.cpp
	$(CXX) -I $(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

victory.o : victory.h runner.h
	$(CXX) -I $(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

pong.o : pong.cpp
	$(CXX) -I $(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

simon-says.o : simon-says.cpp runner.h
	$(CXX) -I $(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

dinosaur-game.o : dinosaur-game.cpp controller.cpp dinosaur-game.h ground-obstacle.h animated-sprite.h runner.h
	$(CXX) -I $(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(BINARIES)

FORCE:
.PHONY: FORCE
