#ifndef _QC_H_
  #include "qc.h"
#endif

    /* usage */
void usage(char *PROGRAM_NAME){
    fprintf(stderr, "\033[35m Usage: %s [-agbAGBieOqQlnrmMcEopsxHTjRtwh?v] [file ...]\n\033[0m",
	    PROGRAM_NAME);
}/* for usage */