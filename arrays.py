#!/usr/bin/env python3
from samplebase import SampleBase
import numpy as np

face = np.loadtxt('faces/baseface.txt', dtype="str", delimiter=None)
print(face)

for i in range(64):
  for j in range(128):
    

