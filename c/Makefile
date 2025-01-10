CC=gcc
CFLAGS=-Wall -Wextra -O2
LDFLAGS=-lz
BUILD_DIR=build

all: $(BUILD_DIR)/main $(BUILD_DIR)/main_static $(BUILD_DIR)/main_musl

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/main: main.c compress.c decompress.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) main.c compress.c decompress.c -o $(BUILD_DIR)/main $(LDFLAGS)

$(BUILD_DIR)/main_static: main.c compress.c decompress.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -static main.c compress.c decompress.c -o $(BUILD_DIR)/main_static $(LDFLAGS)

$(BUILD_DIR)/main_musl: main.c compress.c decompress.c | $(BUILD_DIR)
	musl-gcc $(CFLAGS) main.c compress.c decompress.c -o $(BUILD_DIR)/main_musl $(LDFLAGS)

clean:
	rm -rf $(BUILD_DIR)