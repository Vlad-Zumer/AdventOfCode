#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#define STREAM_BUFFER_SIZE 5

typedef struct
{
    FILE *fileHandle;
    bool isFileClosed;
    bool isEndOfStream;
    char buffer[STREAM_BUFFER_SIZE];
    size_t currentIndex;
    size_t lastValidIndex;
} FileReadStream;

char FRS_peek(FileReadStream *stream);
void FRS_advance(FileReadStream *stream);

FileReadStream FileReadStream_ctor(const char *filePath)
{
    FILE *fileHandle = fopen(filePath, "r");

    if (fileHandle == NULL)
    {
        printf("[ERROR] Cannot open file lol.\n");
        exit(1);
    }

    FileReadStream ret = {
        .fileHandle = fileHandle,
        .isEndOfStream = false,
        .currentIndex = 0,
        .lastValidIndex = 0};

    return ret;
}

char FRS_peek(FileReadStream *stream)
{
    assert(stream != NULL && "Stream cannot be null");
    return stream->buffer[stream->currentIndex];
}

void FRS_advance(FileReadStream *stream)
{
    assert(stream != NULL && "Stream cannot be null");
    if (stream->isEndOfStream)
    {
        // EOF already handled when reading from file
        return;
    }

    if (stream->currentIndex + 1 <= stream->lastValidIndex)
    {
        stream->currentIndex++;

        if (stream->currentIndex == stream->lastValidIndex && stream->isFileClosed)
        {
            // if the file is closed and this is the last position, it's the end of stream
            stream->isEndOfStream = true;
        }

        return;
    }

    // current index would go past the last valid entry
    // so we need to read some more

    // cache char in case we read 0 bytes
    char lastOne = FRS_peek(stream);

    // reset current index
    stream->currentIndex = 0;

    size_t bytesRead = fread(stream->buffer, sizeof(char), STREAM_BUFFER_SIZE, stream->fileHandle);
    stream->lastValidIndex = bytesRead - 1;

    if (bytesRead == STREAM_BUFFER_SIZE)
    {
        // read ok
        return;
    }

    if (bytesRead == 0)
    {
        // if we read 0 bytes, persist the last one
        stream->lastValidIndex = 0;
        stream->buffer[0] = lastOne;
        stream->isEndOfStream = true;
    }

    if (ferror(stream->fileHandle))
    {
        fclose(stream->fileHandle);
        printf("[ERROR] Could not read: %s", strerror(errno));
    }

    if (feof(stream->fileHandle))
    {
        fclose(stream->fileHandle);
        stream->isFileClosed = true;
    }
}