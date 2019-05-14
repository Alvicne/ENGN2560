function p=writeDeepmatches(p,F_bcd)

  Fu=F_bcd(:,:,1);
  Fv=F_bcd(:,:,2);

  iV=find(F_bcd(:,:,3));

  fu=Fu(iV);
  fv=Fv(iV);

  [r1,c1]=ind2sub(size(Fu),iV);

  r2=r1+fv;
  c2=c1+fu;

  quality=ones(size(r1));

  matches=[c1'-1;r1'-1;c2'-1;r2'-1;quality'];

  t=tic;
  p.fnDm=fullfile(p.pDm,p.fnIm1);
  p.fnDm=[p.fnDm(1:end-4),sprintf('_%d',t),'.txt'];
  fh=fopen(p.fnDm,'w');
  fprintf(fh,'%d %d %d %d %f\n',matches);
  fclose(fh);

end
