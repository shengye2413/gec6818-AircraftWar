#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


#include "slide.h"
#include "background.h"
#include "own_side.h"
#include "enemy.h"

int main()
{
    //主界面
    Main_Interface();
    while (1)
    {
        //主界面点击选择
        if(direction()==0)
        {
            //退出
            if(end_x>650 && end_x<800 &&end_y>0 &&end_y<150)
            {
                draw_background(0x000000);
                exit(0);
            }
            //排行榜
            else if(end_x>700 && end_x<800 &&end_y>360 &&end_y<480)
            {
                leaderboard();
                while(direction()==0)
                {

                }
            }
            //开始游戏
            else if(end_x>70 && end_x<200 &&end_y>100 &&end_y<300)
            {
                Interface();
                pid_t pid=fork();
                if(pid==0)
                {
                    while(1)
                    {
                        pthread_t tid;
                        pthread_create(&tid,NULL,enemy_air,NULL);
                    
                        sleep(1);
                    }
                }
                else
                {
                    our_air();
                    wait(NULL);
                    exit(0);
                }

            }
            
        }
        
    }
     
    return 0;
}