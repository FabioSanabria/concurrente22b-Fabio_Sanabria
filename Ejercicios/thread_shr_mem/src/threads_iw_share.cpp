// Copyright 2022 Fabio Sanabria <fabio.sanabria@ucr.ac.cr> CC-BY 4.0

#include <assert.h>  // Already seen
#include <inttypes.h>  // Already seen
// The <stdint.h> header shall declare sets of integer
// types having specified widths, and shall define corresponding sets of macros
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>  // Already seen
#include <thread>
// Library of standard output and input of c++
#include <iostream>
// Library that provide the function
// of vector that is a dinamic list
#include <vector>
// Library that helps to get
// the random number
#include <numeric>
// Library that provides the
// shared_ptr
#include <memory>
// Library that gives a chrono to
// take the time
#include <chrono>
// Header providing parametric manipulators
#include <iomanip>

// using namespace std;

/** @struct shared_data
 *  @brief This structure has the count of
 * threats that the user provides.
 *  @var shared_data::thread_count 
 *  Member 'thread_count' the number of threads
 */
typedef struct shared_data {
  uint64_t thread_count;
} shared_data_t;


/** @struct private_data
 *  @brief This structure has the specific
 * thread number of a thread and has a register
 * of shared_data.
 * @var private_data::thread_number 
 * Member 'thread_number' The specific number of
 * a thread
 * @var private_data::shared_data 
 * Member 'shared_data' A register that
 * has the thread_count of all the threads              
 */
typedef struct private_data {
  uint64_t thread_number;  // rank
  std::shared_ptr<shared_data_t>  shared_data;
} private_data_t;

/** @struct invalidThreads
 *  @brief This structure inherits from the
 * std::exception, that throws an especific 
 * exception
 * @subroutine invalidThreads::what() 
 * Method 'what()': That a macro that 
 * gives the user a warning of error
 */
struct invalidThreads : public std::exception {
const char* what() const noexcept {
  return "Error: invalid thread count\n";}
};

/** @struct concurrency_error
 *  @brief This structure inherits from the
 * std::exception, that throws an especific 
 * exception
 * @subroutine concurrency_error::what() 
 * Method 'what()': That a macro that 
 * gives the user a warning of error
 */
struct concurrency_error : public std::exception {
const char* what() const noexcept {
  return "Error: could not allocate shared data\n";}
};
/**
 * @brief Says hello in a different thread, in this case "greet" 
 * is going to be used by the number of threads requested by the user using
 * shared_data as a shared variable. Another case, we use threads insted of
 * using pthreads
 * @return void
 */
void greet(private_data_t data);

/**
 * @brief Subrutine responsible for creating the threads requested by the user
 * and use of dinamic memory is going to be used by the number of threads requested
 *  by the user using shared_data, the process of creation of threads is different of
 * the phtreads because we use a vector to store the data, then we send it like a
 * parameter to the greet
 * @return int Error code
 */
int create_threads(std::shared_ptr<shared_data_t> shared_data);

// procedure main(argc, argv[])
int main(int argc, char* argv[]) {
  // create thread_count as result of converting argv[1] to integer
  // thread_count := integer(argv[1])
  // Get the number of cpus

  uint64_t thread_count = std::thread::hardware_concurrency();
  try {
    if (argc == 2) {
      if (sscanf(argv[1], "%" SCNu64, &thread_count) == 1) {
      } else {
        throw invalidThreads();  // Throw the exception
      }
    }
  } catch (invalidThreads& e) {  // Catch the exception
    std::cout << e.what();

    // We can use this btw
    // cerr << "Error: invalid thread count" << endl;
    return 11;
  }
  // Takes the time right now
  auto start = std::chrono::high_resolution_clock::now();
  std::shared_ptr<shared_data_t> shared_data = std::shared_ptr
  <shared_data_t>(new shared_data_t);
  try {
    if (shared_data) {
      shared_data->thread_count = thread_count;

      create_threads(shared_data);
      // do some work
      // record end time
      // Takes the time at the end of the process
      auto end = std::chrono::high_resolution_clock::now();
      // Calculate the expected time
      std::chrono::duration<double> diff = end - start;
      std::cout << "Time to finish all the greets"
                << " ints : " << diff.count() << " s\n";
    } else {
      throw concurrency_error();  // Throw the exception
    }
  } catch(concurrency_error& e) {  // Catch the exception
      std::cout << e.what();
      return 14;
  }
}

int create_threads(std::shared_ptr<shared_data_t> shared_data) {
  int error = EXIT_SUCCESS;

    std::vector<std::thread> threads;  // Create the vector of threads
    // Create the vector of private_data
    std::vector<private_data_t> private_data;
    // A temp that stores the private data
    private_data_t private_temp;
    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count
        ; ++thread_number) {
      // Assing the thread_number to the private data
      private_temp.thread_number = thread_number;
      // Assing the shared_data to the private data
      private_temp.shared_data = shared_data;
      // Put the temp inside the vector of private_data
      private_data.push_back(private_temp);
      // Put the thread created inside the vector
      // of threads
      threads.emplace_back(std::thread(greet, private_data[thread_number]));
    }
    // print "Hello from main thread"
    std::cout << "Hello from main thread" << std::endl;
    // This cycle joins all the threads stored inside of the
    // threads vector
    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count
        ; ++thread_number) {
      threads[thread_number].join();
    }
  return error;
}

// procedure greet:
void greet(private_data_t data) {
  // assert(data);
  std::shared_ptr<shared_data_t> shared_data = data.shared_data;
  try {
    if (shared_data) {
    // print "Hello from secondary thread"
    std::cout << "Hello from secondary thread " << data.thread_number <<
    " of " << shared_data->thread_count << std::endl;
    } else {
      throw concurrency_error();
    }
  } catch(concurrency_error& e) {
    std::cerr << "Error: There was a problem of creating the secondary thread";
  }
}  // end procedure
