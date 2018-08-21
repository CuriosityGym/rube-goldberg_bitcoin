from flask import Response, Flask, g, current_app as app
import os
import requests
import random
import string
app = Flask(__name__,static_url_path='/static')
errorText="Error"

@app.route("/")
def hello():
   return app.send_static_file('index.html')   

   
@app.route('/<path:path>')
def static_file(path):
    return app.send_static_file(path)
'''
@app.route("/newPage")
def newPage():
   return "This is the new Page"

@app.route("/add/<a>/<b>")
def parameters(a,b):
   return str(int(a)+int(b))
'''


def email_alert(first, second, key):
    report = {}
    report["value1"] = first
    report["value2"] = second
    #report["value3"] = third
    requests.post("https://maker.ifttt.com/trigger/bitcoin/with/key/" + str(key), data=report)    

def error_email_alert(first, key):
    report = {}
    report["value1"] = first
    #report["value2"] = second
    #report["value3"] = third
    requests.post("https://maker.ifttt.com/trigger/error/with/key/" + str(key), data=report)    

def random_generator(size=6, chars=string.ascii_uppercase):
    return ''.join(random.choice(chars) for x in range(size))

@app.route("/device/<id>")
def device_id(id):
   g.id = id
   user_emailID = os.environ.get(g.id)
   secret_code = random_generator()
   IFTTT_KEY = os.environ.get('IFTTT_KEY')
   
   if(user_emailID is not None):
      
      email_alert(user_emailID, secret_code, IFTTT_KEY)
      return app.send_static_file('index.html')
   else:
      error_email_alert(g.id, IFTTT_KEY)
      return errorText
       
   

 

if __name__ == "__main__":
    port = int(os.environ.get("PORT", 5000))
    app.run(host='0.0.0.0', port=port,debug=True)
