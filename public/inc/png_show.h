#ifndef __PNG_SHOW_H__
#define __PNG_SHOW_H__

#include 	<stdlib.h>
#include 	<string.h>
#include 	<stdio.h>

#include  	<stdint.h>

#include     	<unistd.h>
#include     	<fcntl.h>
#include     	<sys/types.h>
#include     	<sys/stat.h>

#include     	<linux/fb.h>
#include     	<sys/ioctl.h>
#include     	<sys/mman.h>

#include 	"png.h"        //解码库文件
#include 	"pngstruct.h"  //解码库文件
#include 	"pnginfo.h"   //解码库文件



/**
	功能：把解码的PNG图片像素保存到此结构体中。
	color_type	： 图片的类型  
				PNG_COLOR_TYPE_RGB_ALPHA --> 带透明度（一个像素点4字节） -->BGRA
				PNG_COLOR_TYPE_RGB       --> 无透明度（一个像素点3字节） -->BGR
	width, height 	： 图片的宽和高
	pdata 		： 像素信息的保存位置（calloc申请的空间，需要后面自己手动释放）
**/
struct png_info_data{
	
	png_byte color_type;
	int width, height;
	unsigned char * pdata;
	
};



/**
	功能：显示png图片到 gec6818开发板LCD
	参数1：pathname 图片的 路径名
	参数2,3：显示的起点坐标
	参数4：为LCD 屏映射的首地址
	返回值： 0成功  -1失败
**/
int png_display (char *pathname, int x0, int y0, unsigned char * fbp);


/**
	功能：保存图片的像素信息到结构体中
	参数1：pathname 图片的 路径名
	参数2：信息结构体的地址
	返回值： 0成功  -1失败
**/
int png_save_info( char *pathname, struct png_info_data * pinfo );

#endif