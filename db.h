#ifndef DB
#define DB
#include <stdio.h>
#include <mysql.h>
#include <string.h>
#include "tokenizer.h"
#include "calculator.h"
#include "util.h"
#include "conf.h"

void finishWithError(MYSQL *connection);

char *insert_result = "INSERT INTO expresion(expresion) VALUES('')'\0'";
char *update_result = "UPDATE expresion SET expresion_result = '' WHERE expresion = '''\0'";

MYSQL *createDB(Conf conf)
{
    MYSQL *connection = mysql_init(NULL);
    if (!mysql_real_connect(connection, conf.server,
                            conf.user, conf.password, NULL, 0, NULL, 0))
    {
        finishWithError(connection);
    }
    if (mysql_query(connection, "CREATE DATABASE IF NOT EXISTS calculator"))
    {
        finishWithError(connection);
    }
    if (mysql_query(connection, "USE calculator"))
    {
        finishWithError(connection);
    }
    if (mysql_query(connection, "CREATE TABLE IF NOT EXISTS expresion(id INT AUTO_INCREMENT NOT NULL, expresion VARCHAR(600), expresion_result DOUBLE DEFAULT NULL,PRIMARY KEY (id))"))
    {
        finishWithError(connection);
    }
    return connection;
}

void insertExpresionIntoDB(MYSQL *connection, char *insert_result, char *fileContent,
                           int length_of_insert_result, int start_of_row, int end_of_row)
{
    char *line = getSubString(start_of_row, end_of_row, fileContent);
    int line_lenght = strlen(line);
    char *buffer = malloc((line_lenght + length_of_insert_result) * sizeof(char));
    sprintf(buffer, "INSERT INTO expresion(expresion) VALUES('%s')", line);
    if (mysql_query(connection, buffer))
    {
        finishWithError(connection);
    }
    free(buffer);
}

void calculateAndUpdateExpression(MYSQL *connection, int length_of_update_result)
{
    mysql_query(connection, "SELECT expresion FROM expresion");
    MYSQL_ROW rows;
    MYSQL_RES *confres = mysql_store_result(connection);
    int totalrows = mysql_num_rows(confres);
    int numfields = mysql_num_fields(confres);
    MYSQL_FIELD *mfield;
    unsigned long *lengths = mysql_fetch_lengths(confres);
    while ((rows = mysql_fetch_row(confres)))
    {
        unsigned long *lengthOfRows = mysql_fetch_lengths(confres);
        for (int i = 0; i < numfields; i++)
        {
            struct TokenArray tokenArray = tokenize(rows[i], lengthOfRows[i]);
            double result = calculateExpressionTokens(tokenArray);
            char *buf = malloc((lengthOfRows[i] + length_of_update_result) * sizeof(char) + sizeof(double));
            sprintf(buf, "UPDATE expresion SET expresion_result = '%f' WHERE expresion = '%s'", result, rows[i]);
            if (mysql_query(connection, buf))
            {
                finishWithError(connection);
            }
        }
    }
}

void printResultFromDB(MYSQL *connection, FILE *outputFile)
{
    mysql_query(connection, "SELECT expresion_result FROM expresion");
    MYSQL_ROW rows;
    MYSQL_RES *confres = mysql_store_result(connection);
    while (rows = mysql_fetch_row(confres))
    {
        fprintf(outputFile, "%s\n", rows[0]);
    }
}

void finishWithError(MYSQL *connection)
{
    fprintf(stderr, "%s\n", mysql_error(connection));
    mysql_close(connection);
    exit(1);
}

#endif