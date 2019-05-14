####################################################################################
# This is free software; you can redistribute it and/or modify it under the        #
# terms of the GNU General Public License as published by the Free Software        #
# Foundation; either version 3 of the License, or any later version.               #
#                                                                                  #
# This code is distributed in the hope that it will be useful, but WITHOUT ANY     #
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A  #
# PARTICULAR PURPOSE. See the GNU General Public License for more details.         #
#                                                                                  #
# You should have received a copy of the GNU General Public License along with     #
# this code; if not, write to the Free Software Foundation, Inc., 51 Franklin      #
# Street, Fifth Floor, Boston, MA 02110-1301, USA                                  #
####################################################################################

This is the implementation of 'Discrete Optimization for Optical Flow' as
described in:

@INPROCEEDINGS{Menze2015GCPR,
  author = {Moritz Menze and Christian Heipke and Andreas Geiger},
  title = {Discrete Optimization for Optical Flow},
  booktitle = {German Conference on Pattern Recognition (GCPR)},
  year = {2015}
} 

The only difference is that we removed the edge weight from the pairwise 
potential since we found this improves the overall results slightly, thus
the numbers produced by this method are a little better compared to those
described in the paper.

Step 1: Download external dependencies
======================================

- To generate the diverse flow proposals we make use of FLANN:
  http://www.cs.ubc.ca/research/flann/#download
  Please get a copy of FLANN and make sure the matlab code and the
  mex-wrappers can be found. Adapt the FLANN dependencies to
  "flann/src/matlab" and "flann/build/src/matlab" in demo.m accordingly.

Step 2: Build the mex files
===========================

- In the root directory call "makeMexFiles" from Matlab.

- The code was developed and tested under Matlab 2013b and Ubuntu 14.04
  (64 bit). We do not provide support for compilation issues or other OS.

Step 3: Run the demo
====================

- In the root directory call 'demo' from Matlab.

Included dependencies:
======================

The following dependencies are included in this release.

- We use dense DAISY descriptors. The code can be found at
  http://cvlab.epfl.ch/software/daisy, but we include a copy in the
  "daisy_matlab" subdirectory for convenience.

  @ARTICLE{Tola2010PAMI,
    author = {E. Tola and V. Lepetit and P. Fua},
    title = {{DAISY: An Efficient Dense Descriptor Applied to Wide Baseline Stereo}},
    journal = {Transactions on Pattern Analysis and Machine Intelligence (PAMI)},
    year = {2010},
    month = {May},
    pages = {815--830},
    volume = {32},
    number = {5}
  } 

- The code for edge detection depends on Piotr Dollar's MATLAB Toolbox:
  http://vision.ucsd.edu/~pdollar/toolbox/doc/index.html
  We included the relevant parts in the "external" subdirectory.

- For variational postprocessing our code depends on EpicFlow. For convenience
  we provide a copy of Epicflow_v1.00 in the "external" subdirectory.

  @INPROCEEDINGS{Revaud2015CVPR,
    author                   = {Revaud, Jerome and Weinzaepfel, Philippe and Harchaoui, Zaid and Schmid, Cordelia},
    title                    = {{EpicFlow}: Edge-Preserving Interpolation of Correspondences for Optical Flow},
    booktitle                = {Computer Vision and Pattern	Recognition (CVPR)},
    year                     = {2015}
  }

- Edges are computed with Piotr Dollar's Structured Edge Detection Toolbox
  (https://github.com/pdollar/edges). A copy of the toolbox is contained
  in "external".

  @INPROCEEDINGS{Dollar2013ICCV,
    author = {Piotr Doll\'ar and C. Lawrence Zitnick},
    title = {Structured Forests for Fast Edge Detection},
    booktitle = {International Conference on Computer Vision (ICCV)},
    year = {2013},
  }

- We make use of the Middlebury optical flow code for interfacing.
  (http://vision.middlebury.edu/flow/code/flow-code-matlab.zip)
  A copy of the code is contained in "external".

  @ARTICLE{Scharstein2002IJCV,
    author = {Scharstein, Daniel and Szeliski, Richard},
    title = {A taxonomy and evaluation of dense two-frame stereo correspondence algorithms},
    journal = {International Journal of Computer Vision (IJCV)},
    year = {2002},
    pages = {7-42},
    volume = {47}
  }

Citation
========

If you find this project, data or code useful, we would be happy if you cite us:

@INPROCEEDINGS{Menze2015GCPR,
  author = {Moritz Menze and Christian Heipke and Andreas Geiger},
  title = {Discrete Optimization for Optical Flow},
  booktitle = {German Conference on Pattern Recognition (GCPR)},
  year = {2015}
}
