# File: /home/data/questus/app.py
from flask import Flask, render_template

app = Flask(__name__)

@app.route('/')
def show_home_page():
    return render_template("index.html")

@app.route('/explore')
def show_explore_page():
    return render_template("explore.html")

if __name__ == '__main__':
    app.run(debug=True)