#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "FileStream.h"
#include "DynamicArray.h"
#include "AABB.h"

// Think of the input as squares formed from vertices
//
// 0   1   2   3   4   5   6   7   8   9   10
//------------------------------------------- 0
// | 4 | 6 | 7 | . | . | 1 | 1 | 4 | . | . |
//------------------------------------------- 1
// | . | . | . | * | . | . | . | . | . | . |
//------------------------------------------- 2
// | . | . | 3 | 5 | . | . | 6 | 3 | 3 | . |
//------------------------------------------- 3
// | . | . | . | . | . | . | # | . | . | . |
//------------------------------------------- 4
// | 6 | 1 | 7 | * | . | . | . | . | . | . |
//------------------------------------------- 5
// | . | . | . | . | . | + | . | 5 | 8 | . |
//------------------------------------------- 6
// | . | . | 5 | 9 | 2 | . | . | . | . | . |
//------------------------------------------- 7
// | . | . | . | . | . | . | 7 | 5 | 5 | . |
//------------------------------------------- 8
// | . | . | . | $ | . | * | . | . | . | . |
//------------------------------------------- 9
// | . | 6 | 6 | 4 | . | 5 | 9 | 8 | . | . |
//------------------------------------------- 10
//
// in this example number 35 occupies the rectangle
//   formed by the vertices (2,2) & (4,3)
//
// in this example the first symbol * occupies the rectangle
//   formed by the vertices (3,1) & (4,2), but extends to 1 around itself so in the end is (2,0) & (5,3)

typedef struct
{
    int number;
    Rect box;
} NumberBox;

typedef struct
{
    char symbol;
    Rect box;
} SymbolBox;

DefListType(NumberBox);
DefListType(SymbolBox);

void parseInput(const char *filePath, NumberBoxList *numberBoxes, SymbolBoxList *symbolBoxes)
{
    printf("[INFO] Using file %s\n", filePath);

    FileReadStream stream = FileReadStream_ctor(filePath);
    FRS_advance(&stream);
    // current position in file matrix
    int x = 0, y = 0;
    while (!stream.isEndOfStream)
    {
        char c = FRS_peek(&stream);
        if (c == '.')
        {
            x++;
        }
        else if (isdigit(c))
        {
            // continue parsing until not a digit anymore
            // only horizontal numbers
            const int startBoxX = x, startBoxY = y, endBoxY = y + 1;
            int number = 0;

            while (isdigit(c))
            {
                x++;
                number = number * 10 + (c - '0');
                FRS_advance(&stream);
                c = FRS_peek(&stream);
            }

            const int endBoxX = x;
            // printf("Num:%-3d @ (X:%-2d , Y:%-2d) -> (X:%-2d , Y:%-2d)\n", number, startBoxX, startBoxY, endBoxX, endBoxY);

            NumberBox box = {
                .number = number,
                .box = {
                    .minX = startBoxX,
                    .minY = startBoxY,
                    .maxX = endBoxX,
                    .maxY = endBoxY}};

            NumberBoxList_push(numberBoxes, box);

            // skip advancing
            continue;
        }
        else if (c == '\n')
        {
            x = 0;
            y++;
        }
        else
        {
            const int startBoxX = x - 1, startBoxY = y - 1, endBoxX = x + 2, endBoxY = y + 2;
            // printf("Symbol:%-3c @ (X:%-2d , Y:%-2d) -> (X:%-2d , Y:%-2d)\n", c, startBoxX, startBoxY, endBoxX, endBoxY);

            SymbolBox box = {
                .symbol = c,
                .box = {
                    .minX = startBoxX,
                    .minY = startBoxY,
                    .maxX = endBoxX,
                    .maxY = endBoxY}};

            // printf("init:%d el_size:%d cap:%d, size:%d\n",symbolBoxes->init, symbolBoxes->el_size, symbolBoxes->capacity, symbolBoxes->size);

            SymbolBoxList_push(symbolBoxes, box);
            // printf("added symbol { sym:%c, sx:%-4d, sy:%-4d, ex:%-4d, ey:%-4d } \n", box.symbol, box.box.minX, box.box.minY, box.box.maxX, box.box.maxY);

            x++;
            // symbol
        }
        FRS_advance(&stream);
    }
    printf("[INFO] Done reading file.\n");
}

void printLists(NumberBoxList *numberBoxes, SymbolBoxList *symbolBoxes)
{
    for (size_t i = 0; i < numberBoxes->size; i++)
    {
        const NumberBox numBox = numberBoxes->buffer[i];
        printf("NumberBox { num:%-4d, sx:%-4d, sy:%-4d, ex:%-4d, ey:%-4d }\n",
               numBox.number, numBox.box.minX, numBox.box.minY, numBox.box.maxX, numBox.box.maxY);
    }

    for (size_t i = 0; i < symbolBoxes->size; i++)
    {
        const SymbolBox symBox = symbolBoxes->buffer[i];
        printf("SymbolBox { sym:%c, sx:%-4d, sy:%-4d, ex:%-4d, ey:%-4d }\n",
               symBox.symbol, symBox.box.minX, symBox.box.minY, symBox.box.maxX, symBox.box.maxY);
    }
}

void part1(NumberBoxList *numberBoxes, SymbolBoxList *symbolBoxes)
{
    unsigned long long sum = 0;
    for (size_t i = 0; i < numberBoxes->size; i++)
    {
        const NumberBox numBox = numberBoxes->buffer[i];
        for (size_t i = 0; i < symbolBoxes->size; i++)
        {
            const SymbolBox symBox = symbolBoxes->buffer[i];

            if (intersect_rect(numBox.box, symBox.box, IntersectExclusive))
            {
                // printf("Found collision\n");
                // printf("  NumberBox { num:%-4d, sx:%-4d, sy:%-4d, ex:%-4d, ey:%-4d }\n",
                //        numBox.number, numBox.box.minX, numBox.box.minY, numBox.box.maxX, numBox.box.maxY);
                // printf("  SymbolBox { sym:%-4c, sx:%-4d, sy:%-4d, ex:%-4d, ey:%-4d }\n",
                //        symBox.symbol, symBox.box.minX, symBox.box.minY, symBox.box.maxX, symBox.box.maxY);

                // printf("----------------------------------\n");
                sum += numBox.number;
                // only add the number once
                break;
            }
        }
    }

    printf("p1:%u\n", sum);
}

void part2(NumberBoxList *numberBoxes, SymbolBoxList *symbolBoxes)
{
    unsigned long long sum = 0;
    for (size_t i = 0; i < symbolBoxes->size; i++)
    {
        const SymbolBox symBox = symbolBoxes->buffer[i];

        if (symBox.symbol != '*')
        {
            // skip non-gears
            continue;
        }

        int numOfIntersections = 0;
        unsigned long ratio = 1;
        for (size_t i = 0; i < numberBoxes->size; i++)
        {
            const NumberBox numBox = numberBoxes->buffer[i];

            if (intersect_rect(numBox.box, symBox.box, IntersectExclusive))
            {
                numOfIntersections++;
                ratio *= numBox.number;
                // printf("Found collision\n");
                // printf("  NumberBox { num:%-4d, sx:%-4d, sy:%-4d, ex:%-4d, ey:%-4d }\n",
                //        numBox.number, numBox.box.minX, numBox.box.minY, numBox.box.maxX, numBox.box.maxY);
                // printf("  SymbolBox { sym:%-4c, sx:%-4d, sy:%-4d, ex:%-4d, ey:%-4d }\n",
                //        symBox.symbol, symBox.box.minX, symBox.box.minY, symBox.box.maxX, symBox.box.maxY);

                // printf("----------------------------------\n");
            }
            if (numOfIntersections > 2)
            {
                break;
            }
        }

        if (numOfIntersections == 2)
        {
            sum += ratio;
        }
    }

    printf("p2:%u\n", sum);
}

// run with "gcc main.c -o main.exe -std=c99 && ./main.exe"
int main(int argc, char *argv[])
{
    NumberBoxList numberBoxes;
    NumberBoxList_init(&numberBoxes);
    SymbolBoxList symbolBoxes;
    SymbolBoxList_init(&symbolBoxes);

    // const char* filePath = "./test.txt";
    const char *filePath = "./input.txt";
    parseInput(filePath, &numberBoxes, &symbolBoxes);

    // printLists(&numberBoxes, &symbolBoxes);

    part1(&numberBoxes, &symbolBoxes);
    part2(&numberBoxes, &symbolBoxes);

    NumberBoxList_freeBuffer(&numberBoxes);
    SymbolBoxList_freeBuffer(&symbolBoxes);

    return 0;
}
