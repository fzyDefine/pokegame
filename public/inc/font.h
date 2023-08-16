/*
 * @Author: Fu Zhuoyue
 * @Date: 2023-07-19 21:22:07
 * @LastEditors: Fu Zhuoyue
 * @LastEditTime: 2023-08-03 20:02:30
 * @Description: 
 * @FilePath: /share_file/pukegame/public/inc/font.h
 */
#ifndef __font_h__
#define __font_h__

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>


#define color unsigned int
#define getColor(a, b, c, d) (a<<24|b<<16|c<<8|d<<0)	


typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef char s8;
typedef short s16;
typedef int s32;
typedef long long s64;

typedef struct stbtt_fontinfo
{
   void           * userdata;
   unsigned char  * data;              // pointer to .ttf file
   int              fontstart;         // offset of start of font

   int numGlyphs;                     // number of glyphs, needed for range checking

   int loca,head,glyf,hhea,hmtx,kern; // table locations as offset from start of .ttf
   int index_map;                     // a cmap mapping for our chosen character encoding
   int indexToLocFormat;              // format needed to map from glyph index to glyph
} stbtt_fontinfo;


typedef struct{
	u32 height;
	u32 width;
	u32 byteperpixel;
	u8 *map;
}bitmap;

typedef struct{
	stbtt_fontinfo *info;
	u8 *buffer;
	float scale;
}font;

//字库指针
extern font *font_point;

//1.初始化字库 
font *fontLoad(char *fontPath);

//2.设置字体的大小 
void fontSetSize(font *f, s32 pixels);

//3.设置字体输出框的大小
bitmap *createBitmap(u32 width, u32 height, u32 byteperpixel);

//可以指定输出框的颜色
bitmap *createBitmapWithInit(u32 width, u32 height, u32 byteperpixel, color c);

//4.把字体输出到输出框中
void fontPrint(font *f, bitmap *screen, s32 x, s32 y, char *text, color c, s32 maxWidth);


//只使用下面两个就行了<*/ fzy 2023.7.16 */>

// 关闭字体库
void fontUnload(font *f);

//在任意地方显示字体(透明背景框) show_area默认填0
void ShowFont_WithoutBackground(font *font_point,char *lcdpoint,char *text,int pos_x,int pos_y,int font_color_argb,int size,int show_area);


#endif
