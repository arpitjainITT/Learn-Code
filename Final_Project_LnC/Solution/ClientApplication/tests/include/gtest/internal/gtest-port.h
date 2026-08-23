// This is a minimal patch for gtest-port.h to fix the strcasecmp issue
#pragma once

// Define our own strcasecmp function before including the real gtest-port.h
#ifndef strcasecmp
extern "C" {
  inline int my_strcasecmp(const char* s1, const char* s2) {
    while (*s1 && (tolower((unsigned char)*s1) == tolower((unsigned char)*s2))) {
      s1++;
      s2++;
    }
    return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
  }
}
#define strcasecmp my_strcasecmp
#endif

// Include the actual gtest-port.h
#include_next <gtest/internal/gtest-port.h>
