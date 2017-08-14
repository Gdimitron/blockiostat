CC=gcc
CFLAGS=-O2

blockiostat: blockiostat.c
		$(CC) -o $@ $^ $(CFLAGS)

clean: blockiostat
		rm $^
