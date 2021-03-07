OS used Ubuntu (VMBox).

Command used for compiling CacheSimulator.c : gcc cacheprefetchSimulator.c -lm

Command used for running: ./a.out cachetype(c or s) cachesize blocksize associativity prefetcherblocksize confidencebits < trace.din > output.out

The prefetcher block size parameter must be entered as 512,1024,2048,4096,etc

eg: ./a.out c 64 128 4 512 2< trace.din > output.out
this command is used for combined cache 64KB cache size 128 byte block size and 4 way set associative with 512B prefetcher having 2 confidence bits

./a.out s 16 32 1 1024 3< trace.din > output.out
this command is used for split cache 16KB cache size 32byte block size and direct associative with 1024B prefetcher having 3 confidence bits

NOTE: trace.din file should be present in same directory as that of cacheSimulator.c