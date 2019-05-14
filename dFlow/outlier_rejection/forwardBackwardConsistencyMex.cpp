#include "mex.h"
#include <stdint.h>
#include <math.h>

using namespace std;

inline uint32_t getAddressOffsetImage (const int32_t& u,const int32_t& v,const int32_t& width) {
  return v*width+u;
}

inline double* getValue (double* F,const int *dims,const int32_t& i,const int32_t& j,const int32_t& k) {
  return F + i + j * dims[0] + k * (dims[0] * dims[1]);
}

void consistencyCheck (double* F1,double* F2,const int *dims,double s,double t,int32_t u1,int32_t v1) {
  
  // get image width and height
  const int w = dims[1];
  const int h = dims[0];
  
  double f1u   = *getValue(F1,dims,v1,u1,0);
  double f1v   = *getValue(F1,dims,v1,u1,1);
  bool   f1val = *getValue(F1,dims,v1,u1,2)>0.5;
  
  if (!f1val)
    return;
  
  int32_t u2 = (int32_t)((s*u1+f1u)/s);
  int32_t v2 = (int32_t)((s*v1+f1v)/s);
  
  if (u2<0  || v2<0 || u2>=w || v2>=h) {
    *getValue(F1,dims,v1,u1,0) = 0;
    *getValue(F1,dims,v1,u1,1) = 0;
    *getValue(F1,dims,v1,u1,2) = 0;
    return;
  }
  
  if (f1val) {
    double f2u   = *getValue(F2,dims,v2,u2,0);
    double f2v   = *getValue(F2,dims,v2,u2,1);
    bool   f2val = *getValue(F2,dims,v2,u2,2)>0.5;
    
    if (!f2val) {
      *getValue(F1,dims,v1,u1,0) = 0;
      *getValue(F1,dims,v1,u1,1) = 0;
      *getValue(F1,dims,v1,u1,2) = 0;
      return;
    }
    
    double du = f1u+f2u;
    double dv = f1v+f2v;
    double err = sqrt(du*du+dv*dv);
    if (err>t) {
      *getValue(F1,dims,v1,u1,0) = 0;
      *getValue(F1,dims,v1,u1,1) = 0;
      *getValue(F1,dims,v1,u1,2) = 0;
      return;
    }
  }
}

void forwardBackwardConsistency (double* F1,double* F2,const int *dims,double stride,double threshold) {
 
  // get image width and height
  const int width  = dims[1];
  const int height = dims[0];
  
  // for all pixels do
  for (int32_t u=0; u<width; u++)
    for (int32_t v=0; v<height; v++)
      consistencyCheck(F1,F2,dims,stride,threshold,u,v);
  
  // for all pixels do
  for (int32_t u=0; u<width; u++)
    for (int32_t v=0; v<height; v++)
      consistencyCheck(F2,F1,dims,stride,threshold,u,v);
}

void mexFunction (int nlhs,mxArray *plhs[],int nrhs,const mxArray *prhs[]) {

    // check for proper number of arguments
    if(nrhs!=4) 
        mexErrMsgTxt("Four inputs required (F1,F2,stride,threshold).");
    if(nlhs!=0) 
        mexErrMsgTxt("No output required.");
    
    // check for proper argument types and sizes
    if(!mxIsDouble(prhs[0]) || mxGetNumberOfDimensions(prhs[0])!=3)
        mexErrMsgTxt("Input F1 must be a double 3-channel image.");
    if(!mxIsDouble(prhs[1]) || mxGetNumberOfDimensions(prhs[1])!=3)
        mexErrMsgTxt("Input F2 must be a double 3-channel image.");
    if(!mxIsDouble(prhs[2]) || mxGetN(prhs[2])*mxGetM(prhs[2])!=1)
        mexErrMsgTxt("Input stride must be a double scalar.");
    if(!mxIsDouble(prhs[3]) || mxGetN(prhs[3])*mxGetM(prhs[3])!=1)
        mexErrMsgTxt("Input threshold must be a double scalar.");
    
    // get pointers
    double*   F1        =   (double*)mxGetPr(prhs[0]);
    double*   F2        =   (double*)mxGetPr(prhs[1]);
    double    stride    = *((double*)mxGetPr(prhs[2]));
    double    threshold = *((double*)mxGetPr(prhs[3]));
    const int *dims     =  (const int*)mxGetDimensions(prhs[0]);

    // do computation
    forwardBackwardConsistency(F1,F2,dims,stride,threshold);
}
