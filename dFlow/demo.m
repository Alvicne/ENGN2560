% This script demonstrates the usage of discreteFlow.m to run 'Discrete
% Optimization for Optical Flow' as descibed in our 2015 GCPR paper
clear variables; close all; clc;

%% paths to FLANN dependencies (adapt to your needs, cf. README)
addpath('flann-1.8.4-src/src/matlab')
addpath('flann-1.8.4-src/build/src/matlab')

%% set local paths to sub-projects included with this project
setup_paths

%% set paths to data - MPI Sintel
pathDataset  = './data';
pathImages   = 'sintel/testing/final/ambush_1';
pathResults  = './results';
dataset      = 'Sintel'; 
fnParameters = './parameters/sintel_parameters.mat'; 

%% set paths to data - KITTI
% pathDataset  = './data';
% pathImages   = 'kitti12/testing/colored_0';
% pathResults  = './results';
% dataset      = 'Kitti'; 
% fnParameters = './parameters/kitti_parameters.mat';
% 
mode = 'testing';

%% start processing
[F,p] = discreteFlow(pathDataset,pathImages,pathResults,fnParameters,dataset,mode,1);

%% show result
figure,imshow(imread(p.fnImg1));
figure,imshow(flowToColor(F));
