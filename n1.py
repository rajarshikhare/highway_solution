from flask import Flask, jsonify, render_template, request
app = Flask(__name__)

@app.route('/_add')
def add_numbers(): 
    return jsonify(12)

@app.route('/')
def index():
    return render_template('index.html')


if __name__ == "__main__":
    app.run(debug=True)