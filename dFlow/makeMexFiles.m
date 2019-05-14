% compile all mex-wrappers for DiscreteFlow

mex util/collectScaledParticlesMex.cpp -outdir build
mex util/particlesFromNeighborsMex.cpp CXXFLAGS='$CXXFLAGS -std=c++0x -fPIC' -outdir build

mex outlier_rejection/removeSmallSegmentsMex.cpp -outdir build
mex outlier_rejection/forwardBackwardConsistencyMex.cpp -outdir build

mex bcd/bcdMex.cpp CXXFLAGS='$CXXFLAGS -std=c++0x -m64 -O3 -fPIC -msse4' -lpng -lz LDFLAGS='$LDFLAGS -fopenmp -shared' -outdir build
mex daisy_matlab/mex_compute_all_descriptors.cpp -outdir build
