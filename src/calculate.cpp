//
// Created by war on 5/28/25.
//
#include <calculate.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <quadmath.h>
#include <cmath>
#include <cstdlib>
#include <cstring>



inline string to_string128(__float128 value, int precision = 40) {
  char buf[256];
  char fmt[32];
  snprintf(fmt, sizeof(fmt), "%%.%dQg", precision);
  quadmath_snprintf(buf, sizeof(buf), fmt, value);
  return {buf};
}