import sensor, image, time, lcd
from pyb import UART
from pyb import Pin

uart = UART(3,115200)   #定义串口3变量
uart.init(115200, bits=8, parity=None, stop=1) # init with given parameters
button1 = Pin("P1", Pin.IN, Pin.PULL_UP)
button2 = Pin("P9", Pin.IN, Pin.PULL_UP)

lcd.init()

red=(0, 100, 10, 127, -128, 127)

clock = time.clock()
def find_red():
    sensor.reset()
    sensor.set_pixformat(sensor.RGB565) # 灰度更快(160x120 max on OpenMV-M7)
    sensor.set_framesize(sensor.QVGA)
    sensor.skip_frames(time = 2000)
    sensor.set_contrast(2)
    sensor.set_brightness(-2)
    sensor.set_windowing((240,240))
    sensor.set_auto_whitebal(False)
    clock = time.clock()
    while(True):
        clock.tick()
        img = sensor.snapshot().lens_corr(strength = 0.9, zoom = 1.0)
        img.draw_line((120,0,120,240))
        img.draw_circle(120,120,5)
        red_blobs=img.find_blobs([red])
        max_blob = None
        max_area = 0
        if red_blobs:
            for red_blob in red_blobs:
                if red_blob.area() > max_area:  #寻找色块最大色块
                    max_blob = red_blob
                    max_area = red_blob.area()
            img.draw_rectangle(max_blob.rect())
            blob_centre_x=max_blob[5]
            blob_centre_y=max_blob[6]
            print(blob_centre_x)
            print(blob_centre_y)
        else:
            blob_centre_x=120
            blob_centre_y=120
        output_str="%c%c%c" % (round(blob_centre_x),round(blob_centre_y),3)
        uart.write(output_str)
        #lcd.display(img)

def duibiao():
    sensor.reset()
    sensor.set_pixformat(sensor.RGB565) # 灰度更快(160x120 max on OpenMV-M7)
    sensor.set_framesize(sensor.QQVGA)
    sensor.skip_frames(time = 2000)
    sensor.set_contrast(1)
    sensor.set_windowing((120,120))
    clock = time.clock()

    lcd.init()

    while(True):
        clock.tick()
        img = sensor.snapshot().lens_corr(strength = 0.9, zoom = 1.0)
        max_blob = None
        max_area = 0
        '''img.draw_line((xy_endend[0][0],xy_endend[0][1],xy_endend[1][0],xy_endend[1][1]))
        img.draw_line((xy_endend[1][0],xy_endend[1][1],xy_endend[2][0],xy_endend[2][1]))
        img.draw_line((xy_endend[2][0],xy_endend[2][1],xy_endend[3][0],xy_endend[3][1]))
        img.draw_line((xy_endend[3][0],xy_endend[3][1],xy_endend[0][0],xy_endend[0][1]))'''
        img.draw_circle(60,60,5)
        lcd.display(img)

while(True):
    k1=button1.value()
    k2=button2.value()
    if k1==0 and k2==1:
        print('mode1')
        duibiao()
    if k2==0 and k1==1:
        print('mode2')
        clock = time.clock()
        find_red()
