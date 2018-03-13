#ifndef _QC_H_
  #include "qc.h"
#endif
#include "version.h"
 /* version */
void version(char *PROGRAM_NAME){
    fprintf(stderr,"\033[34m %s %s (%s)\n\033[0m", 
        PROGRAM_NAME, VERSION, REVDATE);
}/* for version */
