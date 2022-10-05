/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

#include <time.h>
#include<stdlib.h>

#include "ProducerConsumerTest.hpp"

int main(int argc, char* argv[]) {
  srand(time(NULL));
  return ProducerConsumerTest().start(argc, argv);
}

