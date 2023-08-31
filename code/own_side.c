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
        for(int c=x+85;c<800;c++)
        {
            draw_pixel(c,cannonball,0xff00ff);
        }
    }

}


