#!/bin/bash

#SBATCH -J unsupervisedEdge
#SBATCH -n 16
#SBATCH -t 40:00:00
#SBATCH --mem=105g
#SBATCH -e uEdge-%J.err
#SBATCH -o uEdge-%J.out

cd /users/guest443/scratch/temp/ENGN2560

matlab-threaded -nodisplay -r "restoredefaultpath; mainLoop; exit"
