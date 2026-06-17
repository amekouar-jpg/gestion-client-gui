# Makefile for GTK3 Contact Manager Application
# Compile with: make
# Clean with: make clean
# Run with: make run

CC = gcc
CFLAGS = -Wall -Wextra $(shell pkg-config --cflags gtk+-3.0)
LIBS = $(shell pkg-config --libs gtk+-3.0)

TARGET = contacts_app
SOURCE = projet_gui.c
OBJECT = projet_gui.o

all: $(TARGET)

$(TARGET): $(OBJECT)
	$(CC) -o $(TARGET) $(OBJECT) $(LIBS)
	@echo "✓ Build successful: $(TARGET)"

$(OBJECT): $(SOURCE)
	$(CC) $(CFLAGS) -c $(SOURCE) -o $(OBJECT)
	@echo "✓ Compilation complete"

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJECT) $(TARGET)
	@echo "✓ Clean complete"

.PHONY: all run clean
