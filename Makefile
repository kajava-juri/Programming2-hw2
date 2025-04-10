# Syntax help

# $^ : all dependencies
# $@ : target
# $< : first dependency
# $* : target without extension

SRC = main.c logger.c
OBJ = $(SRC:.c=.o)
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -fanalyzer

TARGET = pwatch

# by default, try to make the target
all: $(TARGET)

# if the taraget does not exist follow below recipe
# this needs the object files, if these are not there, go to next recipe
$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

# to make all of object files, compile each .c one by one
$(OBJ): $(SRC)
	$(CC) -c $< $(CFLAGS)