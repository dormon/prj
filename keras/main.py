#!/usr/bin/python3.4

from keras.models import Sequential
model = Sequential()

from keras.layers import Dense
from keras.utils import plot_model

model.add(Dense(units=64, activation='relu', input_dim=100))
model.add(Dense(units=10, activation='softmax'))

model.compile(loss='categorical_crossentropy',
              optimizer='sgd',
              metrics=['accuracy'])


