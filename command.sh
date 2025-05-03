#!/bin/bash

MALLOC_CHECK=3 ./protovisor --led-gpio-mapping=adafruit-hat-pwm --led-slowdown-gpio=4 #--led-brightness=85

#MALLOC_CHECK=3 ./protovisor --led-gpio-mapping=adafruit-hat-pwm --led-slowdown-gpio=4 --led-brightness=90

#valgrind --leak-check=full ./protovisor --led-gpio-mapping=adafruit-hat-pwm --led-slowdown-gpio=4
