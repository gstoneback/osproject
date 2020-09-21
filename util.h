#ifndef UTIL_H
#define UTIL_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

size_t strlen(const char* str) {
  size_t len;
  for(len = 0; str[len]; ++len);
  return len;
}
#endif
