#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "c-dynamic-array.h"

#if defined(NO_HASH_FUNC_MAX)
    #define HASH_FUNC_MAX 0
#else
    #if !defined(HASH_FUNC_MAX)
        #define HASH_FUNC_MAX 100000
    #endif
#endif

#if !defined(HASH_FUNC)
    unsigned long hash_dhb2(unsigned char *str)
    {
        unsigned long hash = 5381;
        while ((*str) != '\0')
        {
            int c = *str;
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
            str++;
        }
        return (HASH_FUNC_MAX > 1) ? hash % HASH_FUNC_MAX : hash;
    }
    #define HASH_FUNC hash_dhb2
#endif

#define MapBucketType(MapName) CONCAT2(MapName, Bucket)
#define MapBucketTypeFunc(MapName, FUNC) CONCAT3(MapName, Bucket, FUNC)
#define MapBufferTypeElement(MapName) CONCAT2(MapName, Elem)
#define MapBufferTypeName(MapName) CONCAT2(MapName, BuffType)
#define MapBufferTypeFunc(MapName, FUNC) CONCAT3(MapName, BuffType, FUNC)
#define MapFunctionNameFromMapName(MapName, FUNC_NAME) CONCAT2(MapName, FUNC_NAME)

#define DefaultEqFuncName(Type) Eq_##Type

#define DefaultEqFunc(Type)                        \
    bool DefaultEqFuncName(Type)(Type t1, Type t2) \
    {                                              \
        return t1 == t2;                           \
    }

#define DefaultToHashStrFuncName(Type) hashStr_##Type

#define DefaultToHashStrFunc(Type)                                     \
    char *DefaultToHashStrFuncName(Type)(Type t1)                      \
    {                                                                  \
        char *str = (char *)malloc(sizeof(char) * (sizeof(Type) + 1)); \
        if (str == NULL)                                               \
        {                                                              \
            assert(false && "Out of memory. Get more RAM!\n");         \
        }                                                              \
        str[sizeof(Type)] = '\0';                                      \
        for (size_t i = 0; i < sizeof(Type); i++)                      \
        {                                                              \
            str[i] = ((t1 >> (i * 8)) & 255);                          \
        }                                                              \
                                                                       \
        return str;                                                    \
    }

#define DefaultKeyHashFuncName(Type) hashFunc_##Type

#define DefaultKeyHashFunc(Type)                                   \
    DefaultToHashStrFunc(Type)                                     \
    unsigned long DefaultKeyHashFuncName(Type)(Type t1)            \
    {                                                              \
        char *str = DefaultToHashStrFuncName(Type)(t1);            \
        unsigned long hash = HASH_FUNC((unsigned char *)str);      \
        free(str);                                                 \
        return hash;                                               \
    }

#define KeyHashFunc(Type, MakeKeyToStr)                       \
    unsigned long DefaultKeyHashFuncName(Type)(Type t1)       \
    {                                                         \
        char *str = MakeKeyToStr(t1);                         \
        unsigned long hash = HASH_FUNC((unsigned char *)str); \
        free(str);                                            \
        return hash;                                          \
    }

#define DefDefaultImplHashMapType(KeyType, ValueType, Name) \
    DefHashMapType(KeyType, DefaultKeyHashFuncName(KeyType), DefaultEqFuncName(KeyType), ValueType, Name)

#define DefHashMapType(KeyType, KeyHashFunc, KeyEqFunc, ValueType, Name)                                     \
    typedef struct                                                                                           \
    {                                                                                                        \
        KeyType key;                                                                                         \
        ValueType value;                                                                                     \
    } MapBufferTypeElement(Name);                                                                            \
                                                                                                             \
    DefListTypeWithName(MapBufferTypeElement(Name), MapBucketType(Name))                                     \
    DefListTypeWithName(MapBucketType(Name) *, MapBufferTypeName(Name))                                      \
    typedef struct                                                                                           \
    {                                                                                                        \
        MapBufferTypeName(Name) buckets;                                                                     \
    } Name;                                                                                                  \
                                                                                                             \
    void MapFunctionNameFromMapName(Name, _init)(Name * map)                                                 \
    {                                                                                                        \
        MapBufferTypeFunc(Name, _init)(&map->buckets);                                                       \
    }                                                                                                        \
                                                                                                             \
    void MapFunctionNameFromMapName(Name, _initz)(Name * map)                                                \
    {                                                                                                        \
        MapBufferTypeFunc(Name, _initz)(&map->buckets);                                                      \
    }                                                                                                        \
                                                                                                             \
    void MapFunctionNameFromMapName(Name, _add)(Name * map, KeyType key, ValueType value)                    \
    {                                                                                                        \
        unsigned long hash = KeyHashFunc(key);                                                               \
        while (map->buckets.size <= hash)                                                                    \
        {                                                                                                    \
            MapBufferTypeFunc(Name, _push)(&(map->buckets), NULL);                                           \
        }                                                                                                    \
                                                                                                             \
        if (map->buckets.buffer[hash] == NULL)                                                               \
        {                                                                                                    \
            MapBucketType(Name) *newBucket = (MapBucketType(Name) *)malloc(1 * sizeof(MapBucketType(Name))); \
            MapBucketTypeFunc(Name, _init)(newBucket);                                                       \
            map->buckets.buffer[hash] = newBucket;                                                           \
        }                                                                                                    \
                                                                                                             \
        bool replaced = false;                                                                               \
        for (size_t i = 0; i < map->buckets.buffer[hash]->size; i++)                                         \
        {                                                                                                    \
            if (KeyEqFunc(map->buckets.buffer[hash]->buffer[i].key, key))                                    \
            {                                                                                                \
                map->buckets.buffer[hash]->buffer[i].value = value;                                          \
                replaced = true;                                                                             \
                break;                                                                                       \
            }                                                                                                \
        }                                                                                                    \
                                                                                                             \
        if (!replaced)                                                                                       \
        {                                                                                                    \
            MapBufferTypeElement(Name) el = {0};                                                             \
            el.key = key;                                                                                    \
            el.value = value;                                                                                \
            MapBucketTypeFunc(Name, _push)(map->buckets.buffer[hash], el);                                   \
        }                                                                                                    \
    }                                                                                                        \
                                                                                                             \
    bool MapFunctionNameFromMapName(Name, _get)(Name * map, KeyType key, ValueType * found)                  \
    {                                                                                                        \
        assert(found != NULL && "No place to put the found value");                                          \
        unsigned long hash = KeyHashFunc(key);                                                               \
        if (map->buckets.size <= hash || map->buckets.buffer[hash] == NULL)                                  \
        {                                                                                                    \
            return false;                                                                                    \
        }                                                                                                    \
                                                                                                             \
        for (size_t i = 0; i < map->buckets.buffer[hash]->size; i++)                                         \
        {                                                                                                    \
            if (KeyEqFunc(map->buckets.buffer[hash]->buffer[i].key, key))                                    \
            {                                                                                                \
                *found = map->buckets.buffer[hash]->buffer[i].value;                                         \
                return true;                                                                                 \
            }                                                                                                \
        }                                                                                                    \
                                                                                                             \
        return false;                                                                                        \
    }                                                                                                        \
                                                                                                             \
    bool MapFunctionNameFromMapName(Name, _hasKey)(Name * map, KeyType key)                                  \
    {                                                                                                        \
        unsigned long hash = KeyHashFunc(key);                                                               \
        if (map->buckets.size <= hash || map->buckets.buffer[hash] == NULL)                                  \
        {                                                                                                    \
            return false;                                                                                    \
        }                                                                                                    \
                                                                                                             \
        for (size_t i = 0; i < map->buckets.buffer[hash]->size; i++)                                         \
        {                                                                                                    \
            if (KeyEqFunc(map->buckets.buffer[hash]->buffer[i].key, key))                                    \
            {                                                                                                \
                return true;                                                                                 \
            }                                                                                                \
        }                                                                                                    \
        return false;                                                                                        \
    }                                                                                                        \
                                                                                                             \
    void MapFunctionNameFromMapName(Name, _remove)(Name * map, KeyType key)                                  \
    {                                                                                                        \
        unsigned long hash = KeyHashFunc(key);                                                               \
        if (map->buckets.size <= hash || map->buckets.buffer[hash] == NULL)                                  \
        {                                                                                                    \
            return;                                                                                          \
        }                                                                                                    \
                                                                                                             \
        for (size_t i = 0; i < map->buckets.buffer[hash]->size; i++)                                         \
        {                                                                                                    \
            if (KeyEqFunc(map->buckets.buffer[hash]->buffer[i].key, key))                                    \
            {                                                                                                \
                MapBucketTypeFunc(Name, _erase)(map->buckets.buffer[hash], i);                               \
                return;                                                                                      \
            }                                                                                                \
        }                                                                                                    \
    }                                                                                                        \
                                                                                                             \
    void MapFunctionNameFromMapName(Name, _freeBuffers)(Name * map)                                          \
    {                                                                                                        \
        for (size_t bucketIndex = 0; bucketIndex < map->buckets.size; bucketIndex++)                         \
        {                                                                                                    \
            if (map->buckets.buffer[bucketIndex] == NULL)                                                    \
            {                                                                                                \
                continue;                                                                                    \
            }                                                                                                \
            MapBucketTypeFunc(Name, _freeBuffer)(map->buckets.buffer[bucketIndex]);                          \
            free(map->buckets.buffer[bucketIndex]);                                                          \
            map->buckets.buffer[bucketIndex] = NULL;                                                         \
        }                                                                                                    \
        MapBufferTypeFunc(Name, _freeBuffer)(&(map->buckets));                                               \
    }

// DefaultEqFunc(int)
// DefaultKeyHashFunc(int)
// DefDefaultImplHashMapType(int, int, IntToInt)
// DefHashMapType(int, DefaultKeyHashFuncName(int), DefaultEqFuncName(int), int, IntToIntMap)
