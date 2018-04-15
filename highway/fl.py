from flask import Flask, flash, redirect, render_template, request, session, abort, jsonify
import random
import numpy as np
import os
import serial
import time


app = Flask(__name__)

@app.route("/speed1")
def speed1():
    speed = random.randint(1, 100)
    return jsonify(speed)



@app.route("/")
def hello():
    count = np.random.randint(10, size=4)
    speed = np.random.randint(10, size=4)
    return render_template(
        'test.html', **locals())


if __name__ == "__main__":
    app.run(debug = True)
