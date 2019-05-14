function [us,vs,is,io,ic]=getScaledIndex(u,v,s,w,h,r1,c1,cw)
% returns 1-based indices of descriptors for images with scale factor s

  % get scaled image dimensions
  ws=round(w*s);
  hs=round(h*s);

  us=min(ws,max(0,round(u*s)));
  vs=min(hs,max(0,round(v*s)));

  is=(vs-1)*ws+us;
  io=(v-1)*w+u;

  ic=(v-r1)*cw+(u-c1)+1;

  % return unique indices
  [is,iu]=unique(is);
  us=us(iu);
  vs=vs(iu);
  io=io(iu);
  ic=ic(iu);
end
