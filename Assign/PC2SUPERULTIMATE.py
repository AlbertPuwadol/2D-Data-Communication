import serial
import os
import time
from subprocess import Popen,PIPE
import cv2
import numpy as np
from sklearn.externals import joblib
from sklearn.ensemble import RandomForestClassifier



def takepic():
	print("in take picture")
	os.chdir('C:/Program Files (x86)/Java/jdk1.8.0_74/bin')
	process = Popen(['java','code.SimpleRead'],stdout=PIPE,stderr=PIPE)
	while process.poll() is None:
		li = process.stdout.readline()
		if str(li).find('image: 6') != -1:
			time.sleep(4)
			Popen.kill(process)
			print("end process")

servo = serial.Serial('COM9',baudrate = 115200,timeout = 1)				
respond = 'f'
state = 0
spic = ''				#picture L-->M-->R

while 1:
	servo.write(b'\0')
	# 
	if servo.inWaiting():
		respond = servo.readline().decode('ascii')
		print(respond)
	if state == 0:

		if respond == 'y':
			print('Start ')
			respond = 'c'
			spic = ''			#picture L-->M-->R
		elif respond == 'c':
			print('Take picture')
			# servo.write(b'\0')
			takepic()
			#input image
			img = cv2.imread('C:/out/5.bmp',0)
			(thresh,bw)=cv2.threshold(img,40,255,cv2.THRESH_BINARY)
			l=[]
			width = img.shape[1]
			height = img.shape[0]
			#append data 
			for i in range (0,height,10):
				for j in range (0,width,10):
					l.append(bw[i,j])
			X_test = np.array(l, dtype='int64').reshape(1, -1)
			#predict
			model = joblib.load('D:/work/2D/Datacom/Assign/model.joblib')
			y_pred = model.predict_proba(X_test)
			predict = model.predict(X_test)
			print(y_pred)
			print('Prediction : ', predict[0])
			spic = spic + str(predict[0])
			print(spic)
			servo.write(b's')
			respond = '\0'
			if len(spic) == 3:
				servo.write(b'z')		#send next ztate to Arduino Servo
				for i in spic:
					#print(respond)
					if i is '0':
						servo.write(b'0')
						print("send0")
					elif i is '1':
						servo.write(b'1')
						print("send1")
					elif i is '2':
						servo.write(b'2')
						print("send2")
					elif i is '3':
						servo.write(b'3')
						print("send3")
					elif i is '4':
						servo.write(b'4')
						print("send4")	
					elif i is '5':
						servo.write(b'5')
						print("send5")
				print("Send All photo") 
				respond = 'f'
		elif respond == 'x':			#change state for PC2					
			print("Welcome to next state eiei")
			state = 1
			respond = 'f'	
	elif state == 1:
		if respond == 'c':
			print("Take picture")
			# servo.write(b'\0')
			takepic()
			#-----------input image
			img = cv2.imread('C:/out/5.bmp',0)

			width = img.shape[1]
			height = img.shape[0]
			#append data 
			send=[]
			for i in range (0,width,79):
				for j in range (0,width,79):
					send.append(img[i,j])
					send.append(i)
					send.append(j)
			print(send)
			servo.write(b'o')   		#set state = 5 to arduino
			print('inwouza99')
			for i in send:
				data = bytes([i])
				servo.write(data) 

			respond = 'f'