import os
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'
#import tensorflow as tf
from tensorflow import keras
from tensorflow.keras import layers
from tensorflow.keras.datasets import cifar10
import ssl
ssl._create_default_https_context = ssl._create_unverified_context

path1='si_cipa/wagi'
path2='si_cipa/model.h5'

#tf.debugging.set_log_device_placement(True)

#physical_devices = tf.config.list_physical_devices('GPU')
#tf.config.experimental.set_memory_growth(physical_devices[0], True)

(x_train, y_train), (x_test, y_test) = cifar10.load_data()
x_train = x_train.astype("float32")/255.0
x_test = x_test.astype("float32")/255.0

# print(x_train.shape)

# model = keras.Sequential(
#     [
#         keras.Input(shape=(32, 32, 3)),
#         layers.Conv2D(32, 3, padding='valid', activation='relu'),
#         layers.MaxPooling2D(pool_size=(2, 2)),
#         layers.Conv2D(64, 3, activation='relu'),
#         layers.MaxPooling2D(),
#         layers.Conv2D(128, 3, activation='relu'),
#         layers.Flatten(),
#         layers.Dense(64, activation='relu'),
#         layers.Dense(10)
#     ]
# )

def moj_model():
    inputs = keras.Input(shape=(32, 32, 3))
    x = layers.Conv2D(32, 3)(inputs)
    x = layers.BatchNormalization()(x)
    x = keras.activations.relu(x)
    x = layers.MaxPooling2D()(x)
    x = layers.Conv2D(64, 5, padding='same')(x)
    x = layers.BatchNormalization()(x)
    x = keras.activations.relu(x)
    x = layers.Conv2D(128, 3)(x)
    x = layers.BatchNormalization()(x)
    x = keras.activations.relu(x)
    x = layers.Flatten()(x)
    x = layers.Dense(64, activation='relu')(x)
    outputs = layers.Dense(10)(x)
    model = keras.Model(inputs=inputs, outputs=outputs)
    return model

model = moj_model()
# print(model.summary())

# model = tf.keras.models.load_model(path2)

model.compile(
    loss=keras.losses.SparseCategoricalCrossentropy(from_logits=True),
    optimizer=keras.optimizers.Adam(learning_rate=3e-4),
    metrics=["accuracy"]
)

#model.load_weights(path1)
model.fit(x_train, y_train, batch_size=128, epochs=3, verbose=1)
model.evaluate(x_test, y_test, batch_size=128, verbose=1)
#model.save(path2)
#model.save_weights(path1)
