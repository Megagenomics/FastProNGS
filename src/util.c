#ifndef _QC_H_
  #include "qc.h"
#endif

/* 内存分配器 */
void * xmalloc (unsigned size)
{
void * cp = (void *)malloc (size);

if (cp == NULL) {
  error("out of memory");
  do_exit(ERROR);
}
return cp;
}

/* 扩展内存器 */
void * xrealloc (void * ptr,unsigned size)
{
void * cp = (void *)realloc (ptr,size);

if (cp == NULL) {
  error("Failed to extend memory");
  do_exit(ERROR);
}
return cp;
}

/*简单校验,打印错误消息*/
void verification (int c, char *msg)
{
if(c){
  fprintf(stderr,"\033[31m");
  fprintf(stderr,msg);
  fprintf(stderr,"\033[0m");
  do_exit(ERROR);
}
}/*for verification*/

/*从buf中取出一行*/
char *xgetline(char *s)
{
char *p,*b,*r;
int l;
r=s;
p=strchr(s,'\n');
if(p!=NULL){
  b=p;
  while(*p=='\n' && p<=s)*p++='\0';
  while(*b=='\r' || *b=='\n')*p--='\0';
  printf("- - %s - -\n",r);
  return r;
}
l=strlen(s);
while(*s=='\n' || *s=='\0')*s++='\0';
return NULL;
}/*for xgetline*/

  /*按照字符串长度排序函数,携带标识,因只是对参数排序,所以使用简单的冒泡排序*/
void sort_str(char **str,int *flag,int m)
{
int i=0,j=0,f=0;
char *p=NULL;
for(i=0; i<m-1; ++i)
{
  for(j=i+1; j<m ; ++j)
  {
    if(strlen(str[i])<strlen(str[j]))
    {
      p=str[j],f=flag[j];
      str[j]=str[i],flag[j]=flag[i];
      str[i]=p,flag[i]=f;
    }
  }/*for for j*/
}/*for for i*/
}/*for sort_str*/

/*删除末尾换行符*/
char *deletLF(char *ptr)
{
  int L=strlen(ptr);
  char *p=ptr;
  p+=L-1;
  while(*p=='\r' || *p=='\n')*p--='\0';
  return ptr;
}/*for deletLF*/

/*获取当前可执行程序路径及名字*/
size_t GetCurrentExcutableFilePathName( char* processdir,char* processname, size_t len)  
{  
  char* path_end;  
  if(readlink("/proc/self/exe", processdir,len) <=0)  
          return -1;  
  path_end = strrchr(processdir,  '/');  
  if(path_end == NULL)  
          return -1;  
  ++path_end;  
  strcpy(processname, path_end);  
  *path_end = '\0';  
  return (size_t)(path_end - processdir);  
}  


/*单位转换函数*/
void conv_units(uli m,char *msg)
{
  float t=0;
  if(m<1024ul){
    sprintf(msg,"%luB",m);
  }else if(m>=1024ul && m<1048576ul){
    t=(float)m/1024ul;
    sprintf(msg,"%.3gK",t);
  }else if(m>=1048576ul && m<1073741824ul){
    t=(float)m/1048576ul;
    sprintf(msg,"%.3gM",t);
  }else if(m>=1073741824ul){
    t=(float)m/1073741824ul;
    sprintf(msg,"%.3gG",t);
  }
}/*for conv_units*/