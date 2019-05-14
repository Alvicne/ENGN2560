function [ Dzy,timeSpent ] = daisyDescriptors( p,I1 )
%DAISYDESCRIPTORS Computes DAISY descriptors

  tStart = tic;
  
  % compute all descriptors with custom settings
  o = 0; % which orientation to compute the descriptors
    
  for i=1:numel(I1)
    Dzy{i} = init_daisy(I1{i}, p.R, p.RQ, p.TQ, p.HQ, p.SI, p.LI, p.NT );
    Dzy{i}.descs = mex_compute_all_descriptors( ...
                                                Dzy{i}.H,...
                                                Dzy{i}.params,...
                                                Dzy{i}.ogrid(:,:,o+1),...
                                                Dzy{i}.ostable,...
                                                single(o) )';
  end
    
  timeSpent = toc(tStart);

end

