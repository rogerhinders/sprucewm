#ifndef _LINKED_LIST_H
#define _LINKED_LIST_H

#include "general.h"

struct list_node {
	struct list_node *next;
	void *data;
};

struct linked_list {
	struct list_node *root;
	struct list_node *current;
	struct list_node *end;
};

struct linked_list *linked_list_create();
void linked_list_add(struct linked_list *list, void *data);
uint32_t linked_list_count(struct linked_list *list);
void linked_list_rewind(struct linked_list *list);
void *linked_list_next(struct linked_list *list);
void linked_list_remove(struct linked_list *list, void *data);
void linked_list_destroy(struct linked_list *list);

#endif /* _LINKED_LIST_H */
