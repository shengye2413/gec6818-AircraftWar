#include <pthread.h>
#include "background.h"
#include "own_side.h"
#include "slide.h"

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
    static int last_x=0;
    static int last_y=0;
    int x=end_x+80;
    int y=end_y+50;
    //画出激光
    while(x<750)
    {
        draw_pixel(x,y,0xff00ff);
        x++;
    }
    //删除原激光
    if(y!=last_y)
    {
        while(last_x<750)
        {
            draw_pixel(last_x,last_y,0x000000);
            last_x++;
        }
        last_x=x;
        last_y=y;
        pthread_exit(0);
    }
    //保存上一条激光位置

    
}