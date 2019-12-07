import requests 
  
for i in range(1000):
    # api-endpoint 
    URL = "http://127.0.0.1:1235/test123"
      
    # location given here 
    location = "delhi technological university"
      
    # defining a params dict for the parameters to be sent to the API 
    PARAMS = {'address':location} 
      
    # sending get request and saving the response as response object 
    data = {
        'some_input_name': 'some input value',
        'another_input_name': 'another input value',
    }
    
    r = requests.get(url = URL, params = PARAMS)
      
    # extracting data in json format 
    print(r)
    data = r.json() 
    
    print(data)
