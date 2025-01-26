# Compiler to use
CC = gcc

# Compiler flags
CFLAGS = -Wall -g

# Libraries to link
LIBS = -lvirt

# Source files
SRC = core/core.core.c controllers/commands.con.c controllers/network.con.c handlers/iso.hand.c handlers/log.hand.c

# Output executable file
OUT = vm_manager

# Build the executable
$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LIBS)

# Clean the build files
clean:
	rm -f $(OUT)
