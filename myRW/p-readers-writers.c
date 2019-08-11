/*
    @kishan
    i didnt create or code this myself as part of learning OS Udacity
    i tried to understand the code in assignment and did my part to write this
    in fact i am frequently checking the code
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
#define NUM_READ //this defines now of read operaions per thread
#define NUM_WRITE //this says no of write operations per thread
//defining global thread controllers
unsigned int gSharedValue = 0; //this value represents the read write count happening
pthread_mutex_t gSharedLock = PTHREAD_MUTEX_INITIALIZER; //lock to control global variables
pthread_cond_t readerCondition = PTHREAD_COND_INITIALIZER; //condition on which reader threads wait
pthread_cond_t writerCondition = PTHREAD_COND_INITIALIZER;  //condition on which writer threads wait
int activeReaders = 0, readersWaiting = 0;

int main(int argc, void* arguments){
    int i;
    int readerNum[NUM_READERS];
    int writerNum[NUM_WRITERS];

    //we need the thread variables to add them to join
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
    for(i=0; i<=NUM_READERS; i++ )  readerNum[i] = i , readerThreadIDS[i] = pthread_create(&readerThreadIDS[i], NULL, readerMain, &readerNum[i]);
    for(i=0; i<=NUM_READERS; i++ )  readerNum[i] = i , readerThreadIDS[i] = pthread_create(&writerThreadIDS[i], NULL, witerMain, &writerNum[i]);
}