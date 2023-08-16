/*
 * @Author: Fu Zhuoyue
 * @Date: 2023-08-03 10:45:22
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-08-04 19:13:17
 * @Description:
 * @FilePath: /share_file/pukegame/lcd/src/mykeyboard.c
 */

#include "syshead.h"
#include "font.h"
#include "lcd.h"
#include "show_bmp.h"
#include "mykeyboard.h"
#include "stack.h"
#include "login.h"

#define KEY_COLOR 0X00F9D2E4
#define FONT_COLOR 0X0001847F

char alpha_sequence[26] = {
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
    'z', 'x', 'c', 'v', 'b', 'n', 'm'};

#define DEL 127
#define CAP 17
#define ACK 6
#define ESC 27

typedef struct KeyStrct
{
    int circ_x;
    int circ_y;
    int circ_r;
    int pos_x;
    int pos_y;
    int width;
    int height;
    char val[4];
} KeyStrct;

KeyStrct alpha_key[26]; // 字母
KeyStrct num_key[10];   // 数字
KeyStrct other_key[2];
KeyStrct inputbar_key[2];

void Init_MainKey()
{
    int circle_r = 30;
    int start_x = (800 - ((10 - 1) * 2 * circle_r + 9 * circle_r / 3)) / 2;
    int start_y = 480 - ((3 - 1) * 2 * circle_r + 2 * (circle_r / 3)) - 40;
    int clearance = circle_r / 3;

    char temp_val[4];

    for (int i = 0; i < 10; i++) // 第一行
    {
        bzero(temp_val, sizeof(temp_val));
        sprintf(temp_val, "%c", alpha_sequence[i] - ('a' - 'A'));
        memcpy(alpha_key[i].val, temp_val, sizeof(temp_val));

        alpha_key[i].circ_x = start_x + i * (2 * circle_r + clearance);
        alpha_key[i].circ_y = start_y;
        alpha_key[i].circ_r = circle_r;
        alpha_key[i].pos_x = alpha_key[i].circ_x - circle_r;
        alpha_key[i].pos_y = alpha_key[i].circ_y - circle_r;
        alpha_key[i].width = 2 * circle_r;
        alpha_key[i].height = 2 * circle_r;

        num_key[i].circ_x = alpha_key[i].circ_x;
        num_key[i].circ_y = alpha_key[i].circ_y - (2 * circle_r + clearance);
        num_key[i].circ_r = circle_r;
        num_key[i].pos_x = num_key[i].circ_x - circle_r;
        num_key[i].pos_y = num_key[i].circ_y - circle_r;
        num_key[i].width = 2 * circle_r;
        num_key[i].height = 2 * circle_r;

        bzero(temp_val, sizeof(temp_val));
        sprintf(temp_val, "%d", i);
        memcpy(num_key[i].val, temp_val, sizeof(temp_val));
    }

    start_x += circle_r;
    start_y += 2 * circle_r + clearance;

    for (int i = 0; i < 9; i++)
    {
        bzero(temp_val, sizeof(temp_val));
        sprintf(temp_val, "%c", alpha_sequence[i + 10] - ('a' - 'A'));
        memcpy(alpha_key[i + 10].val, temp_val, sizeof(temp_val));

        alpha_key[i + 10].circ_x = start_x + i * (2 * circle_r + clearance);
        alpha_key[i + 10].circ_y = start_y;
        alpha_key[i + 10].circ_r = circle_r;
        alpha_key[i + 10].pos_x = alpha_key[i + 10].circ_x - circle_r;
        alpha_key[i + 10].pos_y = alpha_key[i + 10].circ_y - circle_r;
        alpha_key[i + 10].width = 2 * circle_r;
        alpha_key[i + 10].height = 2 * circle_r;
    }

    start_x += 2 * circle_r + clearance;
    start_y += 2 * circle_r + clearance;

    for (int i = 0; i < 7; i++)
    {
        bzero(temp_val, sizeof(temp_val));
        sprintf(temp_val, "%c", alpha_sequence[i + 19] - ('a' - 'A'));
        memcpy(alpha_key[i + 19].val, temp_val, sizeof(temp_val));

        alpha_key[i + 19].circ_x = start_x + i * (2 * circle_r + clearance);
        alpha_key[i + 19].circ_y = start_y;
        alpha_key[i + 19].circ_r = circle_r;
        alpha_key[i + 19].pos_x = alpha_key[i + 19].circ_x - circle_r;
        alpha_key[i + 19].pos_y = alpha_key[i + 19].circ_y - circle_r;
        alpha_key[i + 19].width = 2 * circle_r;
        alpha_key[i + 19].height = 2 * circle_r;
    }

    other_key[0].pos_x = num_key[0].pos_x;
    other_key[0].pos_y = alpha_key[25].pos_y;
    other_key[0].width = 3 * circle_r;
    other_key[0].height = 2 * circle_r;
    strcat(other_key[0].val, "CAP");

    other_key[1].pos_x = alpha_key[18].pos_x;
    other_key[1].pos_y = alpha_key[25].pos_y;
    other_key[1].width = 3 * circle_r;
    other_key[1].height = 2 * circle_r;
    strcat(other_key[1].val, "ESC");

    inputbar_key[0].pos_x = num_key[8].pos_x;
    inputbar_key[0].pos_y = num_key[8].pos_y - 40;
    inputbar_key[0].width = 60;
    inputbar_key[0].height = 30;
    strcat(inputbar_key[0].val, "删");

    inputbar_key[1].pos_x = num_key[9].pos_x;
    inputbar_key[1].pos_y = num_key[9].pos_y - 40;
    inputbar_key[1].width = 60;
    inputbar_key[1].height = 30;
    strcat(inputbar_key[1].val, "确");
}

void Show_InputBar(color key_color, color font_color, int area)
{
    int bar_width = 550;
    int bar_height = num_key[0].circ_r;
    Fill_RoundedRect(C_WHITE, num_key[0].pos_x, num_key[0].pos_y - 10 - bar_height, bar_width, bar_height, 50, area);

    Fill_RoundedRect(key_color, inputbar_key[0].pos_x, inputbar_key[0].pos_y, inputbar_key[0].width, inputbar_key[0].height, 50, area);
    Fill_RoundedRect(key_color, inputbar_key[1].pos_x, inputbar_key[1].pos_y, inputbar_key[1].width, inputbar_key[1].height, 50, area);

    ShowFont_WithoutBackground(font_point, lcdpoint, "删除", inputbar_key[0].pos_x + 15, inputbar_key[0].pos_y + 7, font_color, 20, area);
    ShowFont_WithoutBackground(font_point, lcdpoint, "确认", inputbar_key[1].pos_x + 15, inputbar_key[1].pos_y + 7, font_color, 20, area);
}

// 主键盘
void Show_MainKeyBoard(color key_color, color font_color, int area)
{
    Fill_RoundedRect(C_PINK, num_key[0].pos_x - 10, num_key[0].pos_y - 30 - num_key[0].circ_r, 710, 340, 30, area);

    for (int i = 0; i < 26; i++)
    {
        FillColor_Cycle(key_color, alpha_key[i].circ_x, alpha_key[i].circ_y, alpha_key[i].circ_r, area);
        int pos_x = alpha_key[i].circ_x - alpha_key[i].circ_r / 5;
        int pos_y = alpha_key[i].circ_y - alpha_key[i].circ_r / 2;
        int font_r = alpha_key[i].circ_r;
        ShowFont_WithoutBackground(font_point, lcdpoint, alpha_key[i].val, pos_x, pos_y, font_color, font_r, area);
    }
    for (int i = 0; i < 10; i++)
    {
        FillColor_Cycle(key_color, num_key[i].circ_x, num_key[i].circ_y, num_key[i].circ_r, area);
        int pos_x = num_key[i].circ_x - num_key[i].circ_r / 5;
        int pos_y = num_key[i].circ_y - num_key[i].circ_r / 2;
        int font_r = num_key[i].circ_r;
        ShowFont_WithoutBackground(font_point, lcdpoint, num_key[i].val, pos_x, pos_y, font_color, font_r, area);
    }

    for (int i = 0; i < 2; i++)
    {
        Fill_RoundedRect(key_color, other_key[i].pos_x, other_key[i].pos_y, other_key[i].width, other_key[i].height, 30, area);
    }
    int font_size = other_key[0].height / 2;
    ShowFont_WithoutBackground(font_point, lcdpoint, "CAP", other_key[0].pos_x + 25, other_key[0].pos_y + 15, font_color, font_size, area);
    ShowFont_WithoutBackground(font_point, lcdpoint, "ESC", other_key[1].pos_x + 25, other_key[1].pos_y + 15, font_color, font_size, area);

    Show_InputBar(key_color, font_color, area);
}

char Get_TouchAscii()
{
    for (int i = 0; i < 26; i++)
    {
        if (IsTouch_AmongAnypx(alpha_key[i].pos_x, alpha_key[i].pos_y, alpha_key[i].width, alpha_key[i].height))
        {
            printf("点击键值是%s\n", alpha_key[i].val);
            return alpha_key[i].val[0];
        }
    }
    for (int i = 0; i < 10; i++)
    {
        if (IsTouch_AmongAnypx(num_key[i].pos_x, num_key[i].pos_y, num_key[i].width, num_key[i].height))
        {
            printf("点击键值是%s\n", num_key[i].val);
            return num_key[i].val[0];
        }
    }
    for (int i = 0; i < 2; i++)
    {
        if (IsTouch_AmongAnypx(other_key[i].pos_x, other_key[i].pos_y, other_key[i].width, other_key[i].height))
        {
            printf("点击键值是%s\n", other_key[i].val);

            if (strstr(other_key[i].val, "CAP") != NULL)
            {
                return CAP;
            }
            else if (strstr(other_key[i].val, "ESC") != NULL)
            {
                return ESC;
            }
        }
    }
    for (int i = 0; i < 2; i++)
    {
        if (IsTouch_AmongAnypx(inputbar_key[i].pos_x, inputbar_key[i].pos_y, inputbar_key[i].width, inputbar_key[i].height))
        {
            printf("点击键值是%s\n", inputbar_key[i].val);

            if (strstr(inputbar_key[i].val, "删") != NULL)
            {
                return DEL;
            }
            else if (strstr(inputbar_key[i].val, "确") != NULL)
            {
                return ACK;
            }
        }
    }

    return 0;
}

char CaseToggle(char ch)
{
    if (isalpha(ch))
    { // 检查字符是否是字母
        if (isupper(ch))
        {                       // 如果字符是大写字母
            return tolower(ch); // 转换为小写并返回
        }
        else
        {                       // 否则，字符是小写字母
            return toupper(ch); // 转换为大写并返回
        }
    }
    // 如果字符不是字母，直接返回原字符
    return ch;
}

// 召唤键盘
int Call_Mykeyboard( char *keyboard_buffer,int bf_size, char ch, int area )
{
    static SquenceStack *head;          
    static bool is_loweralpha = true;
    static bool is_new_keyboard = true; //键盘还需要保留上一轮计数状态
                                        //目的是为了防止重复初始化栈
    if( is_new_keyboard )
    {
        head = Init_Stack(100);     //初始化头结点
        is_new_keyboard = false;    
    }
    
    Show_MainKeyBoard(KEY_COLOR, FONT_COLOR, area); //显示键盘

    if (ch == CAP) // 按了CAP键，大小写转换
    {
        is_loweralpha = !is_loweralpha;
        printf("大小写切换\n");
    }
    else if (ch == ESC)
    {
        free(head->data_p);
        free(head); //释放栈
        is_new_keyboard = true;     //本轮键盘使用结束,下一次就是要打开新键盘了
        is_loweralpha = true;
        return -1;
    }
    else if (ch == DEL)
    {
        char data;
        pop(head, &data);
    }
    else if (ch == ACK)     //点击了确定则将输入数据传递给外部
    {
        strncpy(keyboard_buffer,head->data_p,bf_size);

        free(head->data_p);
        free(head); //释放栈
        is_new_keyboard = true;     //本轮键盘使用结束,下一次就是要打开新键盘了
        is_loweralpha = true;
        return 1;
    }
    else if(ch != 0)
    {
        if (is_loweralpha)
        {
            push(head, ch);
        }
        else
        {
            push(head, CaseToggle(ch));
        }
    }

    int x = num_key[0].pos_x + 5;
    int y = num_key[0].pos_y - 40 + 5;
    int size = 20;

    for (int i = 0; i <= head->top; i++) // 遍历栈
    {
        char txt[2] = "";
        txt[0] = head->data_p[i];
        ShowFont_WithoutBackground(font_point, lcdpoint, txt, x + i * (size / 2), y, C_BLACK, size, area);
    }

    return 0;
}
