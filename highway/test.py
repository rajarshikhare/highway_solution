import numpy as np
import os
import serial
import time
import _thread

ard = serial.Serial('com7', 9600 )

count = np.array([0, 0, 0, 0])
speed = np.array([0.0, 0.0, 0.0])
max_car = 10;

def acc(count):
    car_sum = 0
    started = False
    start_time = 0
    printed = False
    while True:
        car_sum = count.sum(axis = 0)
        if car_sum > 0 and started == False:
            start_time = time.clock()
            started = True
            printed = False
        if car_sum == 0 and started :
            started = False
        if time.clock() - start_time > 10 and printed == False:
            print("               There is an Accident!!!")
            printed = True


#_thread.start_new_thread(acc, (count,))

str1 = str("b'2+08+105+1\r\n")
while True:
    os.system("cls")
    print("======== Segment 1 ======== ======== Segment 2 ======== ======== Segment 3 ======== ")
    print("Traffic level : ", round(count[0]*(100/max_car), 2),"%     ", "Traffic level : ", round(count[1]*(100/max_car), 2),"%      ","Traffic level : ", round(count[2]*(100/max_car), 2),"%")
    print("Speed : ", speed[0],"m/s               ", "Speed : ", speed[1],"m/s              ","Speed : ", speed[2], "m/s")
    print("=========================== =========================== ===========================")
    str1 = str(ard.readline().strip())
    i = str1.strip('\'')
    i = str1.strip('b\'')
    a = i.split("+")
    for i in range(0, 3):
        speed[i] = float(a[i])
    for i in range(3, 6):
        if int(a[i]) < 0:
            count[i-3] = 0
        else:
            count[i-3] = int(a[i])
