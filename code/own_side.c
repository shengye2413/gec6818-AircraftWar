#include <pthread.h>
#include <stdio.h>
#include "background.h"
#include "own_side.h"
#include "slide.h"
int last_x=80,last_y=240;

//我方飞机
void *our_air()
{
    static int x,y;
    draw_picture(x,y,"./black.bmp");//清除原位置飞机
    draw_picture(end_x,end_y,"./ourair.bmp");//生成新的飞机
    //原坐标更新为新坐标
    x=end_x;
    y=end_y;
}

//炮弹轨迹
void *ball_track()
{
    //定义last保存上一条激光位置
    int x=last_x+80;
    int y=last_y+50;
    //删除原激光
    while(x<750)
    {
        draw_pixel(x,y,0x000000);
        x++;
    }
    //画新激光
    int i=end_x+80;
    while(i<750)
    {
        draw_pixel(i,end_y+50,0xff00ff);
        i++;
    }
    //保存上一条激光位置
    last_x=end_x;
    last_y=end_y;
    pthread_exit(0);
}