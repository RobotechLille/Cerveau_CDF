#!/bin/bash

FIFOS="moteurCap moteurEff"

rm -f log/*

# FONCTION USUELLES

echo "On se réveille !"

clearFifos() {
    for fifo in $FIFOS
    do
        rm -f com/$fifo
    done
}

createFifos() {
    for fifo in $FIFOS
    do
        mkfifo com/$fifo
    done
}

launchProgram() {
    bin/$1 &
    echo $! > pid/$1
}

stopProgram() {
    kill -s SIGTERM $(cat pid/$1)
}

# DÉMARRAGE

# Recréation des fifos pour les vider
clearFifos
createFifos
rm -f pid/*

# Lancement des programmes
launchProgram moteur

# ATTENTE DE LA BROCHE
echo "Tout paré"
while bin/testpin 3 4
do
    continue
done

echo "On est ti-par"

#launchProgram chef
# TODO Pipe vers chef pour qu'il se lance

# EN COURS
sleep 10

echo "Time's up!"

# FIN
stopProgram moteur

# FUNNY ACTION
#bin/funny
echo "Funny done"

# On attend que tous les programmes se soient fermés
wait
echo "Game over"
