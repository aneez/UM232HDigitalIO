LIB= -lftdi 
INC=
CC=gcc
CFLAGS= -Wall -Os
TGT=digitalIO
SRCS=$(wildcard *.c)
OBJDEPS=$(patsubst %.c,%.o,$(SRCS))

.SUFFIXES : .c .o \  
.c.o: 
	$(CC) $(CFLAGS) -c $< -o $@ 

all: $(TGT)

$(TGT): $(OBJDEPS)  
	$(CC) -o $@ $^ $(LIB) $(INC)

clean:
	rm -f $(TGT) *.o
