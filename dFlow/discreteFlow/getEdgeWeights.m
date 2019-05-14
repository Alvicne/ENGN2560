function [wr,wc]=getEdgeWeights(E,g)

  [h,w]=size(E);

  wr=zeros(g.h,g.w);
  wc=zeros(g.h,g.w);
  
  u1_grid = sort(repmat(g.u,g.h,1));
  v1_grid = (repmat(g.v,g.w,1));
  i1_grid = sub2ind([h,w],v1_grid,u1_grid);

  E(E<0)=0;E(E>1)=1;
  E=round(E*10);
  
  mask=zeros(size(E));
  for i=1:g.stride-1
    mask(i1_grid+i*h)=i;
    wc(:)=wc(:)+E(i1_grid+i);
    wr(:)=wr(:)+E(i1_grid+i*h);
  end
  
  wc=wc./(g.stride-1);
  wr=wr./(g.stride-1);
end
