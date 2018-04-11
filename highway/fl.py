from flask import Flask, flash, redirect, render_template, request, session, abort
from random import randint
import numpy as np
 
app = Flask(__name__)
 
#@app.route("/")
#def index():
#    return "Flask App!"


c = 1
#@app.route("/hello/<string:name>")
@app.route("/")
def hello():
#    return name
    global c
    count = np.array([1, 2, 3])
    c = c + 1
    count[0] = count[0] + c
    return render_template(
        'test.html',**locals())
 
if __name__ == "__main__":
    app.run()