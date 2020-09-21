#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "vga.h"
/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler."
#endif

void kernel_main(void)
{
	/* Initialize terminal interface */
	vga_initialize();

	/* Newline support is left as an exercise. */
	vga_writestring("Test 1\n");
	vga_writestring("Test 2\n");
	vga_writestring("Test 3\n");
	vga_scroll();
}
