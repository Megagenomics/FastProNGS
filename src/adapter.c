#ifndef _QC_H_
  #include "qc.h"
#endif

int Adapter(
  char *adapter,
  char *query,
  int where, 
  float max_error_rate,
  int min_overlap,
  int noindels,
  int res[6])
{
  int pos=-1;
  int m=strlen(adapter);
  int n=strlen(query);
  char *find=NULL;
  int r=0;
  int temp_res[6]={0};
  find = strstr(query,adapter);
  if(find!=NULL){
    if(where == PREFIX )
      pos=find==query?0:-1;
    else if(where == SUFFIX)
      pos=strlen(find)==m?find-query:-1;
    else if(where == ANYWHERE)
      pos=find-query;
  }
  if(pos>=0){
    res[0]=0,res[1]=m,res[2]=pos,res[3]=pos+m,res[4]=m,res[5]=0;
    return 1;
  }else{
    if(noindels==1 && (where == PREFIX || where==SUFFIX )){
      if(where==PREFIX)
        compare_prefixes(adapter,query,res);
      else
        compare_suffixes(adapter,query,res);
    }else{
      r=locate(adapter,query,max_error_rate,where,min_overlap,res);
      if(r==0)return 0;
    }
  }
  return (res[1]-res[0])>=min_overlap && res[5]/(res[1]-res[0])<=max_error_rate ? 1:0;
}/*for Adapter*/
