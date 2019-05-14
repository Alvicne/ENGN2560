function [i,n]=getNumNeighbors(centers_u,centers_v,u1_grid,v1_grid,maxFu,maxFv)
% returns the number of cells each of the grid points falls into

  c_u=repmat(centers_u,1,numel(centers_v));
  c_v=sort(repmat(centers_v,1,numel(centers_u)));

  % get distances to all centers
  d_u=[u1_grid,-ones(numel(u1_grid),1)]*[ones(1,numel(c_u));c_u];
  d_v=[v1_grid,-ones(numel(v1_grid),1)]*[ones(1,numel(c_v));c_v];
  
  % find distances lower than abs(maxFlow)
  i_u=abs(d_u)<maxFu;
  i_v=abs(d_v)<maxFv; 
  
  i=i_u&i_v;
  n=sum(i,2);
  
end