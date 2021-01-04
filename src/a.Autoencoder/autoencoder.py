import re
import numpy as np
import pandas as pd
from matplotlib import pyplot as plt
import tensorflow as tf
import keras
from keras import layers
from keras import backend as K
from sklearn.model_selection import train_test_split

def convolutionalAutoencoder():
    keras.backend.clear_session()
    print("[INFO] building autoencoder...")
    model = tf.keras.models.Sequential()
    model.add(layers.Input(shape=(28, 28, 1)))
    model.add(layers.Conv2D(32, (3, 3), activation='relu', padding='same', strides=2))
    model.add(layers.Conv2D(64, (3, 3), activation='relu', padding='same', strides=2))
    model.add(layers.Conv2D(128, (3, 3), activation='relu', padding='same', strides=3))

    model.add(layers.Flatten())
    model.add(layers.Dense(10, activation='relu'))

    model.add(layers.Dense(1152, activation='relu'))
    model.add(layers.Reshape((3, 3, 128)))
    model.add(layers.Conv2DTranspose(64, kernel_size=3, strides=2, activation='relu'))
    model.add(layers.Conv2DTranspose(32, kernel_size=3, strides=2, activation='relu', padding='same'))
    model.add(layers.Conv2DTranspose(1, kernel_size=3, strides=2, activation='relu', padding='same'))

    return model

# training 
def training(autoencoder, all_images, epochs=10, batch_size=128):
	print("[INFO] training started...")
	df = pd.DataFrame.from_dict(all_images, orient='index')
	# now in df we have a dataframe with size: dimensions x number_of_images with all of our images
	opt = keras.optimizers.Adam(lr=1e-3)
	# loss = 'binary_crossentropy'
	autoencoder.compile(loss="mse", optimizer=opt, metrics = ['accuracy'])

	# To train it we will need the data
	train, test = train_test_split(df, test_size=0.33, random_state=42)

	# train = train.astype('float32') / 255.
	# test = test.astype('float32') / 255.

	train = train.to_numpy()
	test = test.to_numpy()

	train = np.reshape(train, (len(train), 28, 28, 1))
	test = np.reshape(test, (len(test), 28, 28, 1))

	autoencoder.fit(train, train,
					epochs=epochs,
					batch_size=batch_size,
					shuffle=True,
					validation_data=(test, test),
					callbacks=[keras.callbacks.EarlyStopping(monitor='val_loss', patience=30, restore_best_weights=True)])

	return autoencoder