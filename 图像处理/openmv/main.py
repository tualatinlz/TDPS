# main.py -- put your code here!
# 最终版的主函数
import sensor, image, time,pyb, lcd
from pyb import LED
from pyb import UART
from pid import PID
from patrol import PATROL
from shape import SHAPE
from wave import WAVE
#from apriltag import APRILTAG
import math
#import struct
sensor.reset()
#sensor.set_vflip(True)
#sensor.set_hmirror(True)
sensor.set_pixformat(sensor.RGB565)#设置为灰度图像
sensor.set_framesize(sensor.QQVGA)#不行就用qqq：80x60 (4,800 pixels) - O(N^2) max = 2,3040,000.
sensor.set_auto_gain(True) # 关闭自动增益
sensor.set_auto_whitebal(True) # 关闭白平衡。颜色识别中要关闭
#sensor.set_windowing([0,20,80,40])
#sensor.skip_frames(time = 2000)
uart1=UART(3,115200)
uart2=UART(1,115200)
clock = time.clock()                # to process a frame sometimes.
led = pyb.LED(2)
clock = time.clock()                # to process a frame sometimes.
class ctrl(object):
    work_mode = 0x00 #工作模式，可以通过串口设置成其他模式
ctrl=ctrl()
def change_mod():
    if(uart1.any()):
        txt=uart1.read()
        #LED(1).on()
        if(len(txt)>=3):
            if(txt[0]==0xAA):
                #LED(2).on()
                if(txt[1]==0xff):
                    ctrl.work_mode=txt[2]
                    print('mode:',txt[2])

uart1 = UART(3,115200)

uart_b= UART(1,115200)
while(True):
    clock.tick()
    img = sensor.snapshot()#.binary([THRESHOLD])
    #img.lens_corr(1.2)
    change_mod()
    if (ctrl.work_mode==0x00):#MODE1 待机
        True
    if (ctrl.work_mode==0x01):#MODE1 开始巡线 green
        car_patrol=PATROL(dec_bit=1,uart=uart1)
        retu_val=car_patrol.patrol_line()
        print('the distance is: %d',retu_val)
    if (ctrl.work_mode==0x02):#MODE2 形状识别 blue
        car_shape=SHAPE(uart=uart1)
        retu_val=car_shape.shape_rec()
        LED(3).on()
        print ('shape is ',retu_val)
        LED(3).off()
        ctrl.work_mode=0
    if (ctrl.work_mode==0x03):#MODE3 超声波 red
        car_wave=WAVE(uart=uart1)
        retu_val=car_wave.wave_patrol()
        print ('wave is ',retu_val)
    if (ctrl.work_mode==0x04):#MODE4 标记识别
        car_april=APRILTAG(uart=uart1)
        retu_val=car_april.apriltag_det()
        print ('april is ',retu_val)
    else:
        True
