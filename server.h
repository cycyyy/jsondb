#ifndef SERVER_H
#define SERVER_H

#include <stdlib.h>
#include <string.h>
#include <event2/http.h>
#include <assert.h>
#include <event2/buffer.h>
#include <event2/event.h>
#include <event2/keyvalq_struct.h>

#ifdef RBtree
#include "RBtree.h"
#else
#include "tree.h"
#endif

Node *tree = NULL;

int getHandle(struct evhttp_request *);
int postHandle(struct evhttp_request *);
int deleteHandle(struct evhttp_request *);

void genHandle(struct evhttp_request *, void *);

#endif
