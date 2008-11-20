/****************************************************
 * This is a test that will test mutexes            *
 ****************************************************/

#include <iostream>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include "user_api.h"

using namespace std;

// Functions executed by threads
void* thread_func(void * threadid);

int main(int argc, char* argv[]){ // main begins

   carbonInit();

   // Read in the command line arguments
   const unsigned int numThreads = 1;

   // Declare threads and related variables
   pthread_t threads[numThreads];
   pthread_attr_t attr;
	
   // Initialize threads and related variables
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

   // Initialize the main thread as a core
   int tid;
   CAPI_return_t rtnVal;

   rtnVal = CAPI_Initialize(0);

   // Initialize local variables
   CAPI_rank(&tid);

   for(unsigned int i = 0; i < numThreads; i++) 
      pthread_create(&threads[i], &attr, thread_func, (void *) (i + 1));

   // Wait for all threads to complete
   for(unsigned int i = 0; i < numThreads; i++) 
      pthread_join(threads[i], NULL);

   cout << "UserApplication: About to call carbon finish!" << endl;
   carbonFinish();

   pthread_exit(NULL);

} // main ends


void* thread_func(void *threadid)
{
  // Declare local variables
  int tid;
  CAPI_return_t rtnVal;

  rtnVal = CAPI_Initialize((int)threadid);

  // Initialize local variables
  CAPI_rank(&tid);

  // Thread starts here
  cerr << "UserThread: CAPI Rank: " << tid << endl;

  pthread_exit(NULL);
}

