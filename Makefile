CC=cc
CARGS=-c
LARGS=

%.o: %.c
	$(CC) $(CARGS) $<

cp: cp.o
	$(CC) $(LARGS) -o $@ $<

.PHONY: clean
clean:
	rm -rf *.o
	rm -rf ./cp
