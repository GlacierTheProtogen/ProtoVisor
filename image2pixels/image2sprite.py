from PIL import Image
import sys
im = Image.open(sys.argv[1])


if(len(sys.argv) != 4):
  raise Exception("We need at least 3")


baserange=64
nonmirrored=False

pix = im.load()
blacktuple = (0, 0, 0, 255)

base_width = int(sys.argv[2])
base_height = int(sys.argv[3])


for i in range(base_height):
  for j in range(base_width):
    if(pix[(im.width/base_width)*j, (im.height/base_height)*i] == blacktuple):
       print("O", end="")
    else:
       print("X", end="")
  print("\n", end="")
