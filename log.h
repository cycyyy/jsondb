#ifndef LOG_H
#define LOG_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX 1024

#ifdef RBtree
#include "RBtree.h"
#else
#include "tree.h"
#endif

FILE *log;

void initialize(Node **);
char *log_make(char *, char *, char *);
void log_write(char *);
void log_close();
#endif
