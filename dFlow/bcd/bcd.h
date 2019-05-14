#include<iostream>
#include<omp.h>
#include<cmath>
#include<smmintrin.h>
#include<cstdio>
#include<cstring>

#include<assert.h>
#include<fstream>
#include<time.h>
#include<iomanip>
#include"omp.h"

#include"io_flow.h"

// shorthand loop 
#define fr(i,a,b) for(int i=a;i<=b;++i) 

// represents the contents of a SSE register
#define TYPESSE __m128i

// shift bits left
#define LIMIT (1<<30)

// maximum number of iterations for block coordinate descent
#define MAXITER 100

#define LL int

using namespace std;

// a 2D matrix of m x n integers
int **create2d(int m,int n){
	int **tmp=new int*[m];
	#pragma omp parallel for
	fr(i,0,m-1)
		tmp[i]=new int[n];
	return tmp;
}

// a 3D matrix of m x n x o integers
int ***create3d(int m,int n,int o){
	int ***tmp=new int**[m];
	fr(i,0,m-1)
		tmp[i]=create2d(n,o);
	return tmp;
}

// delete a 2D matrix
template<class T> void delete2d(T** mat,const int& m,const int& n){
  fr(i,0,m-1){
    //printf("2D row % 3d: %d, %p\n",i,*mat[i],mat[i]);
    delete[] mat[i];
  }
  delete[] mat; 
  mat=NULL;
}

// delete a 3D matrix
// void delete3d(int*** mat,const int& m,const int& n,const int& o){
template<class T> void delete3d(T*** mat,const int& m,const int& n,const int& o){
  fr(i,0,m-1){
    //printf("3D row % 3d\n",i);
    delete2d(mat[i],n,o);
  }
  delete[] mat; 
  mat=NULL;
}

// Main struct defining the MRF
struct mrfGrid{

	// *** Member variables ***
	int m; 		// image rows
	int n; 		// image cols
	int l; 		// number of labels
	int l4; 	// number of uint32 to hold the cost for all labels of one pixel
	int L,**wr,**wc,**wc2,**s,**s2,***d,**cur,**cur2;
	int type; // cost function 1=L1, 2=L2
  int tw;   // truncation value
  int maxFu;// maximum allowed flow magnitude
  int maxFv;

	bool weighted;
	LL val; // an integer
	
	// *** More members, added for the 2D case ***
  int imgIdx;       // image index
  bool bruteForce;  // brute force flag for unit tests
  bool hashing;
	int m2,n2; 				// dimension of the search image
	int lambda;				// the weight of the smoothness term
	int **u1,**v1; 		// image coordinates of the nodes in the reference view
	int ***u2,***v2; 	// image coordinates corresponding to the labels
	int ***S;
	short ****Sr;			// precomputed pairwise penalties along row
	short ****Sc;
  int ****Hr;       // the hash map for flow vectors
	int nBins;
  int delta;
  string pathData;
  
	// *** Methods ***
	// adds contents of SSE registers
	// (l4 holds the number of uint32 needed to hold the cost for all labels of one pixel)
	inline void add(int *a,int *b){
		TYPESSE *ma=(TYPESSE*)a,*mb=(TYPESSE*)b;
		fr(iter,0,l4)
			ma[iter]+=mb[iter];
	}
	
	// add integer r to contents of SSE register a
	inline void add(int *a,int r){
		TYPESSE *ma=(TYPESSE*)a;
		TYPESSE tmp=_mm_set1_epi32(r); // sets the 4 signed 32-bit integer values to i
		fr(iter,0,l4)
			ma[iter]+=tmp;
	}
	
	// set contents of SSE register a to minimum of SSE registers a and b
	inline void makeMin(int *a,int *b){
		TYPESSE *ma=(TYPESSE*)a,*mb=(TYPESSE*)b;
		fr(iter,0,l4)
			ma[iter]=_mm_min_epi32(ma[iter],mb[iter]); // minimum of signed packed integer doublewords
	}
	
	// stores results of multiplying SSE register a with integer b in SSE register c
	inline void mul(int* c,int *a,int b){
		TYPESSE *ma=(TYPESSE*)a,tmp=_mm_set1_epi32(b),*mc=(TYPESSE*)c;
		fr(iter,0,l4)
			mc[iter]=_mm_mullo_epi32(ma[iter],tmp); // Packed integer 32-bit multiplication with truncation of upper halves of results
	}
	
	// set contents of SSE register a to minimum of a and integer mi
	inline void makeMin(int *a,int mi){
		TYPESSE *ma=(TYPESSE*)a,tmp=_mm_set1_epi32(mi);
		fr(iter,0,l4)
			ma[iter]=_mm_min_epi32(ma[iter],tmp);
	}
  
  void deleteArrays(){
    // Hr[m][n][nBins*nBins][...]
    fr(i,0,m-1){
      delete3d(Hr[i],n,nBins*nBins,1);
    }
    // Sr,Sc[m][n][l][...]
    fr(i,0,m-1){
      delete3d(Sr[i],n,l,1);
      delete3d(Sc[i],n,l,1);
    }
  }
	
	// ** additional methods for 2D **
	// Computes pairwise penalty between l1 at (r1,c1) and given label l2 at (r2,c2)
	inline int pwPenalty(const int& r1,const int& c1,const int& l1,const int& r2,const int& c2,const int& l2){
		// displacements
		int dc1 = u2[r1][c1][l1]-u1[r1][c1]; int dr1 = v2[r1][c1][l1]-v1[r1][c1];
		int dc2 = u2[r2][c2][l2]-u1[r2][c2]; int dr2 = v2[r2][c2][l2]-v1[r2][c2];
		// l1 endpoint error of displacement vectors
		int d=abs(dc1-dc2)+abs(dr1-dr2);
		return min(type==1?d:d*d,tw);
	}

	// Computes pairwise penalty between all labels of (r1,c1) and given label l2 at (r2,c2)
	void pwPenalty(const int& r1,const int& c1,const int& r2,const int& c2,const int& l2,int* out){
		fr(i,0,l-1){
			// displacements
			int dc1 = u2[r1][c1][i] -u1[r1][c1]; int dr1 = v2[r1][c1][i] -v1[r1][c1];
			int dc2 = u2[r2][c2][l2]-u1[r2][c2]; int dr2 = v2[r2][c2][l2]-v1[r2][c2];
			// l1 endpoint error of displacement vectors
			int d=abs(dc1-dc2)+abs(dr1-dr2);
			out[i]=min(type==1?d:d*d,tw);
		}
	}
	
	void precompPwPenalty(){
		// 4d-matrix for the result 
		Sr=new short***[m];
		Sc=new short***[m];
		
		#pragma omp parallel for
		fr(i,0,m-1){
			Sr[i]=new short**[n];
			Sc[i]=new short**[n];
			// aux variables
			int pr,pc;
			short* tr=new short[2*l]; // stores pairs of [label,pwPenalty]
   		short* tc=new short[2*l]; // stores pairs of [label,pwPenalty]
			int idr,idc;
		
			fr(j,0,n-1){
				Sr[i][j]=new short*[l];
				Sc[i][j]=new short*[l];
				fr(l1,0,l-1){
					//re-initialize tmp, idx
					fr(lt,0,l-1){
						tr[lt]=-1;
						tr[lt]=-1;
					}
					idr=0; idc=0;
					fr(l2,0,l-1){
						if(j!=0){
							pr=pwPenalty(i,j,l1,i,j-1,l2);
							if(pr<tw){
								tr[idr]=l2;
								tr[idr+1]=pr;
								idr+=2;
							}
						}
						tr[idr]=-1; // add end flag
						if(i!=0){
							pc=pwPenalty(i,j,l1,i-1,j,l2);
							if(pc<tw){
								tc[idc]=l2;
								tc[idc+1]=pc;
								idc+=2;
							}
						}
						tc[idc]=-1; // add end flag
					}
					Sr[i][j][l1]=new short[idr+1];
					memcpy(Sr[i][j][l1],tr,(idr+1)*sizeof(short));
					Sc[i][j][l1]=new short[idc+1];
					memcpy(Sc[i][j][l1],tc,(idc+1)*sizeof(short));
				}
			}
			delete[] tr;
			delete[] tc;			
		}
	}
  	
  void precompPwPenaltyFromHash(){
		// 4d-matrix for the result 
		Sr=new short***[m];
		Sc=new short***[m];
    
		#pragma omp parallel for
		fr(i,0,m-1){
			Sr[i]=new short**[n];
			Sc[i]=new short**[n];
			// aux variables
			int pr,pc;
			short* tr=new short[2*l+2]; // stores pairs of [label,pwPenalty]
   		short* tc=new short[2*l+2]; // stores pairs of [label,pwPenalty]
			int idr,idc,iH;
			int rH_max,rH_min,cH_max,cH_min;
			int fu,fv;
      int l2;
		
			fr(j,0,n-1){
				Sr[i][j]=new short*[l];
				Sc[i][j]=new short*[l];
        memset(Sr[i][j],0,l*sizeof(short*));
        memset(Sc[i][j],0,l*sizeof(short*));
				fr(l1,0,l-1){
					//re-initialize tmp, idx
					fr(lt,0,l-1){
						tr[lt]=-1;
						tr[lt]=-1;
					}
					idr=0; idc=0;
					// compute flow in u,v
          fu=u2[i][j][l1]-u1[i][j];
          fv=v2[i][j][l1]-v1[i][j];
          
          // ignore out-of-bounds proposals
          if(abs(fu)<maxFu&&abs(fv)<maxFv){
            // find out which cells to check
            rH_min=max((int)floor((fu-tw)/delta)+(int)floor(nBins/2),0);
            rH_max=min((int)floor((fu+tw)/delta)+(int)floor(nBins/2),nBins-1);
            cH_min=max((int)floor((fv-tw)/delta)+(int)floor(nBins/2),0);
            cH_max=min((int)floor((fv+tw)/delta)+(int)floor(nBins/2),nBins-1);   
            // look-up similar particles in the hashmap of the left neighbor
            if(j!=0){
              fr(rH,rH_min,rH_max)
                fr(cH,cH_min,cH_max){
                  if(Hr[i][j-1][rH*nBins+cH]){
                    iH=0;
                    l2=Hr[i][j-1][rH*nBins+cH][iH];
                    while(l2!=-1){
                      pr=pwPenalty(i,j,l1,i,j-1,l2);
                      if(pr<tw){
                        tr[idr]=l2;
                        tr[idr+1]=pr;
                        idr+=2;
                      }
                      iH+=1;
                      l2=Hr[i][j-1][rH*nBins+cH][iH];
                    }
                  }
                }
            }
            // look-up similar particles in the hashmap of the top neighbor
            if(i!=0){
              fr(rH,rH_min,rH_max)
                fr(cH,cH_min,cH_max){
                  if(Hr[i-1][j][rH*nBins+cH]){
                    iH=0;
                    l2=Hr[i-1][j][rH*nBins+cH][iH];
                    while(l2!=-1){
                      pc=pwPenalty(i,j,l1,i-1,j,l2);
                      if(pc<tw){
                        tc[idc]=l2;
                        tc[idc+1]=pc;
                        idc+=2;
                      }
                      iH+=1;
                      l2=Hr[i-1][j][rH*nBins+cH][iH];          			
                    }
                  }
                }
            }
          }
          // add end flags
          if(idr>0){
            tr[idr]=-1; 
            Sr[i][j][l1]=new short[idr+1];
            memcpy(Sr[i][j][l1],tr,(idr+1)*sizeof(short));
          }
          if(idc>0){
            tc[idc]=-1; 
            Sc[i][j][l1]=new short[idc+1];
            memcpy(Sc[i][j][l1],tc,(idc+1)*sizeof(short));
          }
        }
			}
      delete[] tr;
      delete[] tc;							
		}
	}
  
  // compute a hash map to efficiently look-up neighboring particles
  void hashParticles(int nBins_){
    double start=omp_get_wtime();
    Hr=new int***[m];
    // set number of bins
    nBins=nBins_;
    // compute bin size
    try{
      delta=floor(2*maxFu/(nBins-1));  
    }
    catch(int e){
      printf("Could not compute delta from 2 * %d / (%d-1)\n",maxFu,nBins);
    }
    assert(delta>0);
    printf("Hashing with delta=%d and nBins=%d\n",delta,nBins);

    #pragma omp parallel for
		fr(i,0,m-1){
			Hr[i]=new int**[n];
            
      fr(j,0,n-1){
				Hr[i][j]=new int*[nBins*nBins];
        memset(Hr[i][j],0,nBins*nBins*sizeof(int*));
        
        int fu,fv;
        int cH,rH;
        int*** h=create3d(nBins,nBins,l+1); // enough storage for the hash table
        int** nH=create2d(nBins,nBins);     // holds number of entries per bin
        fr(iB,0,nBins-1)
          fr(jB,0,nBins-1)
            memset(nH[iB],0,nBins*sizeof(int));
        
        fr(k,0,l-1){
          // compute flow in u and v
          fu=u2[i][j][k]-u1[i][j];
          fv=v2[i][j][k]-v1[i][j];
          // ignore out-of-bounds proposals
          if(abs(fu)>maxFu||abs(fv)>maxFv)
            continue;
          // compute hash coordinates
          rH=floor(fu/delta)+floor(nBins/2);
          cH=floor(fv/delta)+floor(nBins/2);
          assert(rH>=0&&rH<nBins);
          assert(cH>=0&&cH<nBins);
          assert(nH[rH][cH]>=0&&nH[rH][cH]<l);
          // hash table entry
          h[rH][cH][nH[rH][cH]]=k;
          nH[rH][cH]+=1;
        } // all labels
        
        // copy local hash table to global matrix
        fr(iB,0,nBins-1)
          fr(jB,0,nBins-1){
            if(nH[iB][jB]!=0){
              Hr[i][j][iB*nBins+jB]=new int[nH[iB][jB]+1];
              h[iB][jB][nH[iB][jB]]=-1;
              memcpy(Hr[i][j][iB*nBins+jB],h[iB][jB],(nH[iB][jB]+1)*sizeof(int));
            }
          }
        delete3d(h,nBins,nBins,l);
        delete2d(nH,nBins,nBins);
      } // (j,0,n-1)
    } // (i,0,m-1)
    printf("hashed particles in %.2f sec.\n",omp_get_wtime()-start);
  }
  
	void saveResult(const char* path,const int& idx){
		char fn[256];
		sprintf(fn,"%s/%06d.txt",path,idx);
		ofstream fout(fn);
		fr(i,0,m-1)
			fr(j,0,n-1)
				fout << cur[i][j] << " ";
	}
	
  void saveResult(const char* fn){
		ofstream fout(fn);
		fr(i,0,m-1)
			fr(j,0,n-1)
				fout << cur[i][j] << " ";
	}
  
	void saveFlow(const char* path,const int& idx){
		char fn[256];
		sprintf(fn,"%s/%06d.png",path,idx);
		FlowImage F(n2,m2);
		fr(i,0,m-1)
			fr(j,0,n-1){
				F.setFlowU(u1[i][j],v1[i][j],u2[i][j][cur[i][j]]-u1[i][j]);
				F.setFlowV(u1[i][j],v1[i][j],v2[i][j][cur[i][j]]-v1[i][j]);
				F.setValid(u1[i][j],v1[i][j],true);
			}
		F.write(std::string(fn));
	}
  
  // overloaded function, takes complete filename as input
  void saveFlow(const char* fn){
		FlowImage F(n2,m2);
		fr(i,0,m-1)
			fr(j,0,n-1){
				F.setFlowU(u1[i][j],v1[i][j],u2[i][j][cur[i][j]]-u1[i][j]);
				F.setFlowV(u1[i][j],v1[i][j],v2[i][j][cur[i][j]]-v1[i][j]);
				F.setValid(u1[i][j],v1[i][j],true);
			}
		F.write(std::string(fn));
	}
  
  void getFlow(int** Fu,int** Fv,int** Fi){
    // init matrices
    fr(i,0,m2-1){
      memset(Fu[i],0,n2*sizeof(int));
      memset(Fv[i],0,n2*sizeof(int));
      memset(Fi[i],0,n2*sizeof(int));
    }
     
    fr(i,0,m-1){
      fr(j,0,n-1){
        Fu[v1[i][j]][u1[i][j]]=u2[i][j][cur[i][j]]-u1[i][j];
        Fv[v1[i][j]][u1[i][j]]=v2[i][j][cur[i][j]]-v1[i][j];
        Fi[v1[i][j]][u1[i][j]]=1;
      }
    }
  }
  
  // mimic output of deepmatching: x1 y1 x2 y2 score
  void deepOutput(){
    char fn[256];
    sprintf(fn,"%s/%06d_deepmatches.txt",pathData.c_str(),imgIdx);
    FILE *fh=fopen(fn,"w"); 
    fr(i,0,m-1)
			fr(j,0,n-1){
        fprintf(fh,"%d %d ",u1[i][j],v1[i][j]);
        fprintf(fh,"%d %d ",u2[i][j][cur[i][j]],v2[i][j][cur[i][j]]);
        fprintf(fh,"%f\n",1.0f);
			}
    fclose(fh);
  }
		
	/* *** Constructor of the grid *** */
	// creates matrices and initializes values in wr & wc with 1
	mrfGrid(int m_=0,int n_=0,int l_=0)
    :m(m_),n(n_),l(l_)
    ,bruteForce(false),hashing(true),nBins(10)
  {
		srand (1); 	 // re-initialize the rng for reproducible results
		L=(l+3)/4*4; // number of uint32 needed to hold the cost for all labels of one pixel
		l4=(L>>2)-1; // number of _m128i registers to hold L*cost (-1 because it is used in loops)
    maxFu=600;
    maxFv=600;
		wr=create2d(m,n);		// edge weights in row direction
		wc=create2d(m,n); 	// edge weights in col direction
		wc2=create2d(n,m);  // transposed edge weights in col direction
		s=create2d(L,L); 		// s contains the smoothness penalty for all combinations of labels
		d=create3d(m,n,L); 	// the cost volume
		// additional stuff for 2D optimization
		u1=create2d(m,n); 	// image coordinates in the reference view
		v1=create2d(m,n); 	
		u2=create3d(m,n,L);	// image coordinates encoded by labels
		v2=create3d(m,n,L);	
		S=create3d(m,n,L); 	// container for pairwise cost
		#pragma omp parallel for
		fr(i,0,m-1)
			fr(j,0,n-1)
				wr[i][j]=wc[i][j]=1;
	}
	
	// computes integer energy for labelling r
	inline LL energy(int **r){
		LL sum=0;
		if(weighted)
			#pragma omp parallel for reduction (+:sum)
			fr(i,0,m-1)
				fr(j,0,n-1)
					sum+=d[i][j][r[i][j]]+(i==m-1?0:wc[i][j]*pwPenalty(i,j,r[i][j],i+1,j,r[i+1][j]))+(j==n-1?0:wr[i][j]*pwPenalty(i,j,r[i][j],i,j+1,r[i][j+1]));
		else
			#pragma omp parallel for reduction (+:sum)
			fr(i,0,m-1)
				fr(j,0,n-1)
					sum+=d[i][j][r[i][j]]+(i==m-1?0:lambda*pwPenalty(i,j,r[i][j],i+1,j,r[i+1][j]))+(j==n-1?0:lambda*pwPenalty(i,j,r[i][j],i,j+1,r[i][j+1]));

    return sum;
	}
	
  // Destructor
  ~mrfGrid(){
    // free allocated memory
    delete2d(wr,m,n);		// edge weights in row direction
		delete2d(wc,m,n); 	// edge weights in col direction
		delete2d(wc2,n,m);  // transposed edge weights in col direction
		delete2d(s,L,L); 	
    delete2d(u1,m,n); 	// image coordinates in the reference view
		delete2d(v1,m,n); 	
    
		delete3d(u2,m,n,L);	// image coordinates encoded by labels
		delete3d(v2,m,n,L);	// image coordinates encoded by labels
		delete3d(S,m,n,L); 	// container for pairwise cost
    delete3d(d,m,n,L); 
  }
  
	// Adapted Dynamic Programming method
	inline void cal2DRow(int *data[],const int& r,int len,int *out,int *w){
    int f_[len][L];	// 2D slice of the cost volume
		int mi;					// holds minimum cumulative cost
		int tmp;
		int rec;				// stores best label
    int recx_[len];
    memcpy(f_[0],data[0],l*sizeof(int)); // copy the slice of the cost volume to a local array
    int pre_[len][l];
		fr(i,1,len-1){ 	// all pixels in the slice
			// find minimum cumulative cost of predecessor
			int minC=INT_MAX;
			int minP=-1;
      int mPW=0;
			fr(k,0,l-1){
				if(f_[i-1][k]<minC){
					minC=f_[i-1][k]; 
					minP=k;
				}
      }
	
			fr(j,0,l-1){	// all labels at current pixel
				mi=minC+w[i-1]*tw;
        mPW=tw;
        f_[i][j]=mi;
				recx_[i]=minP;
        pre_[i][j]=minP;
        
				int idx=0;
        int idxMin=0;
        int tmp_u=minC;
        if(Sr[r][i][j]){
          while(Sr[r][i][j][idx]!=-1){
            tmp=f_[i-1][Sr[r][i][j][idx]]+w[i-1]*Sr[r][i][j][idx+1];
            if(tmp<mi){
              mi=tmp;
              f_[i][j]=mi;
              recx_[i]=Sr[r][i][j][idx];
              pre_[i][j]=Sr[r][i][j][idx];
              mPW=Sr[r][i][j][idx+1];
              idxMin=idx;
            }
            else if(tmp==mi&&Sr[r][i][j][idx+1]<mPW){
              mi=tmp;
              f_[i][j]=mi;
              recx_[i]=Sr[r][i][j][idx];
              pre_[i][j]=Sr[r][i][j][idx];
              mPW=Sr[r][i][j][idx+1];
              idxMin=idx;
            }
            idx+=2;
          }
        }
        int minC_fast=mi;
        int minP_fast=recx_[i];
        int pre_fast=pre_[i][j];
			}
      add(f_[i],data[i]); // add unary of current pixel to min cost
		}
	  // find the label of the last pixel in the line that induces 
		// the smallest cumulative cost
		rec=0;
		fr(k,1,l-1)
			if(f_[len-1][k]<f_[len-1][rec])
				rec=k;
		// backward pass						
		for(int i=len-1;i>0;--i){ // all pixels of the line in reversed order
			out[i]=rec;
			rec=pre_[i][rec];
		}
		out[0]=rec; // store best label for the first pixel in the line		

	}
	
	// Adapted Dynamic Programming method for columns
	inline void cal2DCol(int *data[],const int& c,int len,int *out,int *w){
    int f_[len][L];	
		int mi;					// holds minimum cumulative cost
		int tmp;
		int rec;				// stores best label
    int recx_[len];
    memcpy(f_[0],data[0],l*sizeof(int)); 
    int pre_[len][l];
    
		fr(i,1,len-1){ 	// all pixels in the slice
      // find minimum cumulative cost of predecessor
			int minC=INT_MAX;
			int minP=-1;
      int mPW=0;
			fr(k,0,l-1)
				if(f_[i-1][k]<minC){
					minC=f_[i-1][k]; 
					minP=k;
				}
	
			fr(j,0,l-1){	// all labels at current pixel
        mi=minC+w[i-1]*tw;
        mPW=tw;
        f_[i][j]=mi;
				recx_[i]=minP;
        pre_[i][j]=minP;
       
				int idx=0;
        if(Sc[i][c][j]){
          while(Sc[i][c][j][idx]!=-1){
            tmp=f_[i-1][Sc[i][c][j][idx]]+w[i-1]*Sc[i][c][j][idx+1];
            if(tmp<mi){
              mi=tmp;
              f_[i][j]=mi;
              recx_[i]=Sc[i][c][j][idx];
              pre_[i][j]=Sc[i][c][j][idx];
              mPW=Sc[i][c][j][idx+1];
            }
            else if(tmp==mi&&Sc[i][c][j][idx+1]<mPW){
              mi=tmp;
              f_[i][j]=mi;
              recx_[i]=Sc[i][c][j][idx];
              pre_[i][j]=Sc[i][c][j][idx];
              mPW=Sc[i][c][j][idx+1];            
            }
            idx+=2;
          }
        }
			}
      add(f_[i],data[i]);
		}
    // find the label of the last pixel in the line that induces 
		// the smallest cumulative cost
		rec=0;
		fr(k,1,l-1)
			if(f_[len-1][k]<f_[len-1][rec])
				rec=k;
		// backward pass						
		for(int i=len-1;i>0;--i){ // all pixels of the line in reversed order
			out[i]=rec;
			rec=pre_[i][rec];
		}
		out[0]=rec; // store best label for the first pixel in the line	
	}

  
	inline void updateRow(int **cur,bool init,bool *flag){
          
		fr(st,0,1) // for even and odd rows (st=0;st<1;st+1)
 		#pragma omp parallel
		{
			int id2=omp_get_thread_num()<<1,nthrds2=omp_get_num_threads()<<1;
			int data[n][L]; // 2D slice of the cost volume
			int *data2[n];	// points to data (cf. below)
			int tmp[L];
			int out[n];
			int sTmp[L];
			fr(i,0,n-1)
				data2[i]=data[i];
			for(int row=st+id2;row<m;row+=nthrds2)
				if(flag[row]){ // current row shall be processed
					// 1) get unaries and smoothness term wrt. fixed neighbors 
					if(weighted) // there are edge weights
						fr(j,0,n-1){ // all pixels in the row
							memcpy(data[j],d[row][j],l*sizeof(int)); // copy unary cost to data
							if(!init){ // we already passed the initialization stage
								if(row>0){ // this is not the topmost row -> add smoothness penalty wrt. fixed label of upper pixel 
									pwPenalty(row,j,row-1,j,cur[row-1][j],S[row][j]);
									mul(tmp,S[row][j],wc[row-1][j]);
									add(data[j],tmp);
								}
								if(row<m-1){ // this is not the undermost row -> add smoothness penalty wrt. the fixed label of pixel below			
									pwPenalty(row,j,row+1,j,cur[row+1][j],S[row][j]);
									mul(tmp,S[row][j],wc[row][j]);
									add(data[j],tmp);
								}
							}
						}
					else				// no edge weights	
						fr(j,0,n-1){ // all pixels in the row
							memcpy(data[j],d[row][j],l*sizeof(int)); // copy unary cost to data
							if(!init){  // we already passed the initialization stage
								if(row>0){ // this is not the topmost row
									// add smoothness penalty wrt. label of upper pixel 
									// (method add does this efficiently for all labels at the current pixel)
									// Note that here the ordering of the labels is leveraged and we simply add 
									// one column of s2 since it already holds all pairwise penalties between 
									// the fixed label in the neighboring row and all possible labels of the 
									// current pixel
									pwPenalty(row,j,row-1,j,cur[row-1][j],S[row][j]);
									mul(tmp,S[row][j],lambda);
									add(data[j],tmp);
								}
								if(row<m-1){ // this is not the undermost row
									pwPenalty(row,j,row+1,j,cur[row+1][j],S[row][j]);
									mul(tmp,S[row][j],lambda);			
									add(data[j],tmp);					
								}
							}
						}
					// 2) compute lowest cumulative cost path for each pixel
					// wrt. the predecessor in the direction of optimization
          cal2DRow(data2,row,n,out,wr[row]);
					memcpy(cur[row],out,n*sizeof(int));
				}
		}
	}
	
	
	inline void updateCol(int **cur,bool init,bool *flag){
		fr(st,0,1)
		#pragma omp parallel
		{
			int id2=omp_get_thread_num()<<1,nthrds2=omp_get_num_threads()<<1;
			int data[m][L],*data2[m],tmp[L];
			int out[m];
			fr(j,0,m-1)
				data2[j]=data[j];
			for(int col=st+id2;col<n;col+=nthrds2)
				if(flag[col]){
					if(weighted)
						fr(i,0,m-1){
							memcpy(data[i],d[i][col],l*sizeof(int));
							if(!init){
								if(col>0){
									pwPenalty(i,col,i,col-1,cur[i][col-1],S[i][col]);
									mul(tmp,S[i][col],wr[i][col-1]);
									add(data[i],tmp);
								}
								if(col<n-1){
									pwPenalty(i,col,i,col+1,cur[i][col+1],S[i][col]);
									mul(tmp,S[i][col],wr[i][col]);
									add(data[i],tmp);
								}
							}
						}
					else
						fr(i,0,m-1){
							memcpy(data[i],d[i][col],l*sizeof(int));
							if(!init){
								if(col>0){
									pwPenalty(i,col,i,col-1,cur[i][col-1],S[i][col]);
									mul(tmp,S[i][col],lambda);
									add(data[i],tmp);
								}	
								if(col<n-1){
									pwPenalty(i,col,i,col+1,cur[i][col+1],S[i][col]);
									mul(tmp,S[i][col],lambda);
									add(data[i],tmp);
								}	
							}
						}
          cal2DCol(data2,col,m,out,wc2[col]);

          fr(i,0,m-1)
            cur[i][col]=out[i];
				}
		}
	}
	
	// main method performing block coordinate descent
	int **BCD(){
   
		weighted=false;
		lambda=wr[0][0];
		#pragma omp parallel for
		fr(i,0,m-1)
			fr(j,0,n-1){
				wc2[j][i]=wc[i][j];
				if(wr[i][j]!=lambda&&j<n-1||wc[i][j]!=lambda&&i<m-1)
					weighted=true;
			}
    
		// precompute 2d pairwise potentials
		double start=omp_get_wtime();
    if(hashing){
      precompPwPenaltyFromHash();
      printf("hashed pw precomputed in %.2f sec.\n",omp_get_wtime()-start);
    }
    else{
    	precompPwPenalty();
      printf("pw precomputed in %.2f sec.\n",omp_get_wtime()-start);
    }
    // init / declare variables
		cur=create2d(m,n); // current labeling
		cur2=create2d(m,n);
		int **curRow=create2d(m,n);
		int **curCol=create2d(m,n);
		bool rowFlag[m],colFlag[n],rowFlag2[m],colFlag2[n];
		start=omp_get_wtime();
		memset(rowFlag,true,sizeof(rowFlag)); // set all optimization flags to true
		memset(colFlag,true,sizeof(colFlag));
		updateRow(curRow,true,rowFlag); // initialization of label assignments: optimization of rows
		updateCol(curCol,true,colFlag); // optimization of cols
		// randomly pick one of the initial labels
		//#pragma omp parallel for	
		fr(i,0,m-1)
			fr(j,0,n-1){
				if(rand()&1)
					cur[i][j]=curRow[i][j];
				else
					cur[i][j]=curCol[i][j];
    }
    printf("Initialization completed\n");
    
		LL pre; // integer holding the energy of the previous iteration
		// BCD loop
		fr(ti,1,MAXITER){
			val=energy(cur); // compute energy of current labelling
			double end=omp_get_wtime();
			cout<<"Iteration\t"<<ti<<":\t"<<val<<"\truntime:\t"<<setprecision(3)<<end-start<<" s"<<endl;
			// the optimization is stopped as soon as the energy increases
			if(val>=pre&&ti>1)
				break;
			pre=val; // store energy of the previous iteration
			// copy current labeling to cur2
			start=omp_get_wtime();
			#pragma omp parallel for 
			fr(i,0,m-1)
				memcpy(cur2[i],cur[i],n*sizeof(int));
			// iteration of dynamic programming	
			updateRow(cur,false,rowFlag);		// optimization of rows (curLabelling,init,optFlag)
			updateCol(cur,false,colFlag); 	// optimization of cols
			memcpy(rowFlag2,rowFlag,sizeof(rowFlag));
			memcpy(colFlag2,colFlag,sizeof(colFlag));
			memset(rowFlag,false,sizeof(rowFlag));
			memset(colFlag,false,sizeof(colFlag));
			// re-set optimization flags for rows:
			// If any label changed (except for the first and last pixel), 
			// the adjacent rows have to be iterated
			#pragma omp parallel for
			fr(i,0,m-1)
				if(rowFlag2[i])
					fr(j,0,n-1)
						if(cur[i][j]!=cur2[i][j]){
							if(i>0)
								rowFlag[i-1]=true;
							if(i<m-1)
								rowFlag[i+1]=true;
						}
			// re-set optimization flags for cols						
			#pragma omp parallel for
			fr(j,0,n-1)
				if(colFlag2[j])
					fr(i,0,m-1)
						if(cur[i][j]!=cur2[i][j]){
							if(j>0)
								colFlag[j-1]=true;
							if(j<n-1)
								colFlag[j+1]=true;
						}
		} // BCD loop
		
		return cur;
	}
}; // mrfGrid
