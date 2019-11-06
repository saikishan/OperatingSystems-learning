/*
    @kishan
    i didnt create or code this myself,i did this as part of learning OS Udacity
    i tried to understand the code in assignment and did my part to re-write this
    in fact i am frequently checking the code.
 */
#include "p-readers-writers.h"
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

/*
    here i define my thread limitation constats
*/
#define NUM_READERS 5
#define NUM_WRITERS 5 //this actually says ho many concurent readers and writers can exist
#define NUM_READ 5 //this defines now of read operaions per thread
#define NUM_WRITE 5 //this says no of write operations per thread
//defining global thread controllers

unsigned int gSharedValue = 0; //this value represents the read or write count happening
pthread_mutex_t gSharedLock = PTHREAD_MUTEX_INITIALIZER; //lock to control global variables
pthread_cond_t readerCondition = PTHREAD_COND_INITIALIZER; //condition on which reader threads wait
pthread_cond_t writerCondition = PTHREAD_COND_INITIALIZER;  //condition on which writer threads wait
int activeReaders = 0, readersWaiting = 0;

int main(){
    int i;
    int readerNum[NUM_READERS];
    int writerNum[NUM_WRITERS];

    //we need the thread variables to add them to join so we always track thread objects
    //so we dont end up having zombi threads
    pthread_t readerThreadIDS[NUM_READERS];
    pthread_t writerThreadIDS[NUM_WRITERS];
    srandom((unsigned int)time(NULL));
    /*
        by change in output of the rand func based on time,
        this call just makes sure that all the test runs run diffrent.
    */
    /* now initalize the reader threads and writer threads
        and join them with this parent thread
    */
    //its important to not to send i is as the value of i can be changed by the time it access.
    for(i=0; i < NUM_READERS; i++ ){
        readerNum[i] = i;
        pthread_create(&readerThreadIDS[i], NULL, readerMain, &readerNum[i]);
    }  
    for(i=0; i < NUM_WRITERS; i++ ){
        writerNum[i] = i;
        pthread_create(&writerThreadIDS[i], NULL, writerMain, &writerNum[i]);
    }
    for(i = 0; i < NUM_READERS; i++) {
		pthread_join(readerThreadIDS[i], NULL);
	}

	// Wait on writers to finish
	for(i = 0; i < NUM_WRITERS; i++) {
		pthread_join(writerThreadIDS[i], NULL);
	}
    /*  
        p_thread_create will initilise the thread with method call
        p_thread_join is important is as the main(parent) will/can die early than children
        children threads become zombie threads and the os will clean up the memory as parent dies!!
    */
    return 0;
}
//i need to put readers in to sleep for really long as their is a huge chance that 
void *readerMain(void *threadArguments){
    int id = *((int*)threadArguments);
    //yes we are trying to store the threadid of children
    int i = 0, numReaders = 0; 
    //this just let's us know now of concurent reads happening while print
    for(i = 0; i < NUM_READ ; i++) {
        usleep(1000 * (random() % NUM_READERS + NUM_WRITERS));
        //puting to sleep to make things more concurent
        pthread_mutex_lock(&gSharedLock);
            readersWaiting++;
            while(activeReaders == -1)
                pthread_cond_wait(&readerCondition, &gSharedLock);
            readersWaiting--;
            numReaders = ++activeReaders;
        pthread_mutex_unlock(&gSharedLock);
        //this is important as the operations below need concurent reads executions
        //The gshared will be the shared resource for now consider it as a file.
        fprintf(stdout, "Thread_reader id [r%d] reading %u [readers %2d]\n", id, gSharedValue, numReaders);
        //every active thread exiting should let the other threads know its done
        pthread_mutex_lock(&gSharedLock);
	  	    activeReaders--; // need to lock as if multiple threads try to update activeReaders concurent ill go negative
            //its readers on priority we never put reader on wait while write is going on
	  	    if(activeReaders == 0) 
	  		    pthread_cond_signal(&writerCondition);
	    pthread_mutex_unlock(&gSharedLock);
    }
    pthread_exit(0);
}

void *writerMain(void *threadArguments){
    int id = *((int*)threadArguments);
    int i = 0, numReaders = 0;
    for(i = 0; i < NUM_WRITE; i++){
        usleep(1000 * (random() % NUM_READERS + NUM_WRITERS));
        //puting to sleep to make things more concurent
        pthread_mutex_lock(&gSharedLock);
        while(activeReaders != 0)
            pthread_cond_wait(&writerCondition, &gSharedLock);
        activeReaders = -1;
        numReaders = -1;
        pthread_mutex_unlock(&gSharedLock);
        fprintf(stdout, "Thread_writer id [w%d]* writing %u [readers %2d]\n", id, ++gSharedValue, numReaders);
        //do the write operation
        pthread_mutex_lock(&gSharedLock);
            activeReaders = 0;
            if(readersWaiting >  0) pthread_cond_broadcast(&readerCondition);
            else pthread_cond_signal(&writerCondition);
        pthread_mutex_unlock(&gSharedLock);
    }
    pthread_exit(0); //this states the thread exited with a sucess state
}