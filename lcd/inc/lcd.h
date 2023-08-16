/*
 * @Author: Fu Zhuoyue
 * @Date: 2023-07-25 12:22:27
 * @LastEditors: Fu Zhuoyue
 * @LastEditTime: 2023-08-03 20:09:10
 * @Description: 
 * @FilePath: /share_file/pukegame/lcd/inc/lcd.h
 */
#ifndef _LCD_H
#define _LCD_H

#include "show_bmp.h"
typedef enum TOWARD{
    UP,
    DOWN,
    LEFT,
    RIGHT
}TOWARD;

extern unsigned char cur_lcd_area; //当前屏幕区域 0是A区  1是B区
extern int lcdfd;  

//获取lcd属性、可变参数
extern struct fb_var_screeninfo vinfo; 

//屏幕映射指针
extern unsigned char *lcdpoint; 



extern int touch_pos_x;
extern int touch_pos_y;

void Lock_CurView();
void Change_CurView();
int  Lcd_Init(Color32_T _color);

int Get_TouchValue();
bool IsTouch_AmongAnypx( int position_leftTop_x , int position_leftTop_y ,int px_width, int px_height );
void showlcd_info();

#endif