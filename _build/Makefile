CC = gcc
WEB_CC = emcc

RAYLIB_DIR ?= ~/Desktop/raylib/src
RAYGUI_DIR ?= ~/Desktop/raygui/src

WRN = -Wall
BASE_FLAGS = $(WRN) -I $(RAYLIB_DIR) -I $(RAYGUI_DIR) -std=c23
TEST_FLAGS = -Og -g $(BASE_FLAGS) -L ./lib/ -lraylib -lGL -lm -lpthread -ldl -lX11
WEB_FLAGS = -O3 $(BASE_FLAGS) -L ./web_lib/ -lraylib -s USE_GLFW=3 -DPLATFORM_WEB

SRC_DIR ?= ../src
WEB_OUT_DIR ?= ../docs

SRCS = $(wildcard $(SRC_DIR)/*.c)
HDRS = $(wildcard $(SRC_DIR)/*.h)
OBJS = $(foreach t, $(SRCS), $(basename $(notdir $t)).o)

.PHONY: web minesweeper clean
.SECONDARY:

minesweeper: $(OBJS)
	$(CC) -o $@ $(OBJS) $(TEST_FLAGS)

web: $(WEB_OUT_DIR) clean
	rm $(WEB_OUT_DIR) 2> /dev/null | true
	$(WEB_CC) $(SRCS) -o $(WEB_OUT_DIR)/index.html $(WEB_FLAGS)

%.o: $(SRC_DIR)/%.c $(HDRS)
	$(CC) -c $< $(TEST_FLAGS)

$(WEB_OUT_DIR):
	mkdir -p $@

clean:
	rm *.o 2> /dev/null | true
	rm minesweeper 2> /dev/null | true
