/*
 * @Author: Fu Zhuoyue
 * @Date: 2023-08-03 15:23:52
 * @LastEditors: Fu Zhuoyue
 * @LastEditTime: 2023-08-03 20:03:48
 * @Description: ;
 * @FilePath: /share_file/pukegame/draw/inc/show_bmp.h
 */
#ifndef __SHOW_BMP_H_
#define __SHOW_BMP_H_

#include "syshead.h"

#define C_BLACK     0x00000000
#define C_RED       0x00FF0000
#define C_GREEN     0x0000FF00
#define C_BLUE      0x000000FF
#define C_WHITE     0x00FFFFFF
#define C_PINK      0x00FFC0CB
#define C_VIOLET    0x00EE82EE  //紫色
#define C_GOLD      0X00FFD700

//颜色
typedef int Color32_T;

//图片结构体
typedef struct BmpStruct{

    int pos_x;      //图片显示起始坐标x
    int pos_y;      //图片显示起始坐标y
    int bmp_width;   //图片宽
    int bmp_heigh;   //图片高

    char bmp_path[35];     //图片路径
    int *bmpbuf;        //指向自己图片像素的指针

}BmpStruct; 

void Init_Bmp( BmpStruct *bmp_member , char *bmp_path, int *bmp_point , int width , int heigh);
int Reload_BmpResource(BmpStruct *bmp_member);
int Show_Bmp(BmpStruct *bmp_member , int pos_x , int pos_y,int show_area);
int FillColor_Rectangle( Color32_T _color, int pos_x , int pos_y , int width, int heigh, int show_area);
int FillColor_Cycle( Color32_T _color, int center_x,int center_y , int r, int show_area);
void Fill_RoundedRect( Color32_T _color, int pos_x , int pos_y , int width, int height, int cir_r, int show_area );


#endif //__SHOW_BMP_H_