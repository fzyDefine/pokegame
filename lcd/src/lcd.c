#include "lcd.h"
#include <math.h>

unsigned char cur_lcd_area = 0; //当前屏幕区域 0是A区  1是B区

int touch_pos_x = 0;
int touch_pos_y = 0;

//屏幕文件
int lcdfd;  
int tp;

int touch_width;
int touch_height;

int lcd_width;
int lcd_height;

//获取lcd属性、可变参数
struct fb_var_screeninfo vinfo;   // 可变参数属性
struct fb_fix_screeninfo fixinfo; // 固定属性

//屏幕映射指针
unsigned char *lcdpoint;

int Get_TouchArea();

int Lcd_Init(Color32_T _color)
{
    //打开液晶屏的驱动
	lcdfd=open("/dev/fb0",O_RDWR);
	if(lcdfd==-1)
	{
		perror("打开lcd失败了!\n");
		return -1;
	}

    showlcd_info();

    //打开触摸屏的驱动
    tp = open("/dev/input/event0", O_RDWR);
    if(tp == -1)
    {
        printf("打开触摸失败\r\n");
        return -1;
    }
    else
    {
        printf("打开触摸屏成功\r\n");
        Get_TouchArea();    //获取触摸屏信息
    }

    //映射得到lcd在内存中的首地址   (申请一个两倍的内存)
	lcdpoint=mmap(NULL,2*   800*480*4,PROT_READ|PROT_WRITE,MAP_SHARED,lcdfd,0);
	if(lcdpoint==NULL)
	{
		perror("获取lcd的首地址失败了!\n");
		return -1;
	}

    cur_lcd_area = 0;
    vinfo.xoffset = 0;
    vinfo.yoffset = 0 + 480*(cur_lcd_area);
    ioctl(lcdfd, FBIOPAN_DISPLAY, &vinfo);

    memset(lcdpoint,0,800*480*4 *2);
    FillColor_Rectangle(_color,0,0,800,480,0);
    FillColor_Rectangle(_color,0,0,800,480,1);
    return 0;
}

void get_fixinfo()
{
    if(ioctl(lcdfd, FBIOGET_FSCREENINFO, &fixinfo) != 0)
    {
        perror("获取LCD设备固定属性信息失败");
        return;
    }
}

void get_varinfo()
{
    if(ioctl(lcdfd, FBIOGET_VSCREENINFO, &vinfo) != 0)
    {
        perror("获取LCD设备可变属性信息失败");
        return;
    }
}

void showlcd_info()
{
    // 获取LCD设备硬件fix属性
    get_fixinfo();
    printf("\n获取LCD设备固定属性信息成功：\n");
    printf("[ID]: %s\n", fixinfo.id);
    printf("[FB类型]: ");
    switch(fixinfo.type)
    {
    case FB_TYPE_PACKED_PIXELS:      printf("组合像素\n");break;
    case FB_TYPE_PLANES:             printf("非交错图层\n");break;
    case FB_TYPE_INTERLEAVED_PLANES: printf("交错图层\n");break;
    case FB_TYPE_TEXT:               printf("文本或属性\n");break;
    case FB_TYPE_VGA_PLANES:         printf("EGA/VGA图层\n");break;
    }
    printf("[FB视觉]: ");
    switch(fixinfo.visual)
    {
    case FB_VISUAL_MONO01:             printf("灰度. 1=黑;0=白\n");break;
    case FB_VISUAL_MONO10:             printf("灰度. 0=黑;1=白\n");break;
    case FB_VISUAL_TRUECOLOR:          printf("真彩色\n");break;
    case FB_VISUAL_PSEUDOCOLOR:        printf("伪彩色\n");break;
    case FB_VISUAL_DIRECTCOLOR:        printf("直接彩色\n");break;
    case FB_VISUAL_STATIC_PSEUDOCOLOR: printf("只读伪彩色\n");break;
    }
    printf("[行宽]: %d 字节\n", fixinfo.line_length);

    // 获取LCD设备硬件var属性
    get_varinfo();
    printf("\n获取LCD设备可变属性信息成功：\n");
    printf("[可见区分辨率]: %d×%d\n", vinfo.xres, vinfo.yres);
    printf("[虚拟区分辨率]: %d×%d\n", vinfo.xres_virtual, vinfo.yres_virtual);
    printf("[从虚拟区到可见区偏移量]: (%d,%d)\n", vinfo.xoffset, vinfo.yoffset);
    printf("[色深]: %d bits\n", vinfo.bits_per_pixel);
    printf("[像素内颜色结构]:\n");
    printf("  [红] 偏移量:%d, 长度:%d bits\n", vinfo.red.offset, vinfo.red.length);
    printf("  [绿] 偏移量:%d, 长度:%d bits\n", vinfo.green.offset, vinfo.green.length);
    printf("  [蓝] 偏移量:%d, 长度:%d bits\n", vinfo.blue.offset, vinfo.blue.length);
    printf("  [透明度] 偏移量:%d, 长度:%d bits\n", vinfo.transp.offset, vinfo.transp.length);
    printf("\n");

    lcd_width = vinfo.xres; //屏幕实际宽度
    lcd_height = vinfo.yres;//屏幕实际高度
}

int Get_TouchArea()
{
    struct input_absinfo absinfo;
    if(ioctl(tp, EVIOCGABS(ABS_X), &absinfo) == -1) {
        perror("Cannot get ABS_X information");
        return 1;
    }

    printf("ABS_X range: min=%d, max=%d\n", absinfo.minimum, absinfo.maximum);
    touch_width =  absinfo.maximum;  //

    if(ioctl(tp, EVIOCGABS(ABS_Y), &absinfo) == -1) {
        perror("Cannot get ABS_Y information");
        return 1;
    }
    printf("ABS_Y range: min=%d, max=%d\n", absinfo.minimum, absinfo.maximum);
    touch_height =  absinfo.maximum;
}


//锁定当前屏幕视角
void Lock_CurView()
{
    //根据当前视角确定offset偏移
    vinfo.xoffset = 0;
    vinfo.yoffset = 0 + 480*(cur_lcd_area);
    ioctl(lcdfd, FBIOPAN_DISPLAY, &vinfo);

}

//转移当前屏幕视角
void Change_CurView()
{
    cur_lcd_area = !cur_lcd_area;

    vinfo.xoffset = 0;
    vinfo.yoffset = 0 + 480*(cur_lcd_area);
    ioctl(lcdfd, FBIOPAN_DISPLAY, &vinfo);
}

// char *Storage_CurWindowBuffer(int area)
// {
//     char *storage = malloc(800*480*4);
//     bzero(storage,800*480*4);
//     memcpy(storage,lcdpoint + area*800*480*4, 800*480*4);

//     return storage;
// }


//清除触摸信息
static void Clean_TouchMessage()
{
    touch_pos_x = -1;
    touch_pos_y = -1;
}

//是否点击了某个区域
bool IsTouch_AmongAnypx( int position_leftTop_x , int position_leftTop_y ,int px_width, int px_height )
{
    if(     (touch_pos_x> position_leftTop_x && touch_pos_x< position_leftTop_x + px_width)
            && (touch_pos_y > position_leftTop_y && touch_pos_y <  position_leftTop_y + px_height)   )
    {
            printf("成功点击指定区域:(%d,%d)(%d,%d)\r\n"
                ,position_leftTop_x,position_leftTop_y,position_leftTop_x+px_width,position_leftTop_y+px_height);
            printf("%d,%s\r\n",__LINE__,__FILE__);
            Clean_TouchMessage();       //如果成功响应触摸清空坐标信息
            return true;
    }
    else
        return false;
}

// //是否点击了某个png图片
// bool IsTouch_AtPngPic(PngStruct *png_member)
// {
//     int px_width  = png_member->width;
//     int px_height = png_member->height;

//     return IsTouch_AmongAnypx( png_member->pos_x, png_member->pos_y , px_width , px_height );

// }

//获取触摸值
int Get_TouchValue()
{
    int temp_x = 0;
    int temp_y = 0;
    int first_x = 0;
    int first_y = 0;
    int last_x = 0;
    int last_y = 0;

    int last_pos_type = 10;
    int now_pos_type = 10; 
    struct input_event buf;

    int is_first_press = 0;

    while(1)
    {
        // printf("进入触摸读取\r\n");
        bzero(&buf, sizeof(buf));
        read(tp, &buf, sizeof(buf));

        if(  buf.type == EV_ABS )
        {
            last_pos_type = now_pos_type;
            now_pos_type = buf.code;    

            if(buf.code == ABS_X)
            {
                temp_x = buf.value *lcd_width/touch_width;        
            }

            //
            if( last_pos_type ==  ABS_X && now_pos_type == ABS_Y )    //正确获取到x,y的坐标
            {
                //触摸校准
                temp_y = buf.value *lcd_height/touch_height;   

                touch_pos_x = temp_x;
                touch_pos_y = temp_y;

                is_first_press++;  //if is_first_press == 1,第一次有效读取

                if( is_first_press == 1 )
                {
                    first_x = temp_x;
                    first_y = temp_y;
                }
            }
        }

		if (buf.type == EV_KEY)         //按键事件
        {
			if (buf.code == BTN_TOUCH)  //手指按压事件
            {
				if (buf.value == KEY_RESERVED)  //松手事件
                {
                    is_first_press = 0;
                    last_x = touch_pos_x;
                    last_y = touch_pos_y;
                    printf("last_x = %d, last_y = %d \r\n",touch_pos_x,touch_pos_y);  //抬手的时候输出一下坐标

                    break;
				}
			}
		}

    }
    //判断滑动的阈值是80
    if(abs(last_x-first_x)>80 || abs(last_y -first_y)>80 )
    {
        if(abs(last_x - first_x) > abs(last_y - first_y) )  //左右
        {
            if(last_x > first_x) //右滑
            {
                printf("右滑\n");
                return RIGHT;
            }
            else        //左滑
            {
                printf("zuo滑\n");
                return LEFT;
            }

        }
        else        //上下
        {
            if(last_y > first_y) //下滑
            {
                printf("xia滑\n");
                return DOWN;
            }
            else        //上滑
            {
                printf("shang滑\n");
                return UP;
            }        
        }
    }

}

