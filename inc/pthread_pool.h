/**********************************************************
* ** 文件名：pthread_pool.h
* ** Copyright (C) 2017 xiethon@126.com
*
* ** 创建者：xiethon
* ** 日  期：2017年12月27日18时44分
* ** 描  述：
* ** 版  本：V1.0
*
* ** 修改人：
* ** 日  期：
* ** 修改描述：
* ** 版本号：
*
****************************************************/
#ifndef _PTHREAD_POOL_H_
#define _PTHREAD_POOL_H_

#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<pthread.h>
#include"../inc/list.h"

typedef void (*routine)(void*);

typedef struct _tast_node{
    routine task_fun;  //任务处理函数
    void* arg;                  //处理函数参数
    plist task_list_node;       //内核链表节点
}pooltask_t;

typedef struct _threadpool_manager{
    int thread_num;                 //线程数量
    int task_max;                   //任务最大数量
    int task_cur_num;               //当前列表的任务数量
    bool pool_is_destroy;           //是否销毁线程池
    plist task_list_head;           //任务列表头指针
    pthread_t *pthreads;             //创建的线程数量
    pthread_mutex_t mutex;          //线程互斥锁
    pthread_cond_t cond;            //任务列表为空条件变量
}poolmanager_t;

poolmanager_t* pool_init(int thread_num,int task_max);

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}
#endif

#endif //PTHREAD_POLL_H
