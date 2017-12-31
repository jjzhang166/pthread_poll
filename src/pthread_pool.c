/**********************************************************
* ** 文件名：thread_pool.c
* ** Copyright (C) 2017 xiethon@126.com
*
* ** 创建者：xiethon
* ** 日  期：2017年12月26日23时54分
* ** 描  述：线程池实现函数
* ** 版  本：V1.0
*
* ** 修改人：
* ** 日  期：
* ** 修改描述：
* ** 版本号：
*
****************************************************/
#include<stdio.h>
#include"../inc/pthread_pool.h"
#include"../inc/debug.h"

static void* pthread_fun(void* arg);

/********************************
function：pool_init
desciption:初始化线程池
parament:thread_num,创建线程数量，任务最大数
return: 线程池管理结构体
**********************************/
poolmanager_t* pool_init(int thread_num,int task_max)
{
    //step1: 入口参数检查.
    int i = 0;
    poolmanager_t* poolmanager = (poolmanager_t *)malloc(sizeof(poolmanager_t));
    if(thread_num < 1 || (NULL == poolmanager))
    {
        CONSOLE_MSG("parament err\n");
        return NULL;
    }
    //step2：结构体初始化
    poolmanager->task_max = task_max;
    poolmanager->thread_num = thread_num;
    poolmanager->task_cur_num = 0;
    poolmanager->pool_is_destroy = false;
    if( (poolmanager->task_list_head = (plist)malloc(sizeof(list))) == NULL)
    {
        CONSOLE_MSG("task list head malloc err");
        return NULL;
    }
    INIT_LIST_HEAD(poolmanager->task_list_head);
    if(pthread_mutex_init(&(poolmanager->mutex),NULL))
    {
        CONSOLE_MSG("pthread mutex init error\n");
        return NULL;
    }
    if(pthread_cond_init(&(poolmanager->cond),NULL))
    {
        CONSOLE_MSG("pthread cond init err\n");
        return NULL;
    }
    if( (poolmanager->pthreads=(pthread_t*)malloc(sizeof(pthread_t)*thread_num)) == NULL)
    {
        CONSOLE_MSG("pool->pthreads malloc err");
        return NULL;
    }
    //创建线程
    for(i=0;i<thread_num;i++)
    {
        if(pthread_create(&(poolmanager->pthreads[i]),NULL,pthread_fun,(void*)poolmanager) != 0)
        {
            CONSOLE_MSG("pthread create err\n");
            return NULL;
        }
    }
    DEBUG("pool manager init ok!\n");
    return poolmanager;
}

/********************************
function：pool_destroy
desciption:销毁线程池
parament:poolmanager,线程池管理结构体
return: none
**********************************/
void pool_destroy(poolmanager_t* poolmanager)
{
    int i;
    DEBUG("pool destroy!\n");
    if(poolmanager->pool_is_destroy == true)
    {
    //    poolmanager->pool_is_destroy = false;
    }
    pthread_cond_broadcast(&(poolmanager -> cond));

    //回收线程
    for(i = 0; i < poolmanager->thread_num; i++)
    {
        pthread_join(poolmanager->pthreads[i], NULL);
    }
    free(poolmanager->pthreads);
    //销毁任务链表
     plist pos;
     pooltask_t *task;
     LIST_FOR_EACH(pos, poolmanager->task_list_head)
     {
         task =  CONTAINER_OF(pos,pooltask_t,task_list_node);
         free(task);
         task = NULL;
     }

    //销毁条件变量与互斥量
    pthread_mutex_destroy(&(poolmanager -> mutex));
    pthread_cond_destroy(&(poolmanager -> cond));
    free(poolmanager);
    //释放内存后将指针置空
    poolmanager = NULL;
}

/********************************
function：pthread_fun
desciption:线程执行函数体
parament:arg，函数参数
return: none
**********************************/
static void* pthread_fun(void* arg)
{
    poolmanager_t* poolmanager = (poolmanager_t* )arg;
    pooltask_t *cur_task; //工作任务节点
    plist work_node;       //工作节点
    //线程开始工作
    while(1)
    {
        //step1:从任务链表内取任务之前先上锁
        pthread_mutex_lock(&(poolmanager->mutex));
        //step2：如果任务链表为空，且线程池未关闭，则线程挂起
        if( (poolmanager->pool_is_destroy==false) && (list_is_empty(poolmanager->task_list_head)))
        {
            DEBUG("task list is empty,pthread %u waiting task join\n",(unsigned int)pthread_self());
            pthread_cond_wait(&(poolmanager->cond), &(poolmanager->mutex)); //线程挂起，并解锁线程
        }
        //step3:线程退出之前保证所有的任务链表内的任务都以执行完毕
        if( (poolmanager->pool_is_destroy==true) && (list_is_empty(poolmanager->task_list_head)))
        {
            DEBUG("thread %u work over\n",(unsigned int)pthread_self());
            pthread_mutex_unlock(&(poolmanager->mutex));   //线程退出之前先解锁
            pthread_exit(NULL);
        }
        //step4:其他情况下说明线程可以正常执行任务，则从任务队列的尾部里取任务来执行
        if( !(list_is_empty(poolmanager->task_list_head)) )
        {
            //从链表尾部获取一个结构体
            work_node = poolmanager->task_list_head->next;
            cur_task = CONTAINER_OF(work_node,pooltask_t,task_list_node);
            //从任务链表中删除该节点
            list_del(work_node);
            //step5:执行任务体
            (cur_task->task_fun)(cur_task->arg);
            free(cur_task->task_list_node);
            free(cur_task);
            cur_task = NULL;
            //解锁
            pthread_mutex_unlock(&(poolmanager->mutex));
        }
    }
    return NULL;
}
