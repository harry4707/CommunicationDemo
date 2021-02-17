//
// Created by 姚雨 on 2021/2/16.
//

#include <stdlib.h>
#include "list.h"

LIST *list_init() {
    LIST *list = (LIST *)malloc(sizeof(LIST));
    if(list == NULL) {
        return NULL;
    }
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
    return list;
}

int list_destroy(LIST *list) {
    while (list->size > 0) {
        NODE *temp = list->head;
        list->head = list->head->next;
        free(temp);
        list->size--;
    }
    if (list != NULL) {
        free(list);
    }
    return 0;
}

int list_insert(LIST *list, void *data) {
    NODE *temp = (NODE *)malloc(sizeof(NODE));
    if(temp == NULL) {
        return -1;
    }
    temp->data = data;
    temp->next = NULL;
    if(list->size == 0) {
        list->head = temp;
        list->tail = temp;
    } else {
        list->tail->next = temp;
        list->tail = temp;
    }
    list->size ++;
    return list->size;
}

int list_delete(LIST *list) {
    if(list->size == 0) {
        return -1;
    }
    free(list->head->data);
    NODE *temp = list->head;
    list->head = list->head->next;
    free(temp);
    list->size--;
    return list->size;

}

void * list_value(LIST *list) {
    if(list->head == NULL) {
        return NULL;
    } else {
        return list->head->data;
    }
}
