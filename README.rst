python-lsd
==========

Python bindings for the LSD line segment detector written by Rafael Grompone von Gioi, Jeremie Jakubowicz, Gregory Randall and Jean-Michel Morel.

See http://www.ipol.im/pub/art/2012/gjmr-lsd/

Usage
-----

```
from PIL import Image
import lsd

im = Image.open('chairs.pgm')
pixels = [float(p) for p in im.getdata()]
width, height = im.size
lines = lsd.lsd(pixels, width, height)
```
