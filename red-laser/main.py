import sensor, image, time, lcd
from pyb import UART
from pyb import Pin

uart = UART(3,115200)   #定义串口3变量
uart.init(115200, bits=8, parity=None, stop=1) # init with given parameters
button1 = Pin("P1", Pin.IN, Pin.PULL_UP)
button2 = Pin("P9", Pin.IN, Pin.PULL_UP)

'''sensor.reset()
sensor.set_pixformat(sensor.RGB565) # 灰度更快(160x120 max on OpenMV-M7)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)
sensor.set_contrast(1)
sensor.set_windowing((240,240))
clock = time.clock()

lcd.init()'''

shrink=0.9
centre_xy=[0,0]
red=(0, 100, 10, 127, -128, 127)
#max_blob = None
#max_area = 0

x_y = [[0,0],[0,0],[0,0],[0,0]]
x_y_plus = [[0,0],[0,0],[0,0],[0,0]]
x_y_end = [[0,0],[0,0],[0,0],[0,0]]
xy_endend=[[0,0],[0,0],[0,0],[0,0]]
#x_y_all = [[[],[]],[[],[]],[[],[]],[[],[]]]
centre_xy = [0,0]
#去突变点求平均
def remove_outliers_and_calculate_average(arr):
    avg = sum(arr) / len(arr)
    return avg

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
        img.draw_cross(14,14,size=3)
        img.draw_cross(106,14,size=3)
        img.draw_cross(14,106,size=3)
        img.draw_cross(106,106,size=3)
        lcd.display(img)
        '''red_blobs=img.find_blobs([red])
        if red_blobs:
            for red_blob in red_blobs:
                if red_blob.area() > max_area:  #寻找色块最大色块
                    max_blob = red_blob
                    max_area = red_blob.area()
                img.draw_rectangle(max_blob.rect())
                blob_centre_x=max_blob[5]
                blob_centre_y=max_blob[6]
                print(blob_centre_x)
                print(blob_centre_y)'''
def find_rects_xjl():
    global x_y
    global x_y_end
    global xy_endend
    global x_y_all
    global x_y_plus
    TF=1
    j=0#找到满足要求矩形加1
    x_y_plus[0][0]=0
    x_y_plus[0][1]=0
    x_y_plus[1][0]=0
    x_y_plus[1][1]=0
    x_y_plus[2][0]=0
    x_y_plus[2][1]=0
    x_y_plus[3][0]=0
    x_y_plus[3][1]=0
    while(TF):
        clock.tick()
        max_area = 50
        max_rect = None
        img = sensor.snapshot().lens_corr(strength = 0.9, zoom = 1.0)
        img.draw_circle(120,120,5)
        img.draw_cross(32,32,size=3)
        img.draw_cross(208,32,size=3)
        img.draw_cross(32,208,size=3)
        img.draw_cross(208,208,size=3)
        rect_x = img.find_rects(threshold = 30000)
        jiao_num=0
        if rect_x:
            for r in rect_x:
                if r.w()>30 and r.h()>30:
                    area=r.w()*r.h()
                    if area > max_area:
                        max_area = area
                        max_rect = r
            if ( max_rect != None):
                img.draw_rectangle(max_rect.rect(), color = (255, 0, 0))
                #print(max_rect.w())
                #print(max_rect.h())
                for p in max_rect.corners():
                    img.draw_circle(p[0], p[1], 5, color = (0, 255, 0))
                    x_y[jiao_num][0]=p[0]
                    x_y[jiao_num][1]=p[1]
                    x_y_plus[jiao_num][0]+=p[0]
                    x_y_plus[jiao_num][1]+=p[1]
                    #x_y_all[jiao_num][0]+=[p[0]]
                    #x_y_all[jiao_num][1]+=[p[1]]
                    jiao_num+=1
                j+=1
                print('j='+str(j))
                print(x_y)
        if j>=20:
            for i in range(4):
                for n in range(2):
                    x_y_end[i][n]=x_y_plus[i][n]/j
            TF=0
    #
    '''for i in range(4):
        for n in range(2):
            x_y_end[i][n]=remove_outliers_and_calculate_average(x_y_all[i][n])'''
    print(x_y_end)
    centre_xy[0]=(x_y_end[0][0]+x_y_end[1][0]+x_y_end[2][0]+x_y_end[3][0])/4
    centre_xy[1]=(x_y_end[0][1]+x_y_end[1][1]+x_y_end[2][1]+x_y_end[3][1])/4
    #
    for i in range(4):
        for j in range(2):
            xy_endend[i][j]=round(centre_xy[j]+(x_y_end[i][j]-centre_xy[j])*shrink)
    print(xy_endend)
    output_str="%c%c%c%c%c%c%c%c%c" % (xy_endend[3][0],xy_endend[3][1],xy_endend[2][0],xy_endend[2][1],xy_endend[1][0],xy_endend[1][1],xy_endend[0][0],xy_endend[0][1],9)
    uart.write(output_str)

def find_red():
    global clock
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
        k2=button2.value()
        if k2== 0 :
            print('mode3')
            sensor.reset()
            sensor.set_pixformat(sensor.RGB565) # 灰度更快(160x120 max on OpenMV-M7)
            sensor.set_framesize(sensor.QVGA)
            sensor.skip_frames(time = 2000)
            sensor.set_contrast(1)
            sensor.set_windowing((240,240))
            sensor.set_auto_whitebal(False)
            clock = time.clock()
            find_rects_xjl()
            print('mode2')
            sensor.reset()
            sensor.set_pixformat(sensor.RGB565) # 灰度更快(160x120 max on OpenMV-M7)
            sensor.set_framesize(sensor.QVGA)
            sensor.skip_frames(time = 2000)
            sensor.set_contrast(2)
            sensor.set_brightness(-2)
            sensor.set_windowing((240,240))
            sensor.set_auto_whitebal(False)
            clock = time.clock()

        img = sensor.snapshot().lens_corr(strength = 0.9, zoom = 1.0)
        img.draw_line((120,0,120,240))
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
            output_str="%c%c%c" % (round(blob_centre_x),round(blob_centre_y),3)
            uart.write(output_str)
        img.draw_line((xy_endend[0][0],xy_endend[0][1],xy_endend[1][0],xy_endend[1][1]))
        img.draw_line((xy_endend[1][0],xy_endend[1][1],xy_endend[2][0],xy_endend[2][1]))
        img.draw_line((xy_endend[2][0],xy_endend[2][1],xy_endend[3][0],xy_endend[3][1]))
        img.draw_line((xy_endend[3][0],xy_endend[3][1],xy_endend[0][0],xy_endend[0][1]))

while(True):
    k1=button1.value()
    k2=button2.value()
    if k1==0 and k2==1:
        print('mode1')
        duibiao()
    if k2==0 and k1==1:
        print('mode2')
        find_red()
'''find_rects_xjl()
for i in range(4):
    for n in range(2):
        x_y_end[i][n]=remove_outliers_and_calculate_average(x_y_all[i][n])
print('end')
print(x_y_end)'''
#output_str="%c%c%c%c%c%c%c%c%c" % (round(x_y_end[0][0]),round(x_y_end[0][1]),round(x_y_end[1][0]),round(x_y_end[1][1]),round(x_y_end[2][0]),round(x_y_end[2][1]),round(x_y_end[3][0]),round(x_y_end[3][1]),9)
#uart.write(output_str)

#print(x_y_all)
#int(x_y_end)
#image.draw_line((int(x_y_end[0][0]),int(x_y_end[0][1]),int(x_y_end[1][0]),int(x_y_end[1][1])))
#print(x_y_end)
'''centre_xy[0]=(x_y_end[0][0]+x_y_end[1][0]+x_y_end[2][0]+x_y_end[3][0])/4
centre_xy[1]=(x_y_end[0][1]+x_y_end[1][1]+x_y_end[2][1]+x_y_end[3][1])/4
for i in range(4):
    for j in range(2):
        if x_y_end[i][j]>centre_xy[j]:
            xy_endend[i][j]=round(x_y_end[i][j]-shrink)
        else:
            xy_endend[i][j]=round(x_y_end[i][j]+shrink)
print(xy_endend)'''
'''x_1=round((x_y_end[0][0]>centre_xy[0])?)
y_1=round(x_y_end[0][1])
x_2=round(x_y_end[1][0])
y_2=round(x_y_end[1][1])
x_3=round(x_y_end[2][0])
y_3=round(x_y_end[2][1])
x_4=round(x_y_end[3][0])
y_4=round(x_y_end[3][1])
centre_xy[0]=round(x_1+x_2+x_3+x_4)/4
centre_xy[1]=round(y_1+y_2+y_3+y_4)/4'''
'''def duibiao():
    sensor.reset()
    sensor.set_pixformat(sensor.RGB565) # 灰度更快(160x120 max on OpenMV-M7)
    sensor.set_framesize(sensor.QVGA)
    sensor.skip_frames(time = 1000)
    sensor.set_contrast(1)
    sensor.set_windowing((240,240))
    clock = time.clock()
    while(True):
        clock.tick()
        img = sensor.snapshot()
        max_blob = None
        max_area = 0
        img.draw_line((xy_endend[0][0],xy_endend[0][1],xy_endend[1][0],xy_endend[1][1]))
        img.draw_line((xy_endend[1][0],xy_endend[1][1],xy_endend[2][0],xy_endend[2][1]))
        img.draw_line((xy_endend[2][0],xy_endend[2][1],xy_endend[3][0],xy_endend[3][1]))
        img.draw_line((xy_endend[3][0],xy_endend[3][1],xy_endend[0][0],xy_endend[0][1]))
        img.draw_circle(120,120,5)
        img.draw_cross(32,32,size=3)
        img.draw_cross(208,32,size=3)
        img.draw_cross(32,208,size=3)
        img.draw_cross(208,208,size=3)
        red_blobs=img.find_blobs([red])
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
        #print(clock.fps())'''
