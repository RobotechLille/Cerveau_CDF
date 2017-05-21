# Règle par défaut
#all: bin/chef
all: bin/pince

# Constantes
CC=clang
CFLAGS=-g -Wall -Wextra -pedantic
# buildroot se charge de remplacer ces flags avec des optimisations


# Binaires
bin/chef: obj/chef.o
	$(CC) -o $@ $^ $(LDFLAGS)

bin/pince: obj/pince.o obj/serial.o
	$(CC) -o $@ $^ $(LDFLAGS)

# Objets
obj/%.o: src/%.c src/%.h
	$(CC) -o $@ -c $< $(CFLAGS)


# Meta
clean:
	rm obj/* bin/*

.PHONY: clean

