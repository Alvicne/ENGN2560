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

void removeSmallSegments (double* F,const int *dims,double similarity_threshold,double min_segment_size) {
 
  // get image width and height
  const int width  = dims[1];
  const int height = dims[0];
  
  // allocate memory on heap for dynamic programming arrays
  int32_t *I_done     = (int32_t*)calloc(width*height,sizeof(int32_t));
  int32_t *seg_list_u = (int32_t*)calloc(width*height,sizeof(int32_t));
  int32_t *seg_list_v = (int32_t*)calloc(width*height,sizeof(int32_t));
  int32_t seg_list_count;
  int32_t seg_list_curr;
  int32_t u_neighbor[4];
  int32_t v_neighbor[4];
  int32_t u_seg_curr;
  int32_t v_seg_curr;
  
  // declare loop variables
  int32_t addr_start, addr_curr, addr_neighbor;
  
  // for all pixels do
  for (int32_t u=0; u<width; u++) {
    for (int32_t v=0; v<height; v++) {
      
      // get address of first pixel in this segment
      addr_start = getAddressOffsetImage(u,v,width);
                  
      // if this pixel has not already been processed
      if (*(I_done+addr_start)==0) {
                
        // init segment list (add first element
        // and set it to be the next element to check)
        *(seg_list_u+0) = u;
        *(seg_list_v+0) = v;
        seg_list_count  = 1;
        seg_list_curr   = 0;
        
        // add neighboring segments as long as there
        // are none-processed pixels in the seg_list;
        // none-processed means: seg_list_curr<seg_list_count
        while (seg_list_curr<seg_list_count) {
        
          // get current position from seg_list
          u_seg_curr = *(seg_list_u+seg_list_curr);
          v_seg_curr = *(seg_list_v+seg_list_curr);
          
          // get address of current pixel in this segment
          addr_curr = getAddressOffsetImage(u_seg_curr,v_seg_curr,width);
          
          // fill list with neighbor positions
          u_neighbor[0] = u_seg_curr-1; v_neighbor[0] = v_seg_curr;
          u_neighbor[1] = u_seg_curr+1; v_neighbor[1] = v_seg_curr;
          u_neighbor[2] = u_seg_curr;   v_neighbor[2] = v_seg_curr-1;
          u_neighbor[3] = u_seg_curr;   v_neighbor[3] = v_seg_curr+1;
          
          // for all neighbors do
          for (int32_t i=0; i<4; i++) {
            
            // check if neighbor is inside image
            if (u_neighbor[i]>=0 && v_neighbor[i]>=0 && u_neighbor[i]<width && v_neighbor[i]<height) {
              
              // get neighbor pixel address
              addr_neighbor = getAddressOffsetImage(u_neighbor[i],v_neighbor[i],width);
              
              // check if neighbor has not been added yet and if it is valid
              //if (*(I_done+addr_neighbor)==0 && *(I+addr_neighbor)>=0) {
              if (*(I_done+addr_neighbor)==0 && *getValue(F,dims,v_neighbor[i],u_neighbor[i],2)>0.5) {

                // is the neighbor similar to the current pixel
                // (=belonging to the current segment)
                //if (fabs(*(I+addr_curr)-*(I+addr_neighbor))<=(double)similarity_threshold) {
                if (fabs(*getValue(F,dims,v_seg_curr,u_seg_curr,0)-*getValue(F,dims,v_neighbor[i],u_neighbor[i],0)) + 
                    fabs(*getValue(F,dims,v_seg_curr,u_seg_curr,1)-*getValue(F,dims,v_neighbor[i],u_neighbor[i],1))
                    <= (double)similarity_threshold) {
                  
                  // add neighbor coordinates to segment list
                  *(seg_list_u+seg_list_count) = u_neighbor[i];
                  *(seg_list_v+seg_list_count) = v_neighbor[i];
                  seg_list_count++;            
                  
                  // set neighbor pixel in I_done to "done"
                  // (otherwise a pixel may be added 2 times to the list, as
                  //  neighbor of one pixel and as neighbor of another pixel)
                  *(I_done+addr_neighbor) = 1;
                }
              }
              
            } 
          }
          
          // set current pixel in seg_list to "done"
          seg_list_curr++;
          
          // set current pixel in I_done to "done"
          *(I_done+addr_curr) = 1;

        } // end: while (seg_list_curr<seg_list_count)
        
        // if segment NOT large enough => invalidate pixels
        if (seg_list_count<min_segment_size) {
          
          // for all pixels in current segment invalidate pixels
          for (int32_t i=0; i<seg_list_count; i++) {
            // addr_curr = getAddressOffsetImage(*(seg_list_u+i),*(seg_list_v+i),width);
            *getValue(F,dims,seg_list_v[i],seg_list_u[i],2) = 0;
            // *(I+addr_curr) = -1;
          }
        }
      } // end: if (*(I_done+addr_start)==0)
      
    }
  }
  
  // free memory
  free(I_done);
  free(seg_list_u);
  free(seg_list_v);
}

void mexFunction (int nlhs,mxArray *plhs[],int nrhs,const mxArray *prhs[]) {

    // check for proper number of arguments
    if(nrhs!=3) 
        mexErrMsgTxt("Three inputs required (F,similarity_threshold,min_segment_size).");
    if(nlhs!=0) 
        mexErrMsgTxt("No output required.");
    
    // check for proper argument types and sizes
    if(!mxIsDouble(prhs[0]) || mxGetNumberOfDimensions(prhs[0])!=3)
        mexErrMsgTxt("Input F must be a double 3-channel image.");
    if(!mxIsDouble(prhs[1]) || mxGetN(prhs[1])*mxGetM(prhs[1])!=1)
        mexErrMsgTxt("Input similarity_threshold must be a double scalar.");
    if(!mxIsDouble(prhs[2]) || mxGetN(prhs[2])*mxGetM(prhs[2])!=1)
        mexErrMsgTxt("Input min_segment_size must be a double scalar.");
    
    // get pointers
    double*   F                    =   (double*)mxGetPr(prhs[0]);
    double    similarity_threshold = *((double*)mxGetPr(prhs[1]));
    double    min_segment_size     = *((double*)mxGetPr(prhs[2]));
    const int *dims                =  (const int*)mxGetDimensions(prhs[0]);

    // do computation
    removeSmallSegments(F,dims,similarity_threshold,min_segment_size);
}
