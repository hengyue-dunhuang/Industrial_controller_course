import cv2 as cv
import numpy as np
from time import sleep
print(cv.__version__)

cap = cv.VideoCapture(1)

if not cap.isOpened():
    print("Cannot open camera")
    exit()
sleep(5)
ret, frame = cap.read()
cv.imwrite('my_IMG1.jpg',frame)

# When everything done, release the capture
cap.release()
cv.destroyAllWindows()