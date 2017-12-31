/*********************************************************
 * ** 文件名:调试宏debug.h
 * ** Copyright (c) 2017 xiethon@126.com
 *
 * ** 创建人: xiethon
 * ** 日  期: 2017-12-13
 * ** 描  述: 打印相关函数头文件
 * ** 版  本: V1.0
 *
 * ** 修改人:
 * ** 日  期:
 * ** 修改描述:
 * ** 版  本:
 *
 * ****************************************************/
#ifndef _DEBUG_H_
#define _DEBUG_H_

#include<stdio.h>

#define __DEBUG__ 1				//调试宏开关
#define __CONSOLE_MSG__ 1		//信息打印开关

/*	控制台字体颜色效果*/
#define CONSOLE_MSG_COLOR		STDIO_FONT_GREEN
#define CONSOLE_DEBUG_COLOR		STDIO_FONT_RED
#define CONSOLE_INPUT_COLOR		STDIO_FONT_WHITE


#define STDIO_FONT_BLACK	"\033[30m"
#define STDIO_FONT_RED		"\033[31m"
#define STDIO_FONT_GREEN	"\033[32m"
#define STDIO_FONT_YELLOW	"\033[33m"
#define STDIO_FONT_BLUE		"\033[34m"
#define STDIO_FONT_PURPLE	"\033[35m"
#define STDIO_FONT_CYAN		"\033[30m"
#define STDIO_FONT_WHITE	"\033[37m"
#define STDIO_FONT_FOCUS	"\033[30m"
#define STDIO_FONT_HIGHLIGHT		"\033[1m"
#define STDIO_FONT_BOTTOMLINE		"\033[4m"
#define STDIO_FONT_IVERT_COLOR		"\033[7m"
#define STDIO_FONT_FLASH			"\033[5m"
#define STDIO_FONT_MODE_OFF			"\033[0m"




#if __DEBUG__
	#define DEBUG(...) do{printf(__VA_ARGS__);}while(0)
#else
	#define DEBUG(format,...) do{ ;}while(0)
#endif


#if __CONSOLE_MSG__
	#define CONSOLE_MSG(...) do{printf("%s",CONSOLE_MSG_COLOR); printf(__VA_ARGS__);printf("%s",CONSOLE_INPUT_COLOR);}while(0)
#else
	#define CONSOLE_MSG(...) do{ ;}while(0)
#endif



#endif
