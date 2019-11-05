CC=gcc 
CFLAGS=-Wall
LDFLAGS=-lm
DEPS = scheduling.h
OBJ = main.o

main: $(DEPS) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o main $(LDFLAGS)

clean:
	rm -f main main.o
run: main
	./main
