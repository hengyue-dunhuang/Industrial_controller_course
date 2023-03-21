import cv2 
import numpy as np
# 读取图像文件
img = cv2.imread("IMG2.jpg")
print(img.shape)

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


# 定义高斯核的大小和标准差
ksize = (11, 11)  # 高斯核大小
sigma = 0.0  # 标准差

# 进行高斯模糊
blurred_img = cv2.GaussianBlur(resized_img, ksize, sigma)
# 将图像转换为HSV颜色空间
hsv_img = cv2.cvtColor(blurred_img, cv2.COLOR_BGR2HSV)

def color_detect(hsv_img,color,num):
    K = num
    if(color=='red'):
        '''red'''
        # 定义HSV红色的阈值范围
        lower_red = np.array([0, 50, 50])
        upper_red = np.array([10, 255, 255])
        lower_red2 = np.array([170, 50, 50])
        upper_red2 = np.array([180, 255, 255])

        # 创建一个掩膜来只保留在HSV范围内的红色像素
        mask = cv2.inRange(hsv_img, lower_red, upper_red)
        mask2 = cv2.inRange(hsv_img, lower_red2, upper_red2)
        mask = cv2.add(mask, mask2)
        # 应用掩膜到原始图像中
        res = cv2.bitwise_and(resized_img, resized_img, mask=mask)
        # 将图像转换为灰度图像
        gray = cv2.cvtColor(res, cv2.COLOR_BGR2GRAY)
        # 进行边缘检测
        edges = cv2.Canny(gray, 50, 150)
        # 查找边缘的轮廓
        contours, _ = cv2.findContours(edges, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        # 找到最大的K个轮廓
        # 计算每个轮廓的面积
        areas = [cv2.contourArea(c) for c in contours]
        # 排序并选择前K个最大的面积
        max_areas = sorted(areas, reverse=True)[:K]
        # 遍历所有轮廓，将面积大于等于第K个最大面积的轮廓添加到一个新的列表中
        selected_contours = []
        for c in contours:
            if cv2.contourArea(c) in max_areas:
                selected_contours.append(c)

        # 绘制轮廓
        cv2.drawContours(resized_img, selected_contours, -1, (0, 255, 255), 2)
        return res
    if(color=='green'):
        # 定义HSV绿色的阈值范围
        lower_green = np.array([35, 43, 46])
        upper_green = np.array([77, 255, 255])

        # 创建一个掩膜来只保留在HSV范围内的绿色像素
        mask = cv2.inRange(hsv_img, lower_green, upper_green)

        # 应用掩膜到原始图像中
        res = cv2.bitwise_and(resized_img, resized_img, mask=mask)
        # 将图像转换为灰度图像
        gray = cv2.cvtColor(res, cv2.COLOR_BGR2GRAY)
        # 进行边缘检测
        edges = cv2.Canny(gray, 50, 150)
        # 查找边缘的轮廓
        contours, _ = cv2.findContours(edges, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        # 找到最大的K个轮廓
        # 计算每个轮廓的面积
        areas = [cv2.contourArea(c) for c in contours]
        # 排序并选择前K个最大的面积
        max_areas = sorted(areas, reverse=True)[:K]
        # 遍历所有轮廓，将面积大于等于第K个最大面积的轮廓添加到一个新的列表中
        selected_contours = []
        for c in contours:
            if cv2.contourArea(c) in max_areas:
                selected_contours.append(c)

        # 绘制轮廓
        cv2.drawContours(resized_img, selected_contours, -1, (0, 255, 255), 2)
        return res
    
def show(img):
    cv2.imshow('',img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

if __name__ == '__main__':
    res = color_detect(hsv_img,'green',2)
    show(resized_img)
    


