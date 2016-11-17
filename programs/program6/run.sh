
#!/bin/bash

make -kj 4

./main_cuda 1000 >>cuda_1k.txt
./main_cuda 5000 >>cuda_5k.txt
./main_cuda 10000 >>cuda_10k.txt

