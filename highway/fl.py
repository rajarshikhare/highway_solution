from flask import Flask, flash, redirect, render_template, request, session, abort, jsonify
import random
import numpy as np
import os
import time

app = Flask(__name__)

@app.route("/speed1")
def speed1():
    speed = random.randint(1, 100)
    return jsonify(speed)

@app.route("/")
def hello():
    return render_template(
        'test.html', **locals())

@app.route("/home", methods=['GET', 'POST'])
def home():
    first_name = "Pinii"
    last_name = "mitti"
    age = 21
    sex = "female"
    count = np.random.randint(100, size=4).tolist()
    return jsonify(first_name=first_name, last_name=last_name, age=age, sex=sex, count=count)


@app.route("/index")
def index():
    str = "this is a index page"
    return str

if __name__ == "__main__":
    app.run(debug=True)
