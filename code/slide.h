#ifndef __SLIDE_H__
#define __SLIDE_H__

int end_x,end_y;
int *shm_p;
//打开触摸屏文件
void open_file();

//计算点击位置
int direction();

//关闭触摸屏文件
void close_file();


//获取实时位置
int real_time_location();

#endif