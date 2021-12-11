#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <mysql.h>
#include "tokenizer.h"
#include "calculator.h"
#include "util.h"
#include "db.h"
#include "conf.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    { 
        printf("usage: calculator /path/to/input.txt /path/to/db.conf");
        return 0;
    }
    char *dbdata = readFile(argv[2]);
    Conf conf = reedConfig(dbdata);
    MYSQL *connection = createDB(conf);
    int length_of_insert_result = strlen(insert_result);
    int length_of_update_result = strlen(update_result);
    char *inputFileName = argv[1];
    FILE *outputFile = fopen("output.txt", "w");
    char *fileContent = readFile(inputFileName);
    unsigned int fileContentLength = strlen(fileContent);
    int expressionStartIndex = 0;
    int i;
    for (i = 0; i < fileContentLength; ++i)
    {
        char currentChar = fileContent[i];
        if (currentChar == '\n')
        {
            insertExpresionIntoDB(connection, insert_result, fileContent,
                                  length_of_insert_result, expressionStartIndex, i);
            expressionStartIndex = i + 1;
        }
    }
    if (expressionStartIndex < fileContentLength)
    {
        insertExpresionIntoDB(connection, insert_result, fileContent,
                              length_of_insert_result, expressionStartIndex, i);
    }
    calculatAndUpdateExresion(connection, length_of_update_result);
    printResultFromDb(connection, outputFile);

    mysql_close(connection);
}