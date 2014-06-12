import os.path
from unittest import TestCase

from PIL import Image

import lsd

TEST_DATA_DIR = os.path.join(os.path.dirname(__file__), 'test_data')

def read_lines_from_text(filename):
    lines = []
    with open(filename) as f:
        for l in f.readlines():
            l_clean = l.strip()
            if l_clean:
                lines.append([float(f) for f in l_clean.split(' ')])
    return lines

class LSDTestCase(TestCase):
    def test_lsd(self):
        im_filename = os.path.join(TEST_DATA_DIR, 'chairs.pgm')
        expected = read_lines_from_text(os.path.join(TEST_DATA_DIR,
            'chairs.lsd.txt'))
        im = Image.open(im_filename)
        pixels = [float(p) for p in im.getdata()]
        width, height = im.size
        lines = lsd.lsd(pixels, width, height)
        self.assertEqual(len(lines), len(expected))
