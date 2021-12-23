#ifndef DB_H
#define DB_H
#include <stdio.h>
#include <mysql.h>
#include <string.h>
#include "tokenizer.h"
#include "calculator.h"
#include "util.h"
#include "conf.h"

void *proces(void *con);

void *calculateAndUpdateExpression(void *arg);

void finishWithError(MYSQL *connection);

typedef struct InsertArg
{
    char *fileContent;
    int expressionStartIndex;
    int expressionEndIndex;
} InsertArg;

MYSQL *connectDB()
{
    MYSQL *connection = mysql_init(NULL);
    if (!mysql_real_connect(connection, conf.server,
                            conf.user, conf.password, NULL, 0, NULL, 0))
    {
        finishWithError(connection);
    }
    return connection;
}

MYSQL *createDB()
{
    MYSQL *connection = connectDB(conf);

    if (mysql_query(connection, "CREATE DATABASE IF NOT EXISTS calculator"))
    {
        finishWithError(connection);
    }
    if (mysql_query(connection, "USE calculator"))
    {
        finishWithError(connection);
    }
    if (mysql_query(connection, "CREATE TABLE IF NOT EXISTS expression(id INT AUTO_INCREMENT NOT NULL, expression VARCHAR(600), expression_result DOUBLE DEFAULT NULL,PRIMARY KEY (id))"))
    {
        finishWithError(connection);
    }
    return connection;
}

void *proces(void *con)
{
    MYSQL *connection = (MYSQL *)con;
    mysql_query(connection, "SELECT expression FROM expression");
    MYSQL_ROW rows;
    MYSQL_RES *confres = mysql_store_result(connection);
    int totalrows = mysql_num_rows(confres);
    int numfields = mysql_num_fields(confres);
    int countOfThrs = 0;
    pthread_t threadIDs[totalrows];
    char *calcArgs[totalrows];
    pthread_t threadID;
    char *expression_string;
    while ((rows = mysql_fetch_row(confres)))
    {
        expression_string = strcat(rows[0], "\0");
        calcArgs[countOfThrs] = expression_string;
        pthread_create(&threadID, NULL, calculateAndUpdateExpression, calcArgs[countOfThrs]);
        threadIDs[countOfThrs] = threadID;
        countOfThrs++;
    }
    for (int i = 0; i < countOfThrs; i++)
    {
        pthread_join(threadIDs[i], NULL);
    }
}

void *insertExpresionIntoDB(void *arg)
{
    MYSQL *connection = connectDB();
    if (mysql_query(connection, "USE calculator"))
    {
        finishWithError(connection);
    }
    InsertArg *insertArg = (InsertArg *)arg;
    char *queryTemplate = "INSERT INTO expression(expression) VALUES(\"%s\")";
    int lengthOfInsertQuery = strlen(queryTemplate) - 2;
    char *line = getSubString(insertArg->expressionStartIndex, insertArg->expressionEndIndex, insertArg->fileContent);
    int lineLenght = strlen(line);
    char *to = malloc((lineLenght * 2 + 1) * sizeof(char));
    unsigned long lengthOfTo = mysql_escape_string(to, line, lineLenght);
    to[lengthOfTo] = '\0';
    char *buffer = malloc((lengthOfInsertQuery + lengthOfTo) * sizeof(char));
    sprintf(buffer, queryTemplate, line);
    if (mysql_query(connection, buffer))
    {
        finishWithError(connection);
    }
    free(buffer);
    mysql_close(connection);
    mysql_thread_end();
    pthread_exit(NULL);
}

void *calculateAndUpdateExpression(void *arg)
{
    MYSQL *connection = connectDB();
    if (mysql_query(connection, "USE calculator"))
    {
        finishWithError(connection);
    }
    char *expression_string = (char *)arg;
    char *queryTemplate = "UPDATE expression SET expression_result = \"%f\" WHERE expression = \"%s\"";
    int length_of_update_query = strlen(queryTemplate) - 4;
    struct TokenArray tokenArray = tokenize(expression_string);
    double result = calculateExpressionTokens(tokenArray);
    char *buf = malloc((strlen(expression_string) + length_of_update_query) * sizeof(char) + sizeof(double));
    sprintf(buf, queryTemplate, result, expression_string);
    if (mysql_query(connection, buf))
    {
        finishWithError(connection);
    }
    free(buf);
    mysql_close(connection);
    mysql_thread_end();
    pthread_exit(NULL);
}

void printResultFromDB(MYSQL *connection, FILE *outputFile)
{
    mysql_query(connection, "SELECT expression_result FROM expression");
    MYSQL_ROW rows;
    MYSQL_RES *confres = mysql_store_result(connection);
    while (rows = mysql_fetch_row(confres))
    {
        fprintf(outputFile, "%s\n", rows[0]);
    }
    fclose(outputFile);
}

void finishWithError(MYSQL *connection)
{
    fprintf(stderr, "%s\n", mysql_error(connection));
    mysql_close(connection);
    exit(1);
}
#endif