import serial

CONTROL_BUFFER = ['aa','5a','5a','5a','5a','5a','5a','01','ff']
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
    else:
        CONTROL_BUFFER[7]='00'
    
    for j in range(9):
        control_bytes[j:j+1]=bytes.fromhex(CONTROL_BUFFER[j])
    
    ser.write(control_bytes)
#1  3 4 5； 2号舵机采取了反向措施
if __name__ == '__main__':
    control_motor([1,2,3,4,5,6],[110,80,80,80,80,80],1)
    ser.close()