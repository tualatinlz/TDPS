import time,utime,pyb, machine
from pyb import Pin
from pid import PID
from pyb import UART
from pyb import LED
import math
import ustruct
class WAVE:
    a=0
    distance_initial=15
    distance_pid = PID(p=0.6, i=0.2,d=0.1)
    distance_6cmpid=PID(p=0.1,i=0)
    wave_distance_last=0
    scaler=4
    _uart=UART(3,115200)
    def wave_distance_process(self,wave_echo_pin,wave_trig_pin):
        wave_trig_pin.value(0)
        utime.sleep_us(5)
        wave_trig_pin.value(1)
        utime.sleep_us(10)
        wave_trig_pin.value(0)
        try:
            tim_counter = machine.time_pulse_us(wave_echo_pin, 1, 30000)
        except:
            print('error',wave_echo_pin.value())
        wave_distance = tim_counter*0.017
        if wave_distance<3:
            return self.wave_distance_last
        elif wave_distance>150:
            self.sending_data2(x=0,y=0)
            return self.wave_distance_last
        else:
            self.wave_distance_last=wave_distance
            return wave_distance
    def sending_data2(self,x,y):
        data=ustruct.pack("<bbbbbb",
                0xAA,
                0xFF,
                0xA2,
                int(x),
                int(y),
                0xfe)
        self._uart.write(data)
    def sending_data(self,x,y):
        data=ustruct.pack("<bbbbbb",
                0xAA,
                0xFF,
                0xA1,
                int(x),
                int(y),
                0xfe)
        self._uart.write(data)
    def wave_patrol(self):
        while (True):
            while self.a%2==0:
                wave_echo_pin_1 = Pin('P8', Pin.IN, Pin.PULL_NONE)
                wave_trig_pin_1= Pin('P9', Pin.OUT_PP, Pin.PULL_DOWN)
                distance1=self.wave_distance_process(wave_echo_pin=wave_echo_pin_1,wave_trig_pin=wave_trig_pin_1)
                self.a+=1
                print('distance1:',distance1)
                time.sleep_ms(100)
            else:
                wave_echo_pin_2 = Pin('P2', Pin.IN, Pin.PULL_NONE)
                wave_trig_pin_2 = Pin('P3', Pin.OUT_PP, Pin.PULL_DOWN)
                distance2=self.wave_distance_process(wave_echo_pin=wave_echo_pin_2,wave_trig_pin=wave_trig_pin_2)
                self.a+=1
                print('distance2:',distance2)
                time.sleep_ms(100)
            distance_error=distance1-distance2-0.5
            distance_6cm=distance1+distance2-2*self.distance_initial
            print('distance error',distance_error,'distance to initial',distance_6cm)
            error_pid=self.distance_pid.get_pid(distance_error,1)
            drift_pid=self.distance_6cmpid.get_pid(distance_6cm,1)
            total_error=error_pid+1.2*drift_pid
            print('total error',total_error)
            left=int(20+self.scaler*total_error)
            right=int(20-self.scaler*total_error)
            print('speed,left',left,'right',right)
            LED(1).on() #red
            self.sending_data(x=left,y=right)
            LED(1).off()
            return 1
    def __init__(self,uart):
        self._uart=uart
