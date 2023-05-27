#!/usr/bin/env python3
from samplebase import SampleBase
import numpy as np

face = np.loadtxt('faces/baseface.txt', dtype="str", delimiter=None)
print(face)

class SimpleSquare(SampleBase):
    def __init__(self, *args, **kwargs):
        super(SimpleSquare, self).__init__(*args, **kwargs)

    def run(self):
        offset_canvas = self.matrix.CreateFrameCanvas()
        while True:
            offset_canvas.SetPixel(0, 50, 0, 0, 255)
            offset_canvas.SetPixel(0, 0, 1, 1, 255)
            offset_canvas = self.matrix.SwapOnVSync(offset_canvas)


# Main function
if __name__ == "__main__":
    simple_square = SimpleSquare()
    if (not simple_square.process()):
        simple_square.print_help()
