# Makefile for Mini-Shell (HW1)
# COP 5614 - Operating Systems
# Spring 2026

CC = gcc
CFLAGS = -Wall -Wextra -std=c99

TARGET = mysh

all: $(TARGET)

$(TARGET): mysh.c
	$(CC) $(CFLAGS) -o $(TARGET) mysh.c

clean:
	rm -f $(TARGET)

.PHONY: all clean
