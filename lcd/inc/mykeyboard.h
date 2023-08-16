/*
 * @Author: Fu Zhuoyue
 * @Date: 2023-08-03 15:14:05
 * @LastEditors: Fu Zhuoyue
 * @LastEditTime: 2023-08-03 21:58:17
 * @Description: 
 * @FilePath: /share_file/pukegame/lcd/inc/mykeyboard.h
 */
#ifndef __MYKEYBOARD_H__
#define __MYKEYBOARD_H__



void Init_MainKey();
void Show_MainKeyBoard(color key_color, color font_color, int area);
int Call_Mykeyboard( char *keyboard_buffer,int bf_size, char ch, int area );
char Get_TouchAscii();

#endif // !__MYKEYBOARD_H__
