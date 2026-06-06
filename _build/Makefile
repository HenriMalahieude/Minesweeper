CC = gcc

RAYLIB_DIR ?= ~/Desktop/raylib/src
RAYGUI_DIR ?= ~/Desktop/raygui/src

WRN = -Wall
BASE_FLAGS = $(WRN) -I $(RAYLIB_DIR) -I $(RAYGUI_DIR)
TEST_FLAGS = -Og -g $(BASE_FLAGS) -L ./lib/ -lraylib -lGL -lm -lpthread -ldl -lX11
WEB_FLAGS = -O2 $(BASE_FLAGS) -L ./web_lib/ -lraylib

SRC_DIR ?= ../src

SRCS = $(wildcard $(SRC_DIR)/*.c)
HDRS = $(wildcard $(SRC_DIR)/*.h)
OBJS = $(foreach t, $(SRCS), $(basename $(notdir $t)).o)

.PHONY: web minesweeper clean
.SECONDARY:

minesweeper: $(OBJS)
	gcc -o $@ $(OBJS) $(TEST_FLAGS)

web:
	$(info TODO)

%.o: $(SRC_DIR)/%.c $(HDRS)
	gcc -c $< $(TEST_FLAGS)

clean:
	rm *.o 2> /dev/null | true
	rm minesweeper 2> /dev/null | true
