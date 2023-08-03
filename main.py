import sensor, image, time
from pyb import UART

uart = UART(3,115200)   #定义串口3变量
uart.init(115200, bits=8, parity=None, stop=1) # init with given parameters

sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE) # 灰度更快(160x120 max on OpenMV-M7)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time = 2000)
sensor.set_contrast(1)
sensor.set_windowing((120,120))
clock = time.clock()

i=0

x_y = [[0,0],[0,0],[0,0],[0,0]]
#x_y_plus = [[0,0],[0,0],[0,0],[0,0]]
x_y_end = [[0,0],[0,0],[0,0],[0,0]]
x_y_all = [[[],[]],[[],[]],[[],[]],[[],[]]]

#去突变点求平均
def remove_outliers_and_calculate_average(arr, threshold_x):
    if len(arr) == 0:
        return None
    avg = sum(arr) / len(arr)
    filtered_arr = [x for x in arr if abs(x - avg) <= threshold_x]
    if len(filtered_arr) == 0:
        return None
    print(filtered_arr)
    return sum(filtered_arr) / len(filtered_arr)

def find_rects_xjl():
    global x_y
    global x_y_plus
    TF=1
    j=0#找到满足要求矩形加1
    while(TF):
        clock.tick()
        max_area = 50
        max_rect = None
        img = sensor.snapshot().lens_corr(strength = 0.9, zoom = 1.0)
        #img = sensor.snapshot()
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
                    #x_y_plus[jiao_num][0]+=p[0]
                    #x_y_plus[jiao_num][1]+=p[1]
                    x_y_all[jiao_num][0]+=[p[0]]
                    x_y_all[jiao_num][1]+=[p[1]]
                    jiao_num+=1
                j+=1
                print('j='+str(j))
                print(x_y)
        if j>=100:
            #for i in range(4):
                #for n in range(2):
                    #x_y_end[i][n]=x_y_plus[i][n]/j
            TF=0

threshold_xjl = 5  # 设定阈值
find_rects_xjl()
for i in range(4):
    for n in range(2):
        x_y_end[i][n]=remove_outliers_and_calculate_average(x_y_all[i][n], threshold_xjl)
print('end')
print(x_y_end)
output_str="%c%c%c%c%c%c%c%c%c" % (round(x_y_end[0][0]),round(x_y_end[0][1]),round(x_y_end[1][0]),round(x_y_end[1][1]),round(x_y_end[2][0]),round(x_y_end[2][1]),round(x_y_end[3][0]),round(x_y_end[3][1]),9)
uart.write(output_str)

#print(x_y_all)
#int(x_y_end)
#image.draw_line((int(x_y_end[0][0]),int(x_y_end[0][1]),int(x_y_end[1][0]),int(x_y_end[1][1])))
#print(x_y_end)
x_1=int(x_y_end[0][0])
y_1=int(x_y_end[0][1])
x_2=int(x_y_end[1][0])
y_2=int(x_y_end[1][1])
x_3=int(x_y_end[2][0])
y_3=int(x_y_end[2][1])
x_4=int(x_y_end[3][0])
y_4=int(x_y_end[3][1])
while(True):
    clock.tick()
    img = sensor.snapshot()
    img.draw_line((x_1,y_1,x_2,y_2))
    img.draw_line((x_2,y_2,x_3,y_3))
    img.draw_line((x_3,y_3,x_4,y_4))
    img.draw_line((x_4,y_4,x_1,y_1))
    img.draw_circle(60,60,5)
    img.draw_cross(16,16,size=3)
    img.draw_cross(104,16,size=3)
    img.draw_cross(16,104,size=3)
    img.draw_cross(104,104,size=3)
    #print(clock.fps())