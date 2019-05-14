function p = callEpicflow(p)
% calls EpicFlow refinement

  t=tic;
%   p.fnEf=p.pEf;
%   p.fnEf=[p.fnEf(1:end-4),sprintf('_%d',t),'.flo'];

  call=sprintf('%s/epicflow-static %s %s %s %s %s -p %f -n %f -k %f -i %f -a %f -g %f -d %f -s %f',...
                p.pEfSrc,p.Img1,p.Img2,p.fnEfEdg,p.fnDm,p.fnEf,...                
                p.efPrefnn,p.efNn,p.efK,p.efIter,p.efAlpha,p.efGamma,p.efDelta,p.efSigma);

  display( call );

  system(call);
              
end

