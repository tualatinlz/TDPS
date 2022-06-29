#颜色+形状占空比识别三种图形封装测试完OK

import sensor, image, time, math
from pyb import UART
import ustruct
from pyb import LED
class SHAPE:
    thresholds = [(0, 27, 37, -75, -90, 65)] # generic_black_thresholds，太阳
    #thresholds = [(91, 13, 31, -16, -84, -5)] # generic_black_thresholds，阴天
    _uart=UART(3,115200)
    def sending_data(self,x):
        #global uart
        data=ustruct.pack("<bbbbbb", #格式为两个字符，两个短整型（2字节）
                0xAA, #帧头1
                0xFF, #帧头2
                0xA3, #形状识别码
                int(x),
                0xBB,
                0xfe) #尾帧\r
        self._uart.write(data)
    def shape_rec(self):
      while(True): #不断拍照
        #clock.tick()
        img = sensor.snapshot().lens_corr(1.8) #拍摄一张照片，lens_corr函数用于非鱼眼畸变矫正，默认参数为1.8，
        for blob in img.find_blobs(self.thresholds,pixels_threshold=200,area_threshold=200):#寻找色块
    #pixels_threshold：像素阈值，面积小于这个值的色块就忽略
    #roi：只在这个区域内寻找色块
    #area_threshold：面积阈值，色块被框起来的面积小于这个值就忽略
    #blob.density() ：色块面积/最小外接矩形面积
            if blob.density()>0.85: #判断矩形；理论上矩形和其外接矩形完全重合，但有少许误差，0.805为考虑误差后所得
                print('retangle',blob.density())
                LED(2).on()
                self.sending_data(x=1)
                LED(2).off()
                img.draw_rectangle(blob.rect()) #画个矩形框
                return 1 #1代表正方形或者长方形
            elif blob.density()>0.70: #判断圆
                print('circle',blob.density())
                LED(2).on()
                self.sending_data(x=2)
                LED(2).off()
                img.draw_keypoints([(blob.cx(), blob.cy(), int(math.degrees(blob.rotation())))], size=20)
                img.draw_circle((blob.cx(), blob.cy(),int((blob.w()+blob.h())/4))) #画个圆框
                return 2 #2代表圆形
            elif blob.density()>0.3: #判断三角形
                print('triangle',blob.density())
                LED(2).on()
                self.sending_data(x=3)
                LED(2).off()
                img.draw_cross(blob.cx(), blob.cy()) #中间画个叉
                return 3 #3代表三角形
            else: #占空比小于0.4的基本都是干扰或者三角形
                print('no dectedtion',blob.density())
                LED(2).on()
                self.sending_data(x=3)
                LED(2).off()
                return 0 #0代表没有识别到图形
    def __init__(self,uart):
            self._uart=uart
    #print(clock.fps()) #每秒处理几帧图片。如果FPS过低，可能就是算法过于复杂或者图像太大了，需要降低清晰度
