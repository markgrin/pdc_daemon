import requests 
import random
  
def get (action, args):
    URL = "http://127.0.0.1:1235/" + action
    r = requests.get(url = URL, params = args)
    print (r)

session = str(random.randint(0, 100000))

get ('begin', {'stat_size' : 200, 'session' : session})

for i in range(1000):
    get ('add', {'setup' : random.randint(0, 10), 'talk' : random.randint(0, 10), 'session' : session})
    get ('get', {'free' : random.randint(0, 10), 'incoming' : random.randint(0, 10), 'session' : session})

get ('end', {'session' : session})
