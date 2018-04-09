#ifndef _QC_H_
#define _QC_H_

#ifndef STDIO_H
#  define STDIO_H
#  include <stdio.h>
#endif

#ifndef STDLIB_H
#  define STDLIB_H
#  include <stdlib.h>
#endif

#ifndef STRING_H
#  define STRING_H
#  include <string.h>
#endif

#  include <zlib.h>
#  include <sys/types.h>
#  include <sys/stat.h>
#  include <fcntl.h>

typedef unsigned long int uli;

# define MAX_PATH 2048    

# define OK 0
# define ERROR 1

#  define START_WITHIN_SEQ1 1
#  define START_WITHIN_SEQ2 2
#  define STOP_WITHIN_SEQ1 4
#  define STOP_WITHIN_SEQ2 8
#  define SEMIGLOBAL 15

#  define BACK (START_WITHIN_SEQ2 | STOP_WITHIN_SEQ2 | STOP_WITHIN_SEQ1)
#  define FRONT (START_WITHIN_SEQ2 | STOP_WITHIN_SEQ2 | START_WITHIN_SEQ1)
#  define PREFIX (STOP_WITHIN_SEQ2)
#  define SUFFIX (START_WITHIN_SEQ2)
#  define ANYWHERE (SEMIGLOBAL)

# define memzero(s,n)  memset((void *)(s),0,(n))
# define xcalloc calloc((n)), sizeof(s))
# define FREE(array) {if (array != NULL){ free(array);array=NULL;}} 
# define  setbit(x,y)  ( (x)|= ( 1ul<<(y) ) )  
# define  clrbit(x,y)  ( (x)&=~( 1ul<<(y) ) )  

# define PATH_SEP '/'
# define MAX_LINE_LENGTH 200
# define MAX_ADAPT_NUM 100  
# define MAX_QUALITY 50 
 
typedef void (*sig_type) (int);

#define AN (1ul)   
#define UNQ (2ul)   
#define NR (4ul)    
#define GCC (8ul)   
#define MINL (16ul) 
#define MAXL (32ul) 
#define TF (65ul)   

extern int paired;           
extern char **adapts[2];     
extern int adaptn[2];        
extern int *adaptf[2];       
extern char **fronts[2];     
extern int frontn[2];        
extern int *frontf[2];       
extern char **anywheres[2];  
extern int anywheren[2];     
extern int *anywheref[2];    
extern int no_indels;        
extern float error_rate;     
extern int overlap;          
extern int quality_threshold;
extern float quality_ratio;  
extern int quality_low;      
extern int trim_n;           
extern int trim_b;           
extern float ratio_n;        
extern int min_length;       
extern int max_length;       
extern float gc_content;     
extern int quality_base;     
extern char *outname[2];     
extern char *outpath[2];     
extern char *xml;            
extern char *html;           
extern char *text;           
extern char *json;           
extern char *title;          
extern int split;            
extern int threads;          
extern int reads;            
extern int view_params;      
extern char *ifiles[2];      
extern uli iftype;           
extern uli oftype;           
extern char exe_path[MAX_PATH];
extern char exe_name[MAX_PATH];
extern int max_line_length;    

extern FILE *ifp[2];           
extern gzFile igzfp[2];        
extern FILE *HTML;             
extern FILE *TEXT;             
extern FILE *JSON;             
extern FILE **ofp[2];          
extern char **ofname[2];       
extern gzFile *ogzfp[2];       
extern int foreground;         

extern pthread_mutex_t lock_ifile;
extern pthread_mutex_t lock_ofile;
extern pthread_mutex_t lock_stat;

struct work_area{
  int flag;
  char ***data;
  int num[2];
};

extern struct work_area *wa;

extern pthread_t *tdi;
extern pthread_t *tdo;

extern int itid[2];            
extern int *wtid;              
extern int iflag;              

extern uli file_in_flag;
extern uli file_out_flag;
extern int file_out_loc_flag;

extern uli end3Type[2][MAX_ADAPT_NUM][MAX_LINE_LENGTH];        
extern uli end3TypeN[2][MAX_ADAPT_NUM][MAX_LINE_LENGTH][64];   
extern uli end5Type[2][MAX_ADAPT_NUM][MAX_LINE_LENGTH];        
extern uli end5TypeN[2][MAX_ADAPT_NUM][MAX_LINE_LENGTH][64];   
extern uli endAnyType[2][MAX_ADAPT_NUM][MAX_LINE_LENGTH];      
extern uli endAnyTypeN[2][MAX_ADAPT_NUM][MAX_LINE_LENGTH][64]; 

struct readinfo{
  uli qual;
  char nucl;
};
struct RSTAT{
  uli bases;    
  uli qN;       
  uli q20;      
  uli q30;      
  uli GC;       
  uli N;        
  struct readinfo ri[MAX_LINE_LENGTH];
};

extern uli tstat[2][2][MAX_LINE_LENGTH][MAX_QUALITY][5];
extern uli t_dstat[TF];
extern uli tlenstat[2][2][MAX_LINE_LENGTH];

struct SUMMARY{
  uli reads;
  uli bases;
  uli q20;
  uli q30;
  uli A;
  uli C;
  uli G;
  uli T;
  uli N;
  uli GC;
  uli maxlen;
  uli adaptn[2];
};
extern struct SUMMARY summary[2][2];

  /*help.c*/
extern void help(char *PROGRAM_NAME);
  /*qc.c*/
extern void do_exit(int exitcode);
extern void run();
void workbench(void *itid);
extern void abort_file();
  /*option.c*/
extern void parse_user_parms (int argc, char *argv[]); 
extern void load_default_parms();
extern void checkup_params();
extern void viewparams();
extern void test_iofiles();
  /* util.c */
extern void *xmalloc (unsigned int size); 
extern void * xrealloc (void * ptr,unsigned size);
extern void verification (int c, char *msg);
extern void sort_str(char **str,int *flag,int m);
extern char *deletLF(char *ptr); 
extern size_t GetCurrentExcutableFilePathName( char* processdir,char* processname, size_t len);
extern void conv_units(uli m,char *msg);
  /*io.c*/
extern int get_reads(char ***read_buf);
extern void predatai();
extern void waitpreouti();
// extern void predatao();
// extern void waitpreouto();
extern void fill_reads(void *id);
extern int put_str(int *outlen,char **outbuf);
  /*statistics.c*/
extern void get_stat(
  char *r,
  char *a,
  int w,
  int t,
  struct RSTAT rstat[2][2]);
  
extern void merge_stat_s2t(
  int f,
  uli sstat[2][2][MAX_LINE_LENGTH][MAX_QUALITY][5],
 
  struct RSTAT rstat[2][2]);
extern void merge_stat_t2a(
  
  uli tstat[2][2][MAX_LINE_LENGTH][MAX_QUALITY][5],
  
  uli sstat[2][2][MAX_LINE_LENGTH][MAX_QUALITY][5]);
extern void get_summary();
  /*align.c*/
extern int locate(char *reference,char *query,float max_error_rate,int flags,int _min_overlap,int *res);
extern void compare_prefixes(char *reference,char *query,int *res);
extern void compare_suffixes(char *reference,char *query,int *res);
  /*adapter.c*/
extern int Adapter(char *adapter,char *query,int where,float max_error_rate,int min_overlap,int noindels,int res[6]);
  /*filter.c*/
int filter(int f, struct RSTAT rstat[2][2],uli *d);
  /*html.c*/
extern void html_head();
extern void html_summary();
extern void html_rstat();
extern void html_adapter();
extern void html_qualpic();
extern void html_basepic();
extern void html_length();
extern void html_cmd(char **argv);
extern void html_tail();
extern void psubstr(FILE *F,char *c,int s,int t,int l);
extern void psubstr2(FILE *F,char *c,int s,int t,int l);
  /*json.c*/
extern void json_out();
  /*text.c*/
extern void text_report();
#endif /* FOR _QC_H_ */