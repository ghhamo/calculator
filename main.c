#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <mysql.h>
#include <pthread.h>
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
    int lengthOfDBData = strlen(dbdata);
    conf = reedConfig(dbdata);
    MYSQL *connection = createDB();
    char *inputFileName = argv[1];
    FILE *outputFile = fopen("output.txt", "w");
    char *fileContent = readFile(inputFileName);
    unsigned int fileContentLength = strlen(fileContent);
    int expressionStartIndex = 0;
    pthread_t threadID;
    pthread_t threadIDs[30];
    InsertArg insertArgs[30];
    int countOfThreads = 0;
    int i;
    for (i = 0; i <= fileContentLength; ++i)
    {
        char currentChar = fileContent[i];
        if (currentChar == '\n' || currentChar == '\0')
        {
            if (countOfThreads > 29) {
                printf("max number of expression is 30\n");
                abort();
            }
            InsertArg insertArg;
            insertArg.fileContent = fileContent;
            insertArg.expressionStartIndex = expressionStartIndex;
            insertArg.expressionEndIndex = i;
            insertArgs[countOfThreads] = insertArg;
            pthread_create(&threadID, NULL, insertExpresionIntoDB, &insertArgs[countOfThreads]);
            threadIDs[countOfThreads] = threadID;
            countOfThreads++;
            expressionStartIndex = i + 1;
        }
    }
    for (int i = 0; i < countOfThreads; i++)
    {
        pthread_join(threadIDs[i], NULL);
    }
    proces(connection);
    printResultFromDB(connection, outputFile);
    mysql_close(connection);
}