import cv2
import numpy as np
import csv
# 定义回调函数
def get_four_points(event, x, y, flags, param):
    global counter, pts
    if event == cv2.EVENT_LBUTTONDOWN:
        if counter < 4:
            pts[counter] = [x, y]
            counter += 1
            cv2.circle(img, (x, y), 5, (0, 0, 255), -1)

def get_real_position(pixel_position):
    real_x = pixel_position[0] * A4_LEN/width
    real_y = pixel_position[1] * A4_WIGTH/height
    return real_x,real_y
# 定义全局变量
counter = 0
pts = np.zeros((4, 2), dtype=np.float32)
A4_LEN,A4_WIGTH = 297,210   #mm

# 读取输入图像
img = cv2.imread('my_IMG1.jpg')

# 获取原始图片的宽度和高度
h, w = img.shape[:2]
# 计算缩放比例
scale = 1080 / max(h, w)
# 计算调整后的宽度和高度
new_w = int(w * scale)
new_h = int(h * scale)
# 调整图片大小
resized_img = cv2.resize(img, (new_w, new_h))
print(resized_img.shape)

cv2.imshow('请设定四个透视点',resized_img)
cv2.setMouseCallback('请设定四个透视点', get_four_points)

while True:
    cv2.imshow('请设定四个透视点',resized_img)
    if counter == 4:
        break
    cv2.waitKey(1)
'''设置四个透视变换点到文件'''
with open("data.txt", "w") as f:
    
    for i in range(4):
        f.write(str(pts[i][0]))
        f.write(',')
        f.write(str(pts[i][1]))
        f.write('\n')
    

# 设置输出图像的四个点的坐标
width, height = 707, 500
pts2 = np.float32([[0, 0], [width, 0], [0, height], [width, height]])

M = cv2.getPerspectiveTransform(pts, pts2)
result = cv2.warpPerspective(resized_img, M, (width, height))
test_position = [400,400]
dis = get_real_position(test_position)

cv2.imshow('Output', result)
cv2.waitKey(0)
cv2.destroyAllWindows()
