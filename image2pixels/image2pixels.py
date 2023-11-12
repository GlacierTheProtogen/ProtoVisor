from PIL import Image
import sys

im = Image.open(sys.argv[1])
pix = im.load()

blacktuple = (0, 0, 0, 255)

for i in range(32):
  for j in range(64):
    if(pix[(im.width/64)*j, (im.height/32)*i] == blacktuple):
       print("O", end="")
    else:
       print("X", end="")
  print("\n", end="")
