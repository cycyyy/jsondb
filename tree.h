#include <string.h>
#include <stdlib.h>

typedef struct Node
{
    char *data;
    char *key;
    struct Node *lchild, *rchild;
}Node;

Node *SearchNode(Node *, char *, Node *);
Node *InsertNode(Node **, char *, char *);
