function [F1,F2] = outlierRejection (F1,F2,param)

% downscale
grid = getGrid(size(F1,1),size(F1,2),param.stride,param.stride);
Q1 = F1(grid.v,grid.u,:);
Q2 = F2(grid.v,grid.u,:);

% show before
if param.show
  figure,imshow(flow_to_color(Q1,param.max_flow));
  figure,imshow(flow_to_color(Q2,param.max_flow));
end

% process
if param.fwBw==1
  forwardBackwardConsistencyMex(Q1,Q2,param.stride,param.consistency_threshold);
  removeSmallSegmentsMex(Q1,param.similarity_threshold,param.min_segment_size);
  removeSmallSegmentsMex(Q2,param.similarity_threshold,param.min_segment_size);
else
  removeSmallSegmentsMex(Q1,param.similarity_threshold,param.min_segment_size);
end 

% show after
if param.show
  figure,imshow(flow_to_color(Q1,param.max_flow));
  figure,imshow(flow_to_color(Q2,param.max_flow));
end

% upscale
F1(grid.v,grid.u,:) = Q1;
F2(grid.v,grid.u,:) = Q2;
