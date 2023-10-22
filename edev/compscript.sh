
#!/bin/bash

g++ -Wall test.cpp -o main `pkg-config --cflags --libs libevdev`
