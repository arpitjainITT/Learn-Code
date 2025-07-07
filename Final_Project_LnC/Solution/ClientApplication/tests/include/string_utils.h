#pragma once

#include <string.h>
#include <ctype.h>

// Define our own replacement for strcasecmp
extern "C" {
  inline int strcasecmp_replacement(const char* s1, const char* s2) {
    while (*s1 && (tolower(*s1) == tolower(*s2))) {
      s1++;
      s2++;
    }
    return tolower(*(unsigned char*)s1) - tolower(*(unsigned char*)s2);
  }
}
