# Import modules
import numpy as np
from keras.models import Sequential
from keras.layers import Dense
from sklearn.preprocessing import StandardScaler
from sklearn.model_selection import train_test_split
from keras.callbacks import EarlyStopping

# Setting random seed
np.random.seed(5)

# Load dataset
dataset = np.loadtxt("./forML_V2.csv", delimiter=",")
x_train = dataset[:2660,:200]
y_train = dataset[:2660,200]
x_test = dataset[2660:,:200]
y_test = dataset[2660:,200]

x_train /=1700
x_test /=1700

# Creating the DNN model and Adding layers to the model
model = Sequential()
model.add(Dense(200, input_dim=200, activation='relu'))
model.add(Dense(100, activation='relu'))
model.add(Dense(50, activation='relu'))
model.add(Dense(25, activation='relu'))
model.add(Dense(10, activation='relu'))
model.add(Dense(5, activation='relu'))
model.add(Dense(1, activation='sigmoid'))

# Compiling the model
model.compile(loss='binary_crossentropy', optimizer='adam', metrics=['accuracy'])

# Training the model
#early_stopping = EarlyStopping(patience = 20)
model.fit(x_train, y_train, epochs=100, batch_size=128, verbose=1,validation_data=(x_test, y_test))#,callbacks=[early_stopping])

# Evaulating the model
eval_model=model.evaluate(x_test, y_test)
print(eval_model)

# Prediction
y_pred = model.predict(x_test)

# Saving the model
from keras.models import load_model
model.save('ML_V2.h5')
