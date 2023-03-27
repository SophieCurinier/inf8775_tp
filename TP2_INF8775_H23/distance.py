import math
import numpy as np

def euclidianDistance(x1,y1,x2,y2):
    return int(math.sqrt((x2-x1)**2+(y2-y1)**2))

def calcul(tab):
    length = len(tab)
    matrix = np.zeros((length,length))
    for i in range(length):
        for j in range(length):
            if (i != j):
                matrix[i][j] = euclidianDistance(tab[i][0],tab[i][1],tab[j][0],tab[j][1])
                matrix[j][i] = matrix [i][j]
    return matrix

tableau = [[942, 1701],[1454, 928], [1859, 1434], [1090, 1589], [1923, 566]]
T = calcul(tableau)
length = len(tableau)
print(length)
for i in range(length):
   for j in range(length):
      toPrint = str(T[i][j])
      if (j == length - 1):
        toPrint += ".\n"
      print(toPrint,end=" ")
            

