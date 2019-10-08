'''
IPK - Projekt c.1, varianta 2: Klient pro OpenWeatherMap API
Benjamin Kosa, XKOSAB00
'''
import socket
import ssl
import json
import sys

'''
Ziskanie dat z argumentov
'''
try:
    api = sys.argv[1]#9e1762fd79830eca28488019093282ca
    city = sys.argv[2]
except IndexError:
    print("Error: wrong number of arguments")
    sys.exit()

'''
Odoslanie get requestu, vytvorenie socketu
'''
server_host = "api.openweathermap.org"
server_port = 80

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

request = "GET /data/2.5/weather?q="+city+"&APPID="+api+"&units=metric HTTP/1.1\r\nConnection: close\r\nHost: "+server_host+"\r\n\r\n"

s.connect((server_host,server_port))
s.sendall(request.encode())

'''
Ziskanie dat od servera
'''
data_str = ""
while True:
    recv = s.recv(1024)
    if not recv:
        s.close()
        break
    data_str += recv.decode('utf8')

'''
Odstranienie zbytocnych dat
'''
data = ""
allow = 0
for i in range(0, data_str.__len__()):
    if data_str[i] == "{":
        allow = 1
    if allow:
        data += data_str[i]

data_json = json.loads(data) #prevod dat na json

'''
Spracovanie dat + vystup
'''
try: #ak najde error data
    err_cod = data_json['cod']
    err_msg = data_json['message']
    print("Error: ", err_cod, ", ", err_msg)
    exit()
except KeyError:#ak nenajde error tak nic nespravi
    1+1

try: #vyskusa vytiahnut data
    city = data_json['name']
    print(city)
except KeyError: print(city)#ak sa nepodari

try: #vyskusa vytiahnut data
    description = data_json['weather'][0]['description']
    print(description)
except KeyError: print('No description')#ak sa nepodari

try: 
    temp = data_json['main']['temp']
    print('Temperature: {} °C'.format(temp))
except KeyError: print('Temperature: n/a')

try: 
    humidity = data_json['main']['humidity']
    print('Humidity: {}%'.format(humidity))
except KeyError: print('Humidity: n/a')

try: 
    pressure = data_json['main']['pressure']
    print('Pressure: {} hPa'.format(pressure))
except KeyError: print('Pressure: n/a')

try: 
    wind_speed = data_json['wind']['speed']
    print('Wind Speed: {} km/h'.format('{:03.2f}'.format(wind_speed*3.6)))
except KeyError: print('Wind Speed: n/a')

try: 
    wind_deg = data_json['wind']['deg']
    print('Wind degree: ', '{:03.2f}'.format(wind_deg))
except KeyError: print('Wind degree: n/a')
