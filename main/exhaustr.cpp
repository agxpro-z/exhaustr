#include <iostream>
#include <stdio.h>

#include "BuiltinLED.hpp"

#ifdef __cplusplus
extern "C" {
#endif

void app_main() {
  BuiltInLED builtinLED;

  while (true) {
    builtinLED.blink(1000);
  }
}

#ifdef __cplusplus
} // End extern "C"
#endif
