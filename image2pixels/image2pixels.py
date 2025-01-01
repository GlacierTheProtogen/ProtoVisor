from PIL import Image
import sys
im = Image.open(sys.argv[1])

baserange=64
nonmirrored=False


if(len(sys.argv) == 3):
  if(sys.argv[2] == "--long"):
    baserange=128

  if(sys.argv[2] == "--nomirror"):
    nonmirrored = True

pix = im.load()

blacktuple = (255, 0, 0, 0)

for i in range(32):
  for j in range(baserange):
    #print(pix[(im.width/baserange)*j, (im.height/32)*i])
    if(pix[(im.width/baserange)*j, (im.height/32)*i] == blacktuple):
       print("O", end="")
    else:
       print("X", end="")

  if(nonmirrored):
    for j in range(baserange):
      if(pix[(im.width/baserange)*j, (im.height/32)*i] == blacktuple):
        print("O", end="")
      else:
        print("X", end="")

  print("\n", end="")
