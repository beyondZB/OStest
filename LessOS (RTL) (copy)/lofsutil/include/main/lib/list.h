/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014 kinuxroot
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/** @file       list.h
 *  @brief      C泛型链表实现，提供了链表定义和大量操作链表的宏
 *  @date       2014年11月07日 15时57分28秒
 *  @author     kinuxroot, kinuxroot@163.com
 *  @group      LessOS Development Group
 *  @version    1.0
 *  @note       
 *              该链表是一种侵入式实现，需要显式将链表节点放入自己定义的结构体中
 *              借鉴了Linux源代码实现
 *
 */

#ifndef _LIST_H
#define _LIST_H

#include "type.h"

#include <stdio.h>

typedef struct __ListNode_tag {
    struct __ListNode_tag *prev;
    struct __ListNode_tag *next;
} ListNode;

typedef struct __List_tag {
    ListNode listNode;
} List;

#define listInit(container) \
    __listInit(&((container)->listNode))
#define listInsert(prev, next, newContainer) \
    __listInsert(&((prev)->listNode), &((next)->listNode), \
            &((newContainer)->listNode))
#define listAdd(head, newContainer) \
    __listAdd(&((head)->listNode), &((newContainer)->listNode))
#define listRemove(container) \
    __listRemove(&((container)->listNode))
#define listPrint(headContainer) \
    __listPrint(&((headContainer)->listNode))

#define containerOfListNode(node, type) ( \
    (type*) \
        ( \
            (char*)(node) - \
            (char*)(&(((type*)(0))->listNode)) \
        ) \
)

#define listForEach(type, container, list) \
    for ( container = containerOfListNode((list)->listNode.next, type); \
            (void*)container != (void*)containerOfListNode(&((list)->listNode), type); \
            container = containerOfListNode(container->listNode.next, type) )

PUBLIC void __listInit(ListNode* head);
PUBLIC void __listInsert(ListNode* prev, ListNode* next, ListNode* newNode);
PUBLIC void __listAdd(ListNode* head, ListNode* newNode);
PUBLIC void __listRemove(ListNode* node);
PUBLIC void __listPrint(ListNode* head);

#endif // _LIST_H
