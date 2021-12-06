//
// Created by 97250 on 21/11/2021.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

typedef struct node_t {
	int x;
	struct node_t* next;
} * Node;
typedef enum {
	SUCCESS = 0,
	MEMORY_ERROR,
	EMPTY_LIST,
	UNSORTED_LIST,
	NULL_ARGUMENT,
} ErrorCode;

int getListLength(Node list);

bool isListSorted(Node list);

Node createNode(int x)
{
	Node ptr = malloc(sizeof(*ptr));
	if (!ptr) {
		return NULL;
	}
	ptr->x = x;
	ptr->next = NULL;
	return ptr;
}

void destroyList(Node ptr)
{
	while (ptr) {
		Node toDelete = ptr;
		ptr = ptr->next;
		free(toDelete);
	}
}


ErrorCode mergeSortedLists(Node list1, Node list2, Node* mergedOut)
{
	mergedOut = NULL;
	if (list1 == NULL || list2 == NULL) {
		return EMPTY_LIST;
	}
	/*
	if (!isListSorted(list1) || !isListSorted(list2)) {
		return UNSORTED_LIST;
	}
	 */
	///why need star?
	//int sum_of_length = getListLength(list1) + getListLength(list2);
	Node new_list = NULL;
	//malloc(sizeof(*new_list));
	Node iterator1 = list1;
	Node iterator2 = list2;
	Node iterator3 = NULL;
	if (iterator1->x < iterator2->x) {
		new_list = createNode(iterator1->x);
	} else {
		new_list = createNode(iterator2->x);
	}
	if (new_list == NULL) {
		return MEMORY_ERROR;
	}
	iterator3 = new_list;
	/// remove list1->x>0
	while (iterator1 != NULL && iterator2 != NULL) {
		if (iterator1->x < iterator2->x) {
			iterator3->next = createNode(iterator1->x);
			iterator3 = iterator3->next;
			iterator1 = iterator1->next;
		} else {
			iterator3->next = createNode(iterator2->x);
			iterator3 = iterator3->next;
			iterator2 = iterator2->next;
		}
		if (iterator3 == NULL) {
			return MEMORY_ERROR;
		}
	}

	while (iterator1) {
		iterator3->next = createNode(iterator1->x);
		iterator3 = iterator3->next;
		if (iterator3 == NULL) {
			return MEMORY_ERROR;
		}
		iterator1 = iterator1->next;
	}
	while (iterator2) {
		iterator3->next = createNode(iterator2->x);
		iterator3 = iterator3->next;
		if (iterator3 == NULL) {
			return MEMORY_ERROR;
		}
		iterator2 = iterator2->next;
	}
	iterator3->next = NULL;
	*mergedOut = new_list;
	return SUCCESS;
}


int main()
{
	struct node_t node1_3 = {8, NULL};
	struct node_t node1_2 = {4, &node1_3};
	struct node_t node1_1 = {2, &node1_2};

	struct node_t node2_4 = {10, NULL};
	struct node_t node2_3 = {9, &node2_4};
	struct node_t node2_2 = {4, &node2_3};
	struct node_t node2_1 = {1, &node2_2};
Node a=NULL;
	Node* list = &a;
	ErrorCode error = mergeSortedLists(&node1_1, &node2_1, list);
	printf("ErrorCode: %d\n", error);
	Node iterator = *list;
	while (iterator != NULL) {
		printf("%d\n", iterator->x);
		iterator = iterator->next;
	}
	destroyList(*list);
	return 0;
}
