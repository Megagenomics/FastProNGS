#ifndef _QC_H_
  #include "qc.h"
#endif

void get_stat(
  char *r,
  char *a,
  int w,
  int t,
  struct RSTAT rstat[2][2])
{
  int s=0,i=0;
  while(*r!='\0'){
    rstat[w][t].ri[i].qual=s=*a - quality_base;
    if(s>=quality_threshold)rstat[w][t].qN++;
    if(s>=20)rstat[w][t].q20++;
    if(s>=30)rstat[w][t].q30++;

    switch(*r){
      case 'A':case 'a': rstat[w][t].ri[i].nucl='A'; break;
      case 'C':case 'c': rstat[w][t].ri[i].nucl='C'; rstat[w][t].GC++; break;
      case 'G':case 'g': rstat[w][t].ri[i].nucl='G'; rstat[w][t].GC++; break;
      case 'T':case 't': rstat[w][t].ri[i].nucl='T'; break;
      case 'N':case 'n': rstat[w][t].ri[i].nucl='N'; rstat[w][t].N++; break;
      default:break;
    }
    r++;
    a++;
    i++;
  }/*for while*/
  rstat[w][t].bases=i;
}/* for get_stat */

void merge_stat_s2t(

  int f,

  uli sstat[2][2][MAX_LINE_LENGTH][MAX_QUALITY][5],

  struct RSTAT rstat[2][2])
{
  int m=0,p=0,i=0,j=0,k=0;
  if(f==0)
    m=0,p=2;
  else if(f==1)
    m=0,p=1;
  else if(f==2)
    m=1,p=2;
  
  for(i=0;i<=paired;i++){
  for(j=m;j<p;j++){
    for(k=0;k<rstat[i][j].bases;k++){
      switch(rstat[i][j].ri[k].nucl){
        case 'A':sstat[i][j][k][rstat[i][j].ri[k].qual][0]++; break;
        case 'C':sstat[i][j][k][rstat[i][j].ri[k].qual][1]++; break;
        case 'G':sstat[i][j][k][rstat[i][j].ri[k].qual][2]++; break;
        case 'T':sstat[i][j][k][rstat[i][j].ri[k].qual][3]++; break;
        case 'N':sstat[i][j][k][rstat[i][j].ri[k].qual][4]++; break;
      }
    }/*k*/
    }/*j*/
  }/*i*/
}/*merge_stat_s2t*/

void merge_stat_t2a(

  uli tstat[2][2][MAX_LINE_LENGTH][MAX_QUALITY][5],

  uli sstat[2][2][MAX_LINE_LENGTH][MAX_QUALITY][5])
{
  int i=0,j=0,k=0,s=0,t=0;
  for(i=0;i<=paired;i++){
  for(j=0;j<2;j++){
    for(k=0;k<MAX_LINE_LENGTH;k++){
      for(s=0;s<MAX_QUALITY;s++){
        for(t=0;t<5;t++){
          tstat[i][j][k][s][t]+=sstat[i][j][k][s][t];
        }/*t*/
      }/*s*/
    }/*k*/
  }/*j*/
  }/*i*/
}/*merge_stat_t2a*/

void get_summary(){
int i=0,j=0,k=0,s=0,t=0,b=0,len=0;
uli reads=0ul,total=0ul,f=0ul;
for(i=0;i<=paired;i++){
  for(j=0;j<2;j++){
    for(k=0;k<MAX_LINE_LENGTH;k++){
      reads=0ul;
      for(s=0;s<MAX_QUALITY;s++){
        summary[i][j].A+=tstat[i][j][k][s][0];
        summary[i][j].C+=tstat[i][j][k][s][1];
        summary[i][j].G+=tstat[i][j][k][s][2];
        summary[i][j].T+=tstat[i][j][k][s][3];
        summary[i][j].N+=tstat[i][j][k][s][4];
        summary[i][j].GC+=tstat[i][j][k][s][1]+tstat[i][j][k][s][2];
        for(t=0;t<5;t++){
          if(tstat[i][j][k][s][t]>0){
            if((k+1)>=summary[i][j].maxlen)summary[i][j].maxlen=k+1;
            if(s>=20)summary[i][j].q20+=tstat[i][j][k][s][t];
            if(s>=30)summary[i][j].q30+=tstat[i][j][k][s][t];
            summary[i][j].bases+=tstat[i][j][k][s][t];
            reads+=tstat[i][j][k][s][t];
          }
        }/*t*/
      }/*s*/
      if(reads>=summary[i][j].reads)summary[i][j].reads=reads;
    }/*k*/
  }/*j*/
}/*i*/
for (i = 0; i <= paired; i++){
  total=0;
  if (fronts[i] != NULL)
  { 
    for (j = 0; j < frontn[i]; j++)
    {
      len = strlen(fronts[i][j]);
      for (k = 0; k < MAX_LINE_LENGTH; k++)
      {
        f = end5Type[i][j][k];
        b = 0;
        while (f > 0)
        {
          if (f & 1ul)total+=end5TypeN[i][j][k][b];
          f >>= 1ul;
          b++;
        } /*for while f*/
      }/*k*/
    } /*for for j*/
  }   /*for if fronts*/
  if (adapts[i] != NULL)
  {
    for (j = 0; j < adaptn[i]; j++)
    {
      len = strlen(adapts[i][j]);
      for (k = 0; k < MAX_LINE_LENGTH; k++)
      {
        f = end3Type[i][j][k];
        b = 0;
        while (f > 0)
        {
          if (f & 1ul)total+=end3TypeN[i][j][k][b];
          f >>= 1ul;
          b++;
        } /*for while f*/
      }/*k*/
    } /*for for j*/
  }   /*for if adapts*/
  if (anywheres[i] != NULL)
  { 
    for (j = 0; j < anywheren[i]; j++)
    {
      len = strlen(anywheres[i][j]);
      for (k = 0; k < MAX_LINE_LENGTH; k++)
      {
        f = endAnyType[i][j][k];
        b = 0;
        while (f > 0)
        {
          if (f & 1ul)total+=endAnyTypeN[i][j][k][b];
          f >>= 1ul;
          b++;
        } /*for while f*/
      }/*k*/
    } /*for for j*/
  }   /*for if fronts*/
  summary[i][0].adaptn[0]=total;
}/*i*/
}/*get_summary*/


