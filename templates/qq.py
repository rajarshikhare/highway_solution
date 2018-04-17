from flask import Flask, flash, redirect, render_template, request, session, abort
from random import randint
import numpy as np
import os
import serial
import time

ard = serial.Serial('com7', 9600)

app = Flask(__name__)

@app.route("/")
def hello():

    #    return name
    count = np.array([0, 0, 0, 0])
    speed = np.array([0.0, 0.0, 0.0])
    max_car = 10
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

    return render_template(
        'test.html', **locals())


if __name__ == "__main__":
    app.run()
