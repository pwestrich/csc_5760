#!/bin/bash
#PBS -N pmw+pX_SOMETHING
#PBS -W x=PARTITION:csc
#PBS -l nodes=1:ppn=CORES
#PBS -l mem=AMOUNTmb
#PBS -l walltime=TIME
#PBS -m bea
#PBS -M pmwestrich42@students.tntech.edu

#change to the program directory
cd ~/csc_4760/programX

#run the program
./PROGRAM ARGS >>./OUTFILE
