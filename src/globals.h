#ifndef __GLOBALS__
#define __GLOBALS__

#include <stdint.h>
#include <stdio.h>

const uint16_t win_width = 500;
const uint16_t win_heigh = 500;
const uint16_t target_fps = 60;

#define LOG(fmt, ...) { \
	printf("[%s:%s]> ", __FILE__, __func__); \
	printf(fmt __VA_OPT__(,) __VA_ARGS__); }

#endif
