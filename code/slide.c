#include <stdio.h>
#include <linux/fb.h>
#include <stdlib.h>
#include <linux/input.h>//事件结构体头文件
#include "slide.h"
#include <fcntl.h>
#include <unistd.h>

FILE *abs_screen ;

//打开触摸屏文件
void open_file()
{
    abs_screen = fopen("/dev/input/event0","r");
    if (abs_screen==NULL)
    {
        perror("open abs failed\n");
        exit(0);
    }
}
 
//关闭触摸屏文件
void close_file()
{
    fclose(abs_screen);
}

//计算点击位置
int direction()
{
    //每次读取都要打开触摸屏文件
    open_file();
    //获取触控屏幕信息
    struct input_event ev;

    //定义起始和终点位置
    int start_x=-1,start_y=-1;
    
    //循环一直读取触摸位置
    while (1)
    {
        // 判断读取的数据大小是否正确
        if(fread(&ev,sizeof(ev),1,abs_screen) == 0)
            continue;

        //记录当前位置，start记录滑动开始位置
        if(ev.type==EV_ABS&&ev.code==ABS_X)
        {
            if(start_x==-1)
            start_x=ev.value*0.78;
            end_x=ev.value*0.78;
        }
        if(ev.type==EV_ABS&&ev.code==ABS_Y)
        {
            if(start_y==-1)
            start_y=ev.value*0.78;
            end_y=ev.value*0.78;
        }
        /**判断手指是否离开屏幕
         * - ev.type == EV_KEY: 检查事件类型是否为按键事件EV_KEY
         * - ev.code == BTN_TOUCH: 检查按键代码是否为触摸屏按键代码BTN_TOUCH
         * - ev.value == 0: 检查按键值是否为0,在触摸屏上,0表示释放,1表示按下
        */
        if(ev.type==EV_KEY&&ev.code==BTN_TOUCH&&ev.value==0)
        {
            //消抖，当起始位置和终点位置相差5个像素以下时，判定为点击
            if(abs(end_x-start_x) <5 && abs(end_y-start_y) <5)
            return 0;
            //把起始位置重新置为-1
            start_x=-1;
            start_y=-1;
        }
    }
    close_file();

} 

int real_time_location()
{
    open_file();
    struct input_event ev;
    int x_ready = 0; // 标记 x 坐标是否已准备好
    int y_ready = 0; // 标记 y 坐标是否已准备好

    while (!x_ready || !y_ready)
    {
        fread(&ev, sizeof(ev), 1, abs_screen);

        if (ev.type == EV_ABS)
        {
            if (ev.code == ABS_X)
            {
                end_x = ev.value * 0.78;
                x_ready = 1; // x 坐标已准备好
            }
            else if (ev.code == ABS_Y)
            {
                end_y = ev.value * 0.78;
                y_ready = 1; // y 坐标已准备好
            }
            //限制飞机不要超出屏幕
            if (x_ready && y_ready)
            {
                // if (end_y >= 380 && end_x < 750)
                // {
                //     end_y = end_y - (100 - (480 - end_y));
                // }
                // else if (end_x > 750 && end_x < 800 && end_y > 50 && end_y < 400)
                // {
                //     end_x = end_x - (150 - (800 - end_y));
                // }
                // else
                {
                    break;
                }
            }
        }
    }
}
