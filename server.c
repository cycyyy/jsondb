#define RBtree
#include "server.h"

int getHandle(struct evhttp_request *req)
{
    struct evbuffer *buf;
    char *uri;
    char *data;
    //struct evkeyvalq header;
    buf = evbuffer_new();
    Node *p = NULL;
    if(buf == NULL)
    {
        evhttp_send_error(req, HTTP_NOTFOUND, "Buffer Error!");
        evbuffer_free(buf);
        return 0;
    }
    uri = (char*)evhttp_request_get_uri(req);
    if(uri == NULL || !strcmp(uri, "/"))
    {
        evhttp_send_error(req, HTTP_NOTFOUND, "Uri Error!");
        evbuffer_free(buf);
        return 0;
    }
    if(!strcmp(uri, "/favicon.ico"))
        return 0;
    //evhttp_parse_query(uri, &header);
    //data = evhttp_find_header(&header, "key");

    /*if(data == NULL)
    {
        evhttp_send_error(req, HTTP_NOTFOUND, "No key!");
        evbuffer_free(buf);
        return 0;
    }*/
    p = SearchNode(tree, uri, NULL);
    if(p)
        data = p->data;
    if(!p || !data)
        evbuffer_add_printf(buf, "data not found\n");
    evbuffer_add_printf(buf, "key=%s,data=%s\n", uri, data);
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
    evbuffer_free(buf);
    return 1;
}

int postHandle(struct evhttp_request *req)
{
    char *data;
    char *uri;
    char *p;
    int i;
    struct evbuffer *buf;
    p = (char*)evhttp_request_get_uri(req);
    uri = calloc(sizeof(p), sizeof(char));
    strcpy(uri, p);
    if(uri == NULL || !strcmp(uri, "/"))
    {
        evhttp_send_error(req, HTTP_NOTFOUND, "Uri Error!");
        return 0;
    }
    buf = evhttp_request_get_input_buffer(req);
    i = evbuffer_get_length(buf);
    if(i == 0)
    {
        evhttp_send_error(req, HTTP_NOTFOUND, "Key not Found!");
        return 0;
    }
    data = calloc(i+1, sizeof(char));
    if(data == NULL)
    {
        evhttp_send_error(req, HTTP_NOTFOUND, "Data Error!");
        return 0;
    }
    evbuffer_remove(buf, data, i+1);
    InsertNode(&tree, uri, data);
    //char *q = log_make("i", uri, data);
    //log_write(q);
    //free(q);
    pthread_mutex_lock(&mutex);
    log_text = log_make("i", uri, data);
    sem_post(&sem);
    pthread_mutex_unlock(&mutex);
    evbuffer_add_printf(buf, "key=%s,value=%s\n", uri, data);
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
    return 1;
}

int deleteHandle(struct evhttp_request *req)
{
    struct evbuffer *buf;
    char *uri;
    char *data;
    buf = evbuffer_new();
    Node *p = NULL;
    if(buf == NULL)
    {
        evhttp_send_error(req, HTTP_NOTFOUND, "Buffer Error!");
        evbuffer_free(buf);
        return 0;
    }
    uri = (char *)evhttp_request_get_uri(req);
    if(uri == NULL || !strcmp(uri, "/"))
    {
        evhttp_send_error(req, HTTP_NOTFOUND, "Uri Error!");
        evbuffer_free(buf);
        return 0;
    }
    p = SearchNode(tree, uri, NULL);
    if(p)
    {
        tree = DeleteNode(tree, p->key);
        //char *q = log_make("d", p->key, NULL);
        //log_write(q);
        //free(q);
        pthread_mutex_lock(&mutex);
        log_text = log_make("d", p->key, NULL);
        sem_post(&sem);
        pthread_mutex_unlock(&mutex);
        data = p->data;
    }
    if(!p || !data)
        evbuffer_add_printf(buf, "No such data\n");
    evbuffer_add_printf(buf, "key=%s,data=%s\n", uri, data);
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
    evbuffer_free(buf);
    return 1;
}

void genHandle(struct evhttp_request *req, void *arg)
{
    if(evhttp_request_get_command(req) == EVHTTP_REQ_GET)
    {
        getHandle(req);
    }
    else if(evhttp_request_get_command(req) == EVHTTP_REQ_POST)
    {
        postHandle(req);
    }
    else if(evhttp_request_get_command(req) == EVHTTP_REQ_DELETE)
    {    
        deleteHandle(req);
    }
    else
        evhttp_send_error(req, HTTP_BADMETHOD, "Error 405 Method Not Allowed!");
} 

void w_thread()
{
   while(1)
   {
        sem_wait(&sem);
        pthread_mutex_lock(&mutex);
        log_write(log_text);
        free(log_text);
        pthread_mutex_unlock(&mutex);
   }
}

int main()
{
    initialize(&tree);

    int ret;
    ret = sem_init(&sem, 0, 0);
    assert(!ret);
    ret = pthread_mutex_init(&mutex, NULL);
    assert(!ret);

    pthread_t write_thread;
    pthread_create(&write_thread, NULL, (void *)w_thread, NULL);

    struct event_base *base;
    base = event_base_new();
    assert(base != NULL);

    struct evhttp *http;
    http = evhttp_new(base);
    assert(evhttp_bind_socket(http, "0.0.0.0", 8000) == 0);

    evhttp_set_gencb(http, genHandle, NULL);
    assert(event_base_dispatch(base) == 1);

    evhttp_free(http);
    event_base_free(base);
    log_close();
    return 0;
}

