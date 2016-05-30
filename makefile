CC = gcc
SRC = main.c bdd.c gui.c create_user.c login.c compte.c transactions.c statistique.c bdd_checks.c bdd_updates.c import_releve_from_csv.c
OBJ = main.o bdd.o gui.o create_user.o login.o compte.o transactions.o statistique.o bdd_checks.o bdd_updates.o import_releve_from_csv.o

TEST_SRC = test.c
TEST_OBJ = test.o

EXEC = main

GTK_CFLAGS = $$(pkg-config --cflags gtk+-3.0)
GTK_LDFLAGS = $$(pkg-config --libs gtk+-3.0)

SQLITE_FLAGS = -lsqlite3

ERROR_CFLAGS = -Wall -W -g

LDFLAGS = $(ERROR_CFLAGS)
CFLAGS = $(ERROR_CFLAGS) $(GTK_CFLAGS)
LIBS = $(SQLITE_FLAGS) $(GTK_LDFLAGS)

all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC)
	$(CC) $(LDFLAGS) -o $(EXEC) $(OBJ) $(LIBS)

test:
	$(CC) $(CFLAGS) -c $(TEST_SRC)
	$(CC) $(LDFLAGS) -o test_ $(TEST_OBJ) $(LIBS)


clean:
	rm -rf *.o

cleandb:
	rm -rf *.db

mrproper: clean cleandb
	rm -rf $(EXEC)
