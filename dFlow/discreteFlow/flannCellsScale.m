function [ res_out,u2_out,v2_out,d_out,tIdx,tNN ] = ...
  flannCellsScale( D,Ds,cell_width,cell_height,pFlann,g,maxFu,maxFv,dScale,scales )
%FLANNCELLS Builds search trees for disjunct cells and finds
% nearest neighbors for all grid points within max flow

maxCost=2^16;

% set distance type
flann_set_distance_type(pFlann.distance_type)

% get number of cells in u and v direction
n_cell_u = round( D{2}.w / cell_width );
n_cell_v = round( D{2}.h / cell_height );

fprintf('number of cells in u = %d, v = %d \n',n_cell_u,n_cell_v);

% get edges of the cells
edges_u = round(linspace( 1,D{2}.w,n_cell_u+1 ));
edges_v = round(linspace( 1,D{2}.h,n_cell_v+1 ));

% get centers of the cells
centers_u = edges_u(1:end-1)+round(diff(edges_u)/2);
centers_v = edges_v(1:end-1)+round(diff(edges_v)/2);

% get query descriptors for grid points
% use this section to query all cells for all descriptors
u1_grid = repmat(g.u,g.h,1);
v1_grid = sort(repmat(g.v,g.w,1));
i1_desc_grid = (v1_grid-1)*D{1}.w+(u1_grid-1)+1;
d1 = D{1}.descs(i1_desc_grid,:)';

[i_val,n]=getNumNeighbors(centers_u,centers_v,u1_grid,v1_grid,maxFu,maxFv);
nPerCell=ceil(pFlann.num_neighbors./n);

% loop variables
i_cell = 1;
r1 = 1; % first row of current block

% compute number of neighbors per cell
k=max(nPerCell);
maxNPart=max(nPerCell.*n);

d_out  = zeros(g.nP,pFlann.num_neighbors);
u2_out = zeros(g.nP,pFlann.num_neighbors);
v2_out = zeros(g.nP,pFlann.num_neighbors);
res_out = zeros(g.nP,pFlann.num_neighbors);

distCell = cell(n_cell_u*n_cell_v,1);
resCell = cell(n_cell_u*n_cell_v,1);
startIndices = cell(n_cell_u*n_cell_v,1);


nP=g.w*g.h;
iStart = ones(nP,1);

tIdx = 0;
tNN  = 0;

cellParams=zeros(n_cell_u*n_cell_v,4);

for r=1:n_cell_v
  
  % 1) get lower edge
  r2 = edges_v(r+1);
  
  % reset left edge
  c1=1;
  
  for c=1:n_cell_u
    
    % 2) get right edge
    c2 = edges_u(c+1);
    
    % get cell size
    cw  = c2-c1+1; % real cell width 
    ch  = r2-r1+1; % real cell height 
    
    cellParams(i_cell,:)=[r1,c1,cw,ch];
    
    % 3) get pixels inside the cell 
    [vv,uu] = meshgrid(r1:r2,c1:c2);
    
    % get indices of descriptors in scaled image and in the original image
    is=cell(1,numel(scales));
    for iS=1:numel(scales)
      [us,vs,is{iS},~,ic{iS}]=getScaledIndex(uu(:),vv(:),scales(iS),D{1}.w,D{1}.h,r1,c1,cw);
    end

    % 4) get indices of corresponding descriptors
    idx_flann_cell = (vv(:)-1)*D{2}.w+(uu(:)-1)+1;

    % 5) build search index
    train = D{2}.descs(idx_flann_cell,:)';
    
    % add descriptors from scaled image
    idxLookup=[1:numel(idx_flann_cell)]';
    for iS=1:numel(scales)
      train=[train,Ds{iS}.descs(is{iS},:)'];
      idxLookup=[idxLookup;ic{iS}];
    end
    %%
    tic;
    [index, parameters, ~] = flann_build_index(train, pFlann);
    tIdx = tIdx + toc; 

    % find enuogh nearest neighbors to fill all remaining proposals
    tic;
    [result, dist] = flann_search(index, d1(:,i_val(:,i_cell)), (1+numel(scales))*k, parameters);
    tNN = tNN + toc;
    
    % replace indices from scaled images with original indices
    result=idxLookup(result);
    
    % Discretization of distances
    dist=uint32(max(min(dist*dScale,2^16-1),0));
    
    distCell{i_cell}=dist;
    resCell{i_cell}=int32(result);
    startIndices{i_cell}=int32(iStart);
    
    % 6) update left edge
    c1 = c2+1;
    
    iStart(i_val(:,i_cell))=iStart(i_val(:,i_cell))+nPerCell(i_val(:,i_cell)); %i_cell*k;
    
    i_cell = i_cell+1;
    flann_free_index(index);
  end
  
  % update upper edge
  r1 = r2+1;
  
end

[results_,u2_res,v2_res,dists]=collectScaledParticlesMex(resCell,distCell,int32(i_val)',int32(nPerCell),int32(maxNPart),int32(cellParams'),startIndices,max(cellParams(:,3))*max(cellParams(:,4))+1,numel(scales)+1);
[~,ID]=sort(dists,1,'ascend');

% return the specified number of proposals
for i=1:size(dists,2)
  res_out(i,:)=results_(ID(1:pFlann.num_neighbors,i),i)';
  u2_out(i,:)=u2_res(ID(1:pFlann.num_neighbors,i),i)';
  v2_out(i,:)=v2_res(ID(1:pFlann.num_neighbors,i),i)';
  d_out(i,:) =dists(ID(1:pFlann.num_neighbors,i),i)';
end

assert(max(d_out(:))<maxCost)

end

