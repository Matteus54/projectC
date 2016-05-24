CC = gcc
SRC = main.c bdd.c gui.c
OBJ = main.o bdd.o gui.o
EXEC = main

GTK_CFLAGS = $$(pkg-config --cflags gtk+-3.0)
GTK_LDFLAGS = $$(pkg-config --libs gtk+-3.0)

SQLITE_FLAGS = -lsqlite3

ERROR_CFLAGS = -Wall -W -pedantic

LDFLAGS = $(ERROR_CFLAGS) 
CFLAGS = $(ERROR_CFLAGS) $(GTK_CFLAGS)

all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC)
	$(CC) $(LDFLAGS) -o $(EXEC) $(OBJ) $(SQLITE_FLAGS) $(GTK_LDFLAGS) 


clean:
	rm -rf *.o

cleandb:
	rm -rf *.db

mrproper: clean cleandb
	rm -rf $(EXEC)
