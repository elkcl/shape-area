.PHONY: all clean test

BUILD_DIR = build

all: $(BUILD_DIR)/main

clean:
	rm -rf $(BUILD_DIR)

test: $(BUILD_DIR)/main
	$(BUILD_DIR)/main --full-test --roots --iters

$(BUILD_DIR):
	mkdir -p $@

main.c: functions.h

$(BUILD_DIR)/main.o: main.c | $(BUILD_DIR)
	gcc -c -Wall -m32 -fno-pie -o $@ $<

$(BUILD_DIR)/functions.o: functions.asm | $(BUILD_DIR)
	nasm -f elf32 -DUNIX -o $@ $<

$(BUILD_DIR)/main: $(BUILD_DIR)/main.o $(BUILD_DIR)/functions.o
	gcc -m32 -no-pie $^ -lm -o $@
