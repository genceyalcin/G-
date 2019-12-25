#include <stdio.h>
#include <stdlib.h>

#include "ArrayList.h"

static void ArrayList_EnsureCapacity(ArrayList * array);

/**
 * Creates empty ArrayList struct for a type with typeSize
 * 
 * Pre:		typeSize - size of type that is going to enter array
 * 
 * Post:	Dynamically allocates memory for an array of default size
 * 			for the ArrayList and returns it
 * 
 */
ArrayList * ArrayList_CreateEmpty(size_t typeSize) {
	
	ArrayList * arrayList = calloc(1, sizeof(ArrayList));
	
	void ** array = calloc(DEFAULT_SIZE, typeSize);	
	
	arrayList->array = array;
	arrayList->arraySize = DEFAULT_SIZE;
	arrayList->index = 0;
	arrayList->typeSize = typeSize;
	
	return arrayList;

}

/**
 * Adds to the end of the list
 * 
 * Pre:		array - is the ArrayList to add to
 * 			entry - is the entry to be added
 * 	
 * Post:	Adds entry to the end of the array list and increments
 * 			the index. Increases array capacity if necessary. Returns
 * 			the index that the entry was added to.
 */
uint32_t ArrayList_Add(ArrayList * array, void * entry) {

	ArrayList_EnsureCapacity(array);
	
	uint32_t indexNow = array->index;
	
	array->array[indexNow] = entry;
	array->index++;
	
	return indexNow;
	
}

/**
 *	Removes an entry at index. Slides all entries after removed index 
 *	down. The index is assumed to be within bounds.
 * 
 * 	Pre:	array - is the ArrayList to remove from
 * 			index - is the index which the entry to be removed is from
 * 
 * 	
 * 
 */
void ArrayList_RemoveAtIndex(ArrayList * array, uint32_t index) {
	for (uint32_t i = index; i < array->index - 1; i++) {
		array->array[i] = array->array[i + 1];
	}
	array->array[array->index] = NULL;
	array->index--; 
}

/**
 * 
 * 
 */
void * ArrayList_GetEntryAtIndex(ArrayList * array, uint32_t index) {
	return array->array[index];
}

/**
 * 
 * 
 */
uint32_t ArrayList_GetIndexOf(ArrayList * array, void * entry) {
	return 0;
}

/**
 * 
 */
static void ArrayList_EnsureCapacity(ArrayList * array) {
	if (array->index == array->arraySize - 1) {
		array->array = realloc(array->array, DEFAULT_SIZE * array->typeSize * 2);	
	}
}
