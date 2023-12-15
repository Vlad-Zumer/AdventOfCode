// Header that implements dynamic array

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#if !defined(INIT_DA_BUFFER_CAP)
#define INIT_DA_BUFFER_CAP 256
#endif

#define CONCAT2(A, B) A##B
#define CONCAT3(A, B, C) A##B##C

#define MakeListTypeNameFromItemType(ListElemType) CONCAT2(ListElemType, List)
#define MakeFunctionNameFromItemType(ListElemType, FUNC_NAME) CONCAT3(ListElemType, List, FUNC_NAME)
#define MakeFunctionNameFromListTypeName(ListTypeName, FUNC_NAME) CONCAT2(ListTypeName, FUNC_NAME)

#define DefListType(ElType) _DefListType(ElType, MakeListTypeNameFromItemType(ElType))
#define DefListTypeWithName(ElType, ListName) _DefListType(ElType, ListName)

#define _DefListType(ElType, ListTypeName)                                                         \
    typedef struct                                                                                 \
    {                                                                                              \
        ElType *buffer;                                                                            \
        size_t el_size;                                                                            \
        size_t size;                                                                               \
        size_t capacity;                                                                           \
        bool init;                                                                                 \
    } ListTypeName;                                                                                \
                                                                                                   \
    void MakeFunctionNameFromListTypeName(ListTypeName, _init)(ListTypeName * list)                \
    {                                                                                              \
        list->el_size = sizeof(ElType);                                                            \
        list->capacity = INIT_DA_BUFFER_CAP;                                                       \
        list->buffer = (ElType *)malloc(list->el_size * list->capacity);                           \
        if (list->buffer == NULL)                                                                  \
        {                                                                                          \
            assert(false && "Cannot init buffer. Get more RAM LOL!");                              \
        }                                                                                          \
        list->size = 0;                                                                            \
        list->init = true;                                                                         \
    }                                                                                              \
                                                                                                   \
    void MakeFunctionNameFromListTypeName(ListTypeName, _initz)(ListTypeName * list)               \
    {                                                                                              \
        list->el_size = sizeof(ElType);                                                            \
        list->capacity = INIT_DA_BUFFER_CAP;                                                       \
        list->buffer = (ElType *)calloc(list->capacity, list->el_size);                            \
        if (list->buffer == NULL)                                                                  \
        {                                                                                          \
            assert(false && "Cannot init buffer. Get more RAM LOL!");                              \
        }                                                                                          \
        list->size = 0;                                                                            \
        list->init = true;                                                                         \
    }                                                                                              \
                                                                                                   \
    void MakeFunctionNameFromListTypeName(ListTypeName, _clear)(ListTypeName * list)               \
    {                                                                                              \
        assert(list->buffer != NULL &&                                                             \
               list->init && "Using uninitialized list.");                                         \
        list->size = 0;                                                                            \
    }                                                                                              \
                                                                                                   \
    void MakeFunctionNameFromListTypeName(ListTypeName, _push)(ListTypeName * list, ElType el)     \
    {                                                                                              \
        assert(list->buffer != NULL &&                                                             \
               list->init && "Using uninitialized list.");                                         \
                                                                                                   \
        if (list->size < list->capacity)                                                           \
        {                                                                                          \
            list->buffer[list->size] = el;                                                         \
            list->size++;                                                                          \
            return;                                                                                \
        }                                                                                          \
                                                                                                   \
        const size_t newCapacity = list->capacity * 2;                                             \
        const size_t newSizeInBytes = newCapacity * list->el_size;                                 \
        ElType *newBuffer = realloc(list->buffer, newSizeInBytes);                                 \
        if (newBuffer == NULL)                                                                     \
        {                                                                                          \
            assert(false && "Failed to reallocate buffer. You need more RAM.");                    \
        }                                                                                          \
        list->buffer = newBuffer;                                                                  \
        list->capacity = newCapacity;                                                              \
        list->buffer[list->size] = el;                                                             \
        list->size++;                                                                              \
    }                                                                                              \
                                                                                                   \
    void MakeFunctionNameFromListTypeName(ListTypeName, _erase)(ListTypeName * list, size_t index) \
    {                                                                                              \
        assert(list->buffer != NULL &&                                                             \
               list->init && "Using uninitialized list.");                                         \
        assert(index < list->size && "Out of range index");                                        \
                                                                                                   \
        if (index + 1 >= list->size)                                                               \
        {                                                                                          \
            list->size--;                                                                          \
            return;                                                                                \
        }                                                                                          \
                                                                                                   \
        ElType *copyDest = list->buffer + index;                                                   \
        ElType *copySrc = list->buffer + index + 1;                                                \
        size_t elementsToCopy = list->size - index - 1;                                            \
                                                                                                   \
        memcpy(copyDest, copySrc, elementsToCopy * list->el_size);                                 \
                                                                                                   \
        list->size--;                                                                              \
    }                                                                                              \
                                                                                                   \
    void MakeFunctionNameFromListTypeName(ListTypeName, _freeBuffer)(ListTypeName * list)          \
    {                                                                                              \
        assert(list->buffer != NULL &&                                                             \
               list->init && "Using uninitialized list.");                                         \
                                                                                                   \
        free(list->buffer);                                                                        \
        list->buffer = NULL;                                                                       \
        list->init = false;                                                                        \
        list->size = 0;                                                                            \
        list->capacity = 0;                                                                        \
    }
