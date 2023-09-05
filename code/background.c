#include <stdio.h>
#include <sys/ioctl.h>
#include <linux/fb.h>//开发板显示屏配置头文件
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include "background.h"
#include "enemy.h"

/**
 * 这个结构体用于描述帧缓冲设备的屏幕信息,包括:
 *xres/yres - 屏幕的水平和垂直分辨率
 *bits_per_pixel - 每个像素的位数
 *grayscale - 是否是灰度屏
 *red/green/blue - 红绿蓝色彩亮度范围
 *transp - 透明度值
 *rotate - 屏幕旋转角度
 */
struct fb_var_screeninfo lcd_info;

int lcd_fd=-1;
int *lcd_fp=NULL;


//初始化屏幕
void lcd_init()
{
    //打开屏幕配置文件
    lcd_fd=open("/dev/fb0",O_RDWR);
    if(lcd_fd == -1 )
    {
        perror("open lcd failed");
        return ;
    }
    /**
     *  lcd_fd: 这是打开帧缓冲设备文件后返回的文件描述符。
     *  FBIOGET_VSCREENINFO: 这是一个请求码,表示要获取变量屏幕信息
     *  &lcd_info: 这是struct fb_var_screeninfo类型的指针,用于返回获取到的屏幕信息。
    */
    ioctl(lcd_fd,FBIOGET_VSCREENINFO,&lcd_info);

    //内存映射
    //计算所需要的内存大小，横纵像素数乘每个像素所占的位数除以8，1字节占8bits
    int men_size=lcd_info.xres * lcd_info.yres * lcd_info.bits_per_pixel / 8;
    //对屏幕进行内存映射,lcd_fp为内存映射首地址
    lcd_fp=mmap(NULL,men_size,PROT_WRITE|PROT_READ,MAP_SHARED,lcd_fd,0);
    //如果对其他进程不可见，则内存映射失败
    if(lcd_fp == MAP_FAILED)
    {
        perror("mmap failed");
        close(lcd_fd);
        exit(0);
    }
}

//显示单个像素点
void draw_pixel(int x,int y,int color)
{
    if(x<lcd_info.xres && x>=0 && y<lcd_info.yres && y>=0)
    *(lcd_fp + y*lcd_info.xres + x)=color;

}

//设置全屏背景颜色
void draw_background(int color)
{
    for(int y=0;y<lcd_info.yres;y++)
    {
        for(int x=0;x<lcd_info.xres;x++)
        {
            draw_pixel(x,y,color);
        }
    }
}


//画图片
void draw_picture(int posx,int posy,const char* pic_path)
{
    //打开图片
    int bmpfd=open(pic_path,O_RDONLY);
    if(bmpfd==-1)
    {
        perror("open bmp fail\n");
        exit(0);
    }
    int width;
    int height;
    lseek(bmpfd,0x12,SEEK_SET);//光标定位到宽度位置
	//读取高度和宽度
    read(bmpfd,&width,4);
	read(bmpfd,&height,4);
	lseek(bmpfd,54,SEEK_SET);
    //定位光标到像素数组位置
	char buf[width*height*3];
	read(bmpfd,buf,width*height*3);
    //读取各个像素点保存在 buf中
	int i,j;
	int color;
	int n=0;
	unsigned char r,g,b;
	for(i=height-1;i>=0;i--)
	{
		for(j=0;j<width;j++)
		{
			b = buf[n++];
			g = buf[n++];
			r = buf[n++];
			color = r << 16 | g << 8 | b;
			draw_pixel(j+posx,i+posy,color);
		}
	}
    close(bmpfd);
}


//主界面背景
void Main_Interface()
{
    lcd_init();
    draw_background(0x000000);
    draw_picture(0,14,"./Main_Interface.bmp");
}

//游戏界面背景
void Interface()
{

    draw_background(0x000000);
    draw_picture(0,0,"./Interface.bmp");
}

//排行榜界面
void leaderboard()
{
    draw_picture(0,0,"./leaderboard.bmp");
}

//结束界面
void gameover()
{
    draw_picture(40,0,"./gameover.bmp");
}