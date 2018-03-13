#ifndef _QC_H_
  #include "qc.h"
#endif

#include <libxml/parser.h>
#include <getopt.h>

 /*长短参数*/
static struct option const long_options[] = {
{"adapter", required_argument, 0, 'a'},     /* READ1 3'接头参数 */
{"front", required_argument, 0, 'g'},       /* READ1 5'接头参数 */
{"anywhere", required_argument, 0, 'b'},    /* READ1 任意接头 */
{"adapter2", required_argument, 0, 'A'},    /* READ2 3'接头参数 */
{"front2", required_argument, 0, 'G'},      /* READ2 5'接头参数 */
{"anywhere2", required_argument, 0, 'B'},   /* READ1 3'任意接头 */
{"no—indels", no_argument, 0, 'i'},         /* 默认是允许indels加此参数关闭此选项 */
{"error-rate",required_argument ,0 , 'e'},  /* 允许最大的错配率 */
{"overlap", required_argument, 0, 'O'},     /* 接头与序列的重叠区域 */
{"quality-filter",required_argument ,0,'q'},/* READ 根据质量值和比例过滤 */
{"quality-low",required_argument ,NULL , 'Q'}, /* READ 过滤两端低质量碱基 */
{"trim-n", no_argument, NULL, 'n'},            /* 去掉两端N */
{"ratio-n", required_argument, NULL, 'r'},     /* READ 中N占比例大于此值将被过滤掉 */
{"min-length", required_argument, 0, 'm'},  /* READ最小长度 */
{"max-length", required_argument, 0, 'M'},  /* READ最大长度 */
{"gc-content", required_argument, 0, 'c'},  /* GC含量*/
{"quality-base", required_argument, 0, 'E'},/* GC含量*/
{"output1", required_argument, 0, 'o'},     /* READ1 输出文件 */
{"output2", required_argument, 0, 'p'},     /* READ1 输出文件 */
{"xml", required_argument, 0, 'x'},         /* 默认配置文件路径 */
{"html", required_argument, 0, 'H'},        /* html路径 */
{"text", required_argument, 0, 'T'},        /* text路径 */
{"json", required_argument, 0, 'j'},        /* json路径 */
{"title", required_argument, 0, 'R'},       /* 输出标题字符串 */
{"split", required_argument, 0, 's'},       /* 每个输出文件分几块 */
{"threads", required_argument, 0, 't'},     /* 线程数 */
{"reads", required_argument, 0, 'k'},       /* 线程工作的read数 */
{"view-params", no_argument, 0, 'w'}, /* 预览参数模式 */
/* 帮助、版本 */
{"help", no_argument, 0, 'h'},        /* 帮助文档 */
{"help", no_argument, 0, '?'},        /* 帮助文档 */
{"version", no_argument, 0, 'v'},     /* 版本信息 */
{"version", no_argument, 0, 'V'},     /* 版本信息 */
{0, 0, 0, 0} }; /* option结构闭合处 */

  /*解析用户输入*/
void parse_user_parms (int argc, char *argv[])
{
  if(argc<=1){
  help(argv[0]);
  do_exit(ERROR);
  }

  int fcount=0,L=0;
  int optc=0,m=0,n=0;
  char *p=NULL;

  while ( (optc = getopt_long (argc, argv, "a:g:b:A:G:B:ie:O:q:Q:nr:m:M:c:E:o:p:x:H:T:j:R:s:t:k:wh?vV", long_options, NULL) ) != -1 ){
  switch (optc){
    case 'a':
      ++adaptn[0];
      if(adaptn[0]==1){
        adapts[0]=(char **)xmalloc(sizeof(char *));
        adaptf[0]=(int *)xmalloc(sizeof(int));
      }else{
        adapts[0]=(char **)xrealloc(adapts[0],sizeof(char *)*adaptn[0]);
        adaptf[0]=(int *)xrealloc(adaptf[0],sizeof(int)*adaptn[0]);
      }
      adapts[0][adaptn[0]-1]=optarg;
      L=strlen(optarg);
      if(adapts[0][adaptn[0]-1][L-1]=='$'){
        adapts[0][adaptn[0]-1][L-1]='\0';
        adaptf[0][adaptn[0]-1]=SUFFIX;
      }else{
        adaptf[0][adaptn[0]-1]=BACK;
      }
      break;
    case 'g':
      ++frontn[0];
      if(frontn[0]==1){
        fronts[0]=(char **)xmalloc(sizeof(char *));
        frontf[0]=(int *)xmalloc(sizeof(int));
      }else{
        fronts[0]=(char **)xrealloc(fronts[0],sizeof(char *)*frontn[0]);
        frontf[0]=(int *)xrealloc(frontf[0],sizeof(int)*frontn[0]);
      }
      fronts[0][frontn[0]-1]=optarg;
      if(fronts[0][frontn[0]-1][0]=='^'){
        fronts[0][frontn[0]-1]++;
        frontf[0][frontn[0]-1]=PREFIX;
      }else{
        frontf[0][frontn[0]-1]=FRONT;
      }
      break;
    case 'b':
      ++anywheren[0];
      if(anywheren[0]==1){
        anywheres[0]=(char **)xmalloc(sizeof(char *));
        anywheref[0]=(int *)xmalloc(sizeof(int));
      }else{
        anywheres[0]=(char **)xrealloc(anywheres[0],sizeof(char *)*anywheren[0]);
        anywheref[0]=(int *)xrealloc(anywheref[0],sizeof(int)*anywheren[0]);
      }
      anywheres[0][anywheren[0]-1]=optarg;
      anywheref[0][anywheren[0]-1]=ANYWHERE;
      break;
    case 'A':
      ++adaptn[1];
      if(adaptn[1]==1){
        adapts[1]=(char **)xmalloc(sizeof(char *));
        adaptf[1]=(int *)xmalloc(sizeof(int));
      }else{
        adapts[1]=(char **)xrealloc(adapts[1],sizeof(char *)*adaptn[1]);
        adaptf[1]=(int *)xrealloc(adaptf[1],sizeof(int)*adaptn[1]);
      }
      adapts[1][adaptn[1]-1]=optarg;
      L=strlen(optarg);
      if(adapts[1][adaptn[1]-1][L-1]=='$'){
        adapts[1][adaptn[1]-1][L-1]='\0';
        adaptf[1][adaptn[1]-1]=SUFFIX;
      }else{
        adaptf[1][adaptn[1]-1]=BACK;
      }
      break;
    case 'G':
      ++frontn[1];
      if(frontn[1]==1){
        fronts[1]=(char **)xmalloc(sizeof(char *));
        frontf[1]=(int *)xmalloc(sizeof(int));
      }else{
        fronts[1]=(char **)xrealloc(fronts[1],sizeof(char *)*frontn[1]);
        frontf[1]=(int *)xrealloc(frontf[1],sizeof(int)*frontn[1]);
      }
      fronts[1][frontn[1]-1]=optarg;
      if(fronts[1][frontn[1]-1][0]=='^'){
        fronts[1][frontn[1]-1]++;
        frontf[1][frontn[1]-1]=PREFIX;
      }else{
        frontf[1][frontn[1]-1]=FRONT;
      }
      break;
    case 'B':
      ++anywheren[1];
      if(anywheren[1]==1){
        anywheres[1]=(char **)xmalloc(sizeof(char *));
        anywheref[1]=(int *)xmalloc(sizeof(int));
      }else{
        anywheres[1]=(char **)xrealloc(anywheres[1],sizeof(char *)*anywheren[1]);
        anywheref[1]=(int *)xrealloc(anywheref[1],sizeof(int)*anywheren[1]);
      }
      anywheres[1][anywheren[1]-1]=optarg;
      anywheref[1][anywheren[1]-1]=ANYWHERE;
      break;
    case 'i':
      no_indels=1;break;
    case 'e':
      error_rate=atof(optarg);break;
    case 'O':
      overlap=atoi(optarg);break;
    case 'q':
      sscanf(optarg,"%d,%f",&quality_threshold,&quality_ratio);
      break;
    case 'Q':
      sscanf(optarg,"%d",&quality_low);break;
    case 'n':
      trim_n=1;break;
    case 'r':
      ratio_n=atof(optarg);break;
    case 'm':
      min_length=atoi(optarg);break;
    case 'M':
      max_length=atoi(optarg);break;
    case 'c': 
      gc_content=atof(optarg);break;
    case 'E': 
      quality_base=atoi(optarg);break;
    case 'o':
      if ((p = strrchr(optarg, PATH_SEP)) != NULL){
      outname[0]=(char *)xmalloc(sizeof(char)*strlen(p));
      outpath[0]=(char *)xmalloc(sizeof(char)*(p-optarg+2));
      strcpy(outname[0],p+1);
      strncpy(outpath[0],optarg,p-optarg+1);
      outpath[0][p-optarg+1]='\0';
      }else{
      outname[0]=(char *)xmalloc(sizeof(char)*MAX_PATH);
      outpath[0]=(char *)xmalloc(sizeof(char)*MAX_PATH);
      getcwd(outpath[0],MAX_PATH);
      strcpy(outname[0],optarg);
      strcat(outpath[0],"/");
      }
      /*printf("文件路径:%s,文件名称:%s\n",outpath[0],outname[0]);//测试*/
      break;
    case 'p': 
      if ((p = strrchr(optarg, PATH_SEP)) != NULL){
      outname[1]=(char *)xmalloc(sizeof(char)*strlen(p));
      outpath[1]=(char *)xmalloc(sizeof(char)*(p-optarg+2));
      strcpy(outname[1],p+1);
      strncpy(outpath[1],optarg,p-optarg+1);
      outpath[1][p-optarg+1]='\0';
      }else{
      outname[1]=(char *)xmalloc(sizeof(char)*MAX_PATH);
      outpath[1]=(char *)xmalloc(sizeof(char)*MAX_PATH);
      getcwd(outpath[1],MAX_PATH);
      strcpy(outname[1],optarg);
      strcat(outpath[1],"/");
      }
      /*printf("文件路径:%s,文件名称:%s\n",outpath[1],outname[1]);//测试*/
      break;
    case 'x': 
      xml=optarg;break;
    case 'H': 
      html=optarg;break;
    case 'T': 
      text=optarg;break;
    case 'j': 
      json=optarg;break;
    case 'R': 
      title=optarg;break;
    case 's': 
      split=atoi(optarg);break;
    case 't': 
      threads=atoi(optarg);break;
    case 'k': 
      reads=atoi(optarg);break;
    case 'w': 
      view_params=1;break;
    case 'h':case '?':
      help(argv[0]);do_exit(OK);break;
    case 'v': case 'V':
      version(argv[0]);do_exit(OK);break;
    default:
      usage(argv[0]);do_exit(ERROR);
  }/*for switch*/
  }/*for while*/

    /*输入文件数校验*/
  fcount = argc - optind;
  verification(fcount<=0," Error: at least one input files!\n");
  verification(fcount>2," Error: too many input file!\n");
  
    /* 保存文件名 */
  ifiles[0]=argv[optind];
  if(fcount==2)ifiles[1]=argv[optind+1],paired=1;

    /*将接头排序*/
  for(m=0;m<=paired;m++){
    if(adapts[m]!=NULL) sort_str(adapts[m],adaptf[m],adaptn[m]);
    if(fronts[m]!=NULL) sort_str(fronts[m],frontf[m],frontn[m]);
    if(anywheres[m]!=NULL)sort_str(anywheres[m],anywheref[m],anywheren[m]);
  }/*for for m*/
    /*获取可执行文件路径和名称*/
  GetCurrentExcutableFilePathName(exe_path, exe_name,sizeof(exe_path));
  /*printf("路径:%s,名称:%s\n",exe_path,exe_name);//测试*/
    /*设置xml加载路径*/
  if(xml==NULL){
    xml=(char *)xmalloc(sizeof(char)*MAX_PATH);
    strcpy(xml,exe_path);
    strcat(xml,"conf/default.xml");
    /*printf("xml文件路径:%s\n",xml);//测试*/
  }/*for if xml*/
}/*parse_user_parms*/

  /*加载默认参数*/
void load_default_parms()
{
  int f=0;
  xmlDocPtr doc;   /* xml整个文档的树形结构 */
  xmlNodePtr cur;  /* xml节点 */

    /* 获取树形结构 */
  doc = xmlParseFile(xml); /* 获取XML文档的指针 */
	if(doc == NULL){
		fprintf(stderr, "%s:%d: failed to upload xml file\n",__FUNCTION__, __LINE__);
    do_exit(ERROR);
  }
    
  /* 获取根节点 */
  cur = xmlDocGetRootElement(doc);
  if (cur == NULL) {
    fprintf(stderr, "Root is empty.\n");
    xmlFreeDoc(doc);
    do_exit(ERROR);
  }

  /* 判断根节点是否是"configurations" */
  if ( xmlStrcmp(cur->name, (const xmlChar *)"configurations") ) {
    fprintf(stderr, "document of the wrong type, root node != configurations.\n");
    xmlFreeDoc(doc);
    do_exit(ERROR);
  }
    
  /* 遍历处理根节点的每一个子节点 */
  cur = cur->xmlChildrenNode;

  xmlChar *key=NULL; 
    
  while (cur != NULL) {
    f=0;
    /* 是否允许indels */
    if(-1==no_indels && !strcmp(cur->name, (const xmlChar *)"no_indels")){
      key=xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
      no_indels = atoi(key);
      f=1;
    }  
    /* 错误率 */
    else if( -1==error_rate && !strcmp(cur->name, (const xmlChar *)"error_rate")){
      key=xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
      error_rate = atof(key);
      f=1;
    }
    /* 重叠区域overlap */
    else if( -1==overlap && !strcmp(cur->name, (const xmlChar *)"overlap")){
      key=xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
      overlap = atoi(key);
      f=1;
    }
    /* 质量值阈值 */
    else if( -1==quality_threshold && !strcmp(cur->name, (const xmlChar *)"quality_threshold")){
      key=xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
      quality_threshold = atoi(key);
      f=1;
    }
    /* 符合上面质量阈值最低碱基数占比 */
    else if( -1==quality_ratio && !strcmp(cur->name, (const xmlChar *)"quality_ratio")){
      key=xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
      quality_ratio = atof(key);
      f=1;
    }
    /* 去除READ两端低于此值的碱基 */
    else if( -1==quality_low && !strcmp(cur->name, (const xmlChar *)"quality_low")){
      key=xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
      quality_low = atoi(key);
      f=1;  
    }
    /* 是否去除两端N */
    else if( -1==trim_n && !strcmp(cur->name, (const xmlChar *)"trim_n")){
      key=xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
      trim_n = atoi(key);
      f=1;
    }
    /* N碱基占比最低值 */
    else if( -1==ratio_n && !strcmp(cur->name, (const xmlChar *)"ratio_n")){
      key=xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
      ratio_n = atof(key);
      f=1;
    }
    /* 质量值基数(类型:整数) */
    else if( -1==quality_base && !strcmp(cur->name, (const xmlChar *)"quality_base")){
      key=xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
      quality_base = atoi(key);
      f=1;
    }
    /* 输出文件切割快大小 */
    else if(-1==split && !strcmp(cur->name, (const xmlChar *)"split")){
      key=xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
      split = atoi(key);
      f=1;
    }
    /* 线程相关信息 */
    else if( -1==threads && !strcmp(cur->name, (const xmlChar *)"threads")){
      key=xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
      threads = atoi(key);
      f=1;
    }
    /* 工作块reads数 */
    else if( -1==max_line_length && !strcmp(cur->name, (const xmlChar *)"max_line_length")){
      key=xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
      max_line_length = atoi(key);
      f=1;
    }
    /* 行最大长度 */
    else if( -1==reads && !strcmp(cur->name, (const xmlChar *)"reads")){
      key=xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
      reads = atoi(key);
      f=1;
    }
    else{}
    if(1==f)xmlFree(key);
    cur = cur->next;
  }/*for while*/
  //xmlDictCleanup(); 
  
  
  //xmlMemoryDump();
  xmlCleanupCharEncodingHandlers();
  xmlCleanupParser(); 
  xmlFreeDoc(doc);
}/*load_default_parms()*/

  /*输入和默认参数检验*/
void checkup_params()
{
    /*错配率校验*/
  verification(error_rate<0 || error_rate>1," Error: error rate must between 0~1.\n");
    /*overlap校验*/
  verification(overlap<3," Error: overlap must >= 3.\n");
    /*质量值阈值和比例校验*/
  verification(quality_threshold<0," Error: quality threshold must >= 0.\n");
  verification(quality_ratio <0 || quality_ratio>1," Error: quality ratio must between 0-1.\n");
    /*N占比校验*/
  verification(ratio_n<0 || ratio_n>1," Error: ratio_n must between 0-1.\n");
    /*长度校验*/
  verification(min_length<0," Error: maximum length must >= 0.\n");
  verification(max_length<0," Error: maximum length must >= 0.\n");
  verification(min_length>0 && max_length>0 && max_length<min_length," Error: maximum length must >= minimum length.\n");
    /*GC含量校验*/
  verification(gc_content <0 || gc_content>1," Error: gc content must between 0-1.\n");
    /*质量基数校验*/
  verification(quality_base !=33 && quality_base !=64," Error: quality base must is 33 or 64.\n");
    /*线程数校验*/
  verification(threads<1," Error: threads must >=1.\n");
    /*线程工作块校验*/
  verification(reads<=0," Error: work block too small.\n");
    /*输出文件数量校验*/
  verification(split<1," Error: output split block must >=1.\n");
  verification(split>(sizeof(uli)*8/2)," Error: output split block too much.\n");
    /*接头校验*/
  verification(((adapts[0]!=NULL || fronts[0]!=NULL )&& anywheres[0]!=NULL)," can't set -a/-A or -g/-G with -b/-W at same time.\n");
    /*行最大长度校验*/
  verification(max_line_length<=0,"Error: max_line_length must >=3.\n");
}/*for checkup_input*/

  /*参数预览*/
void viewparams ()
{
  int i=0,j=0;
  printf("\033[36m Configurations from default xml or user input:\n");
  /*输入文件*/
  printf(" |-- sequencing: %s\n",paired?"PE":"SE");
  printf(" |-- input file\n");
  printf(" |         |-- %s\n",ifiles[0]);
  if(ifiles[1])
  printf(" |         |-- %s\n",ifiles[1]);
  /*线程*/
  printf(" |-- thread: %d\n",threads);
  /*工作快*/
  printf(" |-- reads: %d\n",reads);
  /*质量值基数*/
  printf(" |-- quality base: %d\n",quality_base);
  /* 接头 */
  for(i=0;i<=paired;i++){
    /* 3'端接头 */
    if(adapts[i]!=NULL){
      printf(" |-- 3'end adapter of read%d:\n",i+1);
      for(j=0;j<adaptn[i];j++){
        printf(" |         |-- %d:%s\n",j+1,adapts[i][j]);
      }
    }
    /* 5'端接头 */
    if(fronts[i]!=NULL){
      printf(" |-- 5'end adapter of read%d:\n",i+1);
      for(j=0;j<frontn[i];j++){
        printf(" |         |-- %d:%s\n",j+1,fronts[i][j]);
      }
    }
    /* 任意端接头 */
    if(anywheres[i]!=NULL){
      printf(" |-- 5' or 3' end adapter of read%d:\n",i+1);
      for(j=0;j<anywheren[i];j++){
        printf(" |         |-- %d:%s\n",j+1,anywheres[i][j]);
      }
    }
  }/*for for i*/
  
  /* indels  */
  printf(" |-- indels: %s\n",no_indels==0?"yes":"no");
  printf(" |-- overlap: %d\n",overlap);
  if(quality_threshold>0 )
  printf(" |-- quality_threshold: %g\n",quality_threshold);
  
  if(quality_ratio>0 )
  printf(" |-- quality_ratio: %g\n",quality_ratio);
  
  if(quality_low>0)
  printf(" |-- quality_low: %g\n",quality_low);

  if(error_rate>0 )
  printf(" |-- error rate: %g\n",error_rate);
  
  if(min_length>=0 )
  printf(" |-- minimum length: %d\n",min_length);
  
  if(max_length!=0 )
  printf(" |-- maximum length: %d\n",max_length);
  
  /*GC含量*/
  if(gc_content>=0 )
  printf(" |-- gc content: %g\n",gc_content);
  
  /*去除两端N*/
  if(trim_n>=0 )
  printf(" |-- trim_n: %s\n",trim_n?"yes":"no");

  /*N占比*/
  if(ratio_n>=0 )
  printf(" |-- ratio_n: %g\n",ratio_n);
  
  /*xml*/
  printf(" |-- xml: %s\n",xml);
  /*html*/
  if(html)printf(" |-- html: %s\n",html);
  /*text*/
  if(text)printf(" |-- text: %s\n",text);
  /*json*/
  if(json)printf(" |-- json: %s\n",json);
  /*title*/
  if(title)printf(" |-- json: %s\n",title);
  /*split*/
  printf(" |-- split: %d\n",split);
  printf(" |----------------------*\n\n\033[0m");
}/*for viewparams*/

/*测试输出/输出文件*/
void test_iofiles()
{
  int i=0,j=0,k=0;
  char *p=NULL;
  /* 输出文件校验,合格则根据扩展名创建文件描述符 */
  for(i=0;i<=paired;i++){
    if(ifiles[i]==NULL)continue;
    /*无输出路劲使用输入参数配置*/
    if(outname[i]==NULL && ifiles[i]!=NULL ){
      if ((p = strrchr(ifiles[i], PATH_SEP)) != NULL){
      outname[i]=(char *)xmalloc(sizeof(char)*(strlen(p)+1));
      outpath[i]=(char *)xmalloc(sizeof(char)*(p-ifiles[i]+1));
      strcpy(outname[i],p+1);
      strncpy(outpath[i],ifiles[i],p-ifiles[i]+1);
      outpath[i][p-ifiles[i]+1]='\0';
      }else{
      outname[i]=(char *)xmalloc(sizeof(char)*MAX_PATH);
      outpath[i]=(char *)xmalloc(sizeof(char)*MAX_PATH);
      strcpy(outname[i],ifiles[i]);
      strcpy(outpath[i],"./");
      }
    }
    /*printf("路径:%s, 名称:%s\n",outpath[i], outname[i]);//测试代码 */
    /*根据输入文件后缀创建对应文件句柄*/
    if(strcmp((ifiles[i]+strlen(ifiles[i])-2),"gz")==0){
        /*输入文件为gz*/
      setbit(iftype,i);
      igzfp[i] = gzopen(ifiles[i],"rb");
      if(igzfp[i]==NULL) {
        fprintf(stderr,"error: failed to open file \"%s\"\n",ifiles[i]);
        abort_file();
      }
      k=gzbuffer(igzfp[i],1048576);
      if(k==-1) {
        fprintf(stderr,"error: failed to add buffer \"%s\"\n",ifiles[i]);
        abort_file();
      }
    }else{
        /*输入文件为fastq*/
      ifp[i] = fopen(ifiles[i],"r");
      if(ifp[i]==NULL) {
        fprintf(stderr,"error: failed to open file \"%s\"\n",ifiles[i]);
        abort_file();
      }
    }
    ofname[i] = (char **)xmalloc(sizeof(char *)*(split + 1));
    /*根据输出文件后缀类型创建输出句柄*/
    if(strcmp((outname[i]+strlen(outname[i])-2),"gz")==0){
      setbit(oftype,i);
      /*分配内存*/
      ogzfp[i] = (gzFile *)xmalloc(sizeof(gzFile)*(split + 1));
      for(j=0;j<split;j++){
        char *t=(char *)xmalloc(sizeof(char)*(strlen(ifiles[i])+50));
        memzero(t,strlen(ifiles[i])+50);
        sprintf(t,"%s%04d.clean.%s",outpath[i],j+1,outname[i]);
        *(ofname[i]+j)=t;
        /* printf("文件名:%s\n",*(ofname[i]+j)); */
        ogzfp[i][j] = gzopen(t,"wb");//(gzFile *)
        if(ogzfp[i][j]==NULL) {
          fprintf(stderr,"error: failed to create file \"%s\"\n",t);
          do_exit(ERROR);
        }
        // k=gzbuffer(ogzfp[i][j],1048576);
        // if(k==-1) {
        //   fprintf(stderr,"error: failed to add buffer \"%s\"\n",t);
        //   do_exit(ERROR);
        // }
      }
    }else{
      /*分配内存*/
      ofp[i] = (FILE **)xmalloc(sizeof(FILE *)*(split + 1));
      for(j=0;j<split;j++){
        char *t=(char *)xmalloc(sizeof(char)*(strlen(ifiles[i])+50));
        memzero(t,strlen(ifiles[i])+50);
        sprintf(t,"%s%04d.clean.%s",outpath[i],j+1,outname[i]);
        *(ofname[i]+j)=t;
        ofp[i][j] = fopen(t,"w");
        if(ofp[i][j]==NULL) {
          fprintf(stderr,"error: failed to create file \"%s\"\n",t);
          do_exit(ERROR);
        }
      }
    }/*for 根据文件类型创建输出句柄*/
  }/*遍历输入*/
  /*html文件句柄检验*/
  if(html){
    HTML = fopen(html,"w");
    if(HTML==NULL) {
      fprintf(stderr,"error: failed to create file \"%s\"\n",html);
      do_exit(ERROR);
    }
  }
  /*json文件句柄检验*/
  if(json){
    JSON = fopen(json,"w");
    if(JSON==NULL) {
      fprintf(stderr,"error: failed to create file \"%s\"\n",json);
      do_exit(ERROR);
    }
  }
  /*text文件句柄检验*/
  if(text){
    TEXT = fopen(text,"w");
    if(TEXT==NULL) {
      fprintf(stderr,"error: failed to create file \"%s\"\n",text);
      do_exit(ERROR);
    }
  }
}/*for test_iofiles*/
