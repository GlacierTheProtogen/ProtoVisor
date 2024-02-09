from PIL import Image
import sys
im = Image.open(sys.argv[1])

baserange=64

if(sys.argv[2] == "--long"):
  baserange=128


pix = im.load()

blacktuple = (0, 0, 0, 255)

for i in range(32):
  for j in range(baserange):
    if(pix[(im.width/baserange)*j, (im.height/32)*i] == blacktuple):
       print("O", end="")
    else:
       print("X", end="")
  print("\n", end="")
