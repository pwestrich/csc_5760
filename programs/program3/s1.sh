#!/bin/bash
#PBS -N pmw_p3_s1
#PBS -W x=PARTITION:csc
#PBS -l nodes=1:ppn=1
#PBS -l mem=600mb
#PBS -l walltime=1:00:00
#PBS -m bea
#PBS -M pmwestrich42@students.tntech.edu

#change to the program directory
cd ~/csc_4760/program3

#run the program
./matrix_serial 5000 >>./serial1.txt
