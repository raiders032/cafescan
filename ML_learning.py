# Import modules
import numpy as np
from keras.models import Sequential
from keras.layers import Dense

# setting random seed
np.random.seed(5)

# 1. Load dataset
dataset = np.loadtxt("./data.csv", delimiter=",")

# 2. Reshaping the data
x_train = dataset[:700,0:8]
y_train = dataset[:700,8]
x_test = dataset[700:,0:8]
y_test = dataset[700:,8]

# 3. Creating the DNN model and Adding layers to the model
model = Sequential()
model.add(Dense(12, input_dim=8, activation='relu'))
model.add(Dense(8, activation='relu'))
model.add(Dense(1, activation='sigmoid'))

# 4. Compiling the model
model.compile(loss='binary_crossentropy', optimizer='adam', metrics=['accuracy'])

# 5. Training the model
model.fit(x_train, y_train, epochs=3000, batch_size=128)

# 6. Evaulating the model
scores = model.evaluate(x_test, y_test)
print("%s: %.2f%%" %(model.metrics_names[1], scores[1]*100))

#7.Saving the model
from keras.models import load_model
model.save('ML.h5')
