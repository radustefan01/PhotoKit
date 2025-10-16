# General compiler settings
CC := gcc
CFLAGS := -Wall -Wextra -std=c99 -g -I include
LDFLAGS := -lm

# GTK flags (using pkg-config)
GTK_CFLAGS := $(shell pkg-config --cflags gtk4 gdk-pixbuf-2.0)
GTK_LIBS := $(shell pkg-config --libs gtk4 gdk-pixbuf-2.0)

# Directories
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin

# Find all source files
CLI_SRCS := $(wildcard $(SRC_DIR)/cli/*.c)
GUI_SRCS := $(wildcard $(SRC_DIR)/gui/*.c)

# Generate object file paths
CLI_OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(CLI_SRCS))
GUI_OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(GUI_SRCS))

# Create a list of CLI objects WITHOUT main.o, to be used by the GUI
CLI_CORE_OBJS := $(filter-out $(BUILD_DIR)/cli/main.o, $(CLI_OBJS))

# Target Executables
CLI_TARGET := $(BIN_DIR)/image-cli
GUI_TARGET := $(BIN_DIR)/image-gui

.PHONY: all cli gui clean

# --- Build Rules ---

# Default goal: build both the CLI and GUI
all: cli gui

# Rule to build the CLI executable
cli: $(CLI_TARGET)

$(CLI_TARGET): $(CLI_OBJS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "CLI build finished: $@"

# Rule to build the GUI executable
gui: $(GUI_TARGET)

$(GUI_TARGET): $(GUI_OBJS) $(CLI_CORE_OBJS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(GTK_CFLAGS) $^ -o $@ $(GTK_LIBS) $(LDFLAGS)
	@echo "GUI build finished: $@"

# --- Object File Compilation Rules ---

# Rule for compiling CLI object files
$(BUILD_DIR)/cli/%.o: $(SRC_DIR)/cli/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule for compiling GUI object files
$(BUILD_DIR)/gui/%.o: $(SRC_DIR)/gui/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(GTK_CFLAGS) -c $< -o $@

# --- Housekeeping ---

clean:
	@echo "Cleaning up..."
	rm -rf $(BUILD_DIR) $(BIN_DIR)
