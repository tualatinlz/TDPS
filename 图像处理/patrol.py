# 鏈€缁堢増鐨勪富鍑芥暟
import sensor, image, time,pyb, lcd
from pyb import LED
from pyb import UART
from pid import PID
#import car
import math
#import struct
class PATROL:
    rho_pid = PID(p=0.4, i=0)
    theta_pid = PID(p=0.001, i=0)
    kernel_size = 1
    kernel = [-1, -1, -1,\
          -1, +8, -1,\
          -1, -1, -1]#杈圭紭妫€娴嬬殑鍗风Н鐭╅樀
    thresholds = [(200, 255)]
    _uart=UART(3,115200)
    _dec_bit=0
    def edge_detect(self):#鎴戜滑瀹氫箟涓€涓�杈圭紭妫€娴嬪嚱鏁�
        #clock.tick()
        img = sensor.snapshot()
        img.morph(self.kernel_size,self.kernel)
        img.binary(self.thresholds)
        img.erode(3, threshold = 2)
        return img
    def UART_STM_ISR(self,t):
        UART_stm.readline() #To avoid reflection
        msg_stm=UART_stm.readline()
        if msg_stm != None:
            msg_stm=msg_stm.decode('utf-8')
            print("STM32 Message: "+str(msg_stm))
        return
    def median(self,data):
        data.sort()
        half = len(data) // 2
        return (data[half] + data[~half])/2
    #define a method to calaulte the 1/2 length of the data
    def line_to_theta_and_rho_error(self,line,img):
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
    def patrol_line(self):
        while(True):
            ave_num=10
            theta_road_vec=[0]*ave_num
            r_vec=[0]*ave_num
            i=0
            while i<ave_num:
                img = self.edge_detect()
                line = img.get_regression([(255,255,0,0,0,0)], robust = True,x_stride=4,y_stride=4)#edge_detection
                if (line):
                    img.draw_line(line.line(), color = 127,thickness=10)
                    # t: sin(deviated angle)   r: horizontal shift
                    theta_err,rho_err = self.line_to_theta_and_rho_error(line, img)
                    theta_road_vec[i]=float(theta_err)
                    r_vec[i]=float(rho_err)
                    i+=1
            theta_road_ave=-sum(theta_road_vec)/ave_num  # minus sign: + all variables left
            rho_ave=-sum(r_vec)/ave_num   # minus sign: + all variables left

            print('output1:',theta_road_ave,rho_ave)
            #theta_road_ave=int(r_ave/15+theta_road_ave)
            rho_out=self.rho_pid.get_pid(rho_ave,1)
            theta_out=self.theta_pid.get_pid(theta_road_ave,1)
            output=rho_out+theta_out
            print(output)
            output_str='%d,%d' % (50+output,50-output)
            print('you send:',output_str)

            #stm_info=int(output)
            LED(1).on()
            self._uart.write(str(output_str+'\r'))
            LED(1).off()
            #print(clock.fps())
            return 1
    def __init__(self,uart,dec_bit=0):
        self._dec_bit=bool(dec_bit)
        self._uart=uart
