CC=gcc
CFLAGS=-I. -Wall -Wextra
DEPS = shell.h
OBJ = shell.o shell_client.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

shesell: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
