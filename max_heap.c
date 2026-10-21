#include "max_heap.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

static void swap(void* a, void* b, const size_t size)
{
    unsigned char temp[size];

    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
}

maxheap_node_t maxHeapGetLargestElement(const maxheap_node_t heap[])
{
    return *heap;
}

maxheap_node_t maxHeapGetElementAtPos(const maxheap_node_t heap[], const size_t pos)
{
    return heap[pos];
}

void maxHeapHeapify(maxheap_node_t heap[], const size_t heapSize, size_t pos,
     const size_t keySize, const size_t valueSize,
     int (*compare)(const void*, const void*))
{
    assert (pos < heapSize);
    while (1)
    {
        size_t largerPos = maxHeapGetLeftChildPos(pos);

        if (largerPos >= heapSize)
        {
            return;
        }

        size_t rightPos = maxHeapGetRightChildPos(pos);

        if (rightPos < heapSize && compare(heap[rightPos].key, heap[largerPos].key) > 0)
        {
            largerPos = rightPos;
        }

        if (compare(heap[largerPos].key, heap[pos].key) <= 0)
        {
            return;
        }

        swap(heap[largerPos].key, heap[pos].key, keySize);
        swap(heap[largerPos].value, heap[pos].value, valueSize);

        pos = largerPos;
    }
}

void maxHeapBuildHeap(maxheap_node_t heap[], const size_t heapSize,
     const size_t keySize, const size_t valueSize, 
    int (*compare)(const void*, const void*))
{
    size_t i = heapSize / 2U;
    while (i > 0U)
    {
        --i;
        maxHeapHeapify(heap, heapSize, i, keySize, valueSize, compare);
    }

}

void maxHeapSort(maxheap_node_t heap[], const size_t heapSize, maxheap_node_t out[], const size_t keySize,
    const size_t valueSize,
    int (*compare)(const void*, const void*))
{
    maxHeapBuildHeap(heap, heapSize, keySize, valueSize, compare);
    size_t currentSize = heapSize;

    for (size_t i = 0U; i < heapSize; ++i)
    {
        memcpy(out[i].key, heap[0U].key, keySize);
        memcpy(out[i].value, heap[0U].value, valueSize);

        --currentSize;

        if (currentSize > 0U)
        {
            swap(heap[0U].key, heap[currentSize].key, keySize);
            swap(heap[0U].value, heap[currentSize].value, valueSize);

            if (currentSize > 1U)
            {
                maxHeapHeapify(heap, currentSize, 0U, keySize, valueSize, compare);
            }
        }
    }
}


size_t maxHeapInsert(maxheap_node_t* heap[], const size_t heapSize, maxheap_node_t elem,
    const size_t keySize,
    const size_t valueSize,
    int (*compare)(const void*, const void*))
{
    size_t newSize = heapSize + 1U;

    maxheap_node_t* tmp = realloc(*heap, newSize * sizeof(maxheap_node_t));

    if (tmp == nullptr)
    {
        return heapSize;
    }

    *heap = tmp;

    size_t pos = heapSize;

    tmp[pos].key = malloc(keySize);
    tmp[pos].value = malloc(valueSize);

    if (tmp[pos].key == nullptr || tmp[pos].value == nullptr)
    {
        free(tmp[pos].key);
        free(tmp[pos].value);
        return heapSize;
    }

    memcpy(tmp[pos].key, elem.key, keySize);
    memcpy(tmp[pos].value, elem.value, valueSize);

    while (pos > 0U)
    {
        size_t parent = maxHeapGetParentPos(pos);

        if (compare(tmp[pos].key, tmp[parent].key) <= 0)
        {
            break;
        }

        swap(tmp[pos].key, tmp[parent].key, keySize);
        swap(tmp[pos].value, tmp[parent].value, valueSize);

        pos = parent;
    }

    return newSize;
}

size_t maxHeapDelete(maxheap_node_t* heap[], const size_t heapSize, const size_t pos,
    const size_t keySize,
    const size_t valueSize,
    int (*compare)(const void*, const void*))
{
    assert(pos < heapSize);
    size_t newSize = heapSize - 1U;
    maxheap_node_t* tmp = realloc(*heap, newSize * sizeof(maxheap_node_t));
    if (tmp == nullptr)
    {
        return heapSize;
    }

    swap(tmp[pos].key, tmp[newSize].key, keySize);
    swap(tmp[pos].value, tmp[newSize].value, valueSize);
    *heap = tmp;
    maxHeapBuildHeap(*heap, newSize, keySize, valueSize, compare);

    return newSize;
}

void maxHeapIncreaseKey(maxheap_node_t heap[], const size_t heapSize, size_t pos, const void* key,
    const size_t keySize,
    const size_t valueSize,
    int (*compare)(const void*, const void*))
{
    assert(pos < heapSize);
    memcpy(heap[pos].key, key, keySize);
    while (pos > 0U)
    {
        size_t parent = maxHeapGetParentPos(pos);

        if (compare(heap[pos].key, heap[parent].key) <= 0)
        {
            break;
        }

        swap(heap[pos].key, heap[parent].key, keySize);
        swap(heap[pos].value, heap[parent].value, valueSize);

        pos = parent;
    }
}

void maxHeapDecreaseKey(maxheap_node_t heap[], const size_t heapSize, const size_t pos, const void* key,
     const size_t keySize,
    const size_t valueSize,
    int (*compare)(const void*, const void*))
{
    assert(pos < heapSize);
    memcpy(heap[pos].key, key, keySize);
    maxHeapHeapify(heap, heapSize, pos, keySize, valueSize, compare);
}

void maxHeapCleanup(maxheap_node_t heap[], size_t heapSize)
{
    assert(heapSize > 0U);
    while(1)
    {
        --heapSize;
        free(heap[heapSize].key);
        free(heap[heapSize].value);
        if (heapSize == 0U)
        {
            break;
        }
    }
    free(heap);
}

bool maxHeapInitPlace(maxheap_node_t heap[], const size_t pos, maxheap_node_t elem,
     const size_t keySize, const size_t valueSize)
{
    heap[pos].key = malloc(keySize);
    if (heap[pos].key == nullptr)
    {
        return false;
    }

    heap[pos].value = malloc(valueSize);
    if (heap[pos].value == nullptr)
    {
        free(heap[pos].key);
        return false;
    }

    memcpy(heap[pos].key, elem.key, keySize);
    memcpy(heap[pos].value, elem.value, valueSize);
    return true;
}

void maxHeapInit(maxheap_node_t* heap[], const size_t size)
{
    *heap = (maxheap_node_t*)malloc(size * sizeof(maxheap_node_t));
}

int maxHeapInitFromFile(maxheap_node_t* heap[], size_t *heapSize, const char* file,
     const char* keyFormat, const char* valueFormat,
const size_t keySize, const size_t valueSize, int (*compare)(const void*, const void*))
{
    /*{key, value}*/

    size_t heapCount = 0U;

    FILE *f = fopen(file, "r");
    if (f == nullptr)
    {
        return MAXHEAP_FILE_ERROR_OPEN;
    }

    {
        bool braceFound = false;
        for(char c = fgetc(f); c != EOF; c = fgetc(f))
        {
            if (c == '{')
            {
                braceFound = true;
            }
            else if (c == '}')
            {
                if (braceFound)
                {
                    braceFound = false;
                    ++heapCount;
                }
                else
                {
                    fclose(f);
                    return MAXHEAP_FILE_ERROR_FORMAT;
                }
            }
        }
    }

    if (fclose(f) != 0)
    {
        return MAXHEAP_FILE_ERROR_CLOSE;
    }

    maxHeapInit(heap, heapCount);
    maxheap_node_t* heap_ptr = *heap;
    *heapSize = heapCount;

    f = fopen(file, "r");
    if (f == nullptr)
    {
        return MAXHEAP_FILE_ERROR_OPEN;
    }

    size_t iter = 0U;
    while (iter < heapCount)
    {
        /* eat tokens until '{'*/
        for (int c = fgetc(f); c != '{' && c != EOF; c = fgetc(f)) {}
        heap_ptr[iter].key = malloc(keySize);
        heap_ptr[iter].value = malloc(valueSize);
        if (heap_ptr[iter].key == nullptr || heap_ptr[iter].value == nullptr)
        {
            if (heap_ptr[iter].key != nullptr)
            {
                free(heap_ptr[iter].key);
            }

            if (heap_ptr[iter].value != nullptr)
            {
                free(heap_ptr[iter].value);
            }

            for (size_t i = 0U; i < iter; ++i)
            {
                free(heap_ptr[i].key);
                free(heap_ptr[i].value);
            }

            fclose(f);
            return MAXHEAP_FILE_ERROR_MALLOC;
        }

        int ret;
        if (strcmp(keyFormat, "d") == 0 || strcmp(keyFormat, "i") == 0)
        {
            ret = fscanf(f, "%d,", (int*)heap_ptr[iter].key);
        }

        else if (strcmp(keyFormat, "u") == 0)
        {
            ret = fscanf(f, "%u,", (unsigned int*)heap_ptr[iter].key);
        }

        else if (strcmp(keyFormat, "f") == 0)
        {
            ret = fscanf(f, "%f,", (float*)heap_ptr[iter].key);
        }

        else if (strcmp(keyFormat, "lf") == 0)
        {
            ret = fscanf(f, "%lf,", (double*)heap_ptr[iter].key);
        }

        else if (strcmp(keyFormat, "c") == 0)
        {
            ret = fscanf(f, "%c,", (char*)heap_ptr[iter].key);
        }

        else if (strcmp(keyFormat, "s") == 0)
        {
           ret = fscanf(f, " %99[^,],", (char*)heap_ptr[iter].key);
        }

        else if (strcmp(keyFormat, "ld") == 0 || strcmp(keyFormat, "li") == 0)
        {
            ret = fscanf(f, "%ld", (long*)heap_ptr[iter].key);
        }

        else if (strcmp(keyFormat, "lld") == 0 || strcmp(keyFormat, "lli") == 0)
        {
            ret = fscanf(f, "%lld", (long long*)heap_ptr[iter].key);
        }

        else
        {
            free(heap_ptr[iter].key);
            free(heap_ptr[iter].value);
            fclose(f);
            return MAXHEAP_FILE_ERROR_SPECIFIER;
        }

        if (ret != 1)
        {
            break;
        }

        if (strcmp(valueFormat, "d") == 0 || strcmp(valueFormat, "i") == 0)
        {
            ret = fscanf(f, " %d}", (int*)heap_ptr[iter].value);
        }

        else if (strcmp(valueFormat, "u") == 0)
        {
            ret = fscanf(f, " %u}", (unsigned int*)heap_ptr[iter].value);
        }

        else if (strcmp(valueFormat, "f") == 0)
        {
            ret = fscanf(f, " %f}", (float*)heap_ptr[iter].value);
        }

        else if (strcmp(valueFormat, "lf") == 0)
        {
            ret = fscanf(f, " %lf}", (double*)heap_ptr[iter].value);
        }

        else if (strcmp(valueFormat, "c") == 0)
        {
            ret = fscanf(f, " %c", (char*)heap_ptr[iter].value);
        }

        else if (strcmp(valueFormat, "s") == 0)
        {
            ret = fscanf(f, " %99[^}],", (char*)heap_ptr[iter].value);
        }

        else if (strcmp(valueFormat, "ld") == 0 || strcmp(valueFormat, "li") == 0)
        {
            ret = fscanf(f, "%ld", (long*)heap_ptr[iter].value);
        }

        else if (strcmp(valueFormat, "lld") == 0 || strcmp(valueFormat, "lli") == 0)
        {
            ret = fscanf(f, "%lld", (long long*)heap_ptr[iter].value);
        }

        else
        {
            free(heap_ptr[iter].key);
            free(heap_ptr[iter].value);
            fclose(f);
            return MAXHEAP_FILE_ERROR_SPECIFIER;
        }

        if (ret != 1)
        {
            break;
        }

        ++iter;
    }

    if (fclose(f) != 0)
    {
        return MAXHEAP_FILE_ERROR_CLOSE;
    }

    maxHeapBuildHeap(heap_ptr, heapCount, keySize, valueSize, compare);
    return MAXHEAP_FILE_SUCCESS;
}

int maxHeapWriteToFile(const maxheap_node_t heap[], const size_t heapSize, const char* file,
 const char* keyFormat, const char* valueFormat)
{
    assert(heap != nullptr);
    assert(heapSize > 0U);
    FILE *f = fopen(file, "w+");
    if (f == nullptr)
    {
        return MAXHEAP_FILE_ERROR_OPEN;
    }

    for (size_t iter = 0U; iter < heapSize; ++iter)
    {
        fprintf(f, "{");
        if (strcmp(keyFormat, "d") == 0 || strcmp(keyFormat, "i") == 0)
        {
            fprintf(f, "%d, ", *(int*)heap[iter].key);
        }

        else if (strcmp(keyFormat, "u") == 0)
        {
            fprintf(f, "%u, ", *(unsigned int*)heap[iter].key);
        }

        else if (strcmp(keyFormat, "f") == 0)
        {
            fprintf(f, "%f, ", *(float*)heap[iter].key);
        }

        else if (strcmp(keyFormat, "lf") == 0)
        {
            fprintf(f, "%lf, ", *(double*)heap[iter].key);
        }

        else if (strcmp(keyFormat, "c") == 0)
        {
            fprintf(f, "%c, ", *(char*)heap[iter].key);
        }

        else if (strcmp(keyFormat, "s") == 0)
        {
            fprintf(f, "%s, ", (char*)heap[iter].key);
        }

        else if (strcmp(keyFormat, "ld") == 0 || strcmp(keyFormat, "li") == 0)
        {
            fprintf(f, "%ld, ", *(long*)heap[iter].key);
        }

        else if (strcmp(keyFormat, "lld") == 0 || strcmp(keyFormat, "lli") == 0)
        {
            fprintf(f, "%lld, ", *(long long*)heap[iter].key);
        }

        else
        {
            fclose(f);
            return MAXHEAP_FILE_ERROR_SPECIFIER;
        }

        if (strcmp(valueFormat, "d") == 0 || strcmp(valueFormat, "i") == 0)
        {
            fprintf(f, "%d}", *(int*)heap[iter].value);
        }

        else if (strcmp(valueFormat, "u") == 0)
        {
            fprintf(f, "%u}", *(unsigned int*)heap[iter].value);
        }

        else if (strcmp(valueFormat, "f") == 0)
        {
            fprintf(f, "%f}", *(float*)heap[iter].value);
        }

        else if (strcmp(valueFormat, "lf") == 0)
        {
            fprintf(f, "%lf}", *(double*)heap[iter].value);
        }

        else if (strcmp(valueFormat, "c") == 0)
        {
            fprintf(f, "%c}", *(char*)heap[iter].value);
        }

        else if (strcmp(valueFormat, "s") == 0)
        {
            fprintf(f, "%s}", (char*)heap[iter].value);
        }

        else if (strcmp(valueFormat, "ld") == 0 || strcmp(valueFormat, "li") == 0)
        {
            fprintf(f, "%ld}", *(long*)heap[iter].value);
        }

        else if (strcmp(valueFormat, "lld") == 0 || strcmp(valueFormat, "lli") == 0)
        {
            fprintf(f, "%lld}", *(long long*)heap[iter].value);
        }

        else
        {
            fclose(f);
            return MAXHEAP_FILE_ERROR_SPECIFIER;
        }
        if (iter != heapSize - 1U)
        {
            fprintf(f, "\n");
        }
    }

    if (fclose(f) != 0)
    {
        return MAXHEAP_FILE_ERROR_CLOSE;
    }

    return MAXHEAP_FILE_SUCCESS;
}

size_t maxHeapGetParentPos(const size_t pos)
{
    assert(pos >= 1U);
    return (pos - 1U) / 2U;
}

size_t maxHeapGetLeftChildPos(const size_t pos)
{
    return 2U * pos + 1U;
}

size_t maxHeapGetRightChildPos(const size_t pos)
{
    return 2U * pos + 2U;
}