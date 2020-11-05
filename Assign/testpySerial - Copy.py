import serial
import io
servo = serial.Serial('COM10',baudrate = 9600,timeout = 1)
camera = serial.Serial('COM3',baudrate = 9600,timeout = 1)

respondServo = 'f'
respondCamera = 'f'
state = 0
while 1:

    if respondServo == 'f':
        data = input('Start ?(y/n) :')
        if data is 'y':
            servo.write(b's')
            respondServo = 'n'
    else:
        respondServo = servo.readline().decode('ascii')
        respondCamera = camera.readline().decode('ascii')

    if respondServo == 'c':
        camera.write(b'c')

    if respondCamera == 'f':
        servo.write(b's')