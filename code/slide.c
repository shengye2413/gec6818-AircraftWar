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
        
} 


//获取实时位置
int real_time_location()
{
    //每次读取都要打开触摸屏文件
    open_file();
    //获取触控屏幕信息
    struct input_event ev;

    //定义起始和终点位置
    int start_x=end_x,start_y=end_y;
    if(ev.type==EV_KEY&&ev.code==BTN_TOUCH&&ev.value==1){
    //记录当前位置，start记录滑动开始位置 
    if(ev.type==EV_ABS&&ev.code==ABS_X)
    {
        if(start_x==end_x)
        start_x=ev.value*0.78;
        end_x=ev.value*0.78;
    }
    if(ev.type==EV_ABS&&ev.code==ABS_Y)
    {
        if(start_y==end_y)
        start_y=ev.value*0.78;
        end_y=ev.value*0.78;
    }
    fclose(abs_screen);
    return 0;}

    /**判断手指是否离开屏幕
     * - ev.type == EV_KEY: 检查事件类型是否为按键事件EV_KEY
     * - ev.code == BTN_TOUCH: 检查按键代码是否为触摸屏按键代码BTN_TOUCH
     * - ev.value == 0: 检查按键值是否为0,在触摸屏上,0表示释放,1表示按下
    */      
}