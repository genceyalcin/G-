// hashTable.c
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "StringHashTable.h"

#define defaultNumOffsets 4

// Node type for the chained hash table for C-strings.
struct _StringNode {
	char*     key;              // ptr to proper C-string
	uint32_t* locations;        // ptr to dynamic array of string locations
	uint32_t  numLocations;     // number of elements currently in locations array
	uint32_t  maxLocations;     // dimension of locations array
	struct _StringNode* next;   // ptr to next node object in table slot
};

static void StringNode_display(FILE* fp, const StringNode* const pNode);
static void createNewNode(StringHashTable* const pTable, uint32_t arrayIndex, StringNode* nextNode, char* key, uint32_t location);
static void ensureCapacity(StringNode* node);
static uint8_t checkKeyLocationMatch(StringNode* node, char* key, uint32_t location);

/** Writes a formatted display of the fields of a StringNode object; for
 *  example:
 *            New Tank:    [359569, 411752, 857960, 942772]
 * 
 *  Pre:  fp is open on an output file
 *        pNode points to a valid StringNode object
 */
static void StringNode_display(FILE* fp, const StringNode* const pNode) {
	
	assert( pNode != NULL );
	
	fprintf(fp, "     %s:  ", pNode->key);
	
   fprintf(fp, "  [%"PRIu32, pNode->locations[0]);
   for (uint32_t idx = 1; idx < pNode->numLocations; idx++) {
	   fprintf(fp, ", %"PRIu32, pNode->locations[idx]);
   }
   fprintf(fp, "]\n");
}

/** Creates a new StringHashTable object such that:
 *    - the array has dimension Size
 *    - the slots in the array are set to NULL
 *    - the hash pointer is set to Hasher (so the hash table will use
 *      that user-supplied hash function)
 * 
 *  Pre: Size equals the desired number of slots in the table
 *       Hasher is the name of the hash function the table is to use,
 *         and that function conforms to the specified interface
 * 
 *  Returns: pointer to a newly-allocated StringHashTable object;
 *           blows up an assert() if creation fails
 */
StringHashTable* StringHashTable_create(uint32_t Size, 
                                        uint32_t (*Hasher)(const char* str)) {
	
	// Create the new Hash Table struct
	StringHashTable* newTable = calloc(1, sizeof(StringHashTable));
	assert(newTable != NULL);
		
	// Set all the struct variables
	newTable->table = calloc(Size, sizeof(StringNode*));
	assert(newTable->table != NULL);
	
	newTable->tableSz = Size;
	newTable->numEntries = 0;
	
	// Pass the pointer to the hash function
	newTable->hash = Hasher;
	
	return newTable;
}

/** Adds an entry to the hash table corresponding to the given key/location.
 *  
 *  Pre:  pTable points to a proper StringHashTable object
 *        key points to a proper C-string
 *        location is set to a corresponding location
 * 
 *  Post: if the key/location pair is already in the table, fails; otherwise
 *        if the table does not contain an entry for key, a new index entry
 *           has been created and location has been installed in it;
 *        otherwise, location has been installed into the existing index entry
 *           for key;
 *        the user's C-string is duplicated, not linked to the table
 * 
 *  Returns: true iff the key/location have been properly added to the table
 */
bool StringHashTable_addEntry(StringHashTable* const pTable, 
                              char* key, uint32_t location) {
	
	// Calculate the array index using the hash function
	uint32_t hashValue = pTable->hash(key);
	uint32_t arrayIndex = hashValue % pTable->tableSz;
	
	StringNode* thisNode = pTable->table[arrayIndex];
	
	// If there is a stringNode already in this position, go through the 
	// chain to either find existing key or not insert if entry already exists
	while(thisNode != NULL) {
		
		// If it is already occupied, check if the key and location matches
		if (checkKeyLocationMatch(thisNode, key, location) == 1) {
			// If the key and location matches then insertion fails
			return false;
		} else if (strcmp(thisNode->key, key) == 0) {
			// If just the key value matches, then add to the locations
			ensureCapacity(thisNode); // Make sure the array has enough memory allocated
			thisNode->locations[thisNode->numLocations] = location;
			thisNode->numLocations++;
			return true;
		}
		
		// Go to the next node in the chain
		thisNode = thisNode->next;
	}
	
	// Create a node
	createNewNode(pTable, arrayIndex, pTable->table[arrayIndex], key, location);

	
    return true;	
}

/** Finds the locations, if any, that correspond to the given key.
 * 
 *  Pre:  pTable points to a proper StringHashTable object
 *        key points to a proper C-string
 * 
 *  Returns: NULL if there is no entry for key in the table; otherwise
 *           a pointer to an array storing the locations corresponding
 *           to the key, and terminated by a zero location (which would
 *           never be logically valid)
 */
uint32_t* StringHashTable_getLocationsOf(const StringHashTable* const pTable, 
                                         const char* key) {
	
	// Calculate the array index using the hash function
	uint32_t hashValue = pTable->hash(key);
	uint32_t arrayIndex = hashValue % pTable->tableSz;
	
	// Access the StringNode at the hash value location
	StringNode* thisNode = pTable->table[arrayIndex];
	
	// If the StringNode isn't NULL, then try to find the StringNode in
	// the chain with the same key value
	while (thisNode != NULL) {
		
		// If the key value is the same, make a deep copy of the locations array,
		// then return it
		if (strcmp(key, thisNode->key) == 0) {
			
			uint32_t * deepCopy = calloc(thisNode->numLocations + 1, sizeof(uint32_t));
			memcpy(deepCopy, thisNode->locations, sizeof(uint32_t) * thisNode->numLocations);
			
			return deepCopy;
		}
		
		// Go to the ne
		thisNode = thisNode->next;
	}
	
	// If the StringNode doesn't exist or the chain doesn't include the key,
	// return NULL
	return NULL;
}

/** Deallocates memory associated with a StringHashTable.
 * 
 *  Pre:  pTable points to a proper StringHashTable object
 * 
 *  Post: the following have been deallocated
 *           - the key belonging to every index entry in the table
 *           - the array of locations belonging to every index entry in the table
 *           - every index entry in the table
 *           - the array for the table
 *
 *   Note: the function does not attempt to deallocate the StringHashTable object
 *         itself, because that object may or may not have been allocated dynamically;
 *         cleanup that up is the responsibility of the user.
 */
void StringHashTable_clear(StringHashTable* const pTable) {
	// Start going through each StringNode chain in the table
	for (uint32_t i = 0; i < pTable->tableSz; i++) {
		
		// Access this index
		StringNode* thisNode = pTable->table[i];
		
		// If it is not null, there is a chain there so traverse it
		while (thisNode != NULL) {
			// Free the dynamic struct variables
			free(thisNode->key);
			free(thisNode->locations);
			
			// Free the node itself
			StringNode* temp = thisNode;
			thisNode = thisNode->next;
			free(temp);
		}
	}
	// Free the table
	free(pTable->table);
}

/** Writes a clearly-formatted display of the contents of a given
 *  StringHashTable.
 * 
 *  Pre:  fp points to an open file, or is stdout
 *        pTable points to a proper StringHashTable object
 */
void StringHashTable_display(FILE* fp, const StringHashTable* const pTable) {
	
	fprintf(fp, "Hash table contains the following %"PRIu32" entries:\n", 
	            pTable->numEntries);
	
	uint32_t pos = 0;
	while ( pos < pTable->tableSz ) {
		
		StringNode* currEntry = pTable->table[pos];
		if ( currEntry != NULL ) {
		
   		fprintf(fp, "%5"PRIu32": ", pos);
   		StringNode_display(fp, currEntry);
	      currEntry = currEntry->next;
   		while ( currEntry != NULL ) {
				fprintf(fp, "       ");
      		StringNode_display(fp, currEntry);
		      currEntry = currEntry->next;
			}
			fprintf(fp, "\n");
		}
		pos++;
	}
}

/** Create a new StringNode struct
 * 
 *  Pre:	pTable is the StringHashTable that we are adding a StringNode to
 * 			arrayIndex is the index that we are adding the StringNode to
 * 			nextNode is the StringNode after the new node
 * 
 *  Post: 	Create a new StringNode in pTable at arrayIndex with nextNode
 * 			as the next node
 */
 static void createNewNode(StringHashTable* const pTable, uint32_t arrayIndex, StringNode* nextNode, char* key, uint32_t location) {
	
	// Create a new node
	StringNode* newNode = calloc(1, sizeof(StringNode));
	assert(newNode != NULL);
	
	// Allocate memory for the locations array
	newNode->locations = calloc(4, sizeof(uint32_t));
	assert(newNode->locations != NULL);

	// Allocate memory for the key
	newNode->key = malloc(sizeof(char) * (strlen(key) + 1));
	assert(newNode->key != NULL);
	
	// Add local variables (including first location)
	strcpy(newNode->key, key);
	newNode->locations[0] = location; // First location
	newNode->numLocations = 1;
	newNode->maxLocations = 4;
	
	// Set the next node, then make this node the first node
	newNode->next = nextNode;
	pTable->table[arrayIndex] = newNode;
	
	// Increment the number of entries
	pTable->numEntries += 1;
}

/** Increases the capacity of a StringNode locations array if necessary
 *  
 *  Pre:	node is the StringNode that we are going to increase
 * 			the location capacity for if necessary
 */
static void ensureCapacity(StringNode* node) {
	// Check the amount of locations in the array
	if (node->numLocations == node->maxLocations) {
		// If it is at max locations, increase array capacity
		node->locations = realloc(node->locations, sizeof(uint32_t) * 2 * node->maxLocations);
		node->maxLocations *= 2;
	}
}

/** Checks if the key and location matches with the ones of a 
 *  StringNode struct
 * 
 * 	Pre:	node is the StringNode we are checking
 * 			key is the key that is passed in
 * 			location is the location that is passed in
 * 
 * 	Post: 	Returns 1 if the key and location match, 0 if not
 */
static uint8_t checkKeyLocationMatch(StringNode* node, char* key, uint32_t location) {
	// Check if the keys are equal
	if (strcmp(key, node->key) == 0) {
		// Iterate through locations
		for(uint32_t i = 0; i < node->numLocations; i++) {
			// Check if location is already in the array
			if (node->locations[i] == location) {
				// If it is, return 1
				return 1;
			}
		}
	} 
	// If not return 0
	return 0;
}
