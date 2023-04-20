.PHONY: all clean

BUILD_DIR = build

all: $(BUILD_DIR)/main

clean:
	rm -rf $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/main: main.c | $(BUILD_DIR)
	gcc -O2 -Wall -m32 -lm -o $@ $<
