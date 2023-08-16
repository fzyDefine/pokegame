/*
 * @Author: Fu Zhuoyue
 * @Date: 2023-07-20 19:11:03
 * @LastEditors: Fu Zhuoyue
 * @LastEditTime: 2023-07-27 09:16:12
 * @Description: 
 * @FilePath: /pukegame/draw/inc/show_mypng.h
 */
#ifndef __SHOW_MYPNG_H_
#define __SHOW_MYPNG_H_

#include "syshead.h"
#include "png_show.h" //png库头文件

//图片结构体

int Init_Png(struct png_info_data *png_info , char *png_path );
void show_png_pos( int x0, int y0, int SW, int SH,struct png_info_data * pinfo,int show_area);

#endif