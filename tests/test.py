import requests 
import random
  
def get (action, args):
    URL = "http://127.0.0.1:1235/" + action
    r = requests.get(url = URL, params = args)
    print (r.json())

session = str(random.randint(0, 100000))

get ('begin', {'stat_size' : 200, 'session' : session})

for i in range(1000):
    get ('add', {'agents' : 20, 'setup' : random.randint(0, 10), 'service' : random.randint(0, 10), 'session' : session})
    get ('add', {'busy' : 1, 'agents' : 20, 'setup' : random.randint(0, 10), 'service' : random.randint(0, 10), 'session' : session})
    get ('get', {'agents' : 20, 'service' : random.randint(0, 10), 'setup' : random.randint(0, 10),
'session' : session})

get ('end', {'session' : session})
