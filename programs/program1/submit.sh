#!/bin/bash
# My job's description
#PBS -N csc_4760_program1
#PBS -l nodes=1:ppn=10
#PBS -m bea
#PBS -M pmwestrich42@students.tntech.edu

cd ~/csc_4760/program1

#build the program
make -j 10

#run each with 10,000
./pi1 10000	>>./pi1_1.txt
./pi2 10 10000 >>./pi2_1.txt
./pi3 10 10000 >>./pi3_1.txt
./pi4 10 10000 >>./pi4_1.txt
./pi5 10 10000 >>./pi5_1.txt

#then 100,000
./pi1 100000 >>./pi1_2.txt
./pi2 10 100000 >>./pi2_2.txt
./pi3 10 100000 >>./pi3_2.txt
./pi4 10 100000 >>./pi4_2.txt
./pi5 10 100000 >>./pi5_2.txt

#then 1,000,000
./pi1 100000 >>./pi1_3.txt
./pi2 10 100000 >>./pi2_3.txt
./pi3 10 100000 >>./pi3_3.txt
./pi4 10 100000 >>./pi4_3.txt
./pi5 10 100000 >>./pi5_3.txt

#then 100,000,000
./pi1 1000000 >>./pi1_4.txt
./pi2 10 1000000 >>./pi2_4.txt
./pi3 10 1000000 >>./pi3_4.txt
./pi4 10 1000000 >>./pi4_4.txt
./pi5 10 1000000 >>./pi5_4.txt
