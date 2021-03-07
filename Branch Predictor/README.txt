OS used Ubuntu (VMBox).

Command used for compiling branchPredictor.c : gcc branchPredictor.c -lm

Command used for running: ./a.out branch_predictor_type(1,2,3,4) bit_counter_size(1,2,3,4,5,6..)

number - branch predictor type 
1	   - local branch predictor
2	   - two level Global branch predictor
3	   - two level Gshare branch predictor
4	   - two level local branch predictor

Trace should be in below format:
7f50cf8aaeef N
7f50cf8aaf34 T


Command used for compiling hybrid.c : gcc hybrid.c -lm

Command used for running: ./a.out bit_counter_size(1,2,3,4,5,6..)

bit_counter_size: 2,4,6,8,etc

NOTE: Both branchPredictor.c and hybrid.c accept trace file as "trace.din" and should be present in the same directory.