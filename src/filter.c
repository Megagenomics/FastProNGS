#ifndef _QC_H_
#  include "qc.h"
#endif
 
int filter(
  int f,
  struct RSTAT rstat[2][2],
  uli *d
){
int k=0,r=0;
uli s=0ul;
for(k=0;k<=paired;k++){
  s=rstat[k][f].bases;
  if(s>0){
    if(quality_ratio>0 && ((float)rstat[k][f].qN/s)<quality_ratio){*d|=UNQ;r=1;}
    if(ratio_n>0 && (float)rstat[k][f].N/s>ratio_n){*d|=NR;r=1;}
    if(gc_content>0 && (float)(rstat[k][f].GC)/s<gc_content){*d|=GCC;r=1;}
  }else{
    
  }/*for n bases*/
  if(min_length>0 && s<min_length){*d|=MINL;r=1;}
  if(max_length>0 && s>max_length){*d|=MAXL;r=1;}
}/*for for k*/
return r;
}/*for filter*/