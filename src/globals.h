#ifndef __GLOBALS__
#define __GLOBALS__

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

static const uint16_t win_width = 720;
static const uint16_t win_heigh = 720;
static const uint16_t target_fps = 60;

#define LOG(fmt, ...) { \
	printf("[%s:%s]> ", __FILE__, __func__); \
	printf(fmt __VA_OPT__(,) __VA_ARGS__); \
}

#define LOG_ONCE(fmt, ...) { \
	static bool out = false; \
	if (!out) { \
		out = true; \
		LOG(fmt __VA_OPT__(,) __VA_ARGS__); \
	}\
}


#endif
