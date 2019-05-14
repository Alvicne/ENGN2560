/* mex-wrapper for mrf optimization via block coordinate descent */
#include <mex.h>
#include <iostream>
#include <cstring>
#include <cassert>

#include "bcd.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  // check number of arguments
  if (nrhs!=14)
    mexErrMsgTxt("14 inputs required: u1_,v1_,u2_,v2_,d_,wr_,wc_,trun,weight,alpha,h,w,fnFlow,fnLabels");
  if (nlhs!=3)
    mexErrMsgTxt("3 outputs required: Fu,Fv,Fi");
  
  // assert input types
  if (!mxIsClass(prhs[0],"int32"))
    mexErrMsgTxt("prhs[0] must of type int32");
  if (!mxIsClass(prhs[1],"int32"))
    mexErrMsgTxt("prhs[1] must of type int32");
  if (!mxIsClass(prhs[2],"int32"))
    mexErrMsgTxt("prhs[2] must of type int32");
  if (!mxIsClass(prhs[3],"int32"))
    mexErrMsgTxt("prhs[3] must of type int32");
  if (!mxIsClass(prhs[4],"int32"))
    mexErrMsgTxt("prhs[4] must of type int32");
  if (!mxIsClass(prhs[5],"int32"))
    mexErrMsgTxt("prhs[5] must of type int32");
  if (!mxIsClass(prhs[6],"int32"))
    mexErrMsgTxt("prhs[6] must of type int32");
  
  printf("\n");
  
  // get pointers to input data
  int* u1_=(int*)mxGetData(prhs[0]);
  int* v1_=(int*)mxGetData(prhs[1]);
    
  const mwSize* dims_u2 = mxGetDimensions(prhs[2]);
  int l=dims_u2[1];
  printf("nPoints: %d, nLabels: %d\n",(int)dims_u2[0],l);
  
  int* u2_=(int*)mxGetData(prhs[2]);
  int* v2_=(int*)mxGetData(prhs[3]);
  int* d_ =(int*)mxGetData(prhs[4]);
  int* wr_=(int*)mxGetData(prhs[5]);
  int* wc_=(int*)mxGetData(prhs[6]);
  
  const mwSize* dims_wr = mxGetDimensions(prhs[5]);
  int m=dims_wr[0];
  int n=dims_wr[1];
  printf("height (m): %d, width (n): %d\n",m,n);
  
  // get parameters
  int trun=(int)mxGetScalar(prhs[7]);
  float weight=(float)mxGetScalar(prhs[8]);
  float alpha=(float)mxGetScalar(prhs[9]);
  int h=(int)mxGetScalar(prhs[10]);
  int w=(int)mxGetScalar(prhs[11]);
  int type=1;
  printf("truncation pw: %d, smoothness weight: %f, alpha: %f\n",trun,weight,alpha);
  
  if(mxIsChar(prhs[12])!=1)
    mexErrMsgTxt("prhs[12] must be a string.");
  if(mxIsChar(prhs[13])!=1)
    mexErrMsgTxt("prhs[13] must be a string.");
  
  char *fnFlow;
  size_t buflen;
  // get the length of the input string 
  buflen=(mxGetM(prhs[12]) * mxGetN(prhs[12])) + 1;
  // copy the string data from prhs[0] into a C string input_ buf.
  fnFlow=mxArrayToString(prhs[12]);
  
  char *fnLabels;
  // get the length of the input string 
  buflen=(mxGetM(prhs[13]) * mxGetN(prhs[13])) + 1;
  // copy the string data from prhs[0] into a C string input_ buf.
  fnLabels=mxArrayToString(prhs[13]);
  
  /* ** interface to BCD ** */
  
  // instantiate the class
  mrfGrid mrf(m,n,l);
  mrf.m2=h;
  mrf.n2=w;
  // assign image coordinates, edge weights, unary costs and proposals
  fr(i,0,m-1)
		fr(j,0,n-1){
			mrf.u1[i][j]=u1_[i*n+j];
      mrf.v1[i][j]=v1_[i*n+j];
      
      // Edge weights are fixed
      mrf.wr[i][j]=weight*100.0;
      mrf.wc[i][j]=weight*100.0;
      
 			fr(k,0,l-1){
 				mrf.d[i][j][k]=d_[i*n+j+k*m*n]*(1-weight);
        mrf.u2[i][j][k]=u2_[i*n+j+k*m*n];
        mrf.v2[i][j][k]=v2_[i*n+j+k*m*n];
 			}
		}
  
  mrf.imgIdx=1;
  mrf.pathData=string("bcd/data");
  mrf.tw=trun;
  mrf.type=type;

  // prepare optimization
  mrf.hashParticles(25);

  mrf.hashing=true;
  int** e=mrf.BCD();
  mrf.saveFlow(fnFlow);
  mrf.saveResult(fnLabels);
 
  // create output
  const mwSize outDims[]={h,w};
  plhs[0]=mxCreateNumericArray(2,outDims,mxINT32_CLASS,mxREAL);
  plhs[1]=mxCreateNumericArray(2,outDims,mxINT32_CLASS,mxREAL);
  plhs[2]=mxCreateNumericArray(2,outDims,mxINT32_CLASS,mxREAL);
 
  // Debug:
  const mwSize* dims_out = mxGetDimensions(plhs[0]);
  int m3=dims_out[0];
  int n3=dims_out[1];
  printf("height (m3): %d, width (n3): %d\n",m3,n3);

  int **Fu=NULL; int **Fv=NULL; int **Fi=NULL;
  printf("getting flow\n");

  Fu = create2d(h,w);  	
  Fv = create2d(h,w);
  Fi = create2d(h,w);

  mrf.getFlow(Fu,Fv,Fi);

  int* fu=(int*)mxGetData(plhs[0]);
  int* fv=(int*)mxGetData(plhs[1]);
  int* fi=(int*)mxGetData(plhs[2]);
 
  fr(r,0,h-1)
    fr(c,0,w-1){
     fu[c*h+r]=Fu[r][c];
     fv[c*h+r]=Fv[r][c];
     fi[c*h+r]=Fi[r][c];
    }

  delete2d(Fu,h,w);
  delete2d(Fv,h,w);
  delete2d(Fi,h,w);
 
  mrf.deleteArrays();
  
}
