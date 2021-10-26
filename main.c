#include <stdio.h>
#include "tokenizer.h"
#include "calculator.h"

char *readInput() {
#define CHUNK 200
    char *input = NULL;
    char tempBuf[CHUNK];
    size_t inputLen = 0, tempLen;
    do {
        fgets(tempBuf, CHUNK, stdin);
        tempLen = strlen(tempBuf);
        input = realloc(input, inputLen + tempLen + 1);
        strcpy(input + inputLen, tempBuf);
        inputLen += tempLen;
    } while (tempLen == CHUNK - 1 && tempBuf[CHUNK - 2] != '\n');
    return input;
}

int main() {
    char *expression_string = readInput();
    unsigned int size = strlen(expression_string);
    struct TokenArray tokenArray = tokenize(expression_string, size - 1);
    double result = calculate(tokenArray);
    char tmp[255];
    sprintf(tmp, "%f", result);
    printf("%s", tmp);
    free(expression_string);
}
