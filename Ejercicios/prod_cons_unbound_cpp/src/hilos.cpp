#include<hilos.hpp>
#include<vector>
std::vector<std::thread> hilos::create_threads_hilos(size_t count, void*(*subroutine)(void*), void* data) {
  // Create the threads that we need to do all the process
  // We allocate memory in the heap
  std::vector<std::thread> threads;
  std::thread thread(subroutine, data);
    for (size_t index = 0; index < count; ++index) {
      threads.push_back(thread);
      // Wait for all the threads to finish their jobs
    }
  return threads;
}

int hilos::join_threads_hilos(size_t count, std::vector<std::thread>threads) {
  // Wait for each thread and then we eliminate all of
  // them
  for (size_t index = 0; index < count; ++index) {
    // todo: sum could not be right
    threads[index].join();
  }
  return 1;
}