#ifndef __OWN_SIDE_H__
#define __OWN_SIDE_H__

//我方飞机
void *our_air();
//获取实时位置
int real_time_location();

//炮弹轨迹
void ball_track(int fire_x,int fire_y);

int cannonball;

#endif