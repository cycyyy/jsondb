#include "RBtree.h"

//tool function

int isRed(Node *T)
{
    if(!T)
        return 0;
    return (T->color==RED) ? 1:0;
}

Node *rotateLeft(Node *T)
{
    Node *x = T->rchild;
    T->rchild = x->lchild;
    x->lchild = T;
    x->color = T->color;
    T->color = RED;

    return x;
}

Node *rotateRight(Node *T)
{
    Node *x = T->lchild;
    T->lchild = x->rchild;
    x->rchild = T;
    x->color = T->color;
    T->color = RED;
    return x;
}

void flipColors(Node *T)
{
    T->color = RED;
    T->lchild->color = BLACK;
    T->rchild->color = BLACK;
}

//-------------------------------------------------------------------
//insert function

void InsertNode(Node **T, char *key, char *data)
{
    put(T, key, data);
    (*T)->color = BLACK;
}

Node *put(Node **T, char *key, char *data)
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
            (*T)->color = RED;
        }
    }
    else if(strcmp(key, (*T)->key)<0)
        (*T)->lchild = put(&((*T)->lchild), key, data);
    else if(strcmp(key, (*T)->key)>0)
        (*T)->rchild = put(&((*T)->rchild), key, data);
    else if(!strcmp(key, (*T)->key))
        (*T)->data = data;

    if(isRed((*T)->rchild) && !isRed((*T)->lchild))
        *T = rotateLeft(*T);
    if(isRed((*T)->lchild) && isRed((*T)->lchild->lchild))
        *T = rotateRight(*T);
    if(isRed((*T)->lchild) && isRed((*T)->rchild))
        flipColors(*T);
    return *T;
}

//-------------------------------------------------------------------
//search function

Node *SearchNode(Node *T, char *key, Node *f)
{
    if(!T)
        return NULL;
    else if(!strcmp(key, T->key))
        return T;
    else if(strcmp(key, T->key)<0)
        return SearchNode(T->lchild, key, T);
    else
        return SearchNode(T->rchild, key, T);
}

//------------------------------------------------------------------
//delete function

Node *moveRedLeft(Node *T)
{
    flipColors(T);
    if(isRed(T->rchild->lchild))
    {
        T->rchild = rotateRight(T->rchild);
        T = rotateLeft(T);
    }
    return T;
}

Node *deleteMin(Node *T)
{
    if(!T->lchild)
    {
        if(T->key)
            free(T->key);
        if(T->data)
            free(T->data);
        free(T);
        return NULL;
    }
    if(!isRed(T->lchild) && !isRed(T->lchild->lchild))
        T = moveRedLeft(T);
    T->lchild = deleteMin(T->lchild);
    return balance(T);
}

Node *moveRedRight(Node *T)
{
    flipColors(T);
    if(!isRed(T->lchild->lchild))
        T = rotateRight(T);
    return T;
}

Node *balance(Node *T)
{
    if(isRed(T->rchild))
        T = rotateLeft(T);
    if(isRed(T->rchild) && !isRed(T->lchild))
        T = rotateLeft(T);
    if(isRed(T->lchild) && isRed(T->lchild->lchild))
        T = rotateRight(T);
    if(isRed(T->lchild) && isRed(T->rchild))
        flipColors(T);
    return T;
}

Node *min(Node *T)
{
    if(!T->lchild)
        return T;
    return min(T->lchild);
}

Node *DeleteNode(Node *T, char *key)
{
    if(!isRed(T->lchild) && !isRed(T->rchild))
        T->color = RED;
    T = p_delete(T, key);
    return T;
}


Node *p_delete(Node *T, char *key)
{
    if(strcmp(key, T->key)<0)
    {
        if(!isRed(T->lchild) && !isRed(T->lchild->lchild))
            T = moveRedLeft(T);
        T->lchild = p_delete(T->lchild, key);
    }
    else
    {
        if(isRed(T->lchild))
            T = rotateRight(T);
        if(!strcmp(key, T->key) && !T->rchild)
            return NULL;
        if(!isRed(T->rchild) && !isRed(T->rchild->lchild))
            T = moveRedRight(T);
        if(!strcmp(key, T->key))
        {
            T->data = SearchNode(T->rchild,
                                 min(T->rchild)->data,
                                 NULL)->data;
            T->key = min(T->rchild)->key;
            T->rchild = deleteMin(T->rchild);
        }
        else
            T->rchild = p_delete(T->rchild, key);
    }
    return balance(T);
}

//------------------------------------------------------------------
//test case

/*
int main()
{
    char *key1 = "S";
    char *key2 = "E";
    char *key3 = "A";
    char *key4 = "R";
    char *key5 = "C";
    char *key6 = "H";
    char *key7 = "X";
    char *key8 = "M";
    char *key9 = "P";
    char *key10 = "L";
    char *data1 = "S";
    char *data2 = "E";
    char *data3 = "A";
    char *data4 = "R";
    char *data5 = "C";
    char *data6 = "H";
    char *data7 = "X";
    char *data8 = "M";
    char *data9 = "P";
    char *data10 = "L";
    Node *p = NULL;
    Node *s = NULL;
    InsertNode(&p, key1, data1);
    InsertNode(&p, key2, data2);
    InsertNode(&p, key3, data3);
    InsertNode(&p, key4, data4);
    InsertNode(&p, key5, data5);
    InsertNode(&p, key6, data6);
    InsertNode(&p, key7, data7);
    InsertNode(&p, key8, data8);
    InsertNode(&p, key9, data9);
    InsertNode(&p, key10, data10);
    s = SearchNode(p, "R", NULL);
    if(s)
        printf("%s12\n", s->data);
    DeleteNode(p, "R");
    s = SearchNode(p, "L", NULL);
    if(s)
        printf("%s\n", s->data);
}*/
