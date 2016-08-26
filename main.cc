#include "thread_pool.h"
#include "future_vector.h"
#include "variadic.h"
#include "stopwatch.h"
#include "getopt.h"
#include <iostream>
#include <algorithm>


using namespace cu;


int func() {
  std::cout << "hi\n";
  return 1;
}


int main(int argc, char* argv[]) {
  Stopwatch sw;


  ThreadPool tp{3};


  for(int i=0; i<10; i++)
    tp.Enqueue([](int x){ std::cout << x << " "; }, i);
  std::cout << "\n";





  //  std::cout << "time : " << sw.sec() << " sec\n";
  
  return 0;
}
