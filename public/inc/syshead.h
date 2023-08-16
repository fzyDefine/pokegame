/*
 * @Author: Fu Zhuoyue
 * @Date: 2023-07-10 18:58:49
 * @LastEditors: Fu Zhuoyue
 * @LastEditTime: 2023-07-26 10:25:04
 * @Description: 
 * @FilePath: /share_file/pukegame/public/inc/syshead.h
 */
#ifndef _SYSHEAD_H
#define _SYSHEAD_H

#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <dirent.h>
#include <linux/fb.h>
#include <errno.h>        //跟perror有关的头文件
#include <sys/mman.h>     //跟内存映射有关的头文件
#include <linux/input.h>  //跟输入子系统模型有关的头文件

#include <pthread.h>    //线程
#include <semaphore.h>  //POSIX信号量

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <sys/wait.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>    // 消息队列
#include <sys/shm.h>    // 共享内存 
#include <sys/sem.h>    // 信号量

#define SLEEP_MS(ms) usleep(1000*ms)

#endif