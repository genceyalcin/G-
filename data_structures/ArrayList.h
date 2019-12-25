#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <inttypes.h>

#define DEFAULT_SIZE 50

struct _ArrayList {
	uint32_t arraySize;
	uint32_t index;

	size_t typeSize;

	void ** array;
};
typedef struct _ArrayList ArrayList;

ArrayList * ArrayList_CreateEmpty(size_t typeSize);

uint32_t ArrayList_Add(ArrayList * array, void * entry);

void ArrayList_RemoveAtIndex(ArrayList * array, uint32_t index);

void * ArrayList_GetEntryAtIndex(ArrayList * array, uint32_t);

uint32_t ArrayList_GetIndexOf(ArrayList * array, void * entry);

#endif
