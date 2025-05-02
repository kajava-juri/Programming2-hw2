# Syntax help

# $^ : all dependencies
# $@ : target
# $< : first dependency
# $* : target without extension

SRC = main.c logger.c utils.c db/data_api.c db/api/products.c db/api/quotes.c db/api/product_quotes.c
OBJ = $(SRC:.c=.o)
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -fanalyzer -g -fsanitize=address

TARGET = pwatch

# by default, try to make the target
all: $(TARGET)

# if the taraget does not exist follow below recipe
# this needs the object files, if these are not there, go to next recipe
$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

# to make all of object files, compile each .c one by one
%.o: %.c
	$(CC) -c $< $(CFLAGS) -o$@

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJ)