import ctypes
import os.path

# Find and load the library
libpath = os.path.join(os.path.dirname(__file__), 'liblsd.so')
liblsd = ctypes.cdll.LoadLibrary(libpath)

# Set the argument types
liblsd.lsd.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.POINTER(ctypes.c_double), ctypes.c_int, ctypes.c_int]
liblsd.lsd.restype = ctypes.POINTER(ctypes.c_double)

def lsd(img, x, y):
    n_out = ctypes.c_int() 
    img_pix = (ctypes.c_double * len(img))(*img)
    segs = liblsd.lsd(ctypes.byref(n_out), img_pix, x, y)
    lines = []
    for i in range(n_out.value):
        line = (
            segs[7*i+0], 
            segs[7*i+1], 
            segs[7*i+2], 
            segs[7*i+3], 
            segs[7*i+5],
            segs[7*i+6])
        lines.append(line)
    return lines 
