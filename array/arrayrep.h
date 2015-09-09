#ifndef ARRAYREP_H
#define ARRAYREP_H

#define T Array_T

struct T
{
	int length;
	int size;
	char* arr;
};

extern void ArrayRep_init(T arr, int length, int size, void* ary);

#undef T
#endif