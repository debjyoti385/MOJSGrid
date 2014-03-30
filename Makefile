# Makefile for compiling NSGA-II source code
CC=mpic++
LD=mpic++
LDFLAGS=-O3 -lpthread
RM=rm -f
OBJS:=$(patsubst %.c,%.o,$(wildcard *.c))
MAIN=modjsg
all:$(MAIN)
$(MAIN):$(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(MAIN) -lm
%.o: %.c global.h rand.h
	$(CC) $(CFLAGS) -g -c $<
clean:
	$(RM) $(OBJS)

