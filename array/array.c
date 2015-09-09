#include <stdlib.h>
#include <string.h>
#include "..\exception\assert.h"
#include "..\mem\mem.h"
#include "array.h"
#include "arrayrep.h"

#define T Array_T

/*functions*/
void ArrayRep_init(T arr, int length, int size, void* ary)
{
	assert(arr);
	assert(ary && length > 0 || length == 0 && ary == NULL);
	assert(size > 0);
	arr->length = length;
	arr->size = size;
	if (length > 0)
		arr->arr = ary;
	else
		arr->arr = NULL;
}

T Array_new(int length, int size)
{
	T arr;

	NEW(arr);
	if (length > 0)
		ArrayRep_init(arr, length, size, CALLOC(length, size));
	else
		ArrayRep_init(arr, length, size, NULL);
	return arr;
}

void Array_free(T* arr)
{
	assert(arr && *arr);
	FREE((*arr)->arr);
	FREE(*arr);
}

void* Array_get(T arr, int i)
{
	assert(arr);
	assert(i >= 0 && i < arr->length);
	return arr->arr + i*arr->size;
}

void* Array_put(T arr, int i, void* elem)
{
	assert(arr);
	assert(i >= 0 && i < arr->length);
	assert(elem);
	memcpy(arr->arr + i*arr->size, elem, arr->size);
	return elem;
}

int Array_length(T arr)
{
	assert(arr);
	return arr->length;
}

int Array_size(T arr)
{
	assert(arr);
	return arr->size;
}

void Array_resize(T arr, int length)
{
	assert(arr);
	assert(length >= 0);
	if (arr->length == 0)
		arr->arr = ALLOC(length*arr->size);
	else if (length > 0)
		RESIZE(arr->arr, length*arr->size);
	else
		FREE(arr->arr);
	arr->length = length;
}

T Array_copy(T arr, int length)
{
	T copy;

	assert(arr);
	assert(length >= 0);
	copy = Array_new(length, arr->size);
	if (copy->length >= arr->length && arr->length > 0)
		memcpy(copy->arr, arr->arr, arr->length*arr->size);
	else if (arr->length > copy->length && copy->length > 0)
		memcpy(copy->arr, arr->arr, copy->length*arr->size);
	return copy;
}

/*\functions*/
#undef T
