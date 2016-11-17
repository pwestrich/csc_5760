#!/bin/bash
#PBS -N pmw+p4_p_400_8
#PBS -W x=PARTITION:csc
#PBS -l nodes=1:ppn=8
#PBS -l mem=3072mb
#PBS -l walltime=2:00:00
#PBS -m bea
#PBS -M pmwestrich42@students.tntech.edu

#change to the program directory
cd ~/csc_4760/program4

#run the program
./main_omp 40000 400 8 >>./p_400_8.txt
