# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g -lm 


# Executable name
TARGET = client_exe

# Source files
SRCS = main.c marvelmind.c extraction_point.c get_location.c comm.c traj.c send_to_arduino.c

# Object files
OBJS = $(SRCS:.c=.o)

# Header files
HEADERS = get_location.h marvelmind.h extraction_point.h comm.h traj.h send_to_arduino.h

# Default rule
all: $(TARGET)

# Linking the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) -lpthread -lm -lwiringPi

# Compiling source files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

# Clean
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
