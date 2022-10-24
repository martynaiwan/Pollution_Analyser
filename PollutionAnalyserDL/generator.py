import numpy as np
from PIL import Image
from numpy import asarray

# mapa = np.random.rand(1000, 2) * 5000
# ima = np.random.rand(1600,1600,3)

x = (np.random.rand(100,800,800,3) + 1) * 127.5
x = x.astype(int)

y = (np.random.rand(100,1000) + 1) * 0.5
y = y.astype(int)

#print(mapa)

np.save('wejscia1', x)
np.save('etykiety1', y)
#
# image = Image.open('1.jpg')
#
# numpydata = asarray(image)
#
# print(type(numpydata))
#
# # shape
# print(numpydata.shape)
#
# # Below is the way of creating Pillow
# # image from our numpyarray
# pilImage = Image.fromarray(numpydata)
# pilImage = pilImage.save("dupa.jpg")