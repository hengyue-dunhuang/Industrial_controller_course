CONTROL_BUFFER = ['aa','5a','5a','5a','5a','5a','5a','01','ff']
control_bytes=bytearray(9)
def control_bytes_init():
    for i in range(9):
        control_bytes[i:i+1]=bytes.fromhex(CONTROL_BUFFER[i])
control_bytes_init()