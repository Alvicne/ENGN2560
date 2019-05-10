#!/bin/bash

#SBATCH -J unsupervisedEdge
#SBATCH -n 16
#SBATCH -t 24:00:00
#SBATCH --mem=64g
#SBATCH -e uEdge-%J.err
#SBATCH -o uEdge-%J.out
module load opengl/nvidia-410.72
cd /users/guest443/ENGN2560/py_preprocessing 

python process_videos.py --video_list video_list_youtube_part1.txt --output_folder  /users/guest443/scratch/preprocessRes/ --type jpg

python match_pairs.py --pair_list /users/guest443/scratch/preprocessRes/car_motobike/car_motobike_pairs.txt --output_folder /users/guest443/scratch/preprocessRes/car_motobike --dm_bin ../bins/deepmatching_1.2.2_c++/deepmatching-static
