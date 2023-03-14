import serial
import math
from math import atan,acos,atan2,sqrt
from time import sleep
CONTROL_BUFFER = ['aa','5a','5a','5a','5a','5a','00','01','ff']
control_bytes=bytearray(9)

ser = serial.Serial('COM8',115200)
try:
    ser.open()
except:
    print('is open')

while(ser.is_open!=True):
    ser = serial.Serial('COM8',115200)
    print('waitning for connect\n')

print(ser.name)         # check which port was really used

# ser.write(b'hello')     # write a string

def control_bytes_init():
    for i in range(9):
        control_bytes[i:i+1]=bytes.fromhex(CONTROL_BUFFER[i])
control_bytes_init()

def control_motor(index:list,angle:list,mode:int =1):
    if 1 in index:
        angle[index.index(1)]=180-angle[index.index(1)]
    if 3 in index:
        angle[index.index(3)]=180-angle[index.index(3)]
    if 4 in index:
        angle[index.index(4)]=180-angle[index.index(4)]
    if 5 in index:
        angle[index.index(5)]=180-angle[index.index(5)]
   
    for i in range(len(index)):
        if(len(hex(angle[i])[2:])==1):
            CONTROL_BUFFER[index[i]]='0'+hex(angle[i])[2:]
        else: CONTROL_BUFFER[index[i]]=hex(angle[i])[2:]

    if(mode==1):
        CONTROL_BUFFER[7]='01'
    elif(mode==0):
        CONTROL_BUFFER[7]='00'
    elif(mode==2):
        CONTROL_BUFFER[7]='02'

    
    for j in range(9):
        control_bytes[j:j+1]=bytes.fromhex(CONTROL_BUFFER[j])
    
    ser.write(control_bytes)
#1  3 4 5； 2号舵机采取了反向措施


def aim_it(position): #0 - 180
    '''通过给定position的位置求出1号舵机的转动角度，并控制其转动'''
    x = position[0]
    y = position[1]
    if(x == 0):
        Fai=math.pi/2
    else : Fai = atan2(abs(y),abs(x))
    if x>=0 and y>=0:
        control_theta = math.pi-Fai
    if x<0 and y>0:
        control_theta = Fai
    control_theta = int(control_theta/math.pi * 180)
    control_motor([1],[control_theta],2)



def get_theta(a:float,b:float,c:float) -> int:
    '''通过余弦定理，给定三角形的两对边和临边，求出该角角度（°）'''
    theta =  acos((pow(a,2)+pow(b,2)-pow(c,2))/(2*a*b)) 
    theta = theta/math.pi * 180
    return int(theta)

def get_it(position):
    L1 = 10.35  #CM
    L2 = 8.8    #CM
    L3 = 17.0   #CM

    x = position[0]
    y = position[1]
    z = position[2]
    a = sqrt(x*x+y*y+z*z)
    theta1 = get_theta(a,L1,L2)
    fai = (int)(atan2(z,sqrt(x*x+y*y))/math.pi * 180)
    theta1 = 180 - (theta1+fai)     #2号舵机控制量

    theta2 = get_theta(L1,L2,a)
    theta2 = 270 - theta2   #3号舵机控制量

    if(theta1<=180 and theta2 <=180):
        print('I CAN MAKE IT')
    
    else:
        print('I CAN\'T DO THAT')

def get_it2(position):
    
    L1 = 10.35  #CM
    L2 = 8.8    #CM
    L3 = 17.0   #CM

    x = position[0]
    y = position[1]
    z = position[2]

    '''通过相似三角形计算出关节4(从下往上第四个舵机)需要达到的位置'''
    K = sqrt(pow(x,2)+pow(y,2))/(sqrt(pow(x,2)+pow(y,2)) - L3)
    y = (1/K)*y
    x = (1/K)*x

    a = sqrt(x*x+y*y+z*z)
    theta1 = get_theta(a,L1,L2)
    fai = (int)(atan2(z,sqrt(x*x+y*y))/math.pi * 180)
    theta1 = 180 - (theta1+fai)     #2号舵机控制量

    theta2 = get_theta(L1,L2,a)
    theta2 = 270 - theta2   #3号舵机控制量

    Kesai = get_theta(L2,a,L1)
    Gamma = 90 - fai
    theta3 = 360 - (180 + Kesai + Gamma)


    if(theta1<=180 and theta2 <=180):
        print('I CAN MAKE IT')
        control_motor([2,3,4],[theta1,theta2,theta3],2)
    else:
        print('I CAN\'T DO THAT')


position = [0,31,0]   # test cm   31 MIN--36 MAX
    
if __name__ == '__main__':
    # control_motor([1,2,3,4,5,6],[90,90,90,90,90,40],2) #init 
    # aim_it(position = position)
    # sleep(0.01)
    # control_motor([6],[40],2)
    # control_motor([1,2,3,4,5,6],[90,146,168,0,90,40],2)
    control_motor([1,2,3,4,5,6],[90,90,90,80,90,50],2)
    get_it2(position)
    ser.close()