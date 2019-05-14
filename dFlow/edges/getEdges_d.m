function E = getEdges(I,model)

%   opts=edgesTrain();                % default options
%   opts.modelDir=p.pEdgModels;       % model will be in models/forest

%   opts.modelFnm=model;

%   model=edgesTrain(opts);

  E=edgesDetect_d(I{1},model);

end %function
