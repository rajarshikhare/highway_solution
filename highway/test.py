import numpy as np
import os
import serial

ard = serial.Serial('com7', 9600 )

count = list([0, 0, 0, 0])

str1 = str("b'2+08+105+1\r\n")
while True:
    os.system("cls")
    print("======== Segment 1 ======== ======== Segment 2 ======== ======== Segment 3 ======== ======== Segment 4 ======== ")
    print("No of car's : ", count[0],"              ", "No of car's : ", count[1],"              ","No of car's : ", count[2],"      ", "No of car's : ", count[3])
    print("=========================== =========================== =========================== ===========================")
    str1 = str(ard.readline().strip())
    i = str1.strip('\'')
    i = str1.strip('b\'')
    a = i.split("+")
    for i in range(0, 4):
        if int(a[i]) < 0:
            count[i] = 0
        else:
            count[i] = int(a[i])

