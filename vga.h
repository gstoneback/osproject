#ifndef VGA_H
#define VGA_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "util.h"
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg);
static inline uint16_t vga_entry(unsigned char uc, uint8_t color);
void vga_initialize(void);
void vga_setcolor(enum vga_color fg, enum vga_color bg);
void vga_putentryat(char c, uint8_t color, size_t x, size_t y);
void vga_putcharat(char c, size_t x, size_t y);
void vga_scroll(void);
void vga_putchar(char c);
void vga_write(const char* data, size_t size);
void vga_writestring(const char* data);
char vga_getcharat(size_t x, size_t y);

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t vga_row;
size_t vga_column;
uint8_t vga_color;
uint16_t* vga_buffer;

void vga_initialize(void)
{
	vga_row = 0;
	vga_column = 0;
	vga_setcolor(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	vga_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			vga_putcharat(' ', x, y);
		}
	}
}

void vga_setcolor(enum vga_color fg, enum vga_color bg)
{
	vga_color = vga_entry_color(fg, bg);
}

void vga_putentryat(char c, uint8_t color, size_t x, size_t y) {
	vga_buffer[y * VGA_WIDTH + x] = vga_entry(c, color);
}

void vga_putcharat(char c, size_t x, size_t y) {
  vga_putentryat(c, vga_color, x, y);
}

char vga_getcharat(size_t x, size_t y) {
  return vga_buffer[y * VGA_WIDTH + x];
}

void vga_scroll() {
  for (size_t y = 0; y < VGA_HEIGHT - 1; ++y) {
    for (size_t x = 0; x < VGA_WIDTH; ++x) {
      vga_putcharat(vga_getcharat(x, y + 1), x, y);
    }
  }
  for (size_t x = 0; x < VGA_WIDTH; ++x) vga_putcharat(' ', x, VGA_HEIGHT - 1);
}

void vga_putchar(char c) {
  if (c >= 32) {
    vga_putcharat(c, vga_column, vga_row);
    if (++vga_column == VGA_WIDTH) {
  		vga_column = 0;
      ++vga_row;
  	}
  } else if (c == '\r') {
    vga_column = 0;
  } else if (c == '\n') {
    vga_column = 0;
    ++vga_row;
  }
	if (vga_row >= VGA_HEIGHT) {
    vga_scroll();
  }
}

void vga_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		vga_putchar(data[i]);
}

void vga_writestring(const char* data) {
	vga_write(data, strlen(data));
}

#endif
