#ifndef _QC_H_
#  include "qc.h"
#endif
  /*根据输入参数过滤函数
  *返回值：1 被过滤 0 保留
  */
int filter(
  int f,/*处理前后标识*/
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
    /*等于0是因为空read,但对应的read另一条read可能不为空，后面可以再做其他处理*/
  }/*for n bases*/
  if(min_length>0 && s<min_length){*d|=MINL;r=1;}
  if(max_length>0 && s>max_length){*d|=MAXL;r=1;}
}/*for for k*/
return r;
}/*for filter*/