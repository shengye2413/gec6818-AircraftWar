#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <wait.h>
#include <signal.h>
#include "slide.h"
#include "background.h"
#include "own_side.h"
#include "enemy.h"
#define IPC_PATH ("./")
#define IPC_CODE1 (20230826)
#define IPC_CODE2 (20230904)
int shmid;
int main()
{ 
    // 创建System V IPC设施的key
    key_t key1 = ftok(IPC_PATH,IPC_CODE1);
    key_t key2 = ftok(IPC_PATH,IPC_CODE2);
    // 创建或打开一个共享内存
    int shm_id1 = shmget(key1,128,IPC_CREAT|0600);
    int shm_id2 = shmget(key2,128,IPC_CREAT|0600);
    // 映射共享内存
    shm_px = shmat(shm_id1,NULL,0);
    shm_py = shmat(shm_id2,NULL,0);
    //主界面
    MAIN:
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
                    goto MAIN;
                }
            }
            //开始游戏
            else if(end_x>70 && end_x<200 &&end_y>100 &&end_y<300)
            {
                again:
                Interface();//游戏界面
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
                            sleep(1.2);
                        }
                        //回收线程资源
                        for (int i = 0; i < MAX; i++) 
                        {
                            pthread_join(tid[i], NULL);
                        }
                        
                    }
                }
                //我方
                else
                {
                    //飞机初始位置
                    draw_picture(0,190,"./ourair.bmp");
                    while (real_time_location())
                    {
                        *shm_py=end_y;
                        *shm_px=end_x;
                        //清除初始位置的飞机
                        draw_picture(0,190,"./black.bmp");
                        int status;
                        pid_t result = waitpid(pid, &status, WNOHANG);//非阻塞等待
                        //子进程未终止
                        if(result==0)
                        {
                            //游戏运行
                            if (end_x>0 && end_x<750 && end_y>0 && end_y<480)
                            {
                                //飞机与炮弹线程
                                our_air();
                                pthread_t tid;
                                pthread_create(&tid,NULL,ball_track,NULL);
                                pthread_join(tid,NULL);
                            }
                            //返回主界面
                            else if (end_x>750 && end_x<800 && end_y>0 && end_y<50)
                            {
                                kill(pid,SIGKILL);
                                goto MAIN;
                            }
                            //暂停游戏
                            else if (end_x>750 && end_x<800 && end_y>400 && end_y<480)
                            {
                                draw_picture(350,140,"./continue.bmp");
                                kill(pid, SIGSTOP); // 发送SIGSTOP信号暂停子进程
                                //获取到点击事件继续游戏
                                if(direction()==0)
                                {
                                    kill(pid, SIGCONT); // 发送SIGUSR1信号唤醒子进程
                                    draw_picture(0,0,"./Interface.bmp");
                                }
                            }
                        }
                        //子进程终止结束父进程
                        else if(result==pid)
                        {
                            //如果子进程结束游戏结束
                            if(WIFEXITED(status))
                            gameover();
                            while(direction()==0)
                            {
                                //返回主界面
                                if(end_x>50 && end_x<=200)
                                goto MAIN;
                                else if(end_x>220 && end_x<=320)
                                goto again;
                            }
                        } 
                        close_file();
                    }

                }

            }
            
        }
        
    }
    // 解除映射
    shmdt(shm_px);
    shmdt(shm_py);
    // 关掉
    shmctl(shm_id1,IPC_RMID,NULL);
    shmctl(shm_id2,IPC_RMID,NULL);
    return 0;
}