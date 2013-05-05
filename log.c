#define RBtree
#include "log.h"

void initialize(Node **T)
{
    char line[MAX];
    char *p;
    char *key;
    char *value;
    line[0] = '\0';
    log = fopen("log", "a+");
    if(!log)
    {
        fprintf(stderr, "open log fail!\n");
        exit(1);
    }
    while(fgets(line, MAX, log) && line[0] != '\n' && strlen(line))
    {
        p = strtok(line, "\t");
        if(!strcmp(p, "i"))
        {
            p = strtok(NULL, "\t");
            key = calloc(strlen(p)+1, sizeof(char));
            strcpy(key, p);
            p = strtok(NULL, "\t");
            p[strlen(p)-1] = '\0';
            value = calloc(strlen(p)+1, sizeof(char));
            strcpy(value, p);
            InsertNode(T, key, value);
        }
        else if(!strcmp(p, "d"))
        {
            p = strtok(NULL, "\t");
            p[strlen(p)-1] = '\0';
            *T = DeleteNode(*T, p);
        }
        else
        {
            fprintf(stderr, "initialize fali!\n");
            exit(1);
        }
    }
}

char *log_make(char *d, char *key, char *value)
{
    char text[MAX];
    char *p;
    text[0] = '\0';
    strcat(text, d);
    strcat(text, "\t");
    strcat(text, key);
    if(!strcmp(d, "i"))
    {
        strcat(text, "\t");
        strcat(text, value);
    }
    strcat(text, "\n");
    p = calloc(strlen(text)+1, sizeof(char));
    strcpy(p, text);
    return p;
}

void log_write(char *test)
{
    write(fileno(log), test, strlen(test));
}

void log_close()
{
    fclose(log);
}
