#include <stdarg.h>
#include <stddef.h>
#include "..\exception\assert.h"
#include "..\mem\mem.h"
#include "list.h"

#define T List_T

struct T
{
	struct T* rest;
	void* first;
};

/*functions*/
T List_push(T list, void* pElem)
{
	T node;

	NEW(node);
	node->first = pElem;
	node->rest = list;
	return node;
}

T List_list(void* pElem, ...)
{
	va_list argv;
	T list;
	T* pNode = &list;

	for (va_start(argv, pElem); pElem != NULL; pElem = va_arg(argv, void*))
	{
		NEW(*pNode);
		(*pNode)->first = pElem;
		pNode = &(*pNode)->rest;
	}
	*pNode = NULL;
	va_end(argv);
	return list;
}

T List_append(T list, T tail)
{
	T* p = &list;

	while (*p != NULL)
	{
		p = &(*p)->rest;
	}
	*p = tail;
	return list;
}

T List_copy(T list)
{
	T head, *p = &head;
	/*for (; list != NULL; list = list->rest) {
		NEW(*p);
		(*p)->first = list->first;
		p=&(*p)->rest;
		}*/
	while (list)
	{
		NEW(*p);
		(*p)->first = list->first;
		p = &(*p)->rest;
		list = list->rest;
	}
	*p = NULL;
	return head;
}

T List_pop(T list, void** x)
{
	if (list)
	{
		T head = list->rest;
		if (x)
			*x = list->first;
		FREE(list);
		return head;
	}
	else
	{
		return list;
	}
}

T List_reverse(T list)
{
	T head = NULL, next;
	/*for (; list != NULL; list = next) {
		next = list->rest;
		list->rest = head;
		head = list;
		}*/
	while (list)
	{
		next = list->rest;
		list->rest = head;
		head = list;
		list = next;
	}
	return head;
}

int List_length(T list)
{
	int n;

	for (n = 0; list != NULL; list = list->rest)
		n++;
	return n;
}

void List_free(T* list)
{
	T next;

	assert(list);
	while (*list)
	{
		next = (*list)->rest;
		FREE(*list);
		*list = next;
	}
}

void List_map(T list, void apply(void** x, void* cl), void* cl)
{
	assert(apply);
	while (list)
	{
		apply(&list->first, cl);
		list = list->rest;
	}
}

void** List_toArray(T list, void* end)
{
	int i;
	int n = List_length(list);
	void** arr = ALLOC((n + 1)*sizeof(*arr));

	for (i = 0; i < n; i++)
	{
		arr[i] = list->first;
		list = list->rest;
	}
	arr[i] = end;
	return arr;
}