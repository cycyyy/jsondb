CFLAGES=-levent -ltcmalloc

server:
	cc server.c RBtree.c $(CFLAGES) -o server

clean:
	rm server
