CC = gcc
CFLAGS = -Wall -g
LIBS = -lgmp

# Ejecutables
TARGET_A = afin
TARGET_B = afin_hill
TARGET_C = vigenere
TARGET_D = kasiski
TARGET_E = IC
TARGET_F = flujo
TARGET_G = permutacion
TARGET_H = subkeys

# Fuentes
SRC_A = afin.c utils.c lfsr.c
SRC_B = afin_hill.c utils.c lfsr.c
SRC_C = vigenere.c utils.c lfsr.c
SRC_D = kasiski.c utils.c lfsr.c
SRC_E = IC.c utils.c lfsr.c
SRC_F = flujo.c utils.c lfsr.c
SRC_G = permutacion.c utils.c lfsr.c
SRC_H = subkeys.c utils.c lfsr.c

# Regla principal
all: $(TARGET_A) $(TARGET_B) $(TARGET_C) $(TARGET_D) $(TARGET_E) $(TARGET_F) $(TARGET_G) $(TARGET_H)

# Compilar ej1_a
$(TARGET_A): $(SRC_A)
	$(CC) $(CFLAGS) $(SRC_A) -o $(TARGET_A) $(LIBS)

# Compilar ej1_b
$(TARGET_B): $(SRC_B)
	$(CC) $(CFLAGS) $(SRC_B) -o $(TARGET_B) $(LIBS)

# Compilar ej2_a
$(TARGET_C): $(SRC_C)
	$(CC) $(CFLAGS) $(SRC_C) -o $(TARGET_C) $(LIBS)

# Compilar kasiski
$(TARGET_D): $(SRC_D)
	$(CC) $(CFLAGS) $(SRC_D) -o $(TARGET_D) $(LIBS)

# Compilar IC
$(TARGET_E): $(SRC_E)
	$(CC) $(CFLAGS) $(SRC_E) -o $(TARGET_E) $(LIBS)

# Compilar flujo
$(TARGET_F): $(SRC_F)
	$(CC) $(CFLAGS) $(SRC_F) -o $(TARGET_F) $(LIBS)

# Compilar permutacion
$(TARGET_G): $(SRC_G)
	$(CC) $(CFLAGS) $(SRC_G) -o $(TARGET_G) $(LIBS)

# Compilar subkeys
subkeys: $(SRC_H)
	$(CC) $(CFLAGS) $(SRC_H) -o $(TARGET_H) $(LIBS)

# Limpiar
clean:
	rm -f $(TARGET_A) $(TARGET_B) $(TARGET_C) $(TARGET_D) $(TARGET_E) ${TARGET_F} ${TARGET_G} ${TARGET_H} *.o
