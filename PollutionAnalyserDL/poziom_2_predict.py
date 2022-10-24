import os
import tensorflow as tf
#from tensorflow import keras
#from tensorflow.keras import layers
import sys

os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'
path1='si_cipa2/wagi'
path2='si_cipa2/model.h5'
dane = sys.argv[1]

model = tf.keras.models.load_model(path2)
model.load_weights(path1)
n = model.predict(dane)

##### dwie opcje wyplucia predykcji na zewnatrz, ktore mi przyszly do glowy. Do wyboru:

#opcja 1:
with open('dupa2.dat', 'wb') as plik:
    plik.write(n)

#opcja 2:
#sys.stdout.write(n)