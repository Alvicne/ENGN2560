function [p,Fe]=bcdFlow(model,p,pDaisy,pFlann,forward)

% load images
I{1} = imread(p.Img1);
I{2} = imread(p.Img2);
% disp(I{1});
% disp(I{2});
[h,w,~] = size(I{1});

% scale images
s0_2=getSigma(2,0,0.5,3);
gks=fspecial('gaussian',6,s0_2);
S0_2=imfilter(I{2},gks,'replicate');
I_s{1}=imresize(S0_2,0.75);
    
% init grid
g = getGrid(h,w,p.stride,p.stride);
    
u1_grid = repmat(g.u,g.h,1);
v1_grid = sort(repmat(g.v,g.w,1));
    
%% compute all descriptors
[Dzy,tDaisy] = daisyDescriptors(pDaisy,I);
[Dzy_s(1),~] = daisyDescriptors(pDaisy,I_s(1));

%% build kd trees for disjunct cells
if p.n_scales<1
  scales=[];
else
  scales=0.75;
end

[~,u2,v2,dis,t_idx,t_nn] = flannCellsScale(Dzy,Dzy_s,50,50,pFlann,g,250,200,p.dis_scale,scales);
    
%% sample particles from neighbors
[u2,v2,dis]=particlesFromNeighborsMex(int32(u2),... 
                                      int32(v2),... 
                                      int32(dis),...
                                      int32(u1_grid),...
                                      int32(v1_grid),...
                                      Dzy,...
                                      g,...
                                      int32(p.n_radius),...
                                      int32(p.n_ns),...
                                      int32(p.dis_scale));

    
%% get edge weights

% As opposed to the description in the paper, edge weights are set to zero.
% We found this yields better overall performance.
wr = zeros(g.h,g.w);
wc = zeros(g.h,g.w);

    
%% mex interface to bcd

% truncate data term
d=int32(min(dis,p.dis_tau*p.dis_scale));

[Fu,Fv,Fi]=bcdMex(int32(u1_grid-1),int32(v1_grid-1),u2-1,v2-1,d,int32(wr),int32(wc),p.bcd_tau,p.bcd_weight,p.bcd_alpha,h,w,p.fnFlow,p.fnL);
Fe=double(cat(3,Fu,Fv,Fi));

% Compute original EpicFlow edges for post-processing
if forward
  E = getEdges_d(I,model);

  if forward
    fid=fopen(p.fnEfEdg,'wb'); 
    fwrite(fid,transpose(E),'single'); 
    fclose(fid);
  end
end

end % function
