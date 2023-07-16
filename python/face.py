#!/usr/bin/env python3
from samplebase import SampleBase
from pynput.keyboard import Key, Listener
import numpy as np
import json
import time

pressedkey='0'

face1 = np.loadtxt('all-faces/baseface.txt', dtype="str", delimiter=None)
face2 = np.loadtxt('all-faces/uwu.txt', dtype="str", delimiter=None)
face3 = np.loadtxt('all-faces/happy.txt', dtype="str", delimiter=None)
face4 = np.loadtxt('all-faces/heart.txt', dtype="str", delimiter=None)
face5 = np.loadtxt('all-faces/poker.txt', dtype="str", delimiter=None)
face6 = np.loadtxt('all-faces/blink.txt', dtype="str", delimiter=None)

def on_press(key):
  pressedkey=key

def on_release(key):
  pressedkey='0'


class SimpleSquare(SampleBase):
    def __init__(self, *args, **kwargs):
        super(SimpleSquare, self).__init__(*args, **kwargs)

    def run(self):

        listener = Listener(
           on_press=on_press,
           on_release=on_release)
        listener.start()

        offset_canvas = self.matrix.CreateFrameCanvas()
        while True:
          face = face1
          if pressedkey[0] == 'q':
            face = face2
          elif pressedkey[0] == 'a':
            face = face3
          elif pressedkey[0] == 's':
            face = face4
          elif pressedkey[0] == 'w':
            face = face5
          for i in range(64):
            for j in range(32):
              if face[j][i] == 'X':
                offset_canvas.SetPixel(i, j, 0, 0, 255)
                offset_canvas.SetPixel(127-i, j, 0, 0, 225)
              else:
                offset_canvas.SetPixel(i, j, 0, 0, 0)
                offset_canvas.SetPixel(127-i, j, 0, 0, 0)
          offset_canvas = self.matrix.SwapOnVSync(offset_canvas)

# Main function
if __name__ == "__main__":
    simple_square = SimpleSquare()
    if (not simple_square.process()):
        simple_square.print_help()
