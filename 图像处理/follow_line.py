# 最终版的主函数
import sensor, image, time,pyb, lcd
from pyb import LED
from pyb import UART
from pid import PID
#import car
import math
#import struct
rho_pid = PID(p=0.4, i=0)
theta_pid = PID(p=0.001, i=0)
kernel_size = 1
kernel = [-1, -1, -1,\
          -1, +8, -1,\
          -1, -1, -1]#边缘检测的卷积矩阵
thresholds = [(200, 255)]#边缘检测参数,改变左边参数使边缘特征保留的更多
#thresholds = [(188, 255)]
#THRESHOLD = (20, 255, 0, 0, 0, 0) # Grayscale threshold for dark things...寻黑线时解锁
#speed=50#定义小车最大速度
sensor.reset()
#sensor.set_vflip(True)
#sensor.set_hmirror(True)
sensor.set_pixformat(sensor.GRAYSCALE)#设置为灰度图像
sensor.set_framesize(sensor.QQVGA)#不行就用qqq：80x60 (4,800 pixels) - O(N^2) max = 2,3040,000.
#sensor.set_windowing([0,20,80,40])
#sensor.skip_frames(time = 2000)

clock = time.clock()                # to process a frame sometimes.
def edge_detect():#我们定义一个边缘检测函数
    #clock.tick()
    img = sensor.snapshot()
    img.morph(kernel_size, kernel)
    img.binary(thresholds)
    img.erode(3, threshold = 2)
    return img

def UART_STM_ISR(t):
    UART_stm.readline() # To avoid reflection
    msg_stm=UART_stm.readline()
    if msg_stm != None:
        msg_stm=msg_stm.decode('utf-8')
        print("STM32 Message: "+str(msg_stm))
    return
def median(data):
    data.sort()
    half = len(data) // 2
    return (data[half] + data[~half])/2
    #define a method to calaulte the 1/2 length of the data
def line_to_theta_and_rho_error(line,img):
    if(line):
        rho_err=abs(line.rho())-img.width()/2
        if line.theta()>90:
            theta_err=line.theta()-180
        else:
            theta_err=line.theta()
        return (theta_err, rho_err)
    else:
        print('there is something error with line error obtain')
        return 0
#return the distance between the line to the middle of the screen

#return the slope of the line to the middle of the screen
while(True):
    #clock.tick()
    ave_num=10
    theta_road_vec=[0]*ave_num
    r_vec=[0]*ave_num
    i=0
    while i<ave_num:
        img = edge_detect()#edge_detection
        #img = sensor.snapshot().binary([THRESHOLD])#black_line_find
        line = img.get_regression([(255,255,0,0,0,0)], robust = True,x_stride=4,y_stride=4)#edge_detection
        if (line):
            img.draw_line(line.line(), color = 127,thickness=10)
            # t: sin(deviated angle)   r: horizontal shift
            theta_err,rho_err = line_to_theta_and_rho_error(line, img)
            theta_road_vec[i]=float(theta_err)
            r_vec[i]=float(rho_err)
            i+=1
    #print(theta_road_vec)

    # angle direction: +turn left, -right
    # horizontal shift: -on right side, + on left side
    theta_road_ave=sum(theta_road_vec)/ave_num  # minus sign: + all variables left
    rho_ave=sum(r_vec)/ave_num   # minus sign: + all variables left
    #theta_road_ave=int(median(theta_road_vec))
    #print(r_ave)
    #img.draw_line(line.line())
    print('output1:',theta_road_ave,rho_ave)
    #theta_road_ave=int(r_ave/15+theta_road_ave)
    rho_out=rho_pid.get_pid(rho_ave,1)
    theta_out=theta_pid.get_pid(theta_road_ave,1)
    output=rho_out+theta_out
    print(output)
    output_str='%d,%d' % (50+output,50-output)
    print('you send:',output_str)

    UART_stm=pyb.UART(3)
    UART_stm.init(115200, bits=8, parity=1, stop=1, timeout_char=100)

    UART_stm.irq(trigger = pyb.UART.IRQ_RXIDLE, handler = UART_STM_ISR)

    stop_bit=1
    #stm_info=int(output)
    UART_stm.write(str(output_str+'\n')+str(stop_bit))

    print(clock.fps())
    #time.sleep(0.1)
    #lcd.display(img, x=0, y=0, x_scale=0.5, y_scale=0.32)