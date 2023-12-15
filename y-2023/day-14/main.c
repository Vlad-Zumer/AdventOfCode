#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define INIT_DA_BUFFER_CAP 101
#include "../../utils/c-dynamic-array.h"
#include "../../utils/c-hash-map.h"

typedef unsigned long long ull;
typedef unsigned long ul;

DefListType(char)
DefListTypeWithName(charList, CharMatrix)

typedef CharMatrix *CharMatrixPtr;

char *MatrixToStr(const CharMatrix *matrix);
KeyHashFunc(CharMatrixPtr, MatrixToStr) 
bool eqMatrix(const CharMatrix *m1, const CharMatrix *m2);
DefHashMapType(CharMatrixPtr, DefaultKeyHashFuncName(CharMatrixPtr), eqMatrix, int, MatToIndexMap)

void cleanUpMatrix(CharMatrix *matrix)
{
    // printf("[Info] Cleaning Matrix at: 0x%llX\n", (ull)matrix);
    // printf("[Info] +-> Cleaning Matrix buffer at: 0x%llX\n", (ull)matrix->buffer);
    for (size_t i = 0; i < matrix->size; i++)
    {
        charList_freeBuffer(&(matrix->buffer[i]));
    }

    CharMatrix_freeBuffer(matrix);
}

void parseInput(const char *filePath, CharMatrix *inputMatrix)
{
    printf("[Info] Using file %s\n", filePath);

    FILE *fileHandle = fopen(filePath, "r");

    // based on input 128 should be enough to hold the entire line
    char buffer[128] = {0};

    size_t line = -1;
    while (fgets(buffer, 128, fileHandle) != NULL)
    {
        charList *list = malloc(1 * sizeof(charList));
        charList_initz(list);
        CharMatrix_push(inputMatrix, *list);
        line++;
        for (size_t i = 0; buffer[i] != '\0' && buffer[i] != '\n'; i++)
        {
            charList_push(&(inputMatrix->buffer[line]), buffer[i]);
        }
    }

    if (ferror(fileHandle))
    {
        printf("[ERROR] Fucked up reading: %s", strerror(errno));
        fclose(fileHandle);
        exit(1);
    }

    if (!feof(fileHandle))
    {
        printf("[ERROR] Fucked up reading but didn't find an error. Honestly I don't know how.");
        fclose(fileHandle);
        exit(1);
    }

    fclose(fileHandle);
}

void printMatrix(CharMatrix *matrix)
{
    for (size_t i = 0; i < matrix->size; i++)
    {
        char buffer[128] = {0};
        for (size_t j = 0; j < matrix->buffer[i].size; j++)
        {
            buffer[j] = matrix->buffer[i].buffer[j];
        }
        printf("%s\n", buffer);
    }
}

char *MatrixToStr(const CharMatrix *matrix)
{
    charList buffer = {0};
    charList_init(&buffer);
    for (size_t i = 0; i < matrix->size; i++)
    {
        for (size_t j = 0; j < matrix->buffer[i].size; j++)
        {
            charList_push(&buffer, matrix->buffer[i].buffer[j]);
        }
        charList_push(&buffer, '\n');
    }
    char *ret = (char *)calloc(buffer.size + 1, sizeof(char));

    if (ret == NULL)
    {
        printf("[ERROR] Not enough memory\n");
        assert(false && "Not enough memory");
    }

    memcpy(ret, buffer.buffer, buffer.size);

    charList_freeBuffer(&buffer);

    return ret;
}

void copyBuffsMatrix(const CharMatrix *matrix, CharMatrix *clone)
{
    for (size_t i = 0; i < matrix->size; i++)
    {
        for (size_t j = 0; j < matrix->buffer[i].size; j++)
        {
            clone->buffer[i].buffer[j] = matrix->buffer[i].buffer[j];
        }
    }
}

void cloneMatrix(const CharMatrix *matrix, CharMatrix *clone)
{
    assert(clone != NULL && "Clone cannot be null");
    CharMatrix_initz(clone);

    for (size_t i = 0; i < matrix->size; i++)
    {
        charList *list = malloc(1 * sizeof(charList));
        charList_initz(list);
        CharMatrix_push(clone, *list);
        for (size_t j = 0; j < matrix->buffer[i].size; j++)
        {
            charList_push(&(clone->buffer[i]), matrix->buffer[i].buffer[j]);
        }
    }
}

bool eqMatrix(const CharMatrix *m1, const CharMatrix *m2)
{
    if (m1->size != m2->size)
    {
        return false;
    }

    for (size_t i = 0; i < m1->size; i++)
    {
        if (m1->buffer[i].size != m2->buffer[i].size)
        {
            return false;
        }
        for (size_t j = 0; j < m1->buffer[i].size; j++)
        {
            if (m1->buffer[i].buffer[j] != m2->buffer[i].buffer[j])
            {
                return false;
            }
        }
    }

    return true;
}

void moveRoundedRocksVertically(CharMatrix *matrix, bool top)
{
    if (!(matrix->size > 1))
    {
        // nothing to do for one line
        return;
    }

    int change = top ? 1 : -1;
    size_t initLine = top ? 0 : matrix->size - 1;
    size_t lastLine = top ? matrix->size - 1 : 0;

    // skip the first line
    for (size_t i = initLine + change; i != (lastLine + change); i += change)
    {
        for (size_t j = 0; j < matrix->buffer[i].size; j++)
        {
            size_t k = i;
            // printf("(i/k:%llu, j:%llu) = %c, (k-d:%llu, j:%llu) = %c\n", i, j, matrix->buffer[k].buffer[j], k-change, j, matrix->buffer[k-change].buffer[j]);
            while (k != initLine && matrix->buffer[k].buffer[j] == 'O' && matrix->buffer[k - change].buffer[j] == '.')
            {
                matrix->buffer[k].buffer[j] = '.';
                matrix->buffer[k - change].buffer[j] = 'O';
                k -= change;
            }
        }
    }
}

void moveRoundedRocksHorizontally(CharMatrix *matrix, bool left)
{
    if (!(matrix->size > 0) || !(matrix->buffer[0].size > 1))
    {
        // nothing to do for one line
        return;
    }

    int change = left ? 1 : -1;
    size_t initLine = left ? 0 : matrix->buffer[0].size - 1;
    size_t lastLine = left ? matrix->buffer[0].size - 1 : 0;

    for (size_t i = 0; i < matrix->size; i++)
    {
        // skip the first col
        for (size_t j = initLine + change; j != (lastLine + change); j += change)
        {
            size_t k = j;
            // printf("(i:%llu, j/k:%llu) = %c, (i:%llu, k-d:%llu) = %c\n", i, j, matrix->buffer[i].buffer[k], i, k-change, matrix->buffer[i].buffer[k - change]);
            while (k != initLine && matrix->buffer[i].buffer[k] == 'O' && matrix->buffer[i].buffer[k - change] == '.')
            {
                matrix->buffer[i].buffer[k] = '.';
                matrix->buffer[i].buffer[k - change] = 'O';
                k -= change;
            }
        }
    }
}

void doCycle(CharMatrix *matrix)
{
    moveRoundedRocksVertically(matrix, true);
    moveRoundedRocksHorizontally(matrix, true);
    moveRoundedRocksVertically(matrix, false);
    moveRoundedRocksHorizontally(matrix, false);
}

ull calcLoadVertical(const CharMatrix *matrix, bool top)
{
    ull sum = 0;
    size_t change = top ? 1 : -1;
    size_t initLine = top ? 0 : matrix->size;
    size_t lastLine = top ? matrix->size - 1 : 0;

    // skip the first line
    for (size_t i = initLine; i != (lastLine + change); i += change)
    {
        for (size_t j = 0; j < matrix->buffer[i].size; j++)
        {
            if (matrix->buffer[i].buffer[j] == 'O')
            {
                sum += (lastLine / change) - (i / change) + 1;
            }
        }
    }

    return sum;
}

ull Part1(const CharMatrix *matrix)
{
    CharMatrix clone = {0};
    cloneMatrix(matrix, &clone);
    moveRoundedRocksVertically(&clone, true);
    // printf("[Info] [P1] Clone address: %llX\n", (ull)&clone);
    // printf("[Info] [P1] Clone buffer address: %llX\n", (ull)clone.buffer);
    ull p1 = calcLoadVertical(&clone, true);
    cleanUpMatrix(&clone);
    return p1;
}

ull Part2(const CharMatrix *matrix)
{
    MatToIndexMap map = {0};
    MatToIndexMap_initz(&map);

    CharMatrix clone = {0};
    cloneMatrix(matrix, &clone);

    // printf("[Info] [P2] Clone address: %llX\n", (ull)&clone);
    // printf("[Info] [P2] Clone buffer address: %llX\n", (ull)clone.buffer);
    {
        CharMatrix *newClone = malloc(sizeof(CharMatrix));
        assert(newClone != NULL && "Out of memory");
        cloneMatrix(&clone, newClone);

        MatToIndexMap_add(&map, newClone, 0);
    }

    int cycles = 1000000000;

    int firstCycleStartIndex = 0;
    int secondCycleStartIndex = 0;

    for (int i = 1; i <= cycles; i++)
    {
        doCycle(&clone);

        int foundPrevIndex = 0;
        if (MatToIndexMap_get(&map, &clone, &foundPrevIndex))
        {
            firstCycleStartIndex = foundPrevIndex;
            secondCycleStartIndex = i;
            break;
        }

        CharMatrix *newClone = malloc(sizeof(CharMatrix));
        assert(newClone != NULL && "Out of memory");
        cloneMatrix(&clone, newClone);

        MatToIndexMap_add(&map, newClone, i);
    }

    if (firstCycleStartIndex != 0 && secondCycleStartIndex != 0)
    {
        int finalCycle = (cycles - secondCycleStartIndex) % (secondCycleStartIndex - firstCycleStartIndex) + firstCycleStartIndex;
        int finalCycleDelta = finalCycle - firstCycleStartIndex;
        for (int i = 0; i < finalCycleDelta; i++)
        {
            doCycle(&clone);
        }
    }

    ull p2 = calcLoadVertical(&clone, true);

    // clean up the map
    for (size_t i = 0; i < map.buckets.size; i++)
    {
        if (map.buckets.buffer[i] == NULL)
        {
            continue;
        }

        for (size_t j = 0; j < map.buckets.buffer[i]->size; j++)
        {
            cleanUpMatrix(map.buckets.buffer[i]->buffer[j].key);
            free(map.buckets.buffer[i]->buffer[j].key);
            map.buckets.buffer[i]->buffer[j].key = NULL;
        }
    }

    MatToIndexMap_freeBuffers(&map);
    cleanUpMatrix(&clone);

    return p2;
}

// run with "gcc main.c -o main.exe --ggdb3 -Wall -Wpedantic -Werror -Wextra -std=c99 && ./main.exe"
int main()
{

    CharMatrix matrix = {0};
    CharMatrix_initz(&matrix);

    // const char *filePath = "./test.txt";
    const char *filePath = "./input.txt";
    parseInput(filePath, &matrix);

    ull p1 = Part1(&matrix);
    printf("P1: %llu\n", p1);
    ull p2 = Part2(&matrix);
    printf("P2: %llu\n", p2);

    cleanUpMatrix(&matrix);
    return 0;
}
