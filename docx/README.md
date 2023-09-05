# gec6818-AircraftWar
## 游戏简介

 ​   本游戏是一个基于GEC6818开发板开发的飞机射击类的小游戏,玩家可以通过触摸屏控制我方飞机移动,发射炮弹击落敌方飞机。游戏有启动界面、游戏运行界面和游戏结束界面。项目链接：https://github.com/shengye2413/gec6818-AircraftWar

## 游戏逻辑



1. 使用`ftok()、shmget()`函数创建一个共享内存并使用`shmat()`函数将共享内存映射到父子进程的地址空间，使进程间可以进行通信。

2. 进入主界面循环。

3. 在循环中根据鼠标点击的位置进行不同的操作：

   - 如果点击了退出按钮，则清除界面并退出程序。
   - 如果点击了排行榜按钮，则显示排行榜，并在返回按钮被点击后返回主界面。
   - 如果点击了开始游戏按钮，则进入游戏循环。

4. 在游戏循环中：

   - 创建两个进程，分别表示敌方和我方。
   - 在敌方进程中循环生成敌机，并使用多线程来控制敌机的移动。
   - 在我方进程中：
     - 显示我方飞机的初始位置。
     - 根据鼠标移动的实时位置更新共享内存中的数据。
     - 根据鼠标点击的位置执行不同的操作：
       - 如果点击了游戏区域，则更新我方飞机的位置并开启一个新的线程来控制子弹的轨迹。
       - 如果点击了返回按钮，则终止敌方进程，并返回主界面。
       - 如果点击了暂停按钮，则显示暂停界面，并暂停敌方进程。在屏幕再次被点击后唤醒敌方进程，并清除暂停界面。
     - 监测敌方进程是否终止，如果终止则显示游戏结束界面，并在返回主界面或重新开始按钮被点击后执行相应操作。
     - 游戏终止后记录击毁敌机数量，写入到文件“1.TXT”中。

5. 关闭文件和释放资源。

   

## 核心文件及功能

- background.c: 负责绘制不同界面背景和图片。
- enemy.c: 负责生成敌机,移动,被击落逻辑。使用多线程实现敌机的并发生成。
- main.c: 游戏主函数,负责创建界面,生成游戏进程。
- own_side.c: 负责我方飞机移动和子弹发射逻辑。
- slide.c/h: 封装了触摸屏输入,提供计算点击位置和实时获取触摸坐标的接口。

## 代码实现要点

1. 使用framebuffer设备提供的内存映射接口更新屏幕显示。

2. 使用多线程实现敌我双方行为的并发执行。

3. 使用信号机制控制游戏进程的暂停、继续。

4. 使用共享内存使敌我双方可以共享数据(如我方位置)。

5. 封装触摸屏输入,避免在不同文件重复编写事件读取代码。

## 主要函数

1. lcd_init()

初始化LCD屏幕,无参数。

- 作用:打开framebuffer设备,获取屏幕信息,进行内存映射,为后续绘图做准备。

2. draw_pixel(int x, int y, int color) 

在指定坐标画点,参数说明:

- x: 横坐标
- y: 纵坐标
- color: 画点的颜色值 

- 作用:根据传入的坐标,在framebuffer绘图内存上画指定颜色的点。

3. direction()

判断触摸动作是否为点击,无参数。

- 作用:通过读取触摸屏事件,判断用户触摸动作是否为点击,并返回点击判断结果。

4. enemy_air(void *args)

控制敌机移动,参数说明:

- args: 线程参数,这里未使用。

- 作用:在随机位置生成敌机,控制敌机移动,重画敌机位置实现动画效果。

5. ball_track(void *args)

绘制我方炮弹的飞行轨迹,参数说明:

- args: 线程参数,这里未使用。

- 作用:根据我方飞机位置绘制炮弹轨迹动画。

6. our_air() 

重画我方飞机位置,无参数。

- 作用:根据最新我方飞机坐标重画飞机位置,实现我方飞机的移动。

好的,继续解释其余一些主要函数:

7. draw_background(int color)

绘制全屏背景,参数说明:
- color: 背景颜色值

- 作用: 用指定颜色值绘制全屏背景。

8. draw_picture(int posx, int posy, const char* pic_path)

绘制图片,参数说明:
- posx: 图片左上角横坐标 
- posy: 图片左上角纵坐标
- pic_path: 图片路径

- 作用: 在指定位置绘制图片文件。

9. Main_Interface()

绘制主界面,无参数。

- 作用: 绘制主界面背景和图片。

10. Interface() 

绘制游戏界面,无参数。

- 作用: 绘制游戏运行时的界面背景。

11. leaderboard()

绘制排行榜界面,无参数。

- 作用: 绘制排行榜界面,待实现排行榜内容。

12. gameover()

绘制游戏结束界面,无参数。

- 作用:绘制游戏结束画面。

13. real_time_location()

实时获取触摸坐标,无参数。

- 作用:循环读取触摸屏事件,并实时更新触摸坐标值。

## 一些变量及宏定义



- shm_px: 共享内存起始地址指针,保存我方飞机x坐标
- shm_py: 共享内存起始地址指针,保存我方飞机y坐标
- shm_p_num: 共享内存起始地址指针,保存摧毁敌机数量
- lcd_fd: framebuffer 设备文件描述符
- IPC_PATH: 共享内存Key的路径
- IPC_CODE: 共享内存Key的代码
