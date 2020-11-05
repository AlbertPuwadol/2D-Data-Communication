import cv2
import numpy as np
import matplotlib.pyplot as plt
from sklearn.externals import joblib
from sklearn.ensemble import RandomForestClassifier
img = cv2.imread('C:/out/5.bmp',0)
width = img.shape[1]
height = img.shape[0]
print(width,' ',height)
edgeW = width//10 - 1
edgeH = height//10 - 1
scaleH = (img.shape[0] - 2*edgeH)//3
scaleW = (img.shape[1] - 2*edgeW)//3
l=[]
(thresh,bw)=cv2.threshold(img,60,255,cv2.THRESH_BINARY)

l=[]
print()
for i in range (0,height,10):
    for j in range (0,width,10):
        if bw[i,j] == 255:
            #print('0',end = ' ')
            l.append(255)
        else:
            #print("1",end = ' ')
            l.append(0)
    # print()
    # print(end= ' ')

X_test = np.array(l, dtype='int64').reshape(1, -1)
tmp = np.array(l, dtype='int64').reshape(32, 24)
print(X_test)
model = joblib.load('model.joblib')
y_pred = model.predict_proba(X_test)
predict = model.predict(X_test)

print(y_pred)
print('Prediction : ', predict)
plt.imshow(tmp, cmap='gray')
plt.show()
cv2.imshow('image',img)
cv2.imshow('imagebw',bw)
cv2.waitKey(0)
cv2.destroyAllWindows()
