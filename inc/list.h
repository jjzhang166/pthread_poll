/*****************************************************************
** 文件名:链表模块 list.h
** Copyright (c) xiethon@126.com

** 创建人: xiethon
** 日  期: 2017-12-17
** 描  述: 内核链表定义 参考自linux内核中的\include\linux\list.h
** 版  本: V1.0

** 修改人:
** 日  期:
** 修改描述:
** 版  本:
******************************************************************/
#ifndef _LIST_H_
#define _LIST_H_
#include<stdbool.h>

/********内核链表结构定义****/
typedef struct _list
{
	struct _list *next;
	struct _list *prev;
}list,*plist;

/*********根据结构体成员地址计算结构体地址*******/
#define CONTAINER_OF(ptr,type,member) \
    (type *)((char *)(ptr) - (char *) &(((type *)0)->member) - 8)

/******初始化链表结构体******/
#define LIST_HEAD_INIT(name) {&(name),&(name)}
#define LIST_HEAD(name)	list name = LIST_HEAD_INIT(name)

/******初始化链表结构体指针*******/
#define INIT_LIST_HEAD(ptr) do{(ptr)->next = (ptr);(ptr)->prev = (ptr);}while(0)

/******遍历链表*************/
#define LIST_FOR_EACH(pos, head) \
	for(pos = (head)->next; pos != (head); pos = (pos->next))

/*******添加一个节点*************/
static	inline void __list_add(plist new,plist prev,plist next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}
//头插法添加一个节点
static inline void list_add(plist new,plist head)
{
	__list_add(new,head,head->next);
}
//尾插法添加一个节点
static inline void list_add_tail(plist new,plist head)
{
	__list_add(new,head->prev,head);
}

/*************删除一个节点*************/
static inline void __list_del(plist prev,plist next)
{
	next->prev = prev;
	prev->next = next;
}
static inline void list_del(plist entry)
{
	__list_del(entry->prev,entry->next);
	entry->next = NULL;
	entry->prev = NULL;
}

/************将节点从原来的位置移动到头结点后面*******/
static inline void list_move(plist node,plist head)
{
	__list_del(node->prev,node->next);
	list_add(node,head);
}

/************将节点从原来的位置移动到尾部******/
static inline void list_move_tail(plist node,plist head)
{
	__list_del(node->prev,node->next);
	list_add_tail(node,head);
}

/************判断list是否为空**************/
static inline bool list_is_empty(plist head)
{
	return (head->next == head);
}

/************判断list是否为链表尾部********/
static inline bool list_is_last(plist entry,plist head)
{
	return (entry->next == head);
}














#endif
