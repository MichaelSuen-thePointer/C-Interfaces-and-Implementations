#ifndef ARRAY_H
#define ARRAY_H

#define T Array_T
typedef struct T* T;

/*exported functions*/
extern T Array_new(int length, int size);
extern void Array_free(T* arr);
extern int Array_length(T arr);
extern int Array_size(T arr);
extern void* Array_get(T arr, int i);
extern void* Array_put(T arr, int i, void* elem);
extern void Array_resize(T arr, int length);
extern T Array_copy(T arr, int length);
/*\exported functions*/

#undef T

#endif