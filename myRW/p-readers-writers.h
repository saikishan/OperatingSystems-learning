#ifndef P_READERS_WRITERS_H_
#define P_READERS_WRITERS_H_
int main(int argc, void **arguments);
void *readerMain(void* threadArgument);
void *writerMain(void* threadArgument);
#endif