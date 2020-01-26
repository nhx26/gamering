import math

items = 16
r = 30
for i in range(items):
    x = r * math.cos(2 * math.pi * i/items) 
    y =  r * math.sin(2 * math.pi * i/items) 

    print("Coordinates: ")
    print("x: ", x , "y: ", y, '\n')

x = 360/items
for i in range(1,17):
	print(x*i)