# import module
from keras.utils import np_utils
from keras.datasets import mnist
from keras.models import Sequential
from keras.layers import Dense, Activation
import numpy as np
import pymysql
from sklearn.preprocessing import StandardScaler
from sklearn.model_selection import train_test_split
from keras.callbacks import EarlyStopping 

# MySQL Connection
conn = pymysql.connect(host='localhost', user='admin', password='4885',
                       db='cafe', charset='utf8')
# Create Dictoionary Cursor 
curs = conn.cursor(pymysql.cursors.DictCursor) 

# Load dataset
dataset = np.loadtxt("./data2.csv", delimiter=",")
end_idx=(len(dataset))
pre_date = int(dataset[end_idx-1][1])

while(True):
  # Load dataset
  dataset = np.loadtxt("./data2.csv", delimiter=",")
  end_idx=(len(dataset))
  cur_date = int(dataset[end_idx-1][1])
  if cur_date == pre_date :
    continue
  else:
    pre_date=cur_date
  #standardizing the input feature
  X = dataset[end_idx-4:end_idx,2:202]
  X /=1700
  Y = dataset[end_idx-4:end_idx,202]
  
  # Loading the model
  from keras.models import load_model
  model = load_model('ML.h5')

  # prediction
  yhat = model.predict(X)
  yhat2 = model.predict_classes(X)
  flag={'101101101':2,'101101102':2,'101102101':2,'101102102':2}
  for i in range (1,5):
    if flag[str(int(dataset[end_idx-i][0]))]==2:
      print(str(int(dataset[end_idx-i][0]))+"is chaged")
      flag[str(int(dataset[end_idx-i][0]))]=int(yhat2[4-i])
    else:
      break
  print(cur_date)	  
  print(yhat)
  print(yhat2)  
  print(flag)
  sql = """
  insert into result(C01T01S01,C01T01S02,C01T02S01,C01T02S02)
  	     values (%s, %s, %s, %s)"""
  curs.execute(sql, (flag['101101101'],flag['101101102'],flag['101102101'],flag['101102102']))
  conn.commit()
 
conn.close()	
