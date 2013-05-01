#ifndef RBTREE_H
#define RBTREE_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef enum color_t {
    RED,BLACK
} color_t;

typedef struct Node {
    char *data;
    char *key;
    struct Node *lchild, *rchild;
    color_t color;
} Node;

int isRed(Node *);
Node *rotateLeft(Node *);
Node *rotateRight(Node *);
void flipColors(Node *);

void InsertNode(Node **, char *, char *);
Node *put(Node **, char *, char *);
Node *SearchNode(Node *, char *, Node *);

Node *moveRedLeft(Node *);
Node *deleteMin(Node *);
Node *moveRedRight(Node *);

Node *balance(Node *);
Node *min(Node *);

Node *DeleteNode(Node *, char *);
Node *p_delete(Node *, char *);

#endif

