#!/bin/bash

MALLOC_CHECK=3 ./protovisor --led-gpio-mapping=adafruit-hat-pwm --led-slowdown-gpio=4

#valgrind --leak-check=full ./protovisor --led-gpio-mapping=adafruit-hat-pwm --led-slowdown-gpio=4
