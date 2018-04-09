
#ifndef _QC_H_
  #include "qc.h"
#endif

void predatai()
{
  int t=0;
  tdi=(pthread_t *)xmalloc(sizeof(pthread_t)*(paired+1));
  for(t=0;t<=paired;t++){
    itid[t]=t;
    if(pthread_create(&tdi[t],NULL,&fill_reads,(void*)&itid[t])==-1){
      error("create thread error!\n");
      abort_file();
    }
  }
}/*for predata*/

void waitpreouti()
{
  int t=0;
  for(t=0;t<=paired;t++){
    pthread_join(tdi[t],NULL);
  }
  FREE(tdi);
}/*for waitpreout*/

void fill_reads(void *id)
{
  int i=0,j=0,r=reads*4,w=0,f=0;
  int *uid=(int *)id;
  do{
    w=0,j=0,f=0;
    pthread_mutex_lock(&lock_ifile);
    if(iflag>=0){
      w=(wa[iflag].flag)&(1<<*uid)?0:1;
    }else{
      i=0;
      while(i<threads){
        if(wa[i].flag==0){iflag=i;w=1;break;}
        else if(wa[i].flag&16){f++;}
        i++;
      }/*for while*/
    }/*for if iflag*/
    pthread_mutex_unlock(&lock_ifile);

    if(w){
      if(iftype&(1<<*uid)){
        while(j<r && gzgets(igzfp[*uid],wa[iflag].data[*uid][j],max_line_length)!=NULL){j++;};
      }else{
        while(j<r && fgets(wa[iflag].data[*uid][j],max_line_length,ifp[*uid])!=NULL){j++;};
      }/*for iftype*/

      wa[iflag].num[*uid]=j;
      pthread_mutex_lock(&lock_ifile);
      wa[iflag].flag|=(1<<*uid);
      if(paired){
        if((wa[iflag].flag&3)==3){
          wa[iflag].flag|=j>0?7:16;
          /*wa[iflag].flag|=j>0?23:16;//测试代码*/
          iflag=-1;
        }
      }else{
        if(wa[iflag].flag&1){
          wa[iflag].flag|=j>0?5:16;
          /*wa[iflag].flag|=j>0?21:16; //测试代码*/
          iflag=-1;
        }
      }/*for if paired*/
      pthread_mutex_unlock(&lock_ifile);
    }else{
      usleep(10);
    }/*for if w*/
  }while(f<threads);
}/*fill_reads*/

int put_str(int *outlen,char **outbuf)
{
  int f=-1,i=0,j=0,k=0;
  char **p=NULL;
  /*写入退出*/
  do{
    f=-1;
    pthread_mutex_lock(&lock_ofile);
    for(i=file_out_loc_flag;i<split;i++){
      if((file_out_flag&(1<<i))==0){
        setbit(file_out_flag,i);
        file_out_loc_flag=f=i;
        break;
      }
    }
    if(f<0){
      for(i=0;i<file_out_loc_flag;i++){
        if((file_out_flag&(1<<i))==0){
          setbit(file_out_flag,i);
          file_out_loc_flag=f=i;
          break;
        }
      }
    }
    pthread_mutex_unlock(&lock_ofile);
    if(f<0)usleep(10);
  }while(f<0);

  for(i=0;i<=paired;i++){
    if(oftype&(1<<i)){
      gzwrite(ogzfp[i][f],outbuf[i],outlen[i]);
    }else{
      fwrite(outbuf[i],sizeof(char),outlen[i],ofp[i][f]);
    }
  }/*for for i*/
  return f;
}/*for put_str*/
