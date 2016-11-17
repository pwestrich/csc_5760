#!/bin/bash

./matrix_parallel 1000 4 >>1k.txt
./matrix_parallel 5000 4 >>5k.txt
./mareix_parallel 10000 4 >>10k.txt
./matrix_parallel 20000 4 >>20k.txt

