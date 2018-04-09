#ifndef _QC_H_
  #include "qc.h"
#endif

#include <libxml/parser.h>
#include <getopt.h>

static struct option const long_options[] = {
{"adapter", required_argument, 0, 'a'},     
{"front", required_argument, 0, 'g'},       
{"anywhere", required_argument, 0, 'b'},    
{"adapter2", required_argument, 0, 'A'},    
{"front2", required_argument, 0, 'G'},      
{"anywhere2", required_argument, 0, 'B'},   
{"no—indels", no_argument, 0, 'i'},         
{"error-rate",required_argument ,0 , 'e'},  
{"overlap", required_argument, 0, 'O'},     
{"quality-filter",required_argument ,0,'q'},
{"quality-low",required_argument ,NULL , 'Q'}, 
{"trim-n", no_argument, NULL, 'n'},            
{"ratio-n", required_argument, NULL, 'r'},     
{"min-length", required_argument, 0, 'm'},  
{"max-length", required_argument, 0, 'M'},  
{"gc-content", required_argument, 0, 'c'},  
{"quality-base", required_argument, 0, 'E'},
{"output1", required_argument, 0, 'o'},     
{"output2", required_argument, 0, 'p'},     
{"xml", required_argument, 0, 'x'},         
{"html", required_argument, 0, 'H'},        
{"text", required_argument, 0, 'T'},        
{"json", required_argument, 0, 'j'},        
{"title", required_argument, 0, 'R'},       
{"split", required_argument, 0, 's'},       
{"threads", required_argument, 0, 't'},     
{"reads", required_argument, 0, 'k'},       
{"view-params", no_argument, 0, 'w'}, 

{"help", no_argument, 0, 'h'},        
{"help", no_argument, 0, '?'},        
{"version", no_argument, 0, 'v'},     
{"version", no_argument, 0, 'V'},     
{0, 0, 0, 0} }; 

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

  fcount = argc - optind;
  verification(fcount<=0," Error: at least one input files!\n");
  verification(fcount>2," Error: too many input file!\n");
  
  ifiles[0]=argv[optind];
  if(fcount==2)ifiles[1]=argv[optind+1],paired=1;

  for(m=0;m<=paired;m++){
    if(adapts[m]!=NULL) sort_str(adapts[m],adaptf[m],adaptn[m]);
    if(fronts[m]!=NULL) sort_str(fronts[m],frontf[m],frontn[m]);
    if(anywheres[m]!=NULL)sort_str(anywheres[m],anywheref[m],anywheren[m]);
  }/*for for m*/

  GetCurrentExcutableFilePathName(exe_path, exe_name,sizeof(exe_path));
 
  if(xml==NULL){
    xml=(char *)xmalloc(sizeof(char)*MAX_PATH);
    strcpy(xml,exe_path);
    strcat(xml,"conf/default.xml");
  }/*for if xml*/
}/*parse_user_parms*/

void load_default_parms()
{
  int f=0;
  xmlDocPtr doc;   
  xmlNodePtr cur;  

  doc = xmlParseFile(xml); 
	if(doc == NULL){
		fprintf(stderr, "%s:%d: failed to upload xml file\n",__FUNCTION__, __LINE__);
    do_exit(ERROR);
  }
    
  cur = xmlDocGetRootElement(doc);
  if (cur == NULL) {
    fprintf(stderr, "Root is empty.\n");
    xmlFreeDoc(doc);
    do_exit(ERROR);
  }

  if ( xmlStrcmp(cur->name, (const xmlChar *)"configurations") ) {
    fprintf(stderr, "document of the wrong type, root node != configurations.\n");
    xmlFreeDoc(doc);
    do_exit(ERROR);
  }
    
  cur = cur->xmlChildrenNode;

  xmlChar *key=NULL; 
    
  while (cur != NULL) {
    f=0;
    if(-1==no_indels && !strcmp(cur->name, (const xmlChar *)"no_indels")){
      key=xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
      no_indels = atoi(key);
      f=1;
    }  

    else if( -1==error_rate && !strcmp(cur->name, (const xmlChar *)"error_rate")){
      key=xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
      error_rate = atof(key);
      f=1;
    }
    
    else if( -1==overlap && !strcmp(cur->name, (const xmlChar *)"overlap")){
      key=xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
      overlap = atoi(key);
      f=1;
    }
    
    else if( -1==quality_threshold && !strcmp(cur->name, (const xmlChar *)"quality_threshold")){
      key=xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
      quality_threshold = atoi(key);
      f=1;
    }
    
    else if( -1==quality_ratio && !strcmp(cur->name, (const xmlChar *)"quality_ratio")){
      key=xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
      quality_ratio = atof(key);
      f=1;
    }
    
    else if( -1==quality_low && !strcmp(cur->name, (const xmlChar *)"quality_low")){
      key=xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
      quality_low = atoi(key);
      f=1;  
    }
    
    else if( -1==trim_n && !strcmp(cur->name, (const xmlChar *)"trim_n")){
      key=xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
      trim_n = atoi(key);
      f=1;
    }
    
    else if( -1==ratio_n && !strcmp(cur->name, (const xmlChar *)"ratio_n")){
      key=xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
      ratio_n = atof(key);
      f=1;
    }
    
    else if( -1==quality_base && !strcmp(cur->name, (const xmlChar *)"quality_base")){
      key=xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
      quality_base = atoi(key);
      f=1;
    }
    
    else if(-1==split && !strcmp(cur->name, (const xmlChar *)"split")){
      key=xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
      split = atoi(key);
      f=1;
    }
    
    else if( -1==threads && !strcmp(cur->name, (const xmlChar *)"threads")){
      key=xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
      threads = atoi(key);
      f=1;
    }
    
    else if( -1==max_line_length && !strcmp(cur->name, (const xmlChar *)"max_line_length")){
      key=xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
      max_line_length = atoi(key);
      f=1;
    }
    
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

void checkup_params()
{
    
  verification(error_rate<0 || error_rate>1," Error: error rate must between 0~1.\n");
   
  verification(overlap<3," Error: overlap must >= 3.\n");
   
  verification(quality_threshold<0," Error: quality threshold must >= 0.\n");
  verification(quality_ratio <0 || quality_ratio>1," Error: quality ratio must between 0-1.\n");
    
  verification(ratio_n<0 || ratio_n>1," Error: ratio_n must between 0-1.\n");
   
  verification(min_length<0," Error: maximum length must >= 0.\n");
  verification(max_length<0," Error: maximum length must >= 0.\n");
  verification(min_length>0 && max_length>0 && max_length<min_length," Error: maximum length must >= minimum length.\n");
   
  verification(gc_content <0 || gc_content>1," Error: gc content must between 0-1.\n");
   
  verification(quality_base !=33 && quality_base !=64," Error: quality base must is 33 or 64.\n");
    
  verification(threads<1," Error: threads must >=1.\n");
   
  verification(reads<=0," Error: work block too small.\n");
    
  verification(split<1," Error: output split block must >=1.\n");
  verification(split>(sizeof(uli)*8/2)," Error: output split block too much.\n");
    
  verification(((adapts[0]!=NULL || fronts[0]!=NULL )&& anywheres[0]!=NULL)," can't set -a/-A or -g/-G with -b/-W at same time.\n");
    
  verification(max_line_length<=0,"Error: max_line_length must >=3.\n");
}/*for checkup_input*/


void viewparams ()
{
  int i=0,j=0;
  printf("\033[36m Configurations from default xml or user input:\n");
 
  printf(" |-- sequencing: %s\n",paired?"PE":"SE");
  printf(" |-- input file\n");
  printf(" |         |-- %s\n",ifiles[0]);
  if(ifiles[1])
  printf(" |         |-- %s\n",ifiles[1]);
 
  printf(" |-- thread: %d\n",threads);
  
  printf(" |-- reads: %d\n",reads);
  
  printf(" |-- quality base: %d\n",quality_base);
  
  for(i=0;i<=paired;i++){
    if(adapts[i]!=NULL){
      printf(" |-- 3'end adapter of read%d:\n",i+1);
      for(j=0;j<adaptn[i];j++){
        printf(" |         |-- %d:%s\n",j+1,adapts[i][j]);
      }
    }

    if(fronts[i]!=NULL){
      printf(" |-- 5'end adapter of read%d:\n",i+1);
      for(j=0;j<frontn[i];j++){
        printf(" |         |-- %d:%s\n",j+1,fronts[i][j]);
      }
    }

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
  
  if(gc_content>=0 )
  printf(" |-- gc content: %g\n",gc_content);
  
  if(trim_n>=0 )
  printf(" |-- trim_n: %s\n",trim_n?"yes":"no");

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

void test_iofiles()
{
  int i=0,j=0,k=0;
  char *p=NULL;
  for(i=0;i<=paired;i++){
    if(ifiles[i]==NULL)continue;
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
    if(strcmp((ifiles[i]+strlen(ifiles[i])-2),"gz")==0){
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
      ifp[i] = fopen(ifiles[i],"r");
      if(ifp[i]==NULL) {
        fprintf(stderr,"error: failed to open file \"%s\"\n",ifiles[i]);
        abort_file();
      }
    }
    ofname[i] = (char **)xmalloc(sizeof(char *)*(split + 1));
    if(strcmp((outname[i]+strlen(outname[i])-2),"gz")==0){
      setbit(oftype,i);
      
      ogzfp[i] = (gzFile *)xmalloc(sizeof(gzFile)*(split + 1));
      for(j=0;j<split;j++){
        char *t=(char *)xmalloc(sizeof(char)*(strlen(ifiles[i])+50));
        memzero(t,strlen(ifiles[i])+50);
        sprintf(t,"%s%04d.clean.%s",outpath[i],j+1,outname[i]);
        *(ofname[i]+j)=t;
       
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
    }
  }

  if(html){
    HTML = fopen(html,"w");
    if(HTML==NULL) {
      fprintf(stderr,"error: failed to create file \"%s\"\n",html);
      do_exit(ERROR);
    }
  }

  if(json){
    JSON = fopen(json,"w");
    if(JSON==NULL) {
      fprintf(stderr,"error: failed to create file \"%s\"\n",json);
      do_exit(ERROR);
    }
  }

  if(text){
    TEXT = fopen(text,"w");
    if(TEXT==NULL) {
      fprintf(stderr,"error: failed to create file \"%s\"\n",text);
      do_exit(ERROR);
    }
  }
}/*for test_iofiles*/
