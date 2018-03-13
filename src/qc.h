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

# define MAX_PATH 2048      /*最长路径*/

  /*程序退出状态*/
# define OK 0
# define ERROR 1

/* 比对类型 */
#  define START_WITHIN_SEQ1 1
#  define START_WITHIN_SEQ2 2
#  define STOP_WITHIN_SEQ1 4
#  define STOP_WITHIN_SEQ2 8
#  define SEMIGLOBAL 15

  /*接头标识*/
#  define BACK (START_WITHIN_SEQ2 | STOP_WITHIN_SEQ2 | STOP_WITHIN_SEQ1)
#  define FRONT (START_WITHIN_SEQ2 | STOP_WITHIN_SEQ2 | START_WITHIN_SEQ1)
#  define PREFIX (STOP_WITHIN_SEQ2)
#  define SUFFIX (START_WITHIN_SEQ2)
#  define ANYWHERE (SEMIGLOBAL)

# define memzero(s,n)  memset((void *)(s),0,(n))
# define xcalloc calloc((n)), sizeof(s))
# define FREE(array) {if (array != NULL){ free(array);array=NULL;}} 
# define  setbit(x,y)  ( (x)|= ( 1ul<<(y) ) )  /*将X的第Y位置1*/
# define  clrbit(x,y)  ( (x)&=~( 1ul<<(y) ) )  /*将X的第Y位清0*/

/*路径分隔符*/
# define PATH_SEP '/'
# define MAX_LINE_LENGTH 200 /*行最大长度*/
# define MAX_ADAPT_NUM 100  /*命令行接收的接头数量*/
# define MAX_QUALITY 50 /*质量值最大值*/
  /*信号函数指针*/
typedef void (*sig_type) (int);

  /*丢弃READ条件标识*/
#define AN (1ul)    /*带接头标识*/
#define UNQ (2ul)   /*不满足质量过滤条件标识*/
#define NR (4ul)    /*N占比*/
#define GCC (8ul)   /*GC含量*/
#define MINL (16ul) /*最小长度*/
#define MAXL (32ul) /*最大长度*/
#define TF (65ul)   /*总标记*/

  /*全局变量*/
extern int paired;           /*双端测序标识,默认单端*/
extern char **adapts[2];     /*3'端接头序列*/
extern int adaptn[2];        /*3'端接头数量*/
extern int *adaptf[2];       /*3'端接头标识(是否有锚位)*/
extern char **fronts[2];     /*5'端接头序列*/
extern int frontn[2];        /*5'端接头数量*/
extern int *frontf[2];       /*5'端接头标识*/
extern char **anywheres[2];  /*任意位置接头序列*/
extern int anywheren[2];     /*任意位置接头数量*/
extern int *anywheref[2];    /*任意位置接头标识*/
extern int no_indels;        /*是否允许indels,默认查看xml配置*/
extern float error_rate;     /*允许最大错配率,默认查看xml配置*/
extern int overlap;          /*接头与read最小重叠区域*/
extern int quality_threshold;/*质量值阈值*/
extern float quality_ratio;    /*符合上面质量阈值最低碱基数占比*/
extern int quality_low;        /*去除READ两端低于此值的碱基*/
extern int trim_n;             /*去除两端N碱基*/
extern int trim_b;             /*去除n个碱基*/
extern float ratio_n;          /*N碱基占比最低值*/
extern int min_length;         /*READ最小长度*/
extern int max_length;         /*READ最大长度*/
extern float gc_content;       /*GC含量最低值*/
extern int quality_base;       /*碱基质量基值 33或者64*/
extern char *outname[2];       /*输出文件名*/
extern char *outpath[2];       /*输出文件路径*/
extern char *xml;              /*默认xml配置文件路径*/
extern char *html;             /*默认不输出html,加此参数后输出到指定路径*/
extern char *text;             /*默认不输出text,加此参数后输出到指定路径*/
extern char *json;             /*默认不输出json,加此参数后输出到指定路径*/
extern char *title;            /*输出报告标题*/ 
extern int split;              /*输出文件切割*/
extern int threads;            /*线程数*/
extern int reads;              /*工作reads数*/
extern int view_params;        /*预览参数*/
extern char *ifiles[2];        /*保存输入文件全路径*/
extern uli iftype;             /*输入文件类型,0到...表示READ n,置位代表gz文件,否则是普通文件*/
extern uli oftype;             /*输入文件类型,0到...表示READ n,置位代表gz文件,否则是普通文件*/
extern char exe_path[MAX_PATH];/*可执行文件路径*/
extern char exe_name[MAX_PATH];/*可执行文件名*/
extern int max_line_length;    /*行最大长度*/

extern FILE *ifp[2];           /*输入 普通文件描述符*/
extern gzFile igzfp[2];        /*输入 压缩文件描述符*/
extern FILE *HTML;             /*HTML 输出文件句柄*/
extern FILE *TEXT;             /*TEXT 输出文件句柄*/
extern FILE *JSON;             /*JSON 输出文件句柄*/
extern FILE **ofp[2];          /*输出 普通文件描述符*/
extern char **ofname[2];       /*输出 文件名*/
extern gzFile *ogzfp[2];       /*输出 压缩文件描述符*/
extern int foreground;         /*程序 运行在前端*/

  /*全局锁*/
extern pthread_mutex_t lock_ifile;
extern pthread_mutex_t lock_ofile;
extern pthread_mutex_t lock_stat;

/*输入数据结构*/
struct work_area{
  int flag;
  char ***data;
  int num[2];
};

extern struct work_area *wa;

/*数据预备线程*/
extern pthread_t *tdi;
extern pthread_t *tdo;

extern int itid[2];            /*保存输入线程id*/
extern int *wtid;              /*保存工作线程id*/
extern int iflag;              /*总输入标识,等于threads时无数据可退出*/

  /*文件读取状态标识*/
extern uli file_in_flag;
extern uli file_out_flag;
extern int file_out_loc_flag;

/*记录接头标识和数量*/
extern uli end3Type[2][MAX_ADAPT_NUM][MAX_LINE_LENGTH];        /*READ1/2:3'接头种类*/
extern uli end3TypeN[2][MAX_ADAPT_NUM][MAX_LINE_LENGTH][64];   /*READ1/2:3'接头各种类数量*/
extern uli end5Type[2][MAX_ADAPT_NUM][MAX_LINE_LENGTH];        /*READ1/2:5'接头种类*/
extern uli end5TypeN[2][MAX_ADAPT_NUM][MAX_LINE_LENGTH][64];   /*READ1/2:5'接头各种类数量*/
extern uli endAnyType[2][MAX_ADAPT_NUM][MAX_LINE_LENGTH];      /*READ1/2:任意端接头种类*/
extern uli endAnyTypeN[2][MAX_ADAPT_NUM][MAX_LINE_LENGTH][64]; /*READ1/2:任意接头各种类数量*/

/*基本统计信息*/
struct readinfo{
  uli qual;
  char nucl;
};
struct RSTAT{
  uli bases;    /*碱基总数*/
  uli qN;       /*质量N的碱基总数*/
  uli q20;      /*q20*/
  uli q30;      /*q30*/
  uli GC;       /*GC碱基总数*/
  uli N;        /*N碱基总数*/
  struct readinfo ri[MAX_LINE_LENGTH];
};
/*[read][前后][最大行长][最大质量值][ACGTN]*/
extern uli tstat[2][2][MAX_LINE_LENGTH][MAX_QUALITY][5];
extern uli t_dstat[TF];
extern uli tlenstat[2][2][MAX_LINE_LENGTH];
/*汇总信息*/
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

/*全局函数*/
  /*help.c*/
extern void help(char *PROGRAM_NAME);
  /*qc.c*/
extern void do_exit(int exitcode);
extern void run();
void workbench(void *itid);
extern void abort_file();/*清理文件*/
  /*option.c*/
extern void parse_user_parms (int argc, char *argv[]); /* 解析用户输入参数 */
extern void load_default_parms();/*加载默认参数*/
extern void checkup_params();/*检验参数*/
extern void viewparams();/*预览参数*/
extern void test_iofiles();/*输入输出文件校验*/
  /* util.c */
extern void *xmalloc (unsigned int size); /* 简单封装内存分配函数 */
extern void * xrealloc (void * ptr,unsigned size);/* 扩展内存器 */
extern void verification (int c, char *msg);/*简单校验,打印错误消息*/
extern void sort_str(char **str,int *flag,int m);
extern char *deletLF(char *ptr); /*删除末尾换行符*/
extern size_t GetCurrentExcutableFilePathName( char* processdir,char* processname, size_t len);/*获取当前可执行程序路径及名字*/
extern void conv_units(uli m,char *msg);/*单位转换函数*/
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
  char *r,/*序列*/
  char *a,/*质量*/
  int w,/*read标识,0=READ1,1=READ2*/
  int t,/*处理前后标识,0=before,1=after*/
  struct RSTAT rstat[2][2]);
  
extern void merge_stat_s2t(
  /*汇总标识,0=全部,1=前,2=后*/
  int f,
  /*汇总结构*/
  uli sstat[2][2][MAX_LINE_LENGTH][MAX_QUALITY][5],
  /*被汇总结构*/
  struct RSTAT rstat[2][2]);
extern void merge_stat_t2a(
  /*汇总结构*/
  uli tstat[2][2][MAX_LINE_LENGTH][MAX_QUALITY][5],
  /*被汇总结构*/
  uli sstat[2][2][MAX_LINE_LENGTH][MAX_QUALITY][5]);
extern void get_summary();
  /*align.c*/
extern int locate(char *reference,char *query,float max_error_rate,int flags,int _min_overlap,int *res);
extern void compare_prefixes(char *reference,char *query,int *res);
extern void compare_suffixes(char *reference,char *query,int *res);
  /*adapter.c*/
extern int Adapter(char *adapter,char *query,int where,float max_error_rate,int min_overlap,int noindels,int res[6]);
  /*filter.c*/
int filter(int f,/*处理前后标识*/ struct RSTAT rstat[2][2],uli *d);
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