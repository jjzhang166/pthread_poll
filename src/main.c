/**********************************************************
* ** 文件名：main.c
* ** Copyright (C) 2017 xiethon@126.com
*
* ** 创建者：xiethon
* ** 日  期：2017年12月27日18时39分
* ** 描  述：线程池测试代码
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

void task_fun(void* arg);
void task_create(poolmanager_t* poolmanager,void* arg,routine task_fun);
void pool_destroy(poolmanager_t* poolmanager);

int main(int argc,char *argv[])
{
    /*****step1:初始化线程池管理结构体*******/
    poolmanager_t* poolmanager = pool_init(3,100);
    int i = 0;
    plist pos;
    pooltask_t *cur_task;
    /*****step2:创建任务**************/
    sleep(2);
    for(i=0;i<5;i++)
    {
        task_create(poolmanager,NULL,task_fun);
        usleep(50000);
    }
    #if 0
    LIST_FOR_EACH(pos, poolmanager->task_list_head)
    {
         cur_task = CONTAINER_OF(pos,pooltask_t,task_list_node);
         (cur_task->task_fun)(cur_task->arg);
    }
    #endif
    poolmanager->pool_is_destroy = true;
    pool_destroy(poolmanager);
    /*****step4：资源回收*************/
    return 0;
}

/********************************
function：task_create
desciption:创建任务，并把任务加入加入链表
parament:poolmanager，线程池结构体.arg任务函数函数参数._task_fun,任务函数
return: void
**********************************/
void task_create(poolmanager_t* poolmanager,void* arg,routine _task_fun)
{
    pthread_mutex_lock(&(poolmanager->mutex));
    pooltask_t  *task = (pooltask_t*)malloc(sizeof(pooltask_t));
    task->task_list_node = (plist)malloc(sizeof(list));
    if((NULL == task) || (NULL==task->task_list_node))
    {
        CONSOLE_MSG("task malloc error\n");
    }
    task->task_fun = _task_fun;
    task->arg = arg;
    list_add_tail(task->task_list_node,poolmanager->task_list_head); //内核链表尾插
    pthread_mutex_unlock(&(poolmanager->mutex));//解锁
    pthread_cond_signal(&(poolmanager->cond)); //挂起线程发送信号
}

void task_fun(void* arg)
{
   DEBUG("task %u fun working\n",(unsigned int)pthread_self());
}
