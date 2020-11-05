# To add a new cell, type '# %%'
# To add a new markdown cell, type '# %% [markdown]'
# %%
from IPython import get_ipython

# %%
os.listdir('../Assign/')


# %%
import os


# %%
# Train 

# %% [markdown]
# # Train 

# %%
import pandas as pd
import numpy as np


# %%
df_train = pd.read_csv('../Assign/train4.csv')


# %%
df_train.head()


# %%
# df_train


# %%
df_train.shape


# %%
data_col = df_train[df_train.columns[1:]]


# %%
# data_col.head()


# %%
X = data_col
y = df_train['Label']

# %% [markdown]
# **Clean data**

# %%
X = X.replace({1: 0, 0:255})


# %%
# X.head()


# %%
# y.head()

# %% [markdown]
# # Generate Picture

# %%
import matplotlib.pyplot as plt


# %%
def display(v):
  tmp = v.reshape(32,24)
  plt.imshow(tmp, cmap='gray')
  plt.show()


# %%
# X.loc[2].values


# %%
# type(X.loc[2].values)


# %%
# display(X.loc[2].values)
# print(y.loc[2])

# %% [markdown]
# # Model

# %%
from sklearn.metrics import accuracy_score
from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestClassifier


# %%
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.33, random_state=999)


# %%
X_train = X_train.reset_index().drop('index', axis=1)
X_test = X_test.reset_index().drop('index', axis=1)
y_train = y_train.reset_index().drop('index', axis=1)
y_test = y_test.reset_index().drop('index', axis=1)


# %%
model = RandomForestClassifier()


# %%
model.fit(X_train, y_train)


# %%
predict = model.predict(X_test)


# %%
# X_test


# %%
accuracy_score(y_test, predict)


# %%
# X_test.shape


# %%
X_train.shape

# %% [markdown]
# # Test function

# %%
def test(model, X, y):
  X = X.reshape((1, -1))
  y_pred = model.predict_proba(X)
  pred = model.predict(X)
  display(X)
  print(y_pred)
  print('Prediction : ', pred)
  print('Type : ', y)


# %%
test(model, X_test.loc[10].values, y_test.loc[10])


# %%
# for i in range(0, 31):
#   test(model, X_test.loc[i].values, y_test.loc[i])

# %% [markdown]
# # Export Model

# %%
from sklearn.externals import joblib


# %%
filename = 'model.joblib'
joblib.dump(model, filename)

# %% [markdown]
# # Test Model

# %%
files.download('model.joblib')


# %%
get_ipython().system('rm -rf test.sav')


# %%
get_ipython().system('ls')


# %%
x = [1,2,3,4]


# %%
np.array(x).reshape((-1, 1))


# %%
type(X.loc[2].values)


# %%
model_test = joblib.load('model.joblib')


# %%
predict = model_test.predict(X.loc[2].values.reshape(1, -1))


# %%



# %%
import cv2
import numpy as np
from sklearn.externals import joblib
from sklearn.ensemble import RandomForestClassifier
img = cv2.imread('D:/work/2D/Datacom/Assign/upper/209.bmp',0)
width = img.shape[1]
height = img.shape[0]
print(width,' ',height)
edgeW = width//10 - 1
edgeH = height//10 - 1
scaleH = (img.shape[0] - 2*edgeH)//3
scaleW = (img.shape[1] - 2*edgeW)//3
l=[]
# for j in range (0,width,width//3 - 1):
#     px = img[height//3 - 10,j]
#     if px < 25:
#         print('1',end = ' ')
#     else:
#         print("0",end = ' ')
# print()

# for i in range (0,height,10):
#     for j in range (0,width,10):
#         px = img[i,j]
#         if px < 25:
#             print('1',end = ' ')
#         else:
#             print("0",end = ' ')
#     print()
(thresh,bw)=cv2.threshold(img,25,255,cv2.THRESH_BINARY)
l=[]
print()
for i in range (0,height,10):
    for j in range (0,width,10):
        if bw[i,j] == 255:
            print('0',end = ' ')
            l.append(255)
        else:
            print("1",end = ' ')
            l.append(0)
    print()
    print(end= ' ')


# %%
X_test1 = np.array(l).reshape(1, -1)


# %%
X_test1


# %%
model = joblib.load('model.joblib')


# %%
model


# %%
predict = model.predict(X_test1)


# %%
predict


# %%
display(X_test1)


# %%



# %%



# %%
model_test

