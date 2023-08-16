/*
 * @Author: Fu Zhuoyue
 * @Date: 2023-07-20 19:12:13
 * @LastEditors: Fu Zhuoyue
 * @LastEditTime: 2023-07-20 19:36:10
 * @Description: 
 * @FilePath: /share_file/syscoding_project/draw/src/show_mypng.c
 */
#include "../inc/show_mypng.h" //png库头文件
#include "../inc/lcd.h"

int Init_Png(struct png_info_data *png_info , char *png_path )
{

	if (png_save_info( png_path, png_info ) == -1 )//为图片结构体里的数据结构体分配数据
	{	
		puts( "png_path error\n" );
		printf("your input path is %s\n",png_path);
		return -1;
	}
	printf("初始化%s成功\r\n",png_path);
    return 0;
}

/**
	功能   ：显示png图片像素信息 到 gec6818开发板LCD
	参数1,2：显示的起点位置
	参数3,4：图片缩放（放大）后的实际显示大小
	参数5  ：保存图片解压后的像素信息结构体地址
	参数6  ：LCD映射首地址
	返回值： void
**/
void show_png_pos( int x0, int y0, int SW, int SH, struct png_info_data *pinfo,int show_area )
{	
	int sj,si;

	//printf("%s-%d\r\n",__FILE__,__LINE__);

	for( int j=0; j<480-y0 && j<SH; j++)
	{
		sj = j*pinfo->height / SH;
		
		for( int i=0; i<800-x0 && i<SW; i++)
		{
			si = i*pinfo->width / SW;
			
			if( PNG_COLOR_TYPE_RGB_ALPHA == pinfo->color_type)
			{
				if( pinfo->pdata[((sj*pinfo->width)+si)*4+3] > 0 ){
					memcpy(lcdpoint + (show_area*800*480 *4) + ((y0+j)*800 + i+x0)*4, pinfo->pdata+ ((sj*pinfo->width)+si)*4, 3); //+ (show_area*800*480) 
				}				
			}else if( PNG_COLOR_TYPE_RGB == pinfo->color_type )
			{
				memcpy(lcdpoint + (show_area*800*480 *4) + ((y0+j)*800 + i+x0)*4,     pinfo->pdata+ ((sj*pinfo->width)+si)*3    , 3); //+ (!cur_lcd_area*800*480)
			}
			
		}
	}
}

