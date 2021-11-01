#include <stdio.h>
#include "tokenizer.h"
#include "calculator.h"
#include <stdlib.h>
#include <assert.h>

char *readFile(char *input);

char *takeLine(int start, int end, const char *expression_string);

void allProcess(char *expression_string, int size, FILE *output);

int main() {
    FILE *output;
    fopen_s(&output, "..\\output.txt", "w");
    char *expression_string = readFile("..\\input.txt");
    int strLen = strlen(expression_string);
    int size = 0;
    int currentSize = 0;
    int numberOfEnter = 0;
    for (int i = 0; i < strLen; ++i) {
        if (expression_string[i] == '\n') {
            allProcess(takeLine(currentSize, size, expression_string), size, output);
            currentSize += size + 1;
            size = -1;
            numberOfEnter++;
        }
        size++;
    }
    allProcess(takeLine(currentSize, size - numberOfEnter, expression_string), size - numberOfEnter, output);
}

char *readFile(char *input) {
    FILE *currentF;
    fopen_s(&currentF, input, "r");
    assert(currentF);
    fseek(currentF, 0, SEEK_END);
    long length = ftell(currentF);
    fseek(currentF, 0, 0);
    char *buffer = (char *) malloc(length + 1);
    buffer[length] = '\0';
    fread(buffer, 1, length, currentF);
    fclose(currentF);
    return buffer;
}

void allProcess(char *expression_string, int size, FILE *output) {
    struct TokenArray tokenArray = tokenize(expression_string, size);
    if (tokenArray.tokens->operator == '\n') {
        fputc('\n', output);
    } else {
        double result = calculate(tokenArray);
        char tmp[256];
        sprintf(tmp, "%f", result);
        fprintf(output, "%s\n", tmp);
    }
}

char *takeLine(int start, int end, const char *expression_string) {
    char *line = (char *) malloc(end);
    for (int i = 0; i < end; ++i) {
        line[i] = expression_string[start];
        start++;
    }
    return line;
}
