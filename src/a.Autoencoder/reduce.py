import sys
from reader import handleInput, reader, writeToFile
from autoencoder import convolutionalAutoencoder, training
import tensorflow as tf
import keras
from keras import layers
import pandas as pd
import numpy as np

def main(argv):
	(dataset, queryset, output_dataset_file, output_query_file) = handleInput(argv)
	if not dataset:
		sys.exit("You need to provide a dataset file path")
	epochs = 1
	autoencoder = convolutionalAutoencoder()
	all_images = reader(dataset)
	query_images = reader(queryset)
	autoencoder = training(autoencoder, all_images, epochs)

	# 
	# Get the encoder from autoencoder model
	encoder = tf.keras.models.Sequential()
	encoder.add(layers.Input(shape=(28, 28, 1)))
	for i in range(0, len(autoencoder.layers)//2):
		encoder.add(autoencoder.layers[i])

	test = pd.DataFrame.from_dict(query_images, orient='index')
	test = test.to_numpy()
	test = np.reshape(test, (len(test), 28, 28, 1))

	train = pd.DataFrame.from_dict(all_images, orient='index')
	train = train.to_numpy()
	train = np.reshape(train, (len(train), 28, 28, 1))
	# Get the images from the Embedded layer
	query_imgs = encoder.predict(test)
	dataset_imgs = encoder.predict(train)

	# 
	
	normalized_query = []
	for i in range(len(query_imgs)):
		normalized_query.append((25500*(query_imgs[i] - np.min(query_imgs[i]))/np.ptp(query_imgs[i])).astype(int))

	normalized_dataset = []
	for i in range(len(dataset_imgs)):
		normalized_dataset.append((25500*(dataset_imgs[i] - np.min(dataset_imgs[i]))/np.ptp(dataset_imgs[i])).astype(int))

	normalized_query = np.asarray(normalized_query)
	normalized_dataset = np.asarray(normalized_dataset)
	# 
	writeToFile(normalized_query, output_query_file)
	writeToFile(normalized_dataset, output_dataset_file)

	# 
	return 0

if __name__ == "__main__":
	main(sys.argv[1:])