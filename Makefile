.PHONY: all clean

BUILD_DIR = build

all: $(BUILD_DIR)/main

clean:
	rm -rf $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/main.o: main.c functions.h | $(BUILD_DIR)
	gcc -c -g -Og -Wall -m32 -fno-pie -o $@ $<

$(BUILD_DIR)/functions.o: functions.asm | $(BUILD_DIR)
	nasm -f elf32 -DUNIX -g -F dwarf -o $@ $<

$(BUILD_DIR)/main: $(BUILD_DIR)/main.o $(BUILD_DIR)/functions.o
	gcc -g -m32 -lm -no-pie -o $@ $^