// Copyright 2022 Fabio Andres Sanabria Valerin
// <fabio.sanabria@ucr.ac.cr> CC-BY-4

#include <stdlib.h>

#include "common.h"

// Subroutine of the random times
useconds_t random_between(useconds_t min, useconds_t max) {
  return min + (max > min ? (random() % (max - min)) : 0);
}
