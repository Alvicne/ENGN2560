% run flow

% read the images
addpath('/tools');
% imgPath = '/users/guest438/scratch/ENGN2560/data/motorbike/';
% imgDir = dir([imgPath '*.jpg']);
fid = fopen('/users/guest438/scratch/ENGN2560/res/aeroplane/aeroplane_pairs.txt', 'r');
pairlist =textscan(fid,'%s %s');
fclose(fid);
randId = randperm(length(pairlist{1}),1000);
lambda = 10; 
dsum = 0;
for i = 1:length(randId)
    I1 = imread(pairlist{1}{randId(i)});
    I2 = imread(pairlist{2}{randId(i)});
    frame1 = double(rgb2gray(I1));
    frame2 = double(rgb2gray(I2));

    frame1 = imresize(frame1,1/4);
    frame2 = imresize(frame2,1/4);
    % parameters
 % you can try differnt value for different cases
    [h,w] = size(frame1);
% estimate optical flow using 
    uv = estimateHSflow(frame1,frame2,lambda);
    U = uv(:,:,1);
    V = uv(:,:,2);
    d = sum(sum(sqrt(U.^2+V.^2)))/(h*w);
    dsum = dsum+d;
end
average_disp = dsum/1000;
disp(average_disp);

%%
% RubberWhale
% frame1 = imread('00000003.jpg');
% frame2 = imread('00000004.jpg');
% gtflow = readFlowFile('data\other-gt-flow\Urban3\flow10.flo');

% % Grove2
% frame1 = imread('data\other-data\Grove2\frame10.png');
% frame2 = imread('data\other-data\Grove2\frame11.png');
% gtflow = readFlowFile('data\other-gt-flow\Grove2\flow10.flo');
% 
% % Grove3
% frame1 = imread('data\other-data\Grove3\frame10.png');
% frame2 = imread('data\other-data\Grove3\frame11.png');
% gtflow = readFlowFile('data\other-gt-flow\Grove3\flow10.flo');
% 
% % Hydrangea
% frame1 = imread('data\other-data\Hydrangea\frame10.png');
% frame2 = imread('data\other-data\Hydrangea\frame11.png');
% gtflow = readFlowFile('data\other-gt-flow\Hydrangea\flow10.flo');


% more data are included in the data folder

% gtflow(gtflow>1e9) = NaN;
%%











