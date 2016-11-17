#!/bin/bash
#PBS -N pmw_p2_4
#PBS -W x=PARTITION:csc
#PBS -l nodes=1:ppn=4
#PBS -l mem=600mb
#PBS -l walltime=1:00:00
#PBS -m bea
#PBS -M pmwestrich42@students.tntech.edu

#change to the program directory
cd ~/csc_4760/program3

#run the program
./matrix_pthread 5000 4 >>./pthread2_4.txt
