#ifndef _QC_H_
  #include "qc.h"
#endif

#include <signal.h>
#include <time.h>

/*全局变量*/
int paired=0;              /*双端测序标识,默认单端*/
char **adapts[2]={NULL};   /*3'端接头序列*/
int adaptn[2]={0};         /*3'端接头数量*/
int *adaptf[2]={0};        /*3'端接头标识(是否有锚位)*/
char **fronts[2]={NULL};   /*5'端接头序列*/
int frontn[2]={0};         /*5'端接头数量*/
int *frontf[2]={0};        /*5'端接头标识*/
char **anywheres[2]={NULL};/*任意位置接头序列*/
int anywheren[2]={0};      /*任意位置接头数量*/
int *anywheref[2]={0};     /*任意位置接头标识*/
int no_indels=-1;          /*是否允许indels,默认查看xml配置*/
float error_rate=-1;       /*允许最大错配率,默认查看xml配置*/
int overlap=-1;            /*接头与READ最小重叠区域*/
int quality_threshold=-1;  /*质量值阈值*/
float quality_ratio=-1;    /*符合上面质量阈值最低碱基数占比*/
int quality_low=-1;        /*去除READ两端低于此值的碱基*/
int trim_n=-1;             /*去除两端N碱基*/
float ratio_n=-1;          /*N碱基占比最低值*/
int min_length=0;          /*READ最小长度*/
int max_length=0;          /*READ最大长度*/
float gc_content=0;        /*GC含量最低值*/
int quality_base=-1;       /*碱基质量基值 33或者64*/
char *outname[2]={NULL,NULL};/*输出文件名*/
char *outpath[2]={NULL,NULL};/*输出文件路径*/
char *xml=NULL;              /*默认xml配置文件路径*/
char *html=NULL;             /*默认不输出html,加此参数后输出到指定路径*/
char *text=NULL;             /*默认不输出text,加此参数后输出到指定路径*/
char *json=NULL;             /*默认不输出json,加此参数后输出到指定路径*/
char *title=NULL;            /*输出报告标题*/      
int split=-1;                /*输出文件切割*/
int threads=-1;              /*线程数*/
int reads=-1;                /*工作reads数*/
int view_params=-1;          /*预览参数*/
char *ifiles[2]={NULL};      /*保存输入文件全路径*/
uli iftype=0;                /*输入文件类型,0到...表示READ n,置位代表gz文件,否则是普通文件*/
uli oftype=0;                /*输入文件类型,0到...表示READ n,置位代表gz文件,否则是普通文件*/
char exe_path[MAX_PATH]={0}; /*可执行文件路径*/
char exe_name[MAX_PATH]={0}; /*可执行文件名*/
int max_line_length=-1;      /*行最大长度*/

/*文件句柄*/
FILE *ifp[2]={NULL};         /*输入 普通文件描述符*/
gzFile igzfp[2]={0};         /*输入 压缩文件描述符*/
FILE *HTML=NULL;             /*HTML 输出文件句柄*/
FILE *TEXT=NULL;             /*TEXT 输出文件句柄*/
FILE *JSON=NULL;             /*JSON 输出文件句柄*/
FILE **ofp[2]={NULL};        /*输出 普通文件描述符*/
char **ofname[2]={NULL};     /*输出 文件名*/
gzFile *ogzfp[2]={NULL};     /*输出 压缩文件描述符*/
int foreground=0;            /*程序 运行在前端*/

  /*全局锁*/
pthread_mutex_t lock_ifile;
pthread_mutex_t lock_ofile;
pthread_mutex_t lock_stat;

  /*数据预备线程*/
pthread_t *tdi=NULL;
pthread_t *tdo=NULL;

int itid[2]={0};    /*保存输入线程id*/
int *wtid=NULL;     /*保存工作线程id*/
int iflag=-1;       /*往哪个块写数据标识*/

  /*文件读取状态标识*/
uli file_in_flag =0ul;/*0表示可读,1表示正在读*/
uli file_out_flag=0ul;
int file_out_loc_flag=0;

  /*全局数据缓冲区*/
struct work_area *wa=NULL;

uli end3Type[2][MAX_ADAPT_NUM][MAX_LINE_LENGTH]={0ul};        /*READ1/2:3'接头种类*/
uli end3TypeN[2][MAX_ADAPT_NUM][MAX_LINE_LENGTH][64]={0ul};   /*READ1/2:3'接头各种类数量*/
uli end5Type[2][MAX_ADAPT_NUM][MAX_LINE_LENGTH]={0ul};        /*READ1/2:5'接头种类*/
uli end5TypeN[2][MAX_ADAPT_NUM][MAX_LINE_LENGTH][64]={0ul};   /*READ1/2:5'接头各种类数量*/
uli endAnyType[2][MAX_ADAPT_NUM][MAX_LINE_LENGTH]={0ul};      /*READ1/2:任意端接头种类*/
uli endAnyTypeN[2][MAX_ADAPT_NUM][MAX_LINE_LENGTH][64]={0ul}; /*READ1/2:任意接头各种类数量*/

uli tstat[2][2][MAX_LINE_LENGTH][MAX_QUALITY][5]={0ul};
uli t_dstat[TF]={0};
uli tlenstat[2][2][MAX_LINE_LENGTH]={0};/*长度统计*/

/*汇总信息*/
struct SUMMARY summary[2][2]={0ul};

void main(int argc, char *argv[])
{
  parse_user_parms(argc,argv);/* 解析用户输入参数 */
  load_default_parms();/* 从xml加载默认参数 */
  checkup_params();/*检验参数*/
  if(view_params==1)viewparams();/*预览参数*/
  test_iofiles();/*测试文件*/

  /*收到来自键盘的终止信号则清除已经创建的文件*/
  foreground = signal(SIGINT, SIG_IGN) != SIG_IGN;
  if (foreground)
	  (void) signal (SIGINT, (sig_type)abort_file);
  
    /*收到kill终端信号*/
  if (signal(SIGTERM, SIG_IGN) != SIG_IGN)
	  (void) signal(SIGTERM, (sig_type)abort_file);
  
    /*初始化锁*/
  pthread_mutex_init(&lock_ifile,NULL);
  pthread_mutex_init(&lock_ofile,NULL);
  pthread_mutex_init(&lock_stat,NULL);

     /*开始处理数据*/               
  run();

    /*销毁锁*/
  pthread_mutex_destroy(&lock_ifile);
  pthread_mutex_destroy(&lock_ofile);
  pthread_mutex_destroy(&lock_stat);

  get_summary();
    /*输出html统计信息*/
  if(html!=NULL){
    html_head();
    html_summary();/*概要*/
    html_rstat();  /*统计*/
    html_adapter();/*接头*/
    html_qualpic();/*质量*/
    html_basepic();/*碱基*/
    html_length(); /*长度*/
    html_cmd(argv);    /*命令*/
    html_tail();
  }

  if(text!=NULL)text_report();
  
  if(json!=NULL)json_out();

  do_exit(OK);
}/*main*/

  /*释放资源,退出*/
void do_exit(int exitcode)
{
  int i=0,j=0;
  /*释放接头申请资源*/
  for(i=0;i<2;i++){
    for(j=0;j<adaptn[i];j++){
      FREE(adapts[j]);
      FREE(adaptf[j]);
    }
    for(j=0;j<frontn[i];j++){
      FREE(fronts[j]);
      FREE(frontf[j]);
    }
    for(j=0;j<anywheren[i];j++){
      FREE(anywheres[j]);
      FREE(anywheref[j]);
    }
    /*释放输出路径/文件名申请资源*/
    FREE(outpath[i]);
    FREE(outname[i]);
  }/*i*/

  for(i=0;i<=paired;i++){
    /*关闭输入文件句柄*/
    if(ifp[i]!=NULL)fclose(ifp[i]);
    if(igzfp[i]!=NULL)gzclose(igzfp[i]);
    for(j=0;j<split;j++){
      if(oftype&(1<<i)){
        if(ogzfp[i]!=NULL && ogzfp[i][j]!=NULL){ gzclose(ogzfp[i][j]);}
      }else{
        if(ofp[i]!=NULL && ofp[i][j]!=NULL)fclose(ofp[i][j]);
      }
      if(ofname[i]!=NULL)FREE(ofname[i][j]);
    }
    FREE(ogzfp[i]);
    FREE(ofp[i]);
    /*释放输出文件名申请资源*/
    FREE(ofname[i]);
  }
  FREE(xml);

   /*释放为文件名和路径分配的内存*/
  if(HTML!=NULL)fclose(HTML);
  if(TEXT!=NULL)fclose(TEXT);
  if(JSON!=NULL)fclose(JSON);
  if(exitcode!=3)exit(exitcode);
}

/*删除不完整文件,关闭文件句柄*/
void abort_file()
{
  int i=0,j=0;
  for(i=0;i<=paired;i++){
    if(ifiles[i]==NULL)continue;
    /*删除不完整文件*/
    for(j=0;j<split;j++)
      if(ofname[i][j]!=NULL)unlink(ofname[i][j]);
  }/* for i*/

  if(html)unlink(html);
  if(text)unlink(text);
  if(json)unlink(json);
  do_exit(ERROR);
}/*for abort_file*/

 /*处理数据入口*/
void run(){
  pthread_t *th=NULL;
  int t=0,a=0,b=0,c=0,d=0;
  /*数据准备区*/
  /*给缓冲区分配内存*/
  wa=(struct work_area *)xmalloc(threads*sizeof(struct work_area));
  for(a=0;a<threads;a++){
    wa[a].flag=0;
    wa[a].data=(char ***)xmalloc((paired+1)*sizeof(char **));
    for(b=0;b<=paired;b++){
      wa[a].data[b]=(char **)xmalloc((reads*4)*sizeof(char *));
      for(c=0;c<(reads*4);c++){
        wa[a].data[b][c]=(char *)xmalloc(max_line_length*sizeof(char));
      }
    }
  }
  predatai();
  // if(split==1)predatao();

  th=(pthread_t *)xmalloc(sizeof(pthread_t)*threads);
  wtid=(int *)xmalloc(sizeof(int)*threads);
  /*创建线程,注意局部变量问题,因为循环一直进行,局部变量可能已经销毁*/
  for(t=0;t<threads-(paired+1);t++){
    wtid[t]=t;
    if(-1==pthread_create(&th[t],NULL,&workbench,(void*)&wtid[t])){
      error("create thread error!\n");
      abort_file();
    }
  }
  /* 等待所有线程结束 */
  for(t=0;t<threads-(paired+1);t++){
    pthread_join(th[t],NULL);
  }
  /*printf("等待所有线程结束\n"); //测试*/
  FREE(wtid);
  FREE(th);
  // if(split==1)waitpreouto();
  waitpreouti();
  /*printf("等待输入线程结束\n"); //测试*/
  /*释放数据缓存区*/
  for(a=0;a<threads;a++){
    for(b=0;b<=paired;b++){
      for(c=0;c<reads*4;c++){
        FREE(wa[a].data[b][c]);
      }
      FREE(wa[a].data[b]);
    }
    FREE(wa[a].data);
  }
  FREE(wa);
}/*for run*/


/*各线程工作台*/
void workbench(void *itid)
{
int i=0,j=0,f=0,w=0,m=0,n=0,discard=0,L=0,t=0,k=0;
uli disflag=0ul;/*丢弃标识*/
int a=0,b=0,c=0,d=0;
int *id=(int *)itid;
char *p=NULL;

/*线程总统计信息*/
uli sstat[2][2][MAX_LINE_LENGTH][MAX_QUALITY][5]={0ul};
uli s_dstat[TF]={0ul};
uli rlenstat[2][2][MAX_LINE_LENGTH]={0ul};

/*动态分配内存，函数里不允许分配太大数组,这里有点浪费空间有待优化*/
uli ***end3t=(uli ***)xmalloc(sizeof(uli **)*(paired+1));
uli ***end5t=(uli ***)xmalloc(sizeof(uli **)*(paired+1));
uli ***endAt=(uli ***)xmalloc(sizeof(uli **)*(paired+1));
uli ****end3n=(uli ****)xmalloc(sizeof(uli ***)*(paired+1));
uli ****end5n=(uli ****)xmalloc(sizeof(uli ***)*(paired+1));
uli ****endAn=(uli ****)xmalloc(sizeof(uli ***)*(paired+1));
for(a=0;a<=paired;a++){
  end3t[a]=(uli **)xmalloc(sizeof(uli *)*MAX_ADAPT_NUM);
  end5t[a]=(uli **)xmalloc(sizeof(uli *)*MAX_ADAPT_NUM);
  endAt[a]=(uli **)xmalloc(sizeof(uli *)*MAX_ADAPT_NUM);
  end3n[a]=(uli ***)xmalloc(sizeof(uli **)*MAX_ADAPT_NUM);
  end5n[a]=(uli ***)xmalloc(sizeof(uli **)*MAX_ADAPT_NUM);
  endAn[a]=(uli ***)xmalloc(sizeof(uli **)*MAX_ADAPT_NUM);
  for(b=0;b<MAX_ADAPT_NUM;b++){
    end3t[a][b]=(uli *)xmalloc(sizeof(uli)*MAX_LINE_LENGTH);
    end5t[a][b]=(uli *)xmalloc(sizeof(uli)*MAX_LINE_LENGTH);
    endAt[a][b]=(uli *)xmalloc(sizeof(uli)*MAX_LINE_LENGTH);
    end3n[a][b]=(uli **)xmalloc(sizeof(uli *)*MAX_LINE_LENGTH);
    end5n[a][b]=(uli **)xmalloc(sizeof(uli *)*MAX_LINE_LENGTH);
    endAn[a][b]=(uli **)xmalloc(sizeof(uli *)*MAX_LINE_LENGTH);
    for(c=0;c<MAX_LINE_LENGTH;c++){
      end3n[a][b][c]=(uli *)xmalloc(sizeof(uli)*64);
      end5n[a][b][c]=(uli *)xmalloc(sizeof(uli)*64);
      endAn[a][b][c]=(uli *)xmalloc(sizeof(uli)*64);
    }/*for for c*/
  }/*for for b*/
}/*for for a*/

/*记录每条READ处理情况*/
char **outbuf=(char **)xmalloc(sizeof(char *)*(paired+1));
char **ob=(char **)xmalloc(sizeof(char *)*(paired+1));
for(i=0;i<=paired;i++)
  outbuf[i]=(char *)xmalloc(sizeof(char)*max_line_length*reads*4);
int outlen[2]={0};

do{
  f=0,w=-1;
  pthread_mutex_lock(&lock_ifile);
  for(i=n;i<threads;i++){
    if(wa[i].flag&16){
      f++;
    }else if(wa[i].flag&4){
      wa[i].flag=8; n=w=i; break;
    }
  }/*for for i*/
  if(w<0){
    for(i=0;i<n;i++){
      if(wa[i].flag&16){
        f++;
      }else if(wa[i].flag&4){
        wa[i].flag=8; n=w=i; break;
      }
    }/*for for i*/
  }
  pthread_mutex_unlock(&lock_ifile);
  /*有数据处理*/
  if(w>=0){
    /*printf("线程:%d,块:%d,R1:%d,R2:%d\n",*id,w,wa[w].num[0],wa[w].num[1]);//测试请求块*/
    /*简单做个校验,双端数量不一致退出*/
    if(paired && wa[w].num[0]!=wa[w].num[1]){
      error("read1 buf is not equal read2 buf!");
      do_exit(ERROR);
    }
    for(i=0;i<=paired;i++){
      memzero(outbuf[i],sizeof(char)*max_line_length*reads*4);
      outbuf[i][0]='\0';
      outlen[i]=0;
      ob[i]=outbuf[i];
    }

    for(m=0;m<wa[w].num[0];m+=4){
      
      discard=0;
      disflag=0ul;
      char *Info[2]={NULL},*Read[2]={NULL},*Extra[2]={NULL},*Qual[2]={NULL};
      
      /*READ统计信息*/
      struct RSTAT rstat[2][2]={0};

      for(t=0;t<=paired;t++){
        Info[t] =deletLF(wa[w].data[t][m  ]);
        Read[t] =deletLF(wa[w].data[t][m+1]);
        Extra[t]=deletLF(wa[w].data[t][m+2]);
        Qual[t] =deletLF(wa[w].data[t][m+3]);
      }

      /*READ1/2过滤前后统计*/
      for(t=0;t<=paired;t++){
        rlenstat[t][0][strlen(Read[t])]++;
        get_stat(Read[t],Qual[t],t,0,rstat);
      }

      /*去除末端N*/
      if(trim_n>0){
        for(t=0;t<=paired;t++){
          while(*Read[t]=='N'){Read[t]++; Qual[t]++;}
          L=strlen(Read[t]);
          while(Read[t][L-1]=='N'){Read[t][L-1]='\0'; Qual[t][L-1]='\0';L--;}
        }
      }/*for trim_n*/
      discard=paired?(*Read[0]=='\0' && *Read[1]=='\0'):*Read[0]=='\0';
      /*去接头模块*/
      if(!discard){
        for(t=0;t<=paired;t++){
          int best5[6]={0},bestn5=0,best3[6]={0},bestn3=0;
          if(adapts[t]!=NULL){ /*3'端接头*/
            for(j=0;j<adaptn[t];j++){
              int res[6]={0};
              /*printf("接头:%s,接头标识:%d,READ:%s,错误率:%g,Overlap:%d\n",adapts[t][j],adaptf[t][j],Read[t],error_rate,overlap);//测试*/
              if(Adapter(adapts[t][j],Read[t],adaptf[t][j],error_rate,overlap,no_indels,res)){
                if(res[4]>best3[4]){
                  for(k=0;k<6;k++) best3[k]=res[k];
                  bestn3=j;
                }
              }/*for if Adapter()*/
            }/*for for j*/
          }/*for if 3'adapts*/
          if(fronts[t]!=NULL){ /*5'端接头*/
            for(j=0;j<frontn[t];j++){
              int res[6]={0};
              /*printf("接头:%s,接头标识:%d,READ:%s,错误率:%g,Overlap:%d\n",fronts[t][j],frontf[t][j],Read[t],error_rate,overlap);//测试*/
              if(Adapter(fronts[t][j],Read[t],frontf[t][j],error_rate,overlap,no_indels,res)){
                if(res[4]>best5[4]){
                  for(k=0;k<6;k++)best5[k]=res[k];
                  bestn5=j;
                }
              }/*for if Adapter()*/
            }/*for for j*/
          }/*for if 5'adapts*/
          /*下面函数决定切掉哪里*/
          if(best5[4]>best3[4]){
            if(best5[3]>=strlen(Read[t])){
              /*printf("%d: %d,%d,%d,%d,%d,%d,   %d\n",t,best[0],best[1],best[2],best[3],best[4],best[5],strlen(Read[t]));//测试代码*/
              *Read[t]='\0';
              *Qual[t]='\0';
            }else{
              Read[t]+=best5[3];
              Qual[t]+=best5[3];
            }
            end5n[t][bestn5][best5[0]][best5[1]-1ul]++;
            setbit(end5t[t][bestn5][best5[0]],(uli)best5[1]-1ul );
            disflag|=AN;
            /*printf("[5:%d,%d,%d,%d]\n",t,bestn,best[0],best[1]); //测试*/
          }else if(best5[4]<best3[4]){
            *(Read[t]+best3[2])='\0';
            *(Qual[t]+best3[2])='\0';
            end3n[t][bestn3][best3[0]][best3[1]-1ul]++;
            setbit(end3t[t][bestn3][best3[0]],(uli)best3[1]-1ul);
            disflag|=AN;
            /*printf("[3:%d,%d,%d,%d]\n",t,bestn,best[0],best[1]); //测试*/
          }else if(best5[4]==best3[4] && best5[4]!=0){
            // if(best5[3]>=strlen(Read[t])){
            //   /*printf("%d: %d,%d,%d,%d,%d,%d,   %d\n",t,best[0],best[1],best[2],best[3],best[4],best[5],strlen(Read[t]));//测试代码*/
            //   *Read[t]='\0';
            //   *Qual[t]='\0';
            // }else{
            //   Read[t]+=best5[3];
            //   Qual[t]+=best5[3];
            // }
            // end5n[t][bestn5][best5[0]][best5[1]-1ul]++;
            // setbit(end5t[t][bestn5][best5[0]],(uli)best5[1]-1ul );
            // disflag|=AN;

            *(Read[t]+best3[2])='\0';
            *(Qual[t]+best3[2])='\0';
            end3n[t][bestn3][best3[0]][best3[1]-1ul]++;
            setbit(end3t[t][bestn3][best3[0]],(uli)best3[1]-1ul);
            disflag|=AN;
          }/*for if 切割接头*/
          if(anywheres[t]!=NULL){/*任意端接头*/
            int best[6]={0},bestn=0;
            for(j=0;j<anywheren[t];j++){
              /*printf("%d --> %s \n",j,anywheres[t][j]);//测试代码*/
              int res[6]={0};
              if(Adapter(anywheres[t][j],Read[t],anywheref[t][j],error_rate,overlap,no_indels,res)){
                if(res[4]>best[4]){
                  for(k=0;k<6;k++)best[k]=res[k];
                  bestn=j;
                }
              }
            }/*for for w*/
            if(best[4]>0){
              /*printf("匹配结果:%d ,%d ,%d, %d ,%d ,%d\n",best[0],best[1],best[2],best[3],best[4],best[5]);//测试代码*/
              if(best[2]==0){
                if(best[3]>=strlen(Read[t])){
                  *Read[t]='\0';
                  *Qual[t]='\0';
                }else{
                  Read[t]+=best[3];
                  Qual[t]+=best[3];
                  //printf("A:%s\n",Read[t]);
                }
              }else{
                *(Read[t]+best[2])='\0';
                *(Qual[t]+best[2])='\0';
                //printf("B:%s\n",Read[t]);
              }
              setbit(endAt[t][bestn][best[0]],(uli)best[1]-1ul);
              endAn[t][bestn][best[0]][best[1]-1]++;
              disflag|=AN;
              /*if(Read[t]=='\0' || Qual[t]=='\0'){
                printf("匹配数:%d ,匹配开始:%d ,匹配截止:%d\n",best[4],best[2],best[3]);
                printf("%s ** %s\n",Read[t],Qual[t]);
              }//测试代码*/
            }/*for if best*/
          }/*for if anywheres*/
        }/*for for t*/
      }/*for discard 去接头*/
      discard=paired?(*Read[0]=='\0' && *Read[1]=='\0'):*Read[0]=='\0';
      /*去除低质量*/
      if(!discard && quality_low>0){
        for(t=0;t<=paired;t++){
          if(*Read[0]=='\0')continue;
          while((*Qual[t]-quality_base)<quality_low){Read[t]++; Qual[t]++;}
          L=strlen(Read[t]);
          while((Qual[t][L-1]-quality_base)<quality_low){Read[t][L-1]='\0'; Qual[t][L-1]='\0';L--;}
        }
      }/*for trim_n*/
      discard=paired?(*Read[0]=='\0' && *Read[1]=='\0'):*Read[0]=='\0';
      if(!discard){
        for(t=0;t<=paired;t++){
          get_stat(Read[t],Qual[t],t,1,rstat);
        }
        discard=filter(1,rstat,&disflag);
      }
      /*合并丢弃标识*/
      if(disflag!=0)s_dstat[disflag]++;
      /*判断READ过滤情况*/
      if(discard){
        /*只将过滤前的归并*/
        merge_stat_s2t(1,sstat,rstat);
      }else{
        for(t=0;t<=paired;t++){
        L=strlen(Info[t]);
        outlen[t]+=L+1;
        strncpy(ob[t],Info[t],L);
        ob[t]+=L;
        *ob[t]++='\n';
        L=strlen(Read[t]);
        rlenstat[t][1][L]++;
        outlen[t]+=L+1;
        strncpy(ob[t],Read[t],L);
        ob[t]+=L;
        *ob[t]++='\n';
        L=strlen(Extra[t]);
        outlen[t]+=L+1;
        strncpy(ob[t],Extra[t],L);
        ob[t]+=L;
        *ob[t]++='\n';
        L=strlen(Qual[t]);
        outlen[t]+=L+1;
        strncpy(ob[t],Qual[t],L);
        ob[t]+=L;
        *ob[t]++='\n';
        }
        merge_stat_s2t(0,sstat,rstat);
      }/*for if discard*/
    }/*for for m*/
    /*合格READ写入文件*/
    m=put_str(outlen,outbuf);/*写入数据块*/
    
    /*处理完置位置位*/
    pthread_mutex_lock(&lock_ifile);
    clrbit(file_out_flag,m);
    wa[w].flag=0;
    pthread_mutex_unlock(&lock_ifile);
    /*sleep(2);//测试代码*/
  }else{
    usleep(1);
  }/*for if w*/
}while(f<threads);
for(i=0;i<=paired;i++)
  FREE(outbuf[i]);
FREE(outbuf);
FREE(ob);
pthread_mutex_lock(&lock_stat);
merge_stat_t2a(tstat,sstat);
  /*合并接头信息*/
for(a=0;a<=paired;a++){
  for(b=0;b<MAX_ADAPT_NUM;b++){
    for(c=0;c<MAX_LINE_LENGTH;c++){
      end3Type[a][b][c]|=end3t[a][b][c];
      end5Type[a][b][c]|=end5t[a][b][c];
      endAnyType[a][b][c]|=endAt[a][b][c];
      for(d=0;d<64;d++){
        end3TypeN[a][b][c][d]+=end3n[a][b][c][d];
        end5TypeN[a][b][c][d]+=end5n[a][b][c][d];
        endAnyTypeN[a][b][c][d]+=endAn[a][b][c][d];
      }/*for d*/
    }/*for c*/
  }/*for b*/
  for(b=0;b<2;b++){
    for(c=0;c<MAX_LINE_LENGTH;c++){
      tlenstat[a][b][c]+=rlenstat[a][b][c];
    }
  }
}/*for a*/
for(b=0;b<TF;b++){
  t_dstat[b]+=s_dstat[b];
}
pthread_mutex_unlock(&lock_stat); 

/*释放内存*/
for(a=0;a<=paired;a++){ 
  for(b=0;b<MAX_ADAPT_NUM;b++){
    for(c=0;c<MAX_LINE_LENGTH;c++){
      if(end3n[a][b][c]!=NULL)FREE(end3n[a][b][c]);
      if(end5n[a][b][c]!=NULL)FREE(end5n[a][b][c]);
      if(endAn[a][b][c]!=NULL)FREE(endAn[a][b][c]);
    }/*for for c*/
    if(end3t[a][b]!=NULL)FREE(end3t[a][b]);
    if(end5t[a][b]!=NULL)FREE(end5t[a][b]);
    if(endAt[a][b]!=NULL)FREE(endAt[a][b]);
    if(end3n[a][b]!=NULL)FREE(end3n[a][b]);
    if(end5n[a][b]!=NULL)FREE(end5n[a][b]);
    if(endAn[a][b]!=NULL)FREE(endAn[a][b]);
  }/*for for b*/
  if(end3t[a]!=NULL)FREE(end3t[a]);
  if(end5t[a]!=NULL)FREE(end5t[a]);
  if(endAt[a]!=NULL)FREE(endAt[a]);
  if(end3n[a]!=NULL)FREE(end3n[a]);
  if(end5n[a]!=NULL)FREE(end5n[a]);
  if(endAn[a]!=NULL)FREE(endAn[a]);
}/*for for a*/
if(end3t!=NULL)FREE(end3t);
if(end5t!=NULL)FREE(end5t);
if(endAt!=NULL)FREE(endAt);
if(end3n!=NULL)FREE(end3n);
if(end5n!=NULL)FREE(end5n);
if(endAn!=NULL)FREE(endAn);

}/*for workbench*/
