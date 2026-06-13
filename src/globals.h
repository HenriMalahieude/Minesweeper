#ifndef __GLOBALS__
#define __GLOBALS__

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

static const uint16_t win_width = 720;
static const uint16_t win_heigh = 720;
static const uint16_t target_fps = 60;
static const uint16_t font_size = 12;

extern enum HEAT_TYP heat;
extern enum GME_DFC difficulty;
extern struct Board board;
extern bool reload;

#define LOG(fmt, ...) { \
	printf("[%s:%s]> ", __FILE__, __func__); \
	printf(fmt __VA_OPT__(,) __VA_ARGS__); \
}

#define _CONCAT(A, B) A ## B
#define CONCAT(A, B) _CONCAT(A,B)

#define _LOG_ONCE(varn, fmt, ...) { \
	static bool varn = false; \
	if (!varn) { \
		varn = true; \
		LOG(fmt __VA_OPT__(,) __VA_ARGS__); \
	}\
}
#define LOG_ONCE(fmt, ...) \
	_LOG_ONCE(CONCAT(log_once_macro, __COUNTER__), fmt __VA_OPT__(,) __VA_ARGS__)


#define max(a, b) (a > b) ? a : b
#define min(a, b) (a < b) ? a : b

#endif
