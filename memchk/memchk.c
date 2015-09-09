#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "..\exception\assert.h"
#include "..\exception\except.h"
#include "memchk.h"

/*checking types*/
union align
{
	int i;
	long l;
	long long ll;
	long* lp;
	void* p;
	void(*fp)(void);
	float f;
	double d;
	long double ld;
};

struct rawallocmem
{
	void* ptr;
	int size;
	struct rawallocmem* next;
}*rawmem = NULL;

/*checking macros*/
#define hash(p, t) (((unsigned long)(p) >> 3) & (sizeof(t)/sizeof((t)[0]) - 1))
#define NDESCRIPTORS 258
#define MAXFRAGSIZE 16
#define NALLOC 4096
#define NHTAB 2048
static int nalloc = NALLOC;
/*data*/
const Except_T Mem_Failed = {"Allocation Failed"};
/*checking data*/
static struct descriptor
{
	struct descriptor* free;
	//	struct descriptor* rfree;
	struct descriptor* next;
	//	struct descriptor* prev;
	const void* ptr;
	const void* origptr;
	long size;
	const char* file;
	int line;
}*htab[NHTAB];

static struct descriptor freelist = {&freelist, &freelist};

#ifdef MEMCHK_DEBUG
static int totalAllocate;
static int totalUsedUser;
static int totalFreedUser;
static int totalUsedSystem;

static _Bool islogInit;
static FILE* logfile;

typedef enum e_eflag
{
	RawAllocate = 1,
	BlockAllocate = 2,
	BlockFree = 3,
	RawFree = 4
} eflag;
void writelog(const void* rawptr, const void* ptr, const int size,
			  const char* file, const int line, eflag flag)
{
	if (!islogInit)
	{
		fopen_s(&logfile, "C:\\Users\\Crazy95Sun\\Source\\Workspaces\\"
				"C Interfaces and Implementations\\set\\Debug\\memlog.txt",
				"w");
		assert(logfile);
		islogInit = 1;
	}
	switch (flag)
	{
	case RawAllocate:
	{
		fprintf(logfile, "Allocated %d bytes raw memory at %#8p\n",
				size, ptr);
		break;
	}
	case BlockAllocate:
	{
		fprintf(logfile, "Allocate block at %#8p, "
				"size %d, from raw memory %#8p\n",
				ptr, size, rawptr);
		break;
	}
	case BlockFree:
	{
		fprintf(logfile, "Free block at %#8p, size %d, "
				"from raw memory %#8p\n", ptr, size, rawptr);
		break;
	}
	case RawFree:
	{
		fprintf(logfile, "Free raw memory at %#8p, size %d.\n",
				rawptr, size);
		break;
	}
	}
	fprintf(logfile, "at file: %s, line: %d.\n", file, line);
}
#endif

void enrollmem(void* ptr, int size)
{
	static struct rawallocmem* head;
	if (ptr)
	{
		if (rawmem == NULL || head->ptr != NULL)
		{
			struct rawallocmem* newtabs;
			newtabs = calloc(NDESCRIPTORS, sizeof(*head));
			assert(newtabs);
#ifdef MEMCHK_DEBUG
			totalAllocate += NDESCRIPTORS*sizeof(*head);
			totalUsedSystem += NDESCRIPTORS*sizeof(*head);
			writelog(NULL, newtabs, NDESCRIPTORS*sizeof(*head), __FILE__,
					 __LINE__, RawAllocate);
#endif
			if (rawmem == NULL)
			{
				assert(head == NULL);
				rawmem = head = newtabs;
			}
			else if (head->ptr != NULL)
			{
				head->next = newtabs;
				head = head->next;
			}
			for (int i = 1; i < NDESCRIPTORS; i++)
			{
				head->next = newtabs + i;
				head = head->next;
			}
			head->next = NULL;
			head->ptr = newtabs;
			head->size = NDESCRIPTORS*sizeof(*head);
			head = newtabs;
		}
		head->ptr = ptr;
		head->size = size;
		head = head->next;
	}
}
void raise_mem_failed(const char* file, int line)
{
	if (file == NULL)
		RAISE(Mem_Failed);
	else
		Except_raise(&Mem_Failed, file, line);
}

static struct descriptor* find(const void* ptr)
{
	struct descriptor* bp = htab[hash(ptr, htab)];
	while (bp && bp->ptr != ptr)
		bp = bp->next;
	return bp;
}

void finddiscriptor(void* ptr, struct descriptor** bp,
					const char* file, int line)
{
	if (((unsigned long)ptr) % (sizeof(union align)) != 0 ||
		((*bp) = find(ptr)) == NULL || (*bp)->free)
		Except_raise(&Assert_Failed, file, line);
}

void Memchk_free(void* ptr, const char* file, int line)
{
	if (ptr)
	{
		struct descriptor* bp;
		finddiscriptor(ptr, &bp, file, line);
#ifdef MEMCHK_DEBUG
		writelog(bp->origptr, ptr, bp->size, file, line, BlockFree);
		totalFreedUser += bp->size;
#endif
		bp->free = freelist.free;
		freelist.free = bp;
	}
}

void* Memchk_resize(void* ptr, long nbytes, const char* file, int line)
{
	struct descriptor* bp;
	void* newptr;

	assert(ptr);
	assert(nbytes > 0);
	finddiscriptor(ptr, &bp, file, line);
	newptr = Memchk_alloc(nbytes, file, line);
	memcpy(newptr, ptr, nbytes < bp->size ? nbytes : bp->size);
	Memchk_free(ptr, file, line);
	return newptr;
}

void* Memchk_calloc(long count, long nbytes, const char* file, int line)
{
	void* ptr;

	assert(count > 0);
	assert(nbytes > 0);
	ptr = Memchk_alloc(count*nbytes, file, line);
	memset(ptr, 0, count*nbytes);
	return ptr;
}

static struct descriptor* dalloc(const void* origptr, void* ptr, long size,
								 const char* file, int line)
{
	static struct descriptor* avail;
	static int nleft;

	if (nleft <= 0)
	{
		/*allocate descriptors*/
		avail = malloc(NDESCRIPTORS*sizeof(*avail));
		if (avail == NULL)
			return NULL;

		enrollmem(avail, NDESCRIPTORS*sizeof(*avail));
#ifdef MEMCHK_DEBUG
		writelog(NULL, ptr, size, file, line, RawAllocate);
		totalAllocate += NDESCRIPTORS*sizeof(*avail);
		totalUsedSystem += NDESCRIPTORS*sizeof(*avail);
#endif // MEM_CHECKUSEAGE

		nleft = NDESCRIPTORS;
	}
	avail->origptr = origptr;
	avail->ptr = ptr;
	avail->size = size;
	avail->file = file;
	avail->line = line;
	avail->free = avail->next = NULL;
	nleft--;
	return avail++;
}

void* Memchk_alloc(long nbytes, const char* file, int line)
{
	struct descriptor* bp;
	void* ptr;
	const void* origptr;

	assert(nbytes > 0);
	/*round nbytes up to an alignment boundary*/
	nbytes = ((nbytes + sizeof(union align) - 1) /
			  (sizeof(union align)))*(sizeof(union align));
	for (bp = freelist.free; bp != NULL; bp = bp->free)
	{
		if (bp->size > nbytes)
		{
			/*use the end of the block at bp->ptr*/
			bp->size -= nbytes;
			ptr = (char*)bp->ptr + bp->size;
			origptr = bp->ptr;
			if ((bp = dalloc(origptr, ptr, nbytes, file, line)) != NULL)
			{
#ifdef MEMCHK_DEBUG
				writelog(origptr, ptr, nbytes, file, line, BlockAllocate);
				totalUsedUser += nbytes;
#endif // MEM_CHECKUSEAGE
				unsigned h = hash(ptr, htab);
				bp->next = htab[h];
				htab[h] = bp;
				return ptr;
			}
			else
			{
				raise_mem_failed(file, line);
			}
		}
		else //adjust the size of nalloc
		{
			if ((bp->size > MAXFRAGSIZE) && (nalloc < nbytes))
			{
				nalloc += (((nbytes - bp->size) + sizeof(union align) - 1) /
						   (sizeof(union align)))*(sizeof(union align));
				nalloc = ((nalloc > 1048576) || (nalloc <= 0)) ? NALLOC : nalloc;
			}
		}
		if (bp == &freelist)
		{
			struct descriptor* newptr;
			/*newptr = ablock of size nalloc + nbytes*/
			if ((ptr = malloc(nbytes + nalloc)) != NULL &&
				(newptr = dalloc(ptr, ptr, nbytes + nalloc, __FILE__, __LINE__))
				!= NULL)
			{
				enrollmem(ptr, nbytes + nalloc);
#ifdef MEMCHK_DEBUG
				totalAllocate += nbytes + nalloc;
				writelog(NULL, ptr, nbytes + nalloc, file, line, RawAllocate);
#endif // MEM_CHECKUSEAGE
				newptr->free = freelist.free;
				freelist.free = newptr;
			}
			else
			{
				raise_mem_failed(file, line);
			}
		}
	}
	/*this area can never be reached*/
	assert(0);
	return NULL;
}

void Memchk_terminate(void)
{
	struct rawallocmem* head = rawmem, *headPrev;
	while (head)
	{
#ifdef MEMCHK_DEBUG
		if (head->ptr)
		{
			writelog(head->ptr, NULL, head->size, __FILE__, __LINE__, RawFree);
		}
#endif
		headPrev = head;
		head = head->next;
		free(headPrev->ptr);
	}
#ifdef MEMCHK_DEBUG
	printf("Total allocated %d bytes.\n", totalAllocate);
	printf("Total used %d bytes by system.\n", totalUsedSystem);
	printf("Total freed %d bytes by user.\n", totalFreedUser);
	printf("Total used %d bytes by user.\n", totalUsedUser);
	fprintf(logfile, "Total allocated %d bytes.\n", totalAllocate);
	fprintf(logfile, "Total used %d bytes (%.2lf%%) by system.\n",
			totalUsedSystem, (double)totalUsedSystem / totalAllocate * 100);
	fprintf(logfile, "Total freed %d bytes (%.2lf%%) by user.\n",
			totalFreedUser, (double)totalFreedUser / totalAllocate * 100);
	fprintf(logfile, "Total used %d bytes (%.2lf%%) by user.\n",
			totalUsedUser, (double)totalUsedUser / totalAllocate * 100);
	fclose(logfile);
#endif // MEM_CHECKUSEAGE
}