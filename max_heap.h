#ifndef MAX_HEAP_H
#define MAX_HEAP_H
#include <stdlib.h>

typedef struct
{
    void* key;
    void* value;
} maxheap_node_t;

enum
{
    MAXHEAP_FILE_SUCCESS = 0,
    MAXHEAP_FILE_ERROR_OPEN,
    MAXHEAP_FILE_ERROR_SPECIFIER,
    MAXHEAP_FILE_ERROR_CLOSE,
    MAXHEAP_FILE_ERROR_MALLOC,
    MAXHEAP_FILE_ERROR_FORMAT
};

#define MAXHEAP_COMPOUND_NODE(KEY_TYPE, KEY, VALUE_TYPE, VALUE) (maxheap_node_t){&(KEY_TYPE){(KEY)}, &(VALUE_TYPE){(VALUE)}}

void maxHeapInit(maxheap_node_t* heap[], const size_t size);
bool maxHeapInitPlace(maxheap_node_t heap[], const size_t pos, maxheap_node_t elem, const size_t keySize, const size_t valueSize);
int maxHeapInitFromFile(maxheap_node_t* heap[], size_t *heapSize, const char* file,
     const char* keyFormat, const char* valueFormat,
const size_t keySize, const size_t valueSize, int (*compare)(const void* key1, const void* key2));
int maxHeapWriteToFile(const maxheap_node_t heap[], const size_t heapSize, const char* file,
 const char* keyFormat, const char* valueFormat);
void maxHeapCleanup(maxheap_node_t heap[], size_t heapSize);
void maxHeapHeapify(maxheap_node_t heap[], const size_t heapSize, size_t pos, const size_t keySize, const size_t valueSize,
     int (*compare)(const void* key1, const void* key2));
void maxHeapBuildHeap(maxheap_node_t heap[], const size_t heapSize, const size_t keySize, const size_t valueSize, 
    int (*compare)(const void* key1, const void* key2));
void maxHeapSort(maxheap_node_t heap[], const size_t heapSize, maxheap_node_t out[],
     const size_t keySize, const size_t valueSize,
    int (*compare)(const void* key1, const void* key2));
size_t maxHeapInsert(maxheap_node_t* heap[], const size_t heapSize, maxheap_node_t elem, const size_t keySize,
    const size_t valueSize,
     int (*compare)(const void* key1, const void* key2));
size_t maxHeapDelete(maxheap_node_t* heap[], const size_t heapSize, const size_t pos, const size_t keySize,
    const size_t valueSize,
    int (*compare)(const void* key1, const void* key2));
void maxHeapIncreaseKey(maxheap_node_t heap[], size_t heapSize, const size_t pos, const void* key, const size_t keySize,
    const size_t valueSize,
    int (*compare)(const void* key1, const void* key2));
void maxHeapDecreaseKey(maxheap_node_t heap[], const size_t heapSize, size_t pos, const void* key, const size_t keySize,
    const size_t valueSize,
    int (*compare)(const void* key1, const void* key2));
maxheap_node_t maxHeapGetLargestElement(const maxheap_node_t heap[]);
maxheap_node_t maxHeapGetElementAtPos(const maxheap_node_t heap[], const size_t pos);
size_t maxHeapGetParentPos(size_t pos);
size_t maxHeapGetLeftChildPos(size_t pos);
size_t maxHeapGetRightChildPos(size_t pos);

#endif