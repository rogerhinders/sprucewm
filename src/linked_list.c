#include "linked_list.h"

static struct list_node *create_node(void *data) {
	struct list_node *node = malloc(sizeof(struct list_node));

	node->next = NULL;
	node->data = data;

	return node;
}

struct linked_list *linked_list_create() {
	struct linked_list *list = malloc(sizeof(struct linked_list));

	list->root = NULL;
	list->end = NULL;

	return list;
}

void linked_list_add(struct linked_list *list, void *data) {
	if(list == NULL)
		return;

	struct list_node *node = create_node(data);

	if(list->root == NULL) {
		list->root = node;
		list->current = node;
		list->end = node;

		return;
	}

	list->end->next = node;
	list->end = node;
}

uint32_t linked_list_count(struct linked_list *list) {
	if(list == NULL)
		return 0;

	uint32_t n = 0;

	struct list_node *p = list->root;

	while(p != NULL) {
		p = p->next;
		n++;
	}

	return n;
}

void linked_list_rewind(struct linked_list *list) {
	if(list == NULL)
		return;

	list->current = list->root;
}

void *linked_list_next(struct linked_list *list) {
	if(list == NULL || list->current == NULL)
		return NULL;

	void *ret = list->current->data;

	list->current = list->current->next;

	return ret;
}

void linked_list_remove(struct linked_list *list, void *data) {
	if(data == NULL)
		return;

	if(list == NULL)
		return;
	

	linked_list_rewind(list);

	struct list_node *cur, *prev = NULL;

	for(cur = list->root; cur != NULL; prev = cur, cur = cur->next) {
		if(cur->data != data)
			continue;

		if(cur == list->root)
			list->root = cur->next;
		else
			prev->next = cur->next;

		if(cur == list->end)
			list->end = prev;
	
		free(cur);
		break;
	}
}

void linked_list_destroy(struct linked_list *list) {
	if(list == NULL) {
		return;
	}

	struct list_node *node;

	while(list->root != NULL) {
		node = list->root;
		list->root = node->next;
		free(node);
	}

	free(list);
}
