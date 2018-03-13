#ifndef _QC_H_
#include "qc.h"
#endif

#include "version.h"
void text_report()
{
  char fmsga[100] = {0};
  char fmsgb[100] = {0};
  conv_units(summary[0][0].reads, fmsga);
  fprintf(TEXT,
  "=== Summary ===\n"
  "\n"
  "Total read pairs processed:\t%lu\n",summary[0][0].reads);
  fprintf(TEXT,
  "  Read 1 with adapter:\t%lu (%.2f%%)\n",summary[0][0].adaptn[0],((float)summary[0][0].adaptn[0]/summary[0][0].reads)*100);
  if(paired)
  fprintf(TEXT,
  "  Read 2 with adapter:\t%lu (%.2f%%)\n",summary[1][0].adaptn[0],((float)summary[1][0].adaptn[0]/summary[1][0].reads)*100);
  // conv_units(summary[1][0].reads, fmsga);
  fprintf(TEXT,
  "Pairs written (passing filters):\t%lu (%.2f%%)\n\n",summary[0][1].reads,((float)summary[0][1].reads/summary[0][0].reads)*100);
  
  fprintf(TEXT,
  "Total basepairs processed:\t%lu bp\n",paired?summary[0][0].bases+summary[1][0].bases:summary[0][0].bases);
  fprintf(TEXT,
  "  Read 1:\t%lu bp\n",summary[0][0].bases);
  if(paired)
  fprintf(TEXT,
  "  Read 2:\t%lu bp\n",summary[1][0].bases);
  fprintf(TEXT,
  "Total written (filtered):\t%lu bp(%.2f%%)\n",paired?summary[0][1].bases+summary[1][1].bases:summary[0][1].bases,paired?((float)(summary[0][1].bases+summary[1][1].bases)/(summary[0][0].bases+summary[1][0].bases))*100:((float)summary[0][1].bases/summary[0][1].bases)*100 );
  fprintf(TEXT,
  "  Read 1:\t%lu bp\n",summary[0][1].bases);
  if(paired)
  fprintf(TEXT,
  "  Read 2:\t%lu bp\n\n",summary[1][1].bases);
  fprintf(TEXT, "=== === === === ===\n\n");

  fprintf(TEXT,"Type\tRaw data\tClean data\n");
  fprintf(TEXT,"N of fq1:\t%lu(%.2f%%)\t%lu(%.2f%%)\n",summary[0][0].N,((float)summary[0][0].N/summary[0][0].bases)*100,summary[0][1].N,((float)summary[0][1].N/summary[0][1].bases)*100);
  if(paired)
  fprintf(TEXT,"N of fq2:\t%lu(%.2f%%)\t%lu(%.2f%%)\n",summary[1][0].N,((float)summary[1][0].N/summary[1][0].bases)*100,summary[1][1].N,((float)summary[1][1].N/summary[1][1].bases)*100);
  
  fprintf(TEXT,"Q20 of fq1:\t%lu(%.2f%%)\t%lu(%.2f%%)\n",summary[0][0].q20,((float)summary[0][0].q20/summary[0][0].bases)*100,summary[0][1].q20,((float)summary[0][1].q20/summary[0][1].bases)*100);
  if(paired)
  fprintf(TEXT,"Q20 of fq2:\t%lu(%.2f%%)\t%lu(%.2f%%)\n",summary[1][0].q20,((float)summary[1][0].q20/summary[1][0].bases)*100,summary[1][1].q20,((float)summary[1][1].q20/summary[1][1].bases)*100);
  fprintf(TEXT,"Q30 of fq1:\t%lu(%.2f%%)\t%lu(%.2f%%)\n",summary[0][0].q30,((float)summary[0][0].q30/summary[0][0].bases)*100,summary[0][1].q30,((float)summary[0][1].q30/summary[0][1].bases)*100);
  if(paired)
  fprintf(TEXT,"Q30 of fq2:\t%lu(%.2f%%)\t%lu(%.2f%%)\n",summary[1][0].q30,((float)summary[1][0].q30/summary[1][0].bases)*100,summary[1][1].q30,((float)summary[1][1].q30/summary[1][1].bases)*100);
  fprintf(TEXT,"GC of fq1:\t%lu(%.2f%%)\t%lu(%.2f%%)\n",summary[0][0].GC,((float)summary[0][0].GC/summary[0][0].bases)*100,summary[0][1].GC,((float)summary[0][1].GC/summary[0][1].bases)*100);
  if(paired)
  fprintf(TEXT,"GC of fq2:\t%lu(%.2f%%)\t%lu(%.2f%%)\n",summary[1][0].GC,((float)summary[1][0].GC/summary[1][0].bases)*100,summary[1][1].GC,((float)summary[1][1].GC/summary[1][1].bases)*100);


}/*text()*/