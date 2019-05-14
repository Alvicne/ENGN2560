function p=prepareFwBwMatching(p)

  % swap filenames
  tmp=p.Img2;
  p.Img2=p.Img1;
  p.Img1=tmp;
  
  tmp=p.fnIm2;
  p.fnIm2=p.fnIm1;
  p.fnIm1=tmp;
  
  p.mode='testing';
  
  % adapt BCD output filenames
  p.fnFlow=fullfile(p.pBcd,p.fnIm1);
  p.fnFlow=[p.fnFlow(1:end-4),'_back.png'];
  p.n_scales=0;
end
