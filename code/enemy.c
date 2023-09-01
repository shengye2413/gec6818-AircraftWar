#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "own_side.h"
#include "background.h"



//敌方飞机
void *enemy_air()
{

    //六路，飞机随机生成在某一路
    int j,x=650;
    j=rand()%7*60;
        cannonball=240;
    draw_picture(650,j,"./enemy.bmp");
    //飞机向前飞
    while (x)
    {
        draw_picture(x+80,j,"./enemy_black.bmp");
        x--;
        draw_picture(x,j,"./enemy.bmp");
        usleep(0);
        
    }
    //当飞机飞到终点
    if(x==0)
    {
        draw_picture(0,j,"./enemy_end.bmp");
        exit(0);
        //pthread_exit(NULL);
    }
    
}
