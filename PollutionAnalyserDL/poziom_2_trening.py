import os
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'
import tensorflow as tf
from tensorflow import keras
from tensorflow.keras import layers
import numpy as np

#from tensorflow.keras.datasets import cifar10
#import ssl
import sys
#ssl._create_default_https_context = ssl._create_unverified_context

path1='si_cipa2/wagi'
path2='si_cipa2/model.h5'

rzecz = np.load('wejscie.npy')
test = np.load('etykiety.npy')

#x_train = x_train.astype("float32")/255.0
#x_test = x_test.astype("float32")/255.0

model = keras.Sequential(
    [
        keras.Input(shape=(4, 10, 2, 2)),
        layers.Flatten(),
        layers.Dense(160, activation='tanh'),
        layers.BatchNormalization(),
        layers.Dense(320, activation='tanh'),
        layers.BatchNormalization(),
        layers.Dense(640, activation='tanh'),
        layers.BatchNormalization(),
        layers.Dense(320, activation='tanh'),
        layers.BatchNormalization(),
        layers.Dense(300, activation='tanh'),
        layers.BatchNormalization(),
        layers.Dense(128, activation='tanh'),
        layers.BatchNormalization(),
        layers.Dense(64, activation='tanh'),
        layers.BatchNormalization(),
        layers.Dense(64, activation='tanh'),
        layers.BatchNormalization(),
        layers.Dense(32, activation='tanh'),
        layers.Dense(12, activation='tanh'),
        layers.Dense(1)
    ]
)
#print(model.summary())
model.compile(
    loss=keras.losses.MeanSquaredError(),
    optimizer=keras.optimizers.Adam(learning_rate=3e-4),
    metrics=["accuracy"]
)

# if sys.argv[3] == 'lw':
#     model.load_weights(path1)

# if not sys.argv[4]:
#     m = 10
# else:
#     m = sys.argv[4]


model.fit(rzecz, test, batch_size=128, epochs=10, verbose=1)
#model.evaluate(x_test, y_test, batch_size=128, verbose=1)
#
# if sys.argv[2] == 'sm':
#     model.save(path2)
#
# if sys.argv[2] == 'sw':
#     model.save_weights(path1)