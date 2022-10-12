/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

#include "ProducerTest.hpp"
#include "Log.hpp"
#include "Util.hpp"

ProducerTest::ProducerTest(size_t packageCount, int productorDelay,
size_t consumerCount, size_t producerCount) {
  this->packageCount = packageCount;
  this->productorDelay = productorDelay;
  this->consumerCount = consumerCount;
  this->producerCount = producerCount;
}

int ProducerTest::run() {
  // Produce each asked message
  // shared_data_t* shared_data =
  // (shared_data_t*)calloc(1, sizeof(shared_data_t));
  // this->shared_data = shared_data;
  // shared_data->shared_package = packageCount;

  mutex.lock();
  for ( size_t index = 0; index < packageCount / producerCount
  ; ++index ) {
    this->produce(this->createMessage(index));
  }
  mutex.unlock();
  // Produce an empty message to communicate we finished
  this->produce(NetworkMessage());

  // Report production is done
  Log::append(Log::INFO, "Producer", std::to_string(
    this->packageCount / producerCount)
    + " menssages sent");
  return EXIT_SUCCESS;
}

NetworkMessage ProducerTest::createMessage(size_t index) const {
  // Source is always 1: this producer
  const uint16_t source = 1;
  // Target is selected by random
  const uint16_t target = 1 + Util::random(0
    , static_cast<int>(this->consumerCount));
  // IMPORTANT: This simulation uses sleep() to mimics the process of
  // producing a message. However, you must NEVER use sleep() for real projects
  Util::sleepFor(this->productorDelay);
  // Create and return a copy of the network message
  return NetworkMessage(target, source, index);
}
