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

/** @file       list.c
 *  @brief      C泛型链表实现
 *  @date       2014年11月07日 15时57分28秒
 *  @author     kinuxroot, kinuxroot@163.com
 *  @group      LessOS Development Group
 *  @version    1.0
 *  @note       
 *
 */

#include "lib/list.h"

PUBLIC void __listInit(ListNode* head){
    head->prev = head;
    head->next = head;
}

PUBLIC void __listInsert(ListNode* prev, ListNode* next, ListNode* newNode){
    prev->next = newNode;
    newNode->prev = prev;
    next->prev = newNode;
    newNode->next = next;
}

PUBLIC void __listAdd(ListNode* head, ListNode* newNode){
    ListNode* prev = head;
    while ( prev->next != head ) {
        prev = prev->next;
    }
    __listInsert(prev, head, newNode);
}

PUBLIC void __listRemove(ListNode* node){
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

PUBLIC void __listPrint(ListNode* head){
    ListNode* current = head;
    do {
        printf("%p: %p %p\n", current, current->prev, current->next);
        current = current->next;
    } while ( current != head );
}
