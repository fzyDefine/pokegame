#include "../inc/show_bmp.h"
#include "../inc/lcd.h"

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

int desktop_buffer[25];
/**
    breif:初始化Bmp图片结构体, 路径, 包含起始坐标、长宽高、指针指向等属性。 
*/
void Init_Bmp( BmpStruct *bmp_member , char *bmp_path, int *bmp_point , int width , int heigh) 
{
    bmp_member->pos_x = 0;
    bmp_member->pos_y = 0;
    bmp_member->bmp_width = width;
    bmp_member->bmp_heigh = heigh;

	bzero(bmp_member->bmp_path ,35);
	memcpy(bmp_member->bmp_path, bmp_path, 35);
    // bmp_member->bmp_path = bmp_path; 	//错误的
    bmp_member->bmpbuf = bmp_point;

	printf("初始化图片%s成功\r\n",bmp_member->bmpbuf);
}

/**
	breif:加载图片资源
*/
int Reload_BmpResource(BmpStruct *bmp_member)
{
	int bmpfd;  //图片文件
    int width = bmp_member->bmp_width;
    int heigh = bmp_member->bmp_heigh;
    int pos_x = bmp_member->pos_x;
    int pos_y = bmp_member->pos_y;
	int i;
    char tm[30] = "";

	//定义数组存放你读取到的bmp图片的颜色值
	char bmpbuf[width*heigh*3];

	//定义数组存放转换得到的ARGB数据
	int lcdbuf[width*heigh]; //int类型数据占4个字节

	//打开你要显示的w*h的bmp图片
	bmpfd=open( bmp_member->bmp_path ,O_RDWR);
	if(bmpfd==-1)
	{
        sprintf(tm,"打开bmp失败了,路径为:%s\r\n",bmp_member->bmp_path);
		perror(tm);
		return -1;
	}
	
	printf("加载图片%s成功\r\n",bmp_member->bmp_path);
	// //打开液晶屏的驱动
	// lcdfd=open("/dev/fb0",O_RDWR);
	// if(lcdfd==-1)
	// {
    //     perror("打开lcd失败\n");
	// 	return -1;
	// }

	//跳过图片最前面的54字节的头信息
	lseek(bmpfd,54,SEEK_SET);
	
	//从第55字节开始读取--》我发现bmp宽度占的字节数不能被4整除，会填充垃圾
	//所以我设计了算法，用来跳过垃圾，只读取正确的BGR数据
	if((width*3)%4==0)  //能够被4整除，没有填充垃圾
	{
		read(bmpfd,bmpbuf,width*heigh*3);
	}
	else  //不能够被4整除，会填充垃圾
	{
		//一行行读取，跳过填充的垃圾数
		/*
			第一行数据：读取w*3字节，跳过填充的垃圾数(4-(w*3)%4)
			填充到数组：bmpbuf[0]--bmpbuf[w*3-1]
			
			第二行数据：读取w*3字节，跳过填充的垃圾数(4-(w*3)%4)
			填充到数组：bmpbuf[w*3]--bmpbuf[w*6-1]
		*/
		for(i=0; i<heigh; i++)
		{
			read(bmpfd,&bmpbuf[width*3*i],width*3);   //读取每一行正确的BRG数据
			lseek(bmpfd,4-(width*3)%4,SEEK_CUR);  //跳过每一行后面填充的垃圾数
		}
	}
	
	//把三个字节的BGR数据转换成四个字节ARGB
	for(i=0; i<width*heigh; i++)
		bmp_member->bmpbuf[i]= 0x00<<24 | bmpbuf[3*i+2]<<16 | bmpbuf[3*i+1]<<8  |  bmpbuf[3*i];
	
	//关闭
	close(bmpfd);
	return 0;
    
}

/**
	breif:贴图
*/
int Show_Bmp(BmpStruct *bmp_member , int pos_x , int pos_y , int show_area)
{
    int i = 0 , j= 0;

    int width = bmp_member->bmp_width;
    int heigh = bmp_member->bmp_heigh;

    bmp_member->pos_x = pos_x;
    bmp_member->pos_y = pos_y;

	printf("贴图成功，贴图路径: %s\r\n",bmp_member->bmp_path);

    //在相反的视角写入图片
    //显示图像部分: 把图片每一行像素点拷贝到液晶屏对应的位置(根据pos_x,pos_y的位置)  +(!cur_lcd_area*800*480*4) 
	for(i=0; i<heigh; i++)
		memcpy(lcdpoint + (show_area*800*480) + (pos_y+heigh-1-i)*800+pos_x,& bmp_member->bmpbuf[i*width],width*4);

	
    return 0;
}

/**
	breif:颜色填充矩形
*/
int FillColor_Rectangle( Color32_T _color, int pos_x , int pos_y , int width, int heigh, int show_area)
{	
    int i, j;

    // calculate the bounding box
    int min_x = MAX(0, pos_x);
    int max_x = MIN(800, pos_x + width);
    int min_y = MAX(0, pos_y);
    int max_y = MIN(480, pos_y + heigh);

    for (i = min_y; i < max_y; i++) {
        for (j = min_x; j < max_x; j++) {
            memcpy(lcdpoint + (show_area*800*480*4) + (800*i + j)*4, &_color ,4);
        }
    }
    return 0;
}

/**
	breif:颜色填充圆形
*/
int FillColor_Cycle( Color32_T _color, int center_x,int center_y , int r, int show_area)
{
    int i, j;

    // calculate the bounding box
    int min_x = MAX(0, center_x - r);
    int max_x = MIN(800, center_x + r);
    int min_y = MAX(0, center_y - r);
    int max_y = MIN(480, center_y + r);

    for (i = min_y; i < max_y; i++) {
        for (j = min_x; j < max_x; j++) {
            if ((j - center_x) * (j - center_x) + (i - center_y) * (i - center_y) <= r * r) {
                memcpy(lcdpoint +  (show_area * 800 * 480 * 4) + (800 * i + j) * 4, &_color, 4);
            }
        }
    }

    return 0;

}

//画圆角矩形,cir_r是内切圆的半径
void Fill_RoundedRect( Color32_T _color, int pos_x , int pos_y , int width, int height, int cir_r, int show_area )
{
	if(cir_r >= height/2 )
	{
		cir_r = height/2;
		FillColor_Cycle(_color, pos_x+cir_r        , pos_y+cir_r, cir_r, show_area);
		FillColor_Cycle(_color, pos_x+width - cir_r, pos_y+cir_r, cir_r, show_area);
	}
	else
	{
		FillColor_Cycle(_color, pos_x+cir_r        , pos_y+cir_r, cir_r, show_area);
		FillColor_Cycle(_color, pos_x+width - cir_r, pos_y+cir_r, cir_r, show_area);
		FillColor_Cycle(_color, pos_x+cir_r        , pos_y+height-cir_r, cir_r, show_area);
		FillColor_Cycle(_color, pos_x+width - cir_r, pos_y+height-cir_r, cir_r, show_area);
		
		FillColor_Rectangle(_color, pos_x,                 pos_y + cir_r, cir_r, height - 2*cir_r, show_area);
		FillColor_Rectangle(_color, pos_x + width - cir_r, pos_y + cir_r, cir_r, height - 2*cir_r, show_area);
	}

	FillColor_Rectangle(_color,pos_x + cir_r,pos_y,width - 2*cir_r,height,show_area);
}