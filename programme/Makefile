# Règle par défaut
all: bin/chef

# Constantes
CC=clang
CFLAGS=-g -Wall -Wextra -pedantic
# buildroot se charge de remplacer ces flags avec des optimisations


# Binaires
bin/chef: obj/chef.o
	$(CC) -o $@ $^ $(LDFLAGS)

# Objets
obj/%.o: src/%.c src/%.h
	$(CC) -o $@ -c $< $(CFLAGS)


# Meta
clean:
	rm obj/* bin/*

.PHONY: clean

