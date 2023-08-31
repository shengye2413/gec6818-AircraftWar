#include "background.h"
#include "own_side.h"
#include "slide.h"

//我方飞机
void *our_air()
{
    int x=0,y=190;
    draw_picture(0,190,"./ourair.bmp");
    while (real_time_location()==0 && end_x>0 && end_x<750 && end_y<380)
    {
        draw_picture(x,y,"./black.bmp");
        draw_picture(end_x,end_y,"./ourair.bmp");
        x=end_x;
        y=end_y;
        cannonball=y+50;
        while (x<800)
        {
            ball_track(x,cannonball);
            x++;
        }
        
    }

}

//炮弹轨迹
void ball_track(int fire_x,int fire_y)
{
    for(int i=fire_x;i<fire_x+10;fire_x++)
        {
            draw_pixel(i,fire_y,0xff00ff);
        }
}