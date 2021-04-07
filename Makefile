#Group Members: Gouled, Bryson, Rowan
#---Incomplete---
 
 CFLAGS = -Wall -ansi -std=c99 

 CC = gcc 

 all: SSTE 

 SSTE: SSTE.c SSTE.h
	$(CC) $(CFLAGS) -o SSTE SSTE.c

clean: 
	rm *.o