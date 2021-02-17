//
// Created by 姚雨 on 2021/2/16.
//

#ifdef  _LIST_H
#define _LIST_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct myListNode {
    void *data;
    struct myListNode *next;
} NODE;

typedef struct myList {
    int size;
    NODE *head;
    NODE *tail;
} LIST;

LIST *list_init();// 获取链表
int List_destroy(LIST *list);// 清理链表以及其所有元素
int list_insert(LIST *list, void *data); // 插入节点（尾插）
int list_delete(LIST *list);// 删除节点(头删)
void *list_value(LIST *list);// 获取头节点数据
#ifdef __cplusplus
#endif
#endif



