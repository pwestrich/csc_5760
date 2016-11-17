#!/bin/bash
#PBS -N pmw+p4_p_100_2
#PBS -W x=PARTITION:csc
#PBS -l nodes=1:ppn=2
#PBS -l mem=3072mb
#PBS -l walltime=2:00:00
#PBS -m bea
#PBS -M pmwestrich42@students.tntech.edu

#change to the program directory
cd ~/csc_4760/program4

#run the program
./main_omp 40000 100 2 >>./p_100_2.txt
