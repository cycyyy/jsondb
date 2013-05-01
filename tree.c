#include "tree.h"

Node *SearchNode(Node *T, char *key, Node *f)
{
    if(!T)
        return NULL;
    else if(!strcmp(key, T->key))
        return T;
    else if(strcmp(key, T->key)>0)
        return SearchNode(T->lchild, key, T);
    else
        return SearchNode(T->rchild, key, T);
}

Node *InsertNode(Node **T, char *key, char *data)
{
    if(*T == NULL)
    {
        *T = calloc(1, sizeof(Node));
        if(*T == NULL)
            return NULL;
        else
        {
            (*T)->key = key;
            (*T)->data = data;
            (*T)->lchild = NULL;
            (*T)->rchild = NULL;
        }
    }
    else if(strcmp(key, (*T)->key)>0)
        (*T)->lchild = InsertNode(&((*T)->lchild), key, data);
    else if(strcmp(key, (*T)->key)<0)
        (*T)->rchild = InsertNode(&((*T)->rchild), key, data);
    else if(!strcmp(key, (*T)->key))
    {
        (*T)->data = data;
    }
    return *T;
}

/*int main()
{
    char *key1 = "1";
    char *key2 = "2";
    char *key3 = "3";
    char *data1 = "dfs";
    char *data2 = "sdfa";
    char *data3 = "ds";
    Node *p = NULL;
    Node *s = NULL;
    InsertNode(&p, &key1, &data1);
    InsertNode(&p, &key2, &data2);
    InsertNode(&p, &key3, &data3);
    s = SearchNode(p, "4", NULL);
    if(s)
        printf("%s\n", *(s->data));
}*/
