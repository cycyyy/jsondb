CFLAGES=-levent -ltcmalloc -lpthread

server:
	cc server.c RBtree.c log.c $(CFLAGES) -o server

clean:
	rm server
