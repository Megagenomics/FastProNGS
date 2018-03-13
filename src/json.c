#ifndef _QC_H_
  #include "qc.h"
#endif

#include "version.h"

void json_out()
{
int i=0,j=0,k=0,len=0,b=0,p=0,r=0,m=0;
uli f=0ul;
fprintf(JSON,"{");

char fmsga[100] = {0};
char fmsgb[100] = {0};
conv_units(summary[0][0].reads, fmsga);
fprintf(JSON,
"\"Total read pairs processed\":\"%lu\",",summary[0][0].reads);
fprintf(JSON,
"\"Read 1 with adapter\":\"%lu (%.2f%%)\",",summary[0][0].adaptn[0],((float)summary[0][0].adaptn[0]/summary[0][0].reads)*100);
if(paired)
fprintf(JSON,
"\"Read 2 with adapter\":\"%lu (%.2f%%)\",",summary[1][0].adaptn[0],((float)summary[1][0].adaptn[0]/summary[1][0].reads)*100);
conv_units(summary[1][0].reads, fmsga);
fprintf(JSON,
"\"Pairs written (passing filters)\":\"%lu (%.2f%%)\",",summary[0][1].reads,((float)summary[0][1].reads/summary[0][0].reads)*100);

fprintf(JSON,
"\"Total basepairs processed\":\"%lu bp\",",paired?summary[0][0].bases+summary[1][0].bases:summary[0][0].bases);
fprintf(JSON,
"\"Read 1 basepairs\":\"%lu bp\",",summary[0][0].bases);
if(paired)
fprintf(JSON,
"\"Read 2 basepairs\":\"%lu bp\",",summary[1][0].bases);
fprintf(JSON,
"\"Total written (filtered)\":\"%lu bp(%.2f%%)\",",paired?summary[0][1].bases+summary[1][1].bases:summary[0][1].bases,paired?((float)(summary[0][1].bases+summary[1][1].bases)/(summary[0][0].bases+summary[1][0].bases))*100:((float)summary[0][1].bases/summary[0][1].bases)*100 );
fprintf(JSON,
"\"Read 1 written\":\"%lu bp\",",summary[0][1].bases);
if(paired)
fprintf(JSON,
"\"Read 2 written\":\"%lu bp\",",summary[1][1].bases);
fprintf(JSON,"\"N of fq1\":{\"Raw data\":\"%lu(%.2f%%)\",\"Clean data\":\"%lu(%.2f%%)\"},",summary[0][0].N,((float)summary[0][0].N/summary[0][0].bases)*100,summary[0][1].N,((float)summary[0][1].N/summary[0][1].bases)*100);
if(paired)
fprintf(JSON,"\"N of fq2\":{\"Raw data\":\"%lu(%.2f%%)\",\"Clean data\":\"%lu(%.2f%%)\"},",summary[1][0].N,((float)summary[1][0].N/summary[1][0].bases)*100,summary[1][1].N,((float)summary[1][1].N/summary[1][1].bases)*100);

fprintf(JSON,"\"Q20 of fq1\":{\"Raw data\":\"%lu(%.2f%%)\",\"Clean data\":\"%lu(%.2f%%)\"},",summary[0][0].q20,((float)summary[0][0].q20/summary[0][0].bases)*100,summary[0][1].q20,((float)summary[0][1].q20/summary[0][1].bases)*100);
if(paired)
fprintf(JSON,"\"Q20 of fq2\":{\"Raw data\":\"%lu(%.2f%%)\",\"Clean data\":\"%lu(%.2f%%)\"},",summary[1][0].q20,((float)summary[1][0].q20/summary[1][0].bases)*100,summary[1][1].q20,((float)summary[1][1].q20/summary[1][1].bases)*100);
fprintf(JSON,"\"Q30 of fq1\":{\"Raw data\":\"%lu(%.2f%%)\",\"Clean data\":\"%lu(%.2f%%)\"},",summary[0][0].q30,((float)summary[0][0].q30/summary[0][0].bases)*100,summary[0][1].q30,((float)summary[0][1].q30/summary[0][1].bases)*100);
if(paired)
fprintf(JSON,"\"Q30 of fq2\":{\"Raw data\":\"%lu(%.2f%%)\",\"Clean data\":\"%lu(%.2f%%)\"},",summary[1][0].q30,((float)summary[1][0].q30/summary[1][0].bases)*100,summary[1][1].q30,((float)summary[1][1].q30/summary[1][1].bases)*100);
fprintf(JSON,"\"GC of fq1\":{\"Raw data\":\"%lu(%.2f%%)\",\"Clean data\":\"%lu(%.2f%%)\"}",summary[0][0].GC,((float)summary[0][0].GC/summary[0][0].bases)*100,summary[0][1].GC,((float)summary[0][1].GC/summary[0][1].bases)*100);
if(paired)
fprintf(JSON,",\"GC of fq2\":{\"Raw data\":\"%lu(%.2f%%)\",\"Clean data\":\"%lu(%.2f%%)\"}",summary[1][0].GC,((float)summary[1][0].GC/summary[1][0].bases)*100,summary[1][1].GC,((float)summary[1][1].GC/summary[1][1].bases)*100);
/*text()*/
fprintf(JSON,"}");
}/*for json_out*/