#include <stdio.h>
#include "tokenizer.h"
#include "calculator.h"
#include <stdlib.h>
#include <assert.h>

char *readFile(char *input);

char *takeLine(unsigned int start, unsigned int end, const char *expression_string);

void calculateExpressionString(char *expression_string, unsigned int size, FILE *outputFile);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("No path to input text was provided. Provide path as the first argument, like `./calculator path/to/input.txt`.");
        return 0;
    }
    char *inputFileName = argv[1];
    FILE *outputFile = fopen("output.txt", "w");
    char *fileContent = readFile(inputFileName);
    unsigned int fileContentLength = strlen(fileContent);
    int expressionStartIndex = 0;
    for (int i = 0; i < fileContentLength; ++i) {
        char currentChar = fileContent[i];
        if (currentChar == '\n') {
            calculateExpressionString(takeLine(expressionStartIndex, i, fileContent), i - expressionStartIndex,
                                      outputFile);
            expressionStartIndex = i + 1;
        }

    }

    if (expressionStartIndex < fileContentLength) {
        calculateExpressionString(takeLine(expressionStartIndex, fileContentLength, fileContent),
                                  fileContentLength - expressionStartIndex, outputFile);
    }
}

char *readFile(char *input) {
    char *source = NULL;
    FILE *file = fopen(input, "r");
    assert(file);
    if (fseek(file, 0L, SEEK_END) == 0) {
        long sizeOfFile = ftell(file);
        source = malloc(sizeof(char) * (sizeOfFile + 1));
        fseek(file, 0L, SEEK_SET);
        size_t newLen = fread(source, sizeof(char), sizeOfFile, file);
        if (ferror(file) != 0) {
            fputs("Error reading file", stderr);
        } else {
            source[newLen] = '\0';
        }
    }
    fclose(file);
    return source;
}

void calculateExpressionString(char *expression_string, unsigned int size, FILE *outputFile) {
    struct TokenArray tokenArray = tokenize(expression_string, size);
    double result = calculateExpressionTokens(tokenArray);
    fprintf(outputFile, "%f\n", result);
}

char *takeLine(unsigned int start, unsigned int end, const char *expression_string) {
    char *line = (char *) malloc(end);
    for (int i = 0; i < end - start; ++i) {
        line[i] = expression_string[start + i];
    }
    return line;
}