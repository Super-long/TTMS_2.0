/*
* Copyright(C), 2007-2008, XUPT Univ.	 
* File name: listTestDrv.c			 
* Description : ����������� 
* Author:   XUPT  		 
* Version:  v.1 	 
* Date: 	2015��4��22��		 
*/

#include "listTestDrv.h"
#include <stdlib.h>
#include <stdio.h>

#include "../Common/List.h"

//�����ʼ������ 
void ListTest_Init() {
	TestList_t head=NULL;

	List_Init(head, TestList_Node_t);

	if (head!=NULL && head->next == head && head->prev == head)
		printf("List_Init passed!\n");
	else
		printf("List_Init Failed!\n");

	List_Destroy(head, TestList_Node_t);
}

//����Ϊ�ղ��� 
void ListTest_Empty() {
	TestList_Node_t head;
	head.next = head.prev = &head;

	if (List_IsEmpty(&head))
		printf("List_Empty passed!\n");
	else
		printf("List_Empty Failed!\n");
}

//����β�巨��������� 
void ListTest_AddTail() {
	TestList_t head;
	TestList_Node_t *p, *nodes;
	int i = 0;
	int result = 1;

	/*��ʼ������*/
	head = (TestList_Node_t*) malloc(sizeof(TestList_Node_t));
	head->next = head->prev = head;

	/*��ʼ������*/
	nodes = (TestList_Node_t*) malloc(sizeof(TestList_Node_t) * 3);
	/*���ɲ�������*/
	for (i = 0; i < 3; i++) {
		nodes[i].data.value = i;
		List_AddTail(head, &nodes[i]);
	}

	/*У��next����ָ��������*/
	for (p = head->next, i = 0; i < 3; i++, p = p->next) {
		if (p != &nodes[i]) {
			result = 0;
			break;
		}
	}

	if (p != head)
		result = 0;

	/*У��prev����ָ��������*/
	for (p = head->prev, i = 2; i >= 0; i--, p = p->prev) {
		if (p != &nodes[i]) {
			result = 0;
			break;
		}
	}
	if (p != head)
		result = 0;

	free(nodes);
	free(head);

	if (result)
		printf("List_AddTail Passed!\n");
	else
		printf("List_AddTail Failed!\n");

}

//����ͷ�巨��������� 
void ListTest_AddHead() {
	TestList_t head;
	TestList_Node_t *p, *nodes;
	int i = 0;

	int result = 1;

	/*��ʼ������*/
	head = (TestList_Node_t*) malloc(sizeof(TestList_Node_t));
	head->next = head->prev = head;

	/*��ʼ������*/
	nodes = (TestList_Node_t*) malloc(sizeof(TestList_Node_t) * 3);
	/*���ɲ�������*/
	for (i = 0; i < 3; i++) {
		nodes[i].data.value = i;
		List_AddHead(head, &nodes[i]);
	}

	/*У��next����ָ��������*/
	for (p = head->next, i = 2; i >= 0; i--, p = p->next) {
		if (p != &nodes[i]) {
			result = 0;
			break;
		}
	}
	if (p != head)
		result = 0;

	/*У��prev����ָ��������*/
	for (p = head->prev, i = 0; i < 3; i++, p = p->prev) {
		if (p != &nodes[i]) {
			result = 0;
			break;
		}
	}
	if (p != head)
		result = 0;

	free(nodes);
	free(head);

	if (result)
		printf("List_AddHead Passed!\n");
	else
		printf("List_AddHead Failed!\n");
}

//�ͷ��������  
void ListTest_Free() {
	TestList_t head;
	TestList_Node_t *node;

	/*��ʼ������*/
	head = (TestList_Node_t*) malloc(sizeof(TestList_Node_t));
	head->next = head->prev = head;

	/*���Կ������Free*/
	List_Free(head, TestList_Node_t);

	if (NULL==head || head->next != head || head->prev != head){
		printf("List_Free Failed!\n");
		return ;
	}

	/*���Էǿ�����Free*/
	node = (TestList_Node_t*) malloc(sizeof(TestList_Node_t));
	node->data.value=1;
	/*�����node���뵽������*/
	node->next=head;
	node->prev=head;
	head->next=node;
	head->prev=node;

	List_Free(head, TestList_Node_t);
	if (head!=NULL && head->next == head && head->prev == head)
		printf("List_Free passed!\n");
	else
		printf("List_Free Failed!\n");

	free(head);
}

//����������� 
void ListTest_Destroy() {
	TestList_t head;
	TestList_Node_t *node;

	/*���Կ������Free*/
	head = (TestList_Node_t*) malloc(sizeof(TestList_Node_t));
	head->next = head->prev = head;
	List_Destroy(head, TestList_Node_t);

	if (NULL!=head){
		printf("List_Destroy Failed!\n");
		return ;
	}

	/*���Էǿ�����Free*/
	head = (TestList_Node_t*) malloc(sizeof(TestList_Node_t));
	head->next = head->prev = head;
	node = (TestList_Node_t*) malloc(sizeof(TestList_Node_t));
	node->data.value=1;
	/*�����node���뵽������*/
	node->next=head;
	node->prev=head;
	head->next=node;
	head->prev=node;

	List_Destroy(head, TestList_Node_t);
	if (head==NULL)
		printf("List_Destroy passed!\n");
	else
		printf("List_Destroy Failed!\n");
}

//����������� 
void List_TestDriver_Entry() {
	/*���Գ�ʼ��*/
	ListTest_Init();

	ListTest_Empty();

	ListTest_AddTail();

	ListTest_AddHead();

	ListTest_Free();

	ListTest_Destroy();

}
