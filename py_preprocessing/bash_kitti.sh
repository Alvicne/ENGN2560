#!/bin/bash

#SBATCH -J unsupervisedEdge
#SBATCH -n 16
#SBATCH -t 40:00:00
#SBATCH --mem=32g
#SBATCH -e uEdge-%J.err
#SBATCH -o uEdge-%J.out

module load matlab/R2019a

cd /users/guest443/ENGN2560

python py_preprocessing/process_videos.py --video_list py_preprocessing/video_list_kitti.txt --output_folder /users/guest443/scratch/preprocessRes/kitti --type png --frames_per_video 6000 

python py_preprocessing/match_pairs.py --pair_list /users/guest443/scratch/preprocessRes/kitti/kitti_pairs.txt --output_folder /users/guest443/scratch/preprocessRes/kitti --dm_bin bins/deepmatching_1.2.2_c++/deepmatching-static

matlab-threaded -nodisplay -r "restoredefaultpath; mainLoop; exit"
