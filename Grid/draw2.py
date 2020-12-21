import numpy as np
import matplotlib.pyplot as plt

class Point:
    x1 = 0
    y1 = 0
    x2 = 0
    y2 = 0
    x3 = 0
    y3 = 0
    x4 = 0
    y4 = 0
    area = 0
    def __init__(self, x1, y1, x2, y2, x3, y3, x4, y4, area):
        self.x1 = x1
        self.y1 = y1
        self.x2 = x2
        self.y2 = y2
        self.x3 = x3
        self.y3 = y3
        self.x4 = x4
        self.y4 = y4
        self.area = area  

    def toString(self):
        return str(self.x1) + " " + str(self.y1)
        

f = open("outputXYplane.txt", "r")
sizes = f.readline().split(" ")
xSize = int(sizes[0])
ySize = int(sizes[1])
zSize = int(sizes[2])
print(xSize)
print(ySize)

points = []
for i in range(0, zSize * (xSize - 1) * (ySize - 1)):
    p = f.readline().split(" ")
    #print(p)
    points.append(Point(float(p[0]), float(p[1]), float(p[2]), float(p[3]), float(p[4]), float(p[5]), float(p[6]), float(p[7]), float(p[8])))

def styleByArea(area):
    if area == 1:
        return 'r-'
    if area == 2:
        return 'b-'
    if area == 3:
        return 'g-'
    if area == 4:
        return 'y-'
    if area == -1:
        return 'b-.'
    if area == -2:
        return 'g-.'
    if area == -3:
        return 'y-.'
    else:
        return 'empty'


    print(zSize)

for zi in range(0, zSize):
    #print('-------------')
    #print(zi)
    plt.figure(zi)
    for yi in range(0, ySize - 1 ):
        for xi in range(0, xSize - 1):
            ind = zi * ( (xSize - 1) * (ySize - 1)) + yi * (xSize - 1) + xi
            print(ind)
            cur = points[ind]
            plt.plot([cur.x1, cur.x2], [cur.y1, cur.y2], 'ko-')
            plt.plot([cur.x1, cur.x3], [cur.y1, cur.y3], 'ko-')
            plt.plot([cur.x3, cur.x4], [cur.y3, cur.y4], 'ko-')
            plt.plot([cur.x2, cur.x4], [cur.y2, cur.y4], 'ko-')

            style = styleByArea(int(cur.area))
            #print(int(cur.area))
            if style != 'empty':
                plt.plot([cur.x1, cur.x4], [cur.y1, cur.y4], style)

    plt.show()