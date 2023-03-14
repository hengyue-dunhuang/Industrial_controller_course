from math import acos
import math
def get_theta(a:float,b:float,c:float) -> int:
    '''通过余弦定理，给定三角形的两对边和临边，求出该角角度（°）'''
    theta =  acos((pow(a,2)+pow(b,2)-pow(c,2))/(2*a*b)) 
    theta = theta/math.pi * 180
    return int(theta)
x = get_theta(1,1.414,1)
print(x)

