#include "background.h"
#include "own_side.h"
#include "slide.h"

//我方飞机
void *our_air()
{
    static int x,y;
    draw_picture(x,y,"./black.bmp");
    draw_picture(end_x,end_y,"./ourair.bmp");
    x=end_x;
    y=end_y;
    cannonball=y+50;  
}

//炮弹轨迹
void ball_track(int fire_x,int fire_y)
{
    for(int i=fire_x;i<fire_x+10;fire_x++)
        {
            draw_pixel(i,fire_y,0xff00ff);
        }
}