from scipy import ndimage
from PIL import Image

lena = ndimage.imread('lena.png')
img = Image.fromarray(lena, 'RGB')
img.show()