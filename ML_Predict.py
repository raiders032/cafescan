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
import time

# MySQL Connection 연결
conn = pymysql.connect(host='localhost', user='admin', password='4885',
                       db='cafe', charset='utf8')
# Connection 으로부터 Dictoionary Cursor 생성
curs = conn.cursor(pymysql.cursors.DictCursor) 

# Loading the model
from keras.models import load_model
model = load_model('ML_V2.h5')

pre_flag_list=[3,3,3,3]

while(True):
  time.sleep(3);
  # Load dataset
  dataset = np.loadtxt("./data2.csv", delimiter=",")
  end_idx=(len(dataset))
  cur_date = int(dataset[end_idx-1][1])
  
  #standardizing the input feature
  X = dataset[end_idx-4:end_idx,2:102]
  X /=1700
  Y = dataset[end_idx-4:end_idx,202] 

  # prediction
  yhat = model.predict_classes(X)
  flag={'101101101':2,'101101102':2,'101102101':2,'101102102':2}
  for i in range (1,5):
    if flag[str(int(dataset[end_idx-i][0]))]==2:
      flag[str(int(dataset[end_idx-i][0]))]=int(yhat[4-i])
    else:
      break
  if pre_flag_list==list(flag.values()):
    continue
  pre_flag_list = list(flag.values())
  print(cur_date)	  
  print(flag)
  sql = """
  insert into result(C01T01S01,C01T01S02,C01T02S01,C01T02S02)
  	     values (%s, %s, %s, %s)"""
  curs.execute(sql, (flag['101101101'],flag['101101102'],flag['101102101'],flag['101102102']))
  conn.commit()
conn.close()	
