// Header that implements dynamic array

#include <stdlib.h>
#include <string.h>

#if !defined(ASSERT_OUT_OF_MEMORY)
#define ASSERT_OUT_OF_MEMORY true
#endif

#define INIT_DA_BUFFER_CAP 256

#define CONCAT2(A, B) A##B
#define CONCAT3(A, B, C) A##B##C

#define ListTypeName(ListElemType) CONCAT2(ListElemType, List)
#define ListTypeFunctionName(ListElemType, FUNC_NAME) CONCAT3(ListElemType, List, FUNC_NAME)

#define DefListType(ElType)                                                              \
    typedef struct                                                                       \
    {                                                                                    \
        ElType *buffer;                                                                  \
        size_t el_size;                                                                  \
        size_t size;                                                                     \
        size_t capacity;                                                                 \
        bool init;                                                                       \
    } ListTypeName(ElType);                                                              \
                                                                                         \
    void ListTypeFunctionName(ElType, _init)(ListTypeName(ElType) * list)                \
    {                                                                                    \
        list->el_size = sizeof(ElType);                                                  \
        list->capacity = INIT_DA_BUFFER_CAP;                                             \
        list->buffer = malloc(list->el_size * list->capacity);                           \
        if (list->buffer == NULL)                                                        \
        {                                                                                \
            assert(false && "Cannot init buffer. Get more RAM LOL!");                    \
        }                                                                                \
        list->size = 0;                                                                  \
        list->init = true;                                                               \
    }                                                                                    \
                                                                                         \
    void ListTypeFunctionName(ElType, _clear)(ListTypeName(ElType) * list)               \
    {                                                                                    \
        assert(list->buffer != NULL &&                                                   \
               list->init && "Using uninitialized list.");                               \
        list->size = 0;                                                                  \
    }                                                                                    \
                                                                                         \
    void ListTypeFunctionName(ElType, _push)(ListTypeName(ElType) * list, ElType el)     \
    {                                                                                    \
        assert(list->buffer != NULL &&                                                   \
               list->init && "Using uninitialized list.");                               \
                                                                                         \
        if (list->size < list->capacity)                                                 \
        {                                                                                \
            list->buffer[list->size] = el;                                               \
            list->size++;                                                                \
            return;                                                                      \
        }                                                                                \
                                                                                         \
        const size_t newCapacity = list->capacity * 2;                                   \
        const size_t newSizeInBytes = newCapacity * list->el_size;                       \
        ElType *newBuffer = realloc(list->buffer, newSizeInBytes);                       \
        if (newBuffer == NULL)                                                           \
        {                                                                                \
            assert(!ASSERT_OUT_OF_MEMORY && "Get more RAM LOL!");                        \
            printf("[ERROR] Failed to reallocate buffer. You need more RAM.");           \
        }                                                                                \
        list->buffer = newBuffer;                                                        \
        list->capacity = newCapacity;                                                    \
        list->buffer[list->size] = el;                                                   \
        list->size++;                                                                    \
    }                                                                                    \
                                                                                         \
    void ListTypeFunctionName(ElType, _erase)(ListTypeName(ElType) * list, size_t index) \
    {                                                                                    \
        assert(list->buffer != NULL &&                                                   \
               list->init && "Using uninitialized list.");                               \
        assert(index < list->size && "Out of range index");                              \
                                                                                         \
        if (index + 1 >= list->size)                                                     \
        {                                                                                \
            list->size--;                                                                \
            return;                                                                      \
        }                                                                                \
                                                                                         \
        ElType *copyDest = list->buffer + index;                                         \
        ElType *copySrc = list->buffer + index + 1;                                      \
        size_t elementsToCopy = list->size - index - 1;                                  \
                                                                                         \
        memcpy(copyDest, copySrc, elementsToCopy * list->el_size);                       \
                                                                                         \
        list->size--;                                                                    \
    }                                                                                    \
                                                                                         \
    void ListTypeFunctionName(ElType, _freeBuffer)(ListTypeName(ElType) * list)          \
    {                                                                                    \
        assert(list->buffer != NULL &&                                                   \
               list->init && "Using uninitialized list.");                               \
                                                                                         \
        free(list->buffer);                                                              \
        list->buffer = NULL;                                                             \
        list->size = 0;                                                                  \
        list->capacity = 0;                                                              \
    }
