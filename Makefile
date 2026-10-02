# ==============================================================================
# Author: Kaleb Blakemore
# KUID: 3228599
# Course: EECS 348 - Software Engineering
# Assignment: Assignment 2 - CEO Email Priority Queue (C Implementation)
# ==============================================================================

CC = gcc
CFLAGS = -Wall -Wextra -std=c99

all: assignment2

assignment2: assignment2.c
	$(CC) $(CFLAGS) assignment2.c -o assignment2

clean:
	rm -f assignment2
	