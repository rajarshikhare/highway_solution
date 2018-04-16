from flask import Flask, flash, redirect, render_template, request, session, abort, jsonify
import random
import numpy as np
import os
import time
import _thread

count = np.zeros(4)

def get_data():
    global count
    while True:
        #here was ardiuno code for recieving data
        count = np.random.randint(100, size=4)
        time.sleep(1)

_thread.start_new_thread(get_data, ())


app = Flask(__name__)


@app.route("/")
def first():
    return render_template(
        'test.html', **locals())

@app.route("/start_ardiuno", methods=['GET', 'POST'])
def start_ardiuno():
    global count
    data = count.tolist()
    #count = np.random.randint(100, size=4).tolist()
    return jsonify(count=data)


if __name__ == "__main__":
    app.run(debug=True)
