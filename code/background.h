#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__


//初始化屏幕
void lcd_init();

//显示单个像素点
void draw_pixel(int x,int y,int color);

//设置全屏背景颜色
void draw_background(int color);

//画图片
void draw_picture(int posx,int posy,const char* pic_path);

//主界面背景
void Main_Interface();

//游戏界面背景
void Interface();

//排行榜界面
void leaderboard();

//结束界面
void gameover();

#endif 