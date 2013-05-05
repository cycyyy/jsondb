#ifndef SERVER_H
#define SERVER_H

#include <stdlib.h>
#include <string.h>
#include <event2/http.h>
#include <assert.h>
#include <event2/buffer.h>
#include <event2/event.h>
#include <event2/keyvalq_struct.h>
#include <pthread.h>
#include <semaphore.h>
#include "log.h"

#ifdef RBtree
#include "RBtree.h"
#else
#include "tree.h"
#endif

Node *tree = NULL;
sem_t sem;
pthread_mutex_t mutex;
char *log_text;

int getHandle(struct evhttp_request *);
int postHandle(struct evhttp_request *);
int deleteHandle(struct evhttp_request *);

void genHandle(struct evhttp_request *, void *);

void w_thread();

#endif
