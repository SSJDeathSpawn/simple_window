CC := gcc
LIBS := wayland-client xkbcommon
CFLAGS := -g -Wall $(foreach lib,$(LIBS),$$(pkg-config --cflags $(lib)))
LDFLAGS := -lm $(foreach lib,$(LIBS),$$(pkg-config --libs $(lib)))

#Binary folder
BIN := bin

# Get the source files recursively
SRCS := $(wildcard src/**/*.c) $(wildcard src/*.c) $(wildcard src/**/**/*.c) $(wildcard src/**/**/**/*.c)

# generate names of object files
OBJS := $(SRCS:.c=.o)


# default recipe
all: app clean

# recipe for building the final executable
app: $(OBJS)
	$(CC) -o $(BIN)/app $^ $(LDFLAGS)

# recipe for building object files
$(OBJS): $(@:.o=.c)
	$(CC) -o $@ $(@:.o=.c) -c $(CFLAGS)

run: all
	$(BIN)/app

# recipe to clean the workspace
clean:
	rm -f $(OBJS)

.PHONY: all clean run
