#ifndef UTIL_H
#define UTIL_H
#include <assert.h>

char *getSubString(unsigned int start, unsigned int end, const char *expression_string)
{
    char *line = malloc((end - start + 1) * sizeof(char));
    for (int i = 0; i < end - start; ++i)
    {
        line[i] = expression_string[start + i];
    }
    line[end - start] = '\0';
    return line;
}

char *readFile(char *input)
{
    char *source = NULL;
    FILE *file = fopen(input, "r");
    assert(file);
    size_t newLen;
    if (fseek(file, 0L, SEEK_END) == 0)
    {
        long sizeOfFile = ftell(file);
        source = malloc(sizeof(char) * (sizeOfFile + 1));
        fseek(file, 0L, SEEK_SET);
        newLen = fread(source, sizeof(char), sizeOfFile, file);
        if (ferror(file) != 0)
        {
            fputs("Error reading file", stderr);
        }
        else
        {
            source[newLen] = '\0';
        }
    }
    fclose(file);
    return source;
}
#endif