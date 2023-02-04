from flask import Flask,jsonify,make_response
from random import randint
import json
with open("test.json",'r') as file:
    MNISTdata = json.load(file)
app = Flask(__name__)


@app.route('/randM')
def postData():
    v = [i+128 for i in MNISTdata[str(randint(0,100))]['data']]
    return make_response(bytes(v))

if __name__ == '__main__':
    app.run(debug=True,host="0.0.0.0")