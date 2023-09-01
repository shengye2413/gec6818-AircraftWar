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
                //创建两个进程，分别表示敌我双方
                pid_t pid=fork();
                //敌方
                if(pid==0)
                {
                    //循环创建敌机
                    while(1)
                    {
                        //最大同时生成6架敌机
                        pthread_t tid[6];
                        int MAX;
                        //随机生成敌机数量
                        MAX=rand()%6+1;
                        for (int i = 0; i < MAX; i++) 
                        {
                            pthread_create(&tid[i], NULL, enemy_air, NULL);
                            sleep(1);
                        }
                        for (int i = 0; i < MAX; i++) 
                        {
                            pthread_join(tid[i], NULL);
                        }
                        
                    }
                }
                else
                {
                    our_air();
                    //wait(NULL);
                    //exit(0);
                }

            }
            
        }
        
    }
     
    return 0;
}