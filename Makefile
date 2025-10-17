CC = gcc
CFLAGS = -Wall -g
LIBS = -lgmp

# Ejecutables
TARGET_A = ej1_a
TARGET_B = ej1_b

# Fuentes
SRC_A = afin.c utils.c
SRC_B = afin_hill.c utils.c

# Regla principal
all: $(TARGET_A) $(TARGET_B)

# Compilar ej1_a
$(TARGET_A): $(SRC_A)
	$(CC) $(CFLAGS) $(SRC_A) -o $(TARGET_A) $(LIBS)

# Compilar ej1_b
$(TARGET_B): $(SRC_B)
	$(CC) $(CFLAGS) $(SRC_B) -o $(TARGET_B) $(LIBS)

# Limpiar
clean:
	rm -f $(TARGET_A) $(TARGET_B)
