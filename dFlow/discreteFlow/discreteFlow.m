function [F,p] = discreteFlow(outFileName,model,fileName1,fileName2,Img1,Img2,pathResults,fnParameters,dataset)

%% parameters
disp('in discreteFlow')
load(fnParameters);
% 1=2;
disp(fnParameters);
p.mode = 'testing'; 
p.dSet = dataset;

p.pEfSrc = '/users/guest443/scratch/temp/ENGN2560/dFlow/external/EpicFlow_v1.00'; 

p.Img1 = Img1;
p.Img2 = Img2;
%% convert shot index to double if necessary
% if ischar(i_shot)
%   i_shot = str2double(i_shot);
% end

% p.shot = i_shot;
disp('discrete flow start+++++++++++++++++++++++++++++++++++++++++++++++++++++++')
% set Daisy parameters
pDaisy.R  =  p.daisy_R;  % radius of the descriptor
pDaisy.RQ =  p.daisy_RQ; % number of rings
pDaisy.TQ =  p.daisy_TQ; % number of histograms on each ring
pDaisy.HQ =  p.daisy_HQ; % number of bins of the histograms

pDaisy.SI =  p.daisy_SI; % spatial interpolation enable/disable
pDaisy.LI =  p.daisy_LI; % layered interpolation enable/disable
pDaisy.NT =  p.daisy_NT; % normalization type

% set FLANN parameters
pFlann.cores = 4;
pFlann.algorithm = 'kdtree';
pFlann.distance_type = 'manhattan';
pFlann.num_neighbors=p.nParticles-p.n_ns;
pFlann.random_seed = 1;
pFlann.trees = 4;

% set paths
% p.pRoot = pathDataset;
% p.pImg  = pathImages;
p.pRes  = pathResults;

% if strcmp(p.dSet,'sintel') || strcmp(p.dSet,'Sintel')
%   p=getFilenamesSintel(p);
% else
%   p=getFilenamesKitti12(p);
% end

% Info
% fprintf('\nProcessing:\t%s\n',p.fnImg1);

%% processing

% forward matching
  p.fnIm1 = fileName2;
  p.fnIm2 = fileName2;
  % result directories
  p.pBcd=fullfile(p.pRes,'bcd');
  if ~exist(p.pBcd,'dir'), mkdir(p.pBcd); end
  disp(p.pBcd);
  p.pL=fullfile(p.pRes,'labels');
  if ~exist(p.pL,'dir'), mkdir(p.pL); end
  disp(p.pL);
  p.pDm=fullfile(p.pRes,'deepmatches');
  if ~exist(p.pDm,'dir'), mkdir(p.pDm); end
  disp(p.pDm);
  p.pClean=fullfile(p.pRes,'clean');
  if ~exist(p.pClean,'dir'), mkdir(p.pClean); end
  disp(p.pClean);
  if p.refinement>0
    p.pEf=p.pRes;
%     if ~exist(p.pEf,'dir'), mkdir(p.pEf); end
  end
p.fnFlow = fullfile(p.pBcd,fileName1)
p.fnL = fullfile(p.pL,fileName1);
p.fnL(end-3:end)='.txt';
p.pEdges = fullfile(p.pRes,'edges');
p.fnEdges = fullfile(p.pEdges,fileName1);
if ~exist(p.pEdges,'dir')
  mkdir(p.pEdges); 
end
  
p.fnEdges(end-3:end)='.mat';

  % EpicFlow edges
p.pEdgModels=fullfile('../edges/models'); 
p.fnEfEdg=fullfile(p.pEdges,fileName1);

p.fnEfEdg=[p.fnEfEdg(1:end-4),'_edges.bin'];
disp(p.stride);

[p,F_bcd]=bcdFlow(model,p,pDaisy,pFlann,1);
disp('outliers++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++')
if p.outliers==1
  %No fw/bw check: simply remove small isolated segments
  pOutliers.fwBw = 0;
  F_bcd_bw=zeros(size(F_bcd));
  F_bcd=outlierRejection(F_bcd,F_bcd_bw,pOutliers);
  flow_write(F_bcd,fullfile(p.pClean,p.fnIm1));
elseif p.outliers==2
  pOutliers.fwBw = 1;
  % compute backward matches
  p_=prepareFwBwMatching(p);
  [p_,F_bcd_bw]=bcdFlow(model,p_,pDaisy,pFlann,0);

  % full outlier rejection
  F_bcd=outlierRejection(F_bcd,F_bcd_bw,pOutliers); 

  % save result
  flow_write(F_bcd,fullfile(p.pClean,p.fnIm1));
end
disp('write deep matches++++++++++++++++++++++++++++++++++++++++++++++++++++++')
% mimic DeepMatches
p=writeDeepmatches(p,F_bcd);
F = F_bcd;
p.fnEf = outFileName;
% post-processing
if p.refinement>0
  p=callEpicflow(p);
end

% rename temporary files
if exist(p.fnDm,'file')
  movefile(p.fnDm,[p.fnDm(1:end-21),'.txt'])
end
disp('tset out here!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!')
if p.refinement>0
  if exist(p.fnEf,'file')
%     filename = [p.fnEf(1:end-21),'.flo'];
    disp('out here ++++++++++++++++++++++++++++++++++++++++++++++++++++++++')
    disp(p.fnEf)
    disp(outFileName)
%     movefile(p.fnEf,outFileName);
%     fprintf('Flow file has been written to: %s\n',outFileName);
    F = readFlowFile(outFileName);
  end
end
 
