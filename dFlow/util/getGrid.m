function [ grid ] = getGrid( imgHeight, imgWidth, stride, minMargin )
%GRID Computes grid coordinates
% based on image size, stride and margin towards image boundary

  grid.stride = stride;
  
  % available sidelength
  height = imgHeight - 2*minMargin;
  width  = imgWidth  - 2*minMargin;

  grid.h  = floor(height / stride) + 1;
  grid.w  = floor(width / stride) + 1;
  grid.nP = grid.h*grid.w;

  grid.dh_margin = mod(height,stride);
  grid.dw_margin = mod(width,stride);

  grid.offV = minMargin + floor(grid.dh_margin/2.0);
  grid.offU = minMargin + floor(grid.dw_margin/2.0);
  
  grid.u = (grid.offU:grid.stride:imgWidth-minMargin)';
  grid.v = (grid.offV:grid.stride:imgHeight-minMargin)';
  
  grid.c = (1:grid.w)';
  grid.r = (1:grid.h)';
end

