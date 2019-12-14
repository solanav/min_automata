# NAMES
EXEC_NAME=mach

# FOLDERS
INCLUDE=include/
SRC=src/
BIN=bin/

# COMPILER
CC=gcc
CFLAGS=-g -Wall $(LIBS)# -DDEBUG
LIBS=-lm

all: mk $(EXEC_NAME) rm

# CREATE AND REMOVE BIN
mk:
	mkdir -p $(BIN)

rm:
	rm -rf $(BIN)

# MAIN EXECUTABLE
$(EXEC_NAME): main.o transforma.o minimiza.o sbc.o afnd.o afnd_plus.o 
	$(CC) -o $@ $(BIN)main.o $(BIN)transforma.o $(BIN)minimiza.o $(BIN)sbc.o $(BIN)afnd.o $(BIN)afnd_plus.o $(CFLAGS)

# OBJECTS
main.o: $(SRC)main.c
	$(CC) -c -o $(BIN)$@ $< $(CFLAGS)

afnd.o: $(SRC)afnd.c
	$(CC) -c -o $(BIN)$@ $< -g

afnd_plus.o: $(SRC)afnd_plus.c
	$(CC) -c -o $(BIN)$@ $< $(CFLAGS)

transforma.o: $(SRC)transforma.c
	$(CC) -c -o $(BIN)$@ $< $(CFLAGS)

minimiza.o: $(SRC)minimiza.c
	$(CC) -c -o $(BIN)$@ $< $(CFLAGS)

sbc.o: $(SRC)sbc.c
	$(CC) -c -o $(BIN)$@ $< $(CFLAGS)
	
# UTILS
valgrind:
	valgrind --leak-check=full ./$(EXEC_NAME)

dot:
	for f in *.dot; do dot -Tpng $f -o $f.png; done

clean:
	rm -f *.dot *.png $(EXEC_NAME)