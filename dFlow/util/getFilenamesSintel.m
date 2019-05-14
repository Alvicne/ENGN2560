function params = getFilenamesSintel( params )
%GETFILENAMESSINTEL
% get Sintel dataset filenames

  % base filenames
  params.fnIm1 =sprintf('frame_%04d.png',params.shot);
  params.fnIm2 =sprintf('frame_%04d.png',params.shot+1);
  
  % result directories
  params.pBcd=fullfile(params.pRes,params.pImg,'bcd');
  if ~exist(params.pBcd,'dir'), mkdir(params.pBcd); end
  
  params.pL=fullfile(params.pRes,params.pImg,'labels');
  if ~exist(params.pL,'dir'), mkdir(params.pL); end
  
  params.pDm=fullfile(params.pRes,params.pImg,'deepmatches');
  if ~exist(params.pDm,'dir'), mkdir(params.pDm); end
  
  params.pClean=fullfile(params.pRes,params.pImg,'clean');
  if ~exist(params.pClean,'dir'), mkdir(params.pClean); end

  if params.refinement>0
    params.pEf=fullfile(params.pRes,params.pImg,'epicflow');
    if ~exist(params.pEf,'dir'), mkdir(params.pEf); end
  end
  
  % base filename including path
  params.fnImg1=fullfile(params.pRoot,params.pImg,params.fnIm1);
  params.fnImg2=fullfile(params.pRoot,params.pImg,params.fnIm2);
  
  % result of BCD
  params.fnFlow=fullfile(params.pBcd,params.fnIm1);
  
  % labels
  params.fnL   =fullfile(params.pL,params.fnIm1);
  params.fnL(end-3:end)='.txt';

  % edges for BCD
  params.pEdges = fullfile(params.pRes,params.pImg,'edges');
  params.fnEdges = fullfile(params.pEdges,params.fnIm1);
  if ~exist(params.pEdges,'dir')
    mkdir(params.pEdges); 
  end
  
  params.fnEdges(end-3:end)='.mat';

  % EpicFlow edges
  params.pEdgModels=fullfile('./edges/models'); 
  params.fnEfEdg=fullfile(params.pEdges,params.fnIm1);

  params.fnEfEdg=[params.fnEfEdg(1:end-4),'_edges.bin'];

end

