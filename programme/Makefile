# Règle par défaut
#all: bin/chef
all: bin/testpin bin/premier

# Constantes
CC=clang
CFLAGS=-g -Wall -Wextra -pedantic
# buildroot se charge de remplacer ces flags avec des optimisations


# Binaires
bin/chef: obj/chef.o obj/common.o
	$(CC) -o $@ $^ $(LDFLAGS)

bin/premier: obj/premier.o obj/common.o
	$(CC) -o $@ $^ -lm $(LDFLAGS)

bin/pince: obj/pince.o obj/common.o
	$(CC) -o $@ $^ $(LDFLAGS)

bin/testpin: obj/testpin.o
	$(CC) -o $@ $^ -lwiringPi $(LDFLAGS)

# Objets
obj/%.o: src/%.c src/%.h
	$(CC) -o $@ -c $< $(CFLAGS)


# Meta
clean:
	rm obj/* bin/*

# Envoi au robot
send:
	make -C ../root upgrade-programme

.PHONY: clean

