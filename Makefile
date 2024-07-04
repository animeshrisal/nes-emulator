CC = gcc
CFLAGS = -Wall -Wextra -pedantic -Wno-return-type
LIBS = `sdl2-config --cflags --libs` -lSDL2_ttf

# List of source files
SRCS = $(wildcard *.c)
# List of object files (each .c file will be compiled to a corresponding .o file)
OBJS = $(SRCS:.c=.o)

# The target executable
TARGET = main

# Default rule
all: $(TARGET)

# Rule to link the object files to create the final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

# Rule to compile .c files to .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJS) $(TARGET)

