#include <mex.h>
#include <iostream>
#include <cstring>
#include <cassert>

using namespace std;

#define endll endl<<endl
#define fr(i,a,b) for(int i=a;i<=b;++i) 

/* *************** *
 *  aux functions  *
 * *************** */
// a 2D matrix of m x n integers
template <typename T> T **create2d(int m,int n){
	T **tmp=new T*[m];
	fr(i,0,m-1)
		tmp[i]=new T[n];
	return tmp;
}

// delete a 2D matrix
template <typename T> void delete2d(T** mat,const int& m,const int& n){
  fr(i,0,m-1){
    delete[] mat[i];
  }
  delete[] mat; 
  mat=NULL;
}

// This mexFunction efficiently copies proposals to indivisual elements of
// the resulting matrices
// To be able to copy chunks of memory efficiently, the input has to be
// odered as nProposals x nPoints
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
  
  // check number of arguments
  if (nrhs!=9)
    mexErrMsgTxt("9 inputs required: cell(result), cell(dist), i_val, nPerCell, maxNParticles, cellParameters, startIndices, maxIdxResult, nScales");
  if (nlhs!=4)
    mexErrMsgTxt("4 outputs required: R, U2, V2, D");

  // check data types
  if (!mxIsCell(prhs[0]))
    mexErrMsgTxt("prhs[0] must be a cell");
  if (!mxIsCell(prhs[1]))
    mexErrMsgTxt("prhs[1] must be a cell");
  if (!mxIsCell(prhs[6]))
    mexErrMsgTxt("prhs[6] must be a cell");
  
  // get number of cells
  int nC = mxGetNumberOfElements(prhs[0]); 
  
  // get number of points
  const mwSize *dims=mxGetDimensions(prhs[2]);
 	int nP=dims[1];
 
  int* i_val=(int*)mxGetData(prhs[2]);
  
  // get max number of particles per cell
  int *nPerCell=(int*)mxGetData(prhs[3]);
  int maxNPerCell=0;
  fr(j,0,nC-1)
    maxNPerCell=max(maxNPerCell,nPerCell[j]);
  
  // get max number of drawn particles
  int maxNPart=mxGetScalar(prhs[4]);
  
  // get cell parameters
  int* cellParams=(int*)mxGetData(prhs[5]);
  
  // get the maximum value of the result indices for hashing
  int resMax=mxGetScalar(prhs[7])+1;
  char **hash=create2d<char>(nP,resMax);
  
  int nScales=mxGetScalar(prhs[8]);
    
  // create output matrices
  mwSize outDims[2];
  outDims[0]=maxNPart;
  outDims[1]=nP;
	
  mxArray* R  = mxCreateNumericArray(2,outDims,mxINT32_CLASS,mxREAL);
	mxArray* U2 = mxCreateNumericArray(2,outDims,mxINT32_CLASS,mxREAL);
	mxArray* V2 = mxCreateNumericArray(2,outDims,mxINT32_CLASS,mxREAL);
	mxArray* D = mxCreateNumericArray(2,outDims,mxINT32_CLASS,mxREAL);
  
  // init distances
  int *ptrD=(int*)mxGetData(D);
  fr(i,0,outDims[0]-1)
    fr(j,0,outDims[1]-1)
      ptrD[i*outDims[1]+j]=65536;
     
  // pointers to output
  int *ptrR, *ptrU2, *ptrV2;
  int *start;
  
  int *ptrR_,*ptrD_;
  int idxP,idxCheck;
  
  // move particles for each point
  fr(iC,0,nC-1){// all cells
    mxArray* resCell=mxGetCell(prhs[0],iC);
    mxArray* disCell=mxGetCell(prhs[1],iC);
    mxArray* staCell=mxGetCell(prhs[6],iC);
    start=((int*)mxGetData(staCell));
    ptrR_=((int*)mxGetData(resCell));
    ptrD_=((int*)mxGetData(disCell));

    fr(iP,0,nP-1){// all points
      if(i_val[iP*nC+iC]){
        // set pointers to output
        ptrR=((int*)mxGetData(R))+iP*outDims[0]+start[iP]-1; // set pointer to first element in col
        ptrD=((int*)mxGetData(D))+iP*outDims[0]+start[iP]-1;
        ptrU2=((int*)mxGetData(U2))+iP*outDims[0]+start[iP]-1;
        ptrV2=((int*)mxGetData(V2))+iP*outDims[0]+start[iP]-1;
        
        idxP=0;idxCheck=0;
        memset(hash[iP],0,resMax*sizeof(char));
        while(idxP<nPerCell[iP]){
          if(ptrR_[idxCheck]>resMax) printf("iC: %d, iP: %d, idxP: %d, idxCheck: %d\n",iC,iP,idxP,idxCheck);
          mxAssert(ptrR_[idxCheck]<resMax,"");
          
          if(hash[iP][ptrR_[idxCheck]]==0){
            ptrR[idxP]=ptrR_[idxCheck];
            ptrD[idxP]=ptrD_[idxCheck];
            hash[iP][ptrR_[idxCheck]]=1;
            
            // compute resulting image coordinates
            ptrU2[idxP]=cellParams[iC*4+1]+(ptrR_[idxCheck]-1)%cellParams[iC*4+2];
            ptrV2[idxP]=cellParams[iC*4+0]+(ptrR_[idxCheck]-1)/cellParams[iC*4+2];
            
            idxP++;
          }
          idxCheck++;
        }
                    
        // increment pointers to input matrices
        ptrR_+=maxNPerCell*nScales;
        ptrD_+=maxNPerCell*nScales;
      }      
    }// all points
  }// all cells
  
  // assign pointers to output
  plhs[0]=R;
	plhs[1]=U2;
	plhs[2]=V2;
	plhs[3]=D;
  
  delete2d<char>(hash,nP,resMax);
}  
