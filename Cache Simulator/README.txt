OS used Ubuntu (VMBox).

Command used for compiling CacheSimulator.c : gcc cacheSimulator.c -lm

Command used for running: ./a.out cachetype(c or s) cachesize blocksize associativity < trace.din > output.out

eg: ./a.out c 64 128 4 < trace.din > output.out
this command is used for combined cache 64KB cache size 128 byte block size and 4 way set associative

./a.out s 16 32 1 < trace.din > output.out
this command is used for split cache 16KB cache size 32byte block size and direct associative

NOTE: trace.din file should be present in same directory as that of cacheSimulator.c