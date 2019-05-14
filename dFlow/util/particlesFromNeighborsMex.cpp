#include <mex.h>
#include <iostream>
#include <cstring>
#include <cassert>

#include <random>

using namespace std;

#define endll endl<<endl
#define fr(i,a,b) for(int i=a;i<=b;++i) 

struct Grid{
  int w;
  int h;
  int iw;
  int ih;
};

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

// a 3D matrix of m x n x o integers
template <typename T> T ***create3d(int m,int n,int o){
	T ***tmp=new T**[m];
	fr(i,0,m-1)
		tmp[i]=create2d<T>(n,o);
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

/* *************** *
 *  main function  *
 * *************** */
void sampleFromNeighbors(
        int *u1_,int *v1_,
        int *u0,int *v0,
        float *d0,float *d1,
        int nDesc,int sDesc,
        const Grid& g,int radius,
        int nPart,int n,int dScale,
        int *u1,int *v1,
        int *d)
{
  int nP=g.h*g.w;
  int nBins=2*radius+1;
  int rad1=radius+1;
  assert(nBins%2!=0);
  short **H=create2d<short>(g.ih,g.iw);
  
  printf("nPoints: %d, nPart: %d\n",nP,nPart);
  printf("nDesc: %d, sDesc: %d\n",nDesc,sDesc);
  
  int nN; // number of sampled neighbors
  int ug,vg,un,vn; // current grid point and neighbor
  int u1n,v1n;
  int iNP; // the index where a particle shall be stored
  int idxG,idxP,idxI0,idxI1; // indices in grid and image coordinates
  int du,dv;
  int nCol=0;
  float dist;
  
  fr(ip,0,nP-1){//all points
  
    // init hash table of already drawn particles
    fr(ir,0,g.ih-1)
      memset(H[ir],0,g.iw*sizeof(short));
    
    // get current grid coordinates
    vg=floor(ip/g.w);
    ug=ip%g.w;

    // fill hash table with already drawn particles
    fr(iPart,0,nPart-1){//all existing particles
      assert(v1_[ip+iPart*nP]-1>=0 && v1_[ip+iPart*nP]-1<g.ih);
      assert(u1_[ip+iPart*nP]-1>=0 && u1_[ip+iPart*nP]-1<g.iw);
      H[v1_[ip+iPart*nP]-1][u1_[ip+iPart*nP]-1]=1;
    }//all existing particles
     
    // sample particles from neighboring grid points
    std::default_random_engine generator;
    std::normal_distribution<double> dist_u(ug,radius);
    std::normal_distribution<double> dist_v(vg,radius);
    nN=0;
    iNP=nPart;
     
    while(nN<n){
      un=round(dist_u(generator));

      if(un>=0&&un<g.w){
        vn=round(dist_v(generator));

        if(vn>=0&&vn<g.h){
          // idx of the drawn neighbor (in grid coordinates)
          idxG=vn*g.w+un;
          
          // idx of the next particle to be checked at the neighboring point
          idxP=0;
          
          // get difference between the grid point and the drawn neighbor 
          // in full img coordinates
          du=u0[ip]-u0[idxG];
          dv=v0[ip]-v0[idxG];
          
          u1n=min(g.iw-1,max(0,u1_[idxG+idxP*nP]-1+du));
          v1n=min(g.ih-1,max(0,v1_[idxG+idxP*nP]-1+dv));
          
          // check hash table
          while(H[v1n][u1n]&&idxP<nPart-1){
            nCol++;
            idxP++;
            
            u1n=min(g.iw-1,max(0,u1_[idxG+idxP*nP]-1+du));
            v1n=min(g.ih-1,max(0,v1_[idxG+idxP*nP]-1+dv));
          }
          
          if(idxP==nPart-1) continue; 
          
          // compute distance
                    
          // idx in full image coordinates
          idxI0=(v0[ip]-1)*g.iw+(u0[ip]-1);
          idxI1=v1n*g.iw+u1n;
          
          dist=0;
          fr(id,0,sDesc-1)
            dist+=abs(d0[idxI0+id*nDesc]-d1[idxI1+id*nDesc]);

          // save valid particle
          u1[ip+iNP*nP]=u1n+1; // conversion to matlab coordinates
          v1[ip+iNP*nP]=v1n+1;
          d [ip+iNP*nP]=dist*dScale;
          
          // set hash entry
          H[v1n][u1n]=1;
          
          // increment counters
          nN++;
          iNP++;
        }
      }
    }//while nN<n 
  }//all points
  delete2d(H,g.ih,g.iw);
  printf("Number of collisions: %d\n",nCol);
}


/* ************************************************************ *
 *  samples optical flow particles from neighboring gridpoints  *
 * ************************************************************ */

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  // check number of arguments
  if (nrhs!=10)
    mexErrMsgTxt("10 inputs required: u2_,v2_,dis_,u1_grid,v1_grid,d_,g,radius,n,dScale");
  if (nlhs!=3)
    mexErrMsgTxt("3 outputs required: u2,v2,d");
  
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
  if(!mxIsCell(prhs[5]))
    mexErrMsgTxt("prhs[5] must be a cell");
  
  // get pointers to input data
  int* u1_=(int*)mxGetData(prhs[0]);
  const mwSize* dims = mxGetDimensions(prhs[0]);
  int nPart=dims[1];
  
  int* v1_=(int*)mxGetData(prhs[1]);
  int* d_ =(int*)mxGetData(prhs[2]);
  int* u0 =(int*)mxGetData(prhs[3]);
  int* v0 =(int*)mxGetData(prhs[4]);
  mxArray *D0=mxGetCell(prhs[5],0);
  mxArray *D1=mxGetCell(prhs[5],1);
  
  int radius=(int)mxGetScalar(prhs[7]);
  int n     =(int)mxGetScalar(prhs[8]);
  int scale =(int)mxGetScalar(prhs[9]);
  printf("nParticles: %d, radius: %d, n: %d, scale: %d\n",nPart,radius,n,scale);
 
  Grid g;
  
  // access fields of descriptor struct
  if(!mxIsStruct(D0)) mexErrMsgTxt("D0 must be a struct");
  mxArray *ih_=mxGetField(D0,0,"h");
  g.ih=(int)mxGetScalar(ih_);
  mxArray *iw_=mxGetField(D0,0,"w");
  g.iw=(int)mxGetScalar(iw_);
  printf("ih: %d, iw: %d\n",g.ih,g.iw);
  mxArray *d0_=mxGetField(D0,0,"descs");
  float *d0=(float*)mxGetData(d0_);
  mxArray *d1_=mxGetField(D1,0,"descs");
  float *d1=(float*)mxGetData(d1_);
  const mwSize* descDims = mxGetDimensions(d0_);
  printf("nDesc: %d, sDesc: %d\n",descDims[0],descDims[1]);
 
  // access elements of grid struct 
  mxArray *gh_=mxGetField(prhs[6],0,"h");
  g.h=(int)mxGetScalar(gh_);
  mxArray *gw_=mxGetField(prhs[6],0,"w");
  g.w=(int)mxGetScalar(gw_);
  int nP=g.h*g.w;
  printf("gh: %d, gw: %d\n",g.h,g.w);
  
  // declare output matrices
  mwSize outDims[2]={nP,nPart+n};
  mxArray* u1_mA=mxCreateNumericArray(2,outDims,mxINT32_CLASS,mxREAL);
  mxArray* v1_mA=mxCreateNumericArray(2,outDims,mxINT32_CLASS,mxREAL);
  mxArray* d_mA =mxCreateNumericArray(2,outDims,mxINT32_CLASS,mxREAL);
  
  int *u1=(int*)mxGetData(u1_mA);
  int *v1=(int*)mxGetData(v1_mA);
  int *d =(int*)mxGetData(d_mA);
  
  // copy particles
  fr(i,0,nPart-1){
    memcpy(u1+i*nP,u1_+i*nP,nP*sizeof(int));
    memcpy(v1+i*nP,v1_+i*nP,nP*sizeof(int));
    memcpy(d+i*nP,d_+i*nP,nP*sizeof(int));
  }
   
  sampleFromNeighbors(u1_,v1_,u0,v0,d0,d1,descDims[0],descDims[1],g,radius,nPart,n,scale,u1,v1,d);
  
  // assign pointers to output
  plhs[0]=u1_mA;
  plhs[1]=v1_mA;
  plhs[2]=d_mA;
}
