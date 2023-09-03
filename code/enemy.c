#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "own_side.h"
#include "background.h"
#include "slide.h"

//敌方飞机
void *enemy_air()
{

    //六路，飞机随机生成在某一路
    int j,x=650;
    j=rand()%7*60;
    draw_picture(650,j,"./enemy.bmp"); 
    //飞机向前飞
    while (x)
    {
        //销毁原来位置的敌机
        draw_picture(x+80,j,"./enemy_black.bmp");
        //当敌机在炮弹线上
        if(end_y>=j-50 && end_y<=j+50)
        {
            draw_picture(x+80,j,"./enemy_end.bmp");
            pthread_exit(NULL);
        }
        //不在炮弹线上
        else
        x--;
        //更新敌机位置
        draw_picture(x,j,"./enemy.bmp");
        usleep(300);
    }
    
    //当飞机飞到终点
    if(x==0)
    {
        draw_picture(0,j,"./enemy_end.bmp");
        exit(0);
    }
}
