#ifndef _QC_H_
  #include "qc.h"
#endif

#include <signal.h>
#include <time.h>

int paired=0;              
char **adapts[2]={NULL};   
int adaptn[2]={0};        
int *adaptf[2]={0};        
char **fronts[2]={NULL};   
int frontn[2]={0};         
int *frontf[2]={0};        
char **anywheres[2]={NULL};
int anywheren[2]={0};      
int *anywheref[2]={0};     
int no_indels=-1;         
float error_rate=-1;      
int overlap=-1;           
int quality_threshold=-1; 
float quality_ratio=-1;   
int quality_low=-1;       
int trim_n=-1;             
float ratio_n=-1;          
int min_length=0;          
int max_length=0;          
float gc_content=0;        
int quality_base=-1;       
char *outname[2]={NULL,NULL};
char *outpath[2]={NULL,NULL};
char *xml=NULL;              
char *html=NULL;             
char *text=NULL;             
char *json=NULL;             
char *title=NULL;            
int split=-1;                
int threads=-1;              
int reads=-1;                
int view_params=-1;          
char *ifiles[2]={NULL};      
uli iftype=0;                
uli oftype=0;                
char exe_path[MAX_PATH]={0}; 
char exe_name[MAX_PATH]={0}; 
int max_line_length=-1;      

FILE *ifp[2]={NULL};         
gzFile igzfp[2]={0};         
FILE *HTML=NULL;             
FILE *TEXT=NULL;             
FILE *JSON=NULL;             
FILE **ofp[2]={NULL};        
char **ofname[2]={NULL};     
gzFile *ogzfp[2]={NULL};     
int foreground=0;            

pthread_mutex_t lock_ifile;
pthread_mutex_t lock_ofile;
pthread_mutex_t lock_stat;

pthread_t *tdi=NULL;
pthread_t *tdo=NULL;

int itid[2]={0};   
int *wtid=NULL;    
int iflag=-1;      

uli file_in_flag =0ul;
uli file_out_flag=0ul;
int file_out_loc_flag=0;

struct work_area *wa=NULL;

uli end3Type[2][MAX_ADAPT_NUM][MAX_LINE_LENGTH]={0ul};        
uli end3TypeN[2][MAX_ADAPT_NUM][MAX_LINE_LENGTH][64]={0ul};   
uli end5Type[2][MAX_ADAPT_NUM][MAX_LINE_LENGTH]={0ul};        
uli end5TypeN[2][MAX_ADAPT_NUM][MAX_LINE_LENGTH][64]={0ul};   
uli endAnyType[2][MAX_ADAPT_NUM][MAX_LINE_LENGTH]={0ul};      
uli endAnyTypeN[2][MAX_ADAPT_NUM][MAX_LINE_LENGTH][64]={0ul}; 

uli tstat[2][2][MAX_LINE_LENGTH][MAX_QUALITY][5]={0ul};
uli t_dstat[TF]={0};
uli tlenstat[2][2][MAX_LINE_LENGTH]={0};

struct SUMMARY summary[2][2]={0ul};

void main(int argc, char *argv[])
{
  parse_user_parms(argc,argv);
  load_default_parms();
  checkup_params();
  if(view_params==1)viewparams();
  test_iofiles();

  foreground = signal(SIGINT, SIG_IGN) != SIG_IGN;
  if (foreground)
	  (void) signal (SIGINT, (sig_type)abort_file);
  
  if (signal(SIGTERM, SIG_IGN) != SIG_IGN)
	  (void) signal(SIGTERM, (sig_type)abort_file);
  
  pthread_mutex_init(&lock_ifile,NULL);
  pthread_mutex_init(&lock_ofile,NULL);
  pthread_mutex_init(&lock_stat,NULL);
             
  run();

  pthread_mutex_destroy(&lock_ifile);
  pthread_mutex_destroy(&lock_ofile);
  pthread_mutex_destroy(&lock_stat);

  get_summary();
  if(html!=NULL){
    html_head();
    html_summary();
    html_rstat();  
    html_adapter();
    html_qualpic();
    html_basepic();
    html_length(); 
    html_cmd(argv);
    html_tail();
  }

  if(text!=NULL)text_report();
  
  if(json!=NULL)json_out();

  do_exit(OK);
}/*main*/

void do_exit(int exitcode)
{
  int i=0,j=0;

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
    
    FREE(outpath[i]);
    FREE(outname[i]);
  }/*i*/

  for(i=0;i<=paired;i++){
 
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
  
    FREE(ofname[i]);
  }
  FREE(xml);

  if(HTML!=NULL)fclose(HTML);
  if(TEXT!=NULL)fclose(TEXT);
  if(JSON!=NULL)fclose(JSON);
  if(exitcode!=3)exit(exitcode);
}

void abort_file()
{
  int i=0,j=0;
  for(i=0;i<=paired;i++){
    if(ifiles[i]==NULL)continue;
    for(j=0;j<split;j++)
      if(ofname[i][j]!=NULL)unlink(ofname[i][j]);
  }/* for i*/

  if(html)unlink(html);
  if(text)unlink(text);
  if(json)unlink(json);
  do_exit(ERROR);
}/*for abort_file*/

void run(){
  pthread_t *th=NULL;
  int t=0,a=0,b=0,c=0,d=0;

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

  for(t=0;t<threads-(paired+1);t++){
    wtid[t]=t;
    if(-1==pthread_create(&th[t],NULL,&workbench,(void*)&wtid[t])){
      error("create thread error!\n");
      abort_file();
    }
  }

  for(t=0;t<threads-(paired+1);t++){
    pthread_join(th[t],NULL);
  }
 
  FREE(wtid);
  FREE(th);
  // if(split==1)waitpreouto();
  waitpreouti();

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

void workbench(void *itid)
{
int i=0,j=0,f=0,w=0,m=0,n=0,discard=0,L=0,t=0,k=0;
uli disflag=0ul;
int a=0,b=0,c=0,d=0;
int *id=(int *)itid;
char *p=NULL;

uli sstat[2][2][MAX_LINE_LENGTH][MAX_QUALITY][5]={0ul};
uli s_dstat[TF]={0ul};
uli rlenstat[2][2][MAX_LINE_LENGTH]={0ul};

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

  if(w>=0){

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
      
      struct RSTAT rstat[2][2]={0};

      for(t=0;t<=paired;t++){
        Info[t] =deletLF(wa[w].data[t][m  ]);
        Read[t] =deletLF(wa[w].data[t][m+1]);
        Extra[t]=deletLF(wa[w].data[t][m+2]);
        Qual[t] =deletLF(wa[w].data[t][m+3]);
      }

      for(t=0;t<=paired;t++){
        rlenstat[t][0][strlen(Read[t])]++;
        get_stat(Read[t],Qual[t],t,0,rstat);
      }

      if(trim_n>0){
        for(t=0;t<=paired;t++){
          while(*Read[t]=='N'){Read[t]++; Qual[t]++;}
          L=strlen(Read[t]);
          while(Read[t][L-1]=='N'){Read[t][L-1]='\0'; Qual[t][L-1]='\0';L--;}
        }
      }/*for trim_n*/
      discard=paired?(*Read[0]=='\0' && *Read[1]=='\0'):*Read[0]=='\0';
      if(!discard){
        for(t=0;t<=paired;t++){
          int best5[6]={0},bestn5=0,best3[6]={0},bestn3=0;
          if(adapts[t]!=NULL){ 
            for(j=0;j<adaptn[t];j++){
              int res[6]={0};
              if(Adapter(adapts[t][j],Read[t],adaptf[t][j],error_rate,overlap,no_indels,res)){
                if(res[4]>best3[4]){
                  for(k=0;k<6;k++) best3[k]=res[k];
                  bestn3=j;
                }
              }/*for if Adapter()*/
            }/*for for j*/
          }/*for if 3'adapts*/
          if(fronts[t]!=NULL){ 
            for(j=0;j<frontn[t];j++){
              int res[6]={0};
              
              if(Adapter(fronts[t][j],Read[t],frontf[t][j],error_rate,overlap,no_indels,res)){
                if(res[4]>best5[4]){
                  for(k=0;k<6;k++)best5[k]=res[k];
                  bestn5=j;
                }
              }/*for if Adapter()*/
            }/*for for j*/
          }/*for if 5'adapts*/

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
            /*printf("[3:%d,%d,%d,%d]\n",t,bestn,best[0],best[1]);*/
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
          }/*for if */
          if(anywheres[t]!=NULL){
            int best[6]={0},bestn=0;
            for(j=0;j<anywheren[t];j++){
              /*printf("%d --> %s \n",j,anywheres[t][j]);*/
              int res[6]={0};
              if(Adapter(anywheres[t][j],Read[t],anywheref[t][j],error_rate,overlap,no_indels,res)){
                if(res[4]>best[4]){
                  for(k=0;k<6;k++)best[k]=res[k];
                  bestn=j;
                }
              }
            }/*for for w*/
            if(best[4]>0){
              
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
                printf("%s ** %s\n",Read[t],Qual[t]);
              }*/
            }/*for if best*/
          }/*for if anywheres*/
        }/*for for t*/
      }/*for discard */
      discard=paired?(*Read[0]=='\0' && *Read[1]=='\0'):*Read[0]=='\0';
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

      if(disflag!=0)s_dstat[disflag]++;

      if(discard){

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

    m=put_str(outlen,outbuf);
    
    pthread_mutex_lock(&lock_ifile);
    clrbit(file_out_flag,m);
    wa[w].flag=0;
    pthread_mutex_unlock(&lock_ifile);
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
