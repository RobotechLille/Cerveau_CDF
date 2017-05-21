# À définir dans les variables d'environnement
#ARDUINO_DIR       = /usr/share/arduino

# Configuration de la carte
#BOARD_TAG         = mega
#BOARD_SUB         = atmega2560
#MONITOR_BAUDRATE  = 115200
BOARD_TAG         = uno
MONITOR_BAUDRATE  = 115200

# On peut écraser cette valeur en utilisant
# make MONITOR_PORT=/dev/ttyACM1 upload
# (par exemple)
MONITOR_PORT      = /dev/ttyACM*

# Flags du compilateur
CFLAGS_STD        = -std=gnu11
CXXFLAGS_STD      = -std=gnu++11

CXXFLAGS         = -pedantic -Wall -Wextra

# Colorisation de la sortie pour certaines versions d'avr-gcc
ifeq "$(shell expr `avr-gcc -dumpversion | cut -f1` \>= 4.9)" "1"
    CXXFLAGS += -fdiagnostics-color
endif

# Fichires de sortie
TARGET=main
OBJDIR=build

include Arduino-Makefile/Arduino.mk

