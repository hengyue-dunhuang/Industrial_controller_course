import numpy as np
pts = np.zeros((4, 2), dtype=np.float32)
'''LOAD透视变换四个点'''

lines = open("data.txt", "r").readlines() 

for i in range(4):
    pts[i][0] = float((lines[i]).strip('\n').split(',')[0])
    pts[i][1] = float((lines[i]).strip('\n').split(',')[1])
pts

       
