import numpy as np
import matplotlib.pyplot as plt

class Point:
    x = 0
    y = 0
    def __init__(self, x, y):
        self.x = x
        self.y = y  

    def toString(self):
        return str(self.x) + " " + str(self.y)
        

f = open("output.txt", "r")
sizes = f.readline().split(" ")
xSize = int(sizes[0])
ySize = int(sizes[1])
print(xSize)
print(ySize)

points = []
for i in range(0, xSize * ySize):
    p = f.readline().split(" ")
    points.append(Point(float(p[0]), float(p[1])))

print(points[8].toString())


for yi in range(0, ySize - 1):
    for xi in range(0, xSize - 1):
        cur = points[yi * xSize + xi]
        nextX = points[yi * xSize + xi + 1]
        nextY = points[(yi+1) * xSize + xi]
        plt.plot([cur.x, nextX.x], [cur.y, nextX.y], 'ko-')
        plt.plot([cur.x, nextY.x], [cur.y, nextY.y], 'ko-')

for xi in range(0, xSize - 1):
    cur = points[(ySize - 1) * xSize + xi]
    nextX = points[(ySize -1)* xSize + xi + 1]
    plt.plot([cur.x, nextX.x], [cur.y, nextX.y], 'ko-')

for yi in range(0, ySize - 1):
    cur = points[yi * xSize - 1]
    nextY = points[(yi+1) * xSize - 1]
    plt.plot([cur.x, nextY.x], [cur.y, nextY.y], 'ko-')



plt.show()