#ifndef CONFUTIL_H
#define CONFUTIL_H
#include <string.h>
#include <stdio.h>

typedef struct ConfEntry
{
    char *key;
    char *value;
} ConfEntry;

typedef struct Conf
{
    char *server;
    char *user;
    char *password;
} Conf;

Conf conf;

ConfEntry parseConfEntry(char *conf);

Conf reedConfig(char *data)
{
    ConfEntry confEntry;
    Conf conf;
    int dataLength = strlen(data);
    int lineStart = 0;
    for (int i = 0; i <= dataLength; i++)
    {
        char currentChar = data[i];
        if (currentChar == '\n' || currentChar == '\0')
        {
            char *ex = getSubString(lineStart, i, data);
            lineStart = i + 1;
            confEntry = parseConfEntry(ex);
            if (strcmp(confEntry.key, "server") == 0)
            {
                conf.server = confEntry.value;
            }
            else if (strcmp(confEntry.key, "user") == 0)
            {
                conf.user = confEntry.value;
            }
            else if (strcmp(confEntry.key, "password") == 0)
            {
                conf.password = confEntry.value;
            }
            else
            {
                printf("ERROR: Unrecognised key.");
                abort();
            }
        }
    }
    return conf;
}

ConfEntry parseConfEntry(char *conf)
{
    int confLength = strlen(conf);
    ConfEntry confEntry;
    int i;
    for (i = 0; i < confLength; i++)
    {
        char currentChar = conf[i];
        if (currentChar == '=')
        {
            confEntry.key = getSubString(0, i, conf);
            break;
        }
    }
    confEntry.value = getSubString(i + 1, confLength, conf);
    return confEntry;
}

#endif