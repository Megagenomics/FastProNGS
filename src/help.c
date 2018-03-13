#ifndef _QC_H_
  #include "qc.h"
#endif

#include "version.h"

  /* 帮助文档 */
void help(char *PROGRAM_NAME){
static char  *help_msg[] = {
 " -a --adapter     The 3' end adapter (if paired data, represent first read). Supporting anchoring '$'.",
 " -g --front       The 5' end adapter (if paired data, represent first read). Supporting anchoring '^'.",
 " -b --anywhere    The 3' or 5' end adapter (if paired data, represent first read).",
 " -A --adapter2    The 3' end adapter (if paired data, represent second read). Supporting anchoring '$'.",
 " -G --front2      The 5' end adapter (if paired data, represent second read). Supporting anchoring '^'.",
 " -B --anywhere2   The 3' or 5' end adapter (if paired data, represent second read).",
 " -i --no-indels   Allow only mismatches in alignments, (Default: allow both mismatches and indels).",
 " -e --error-rate  Maximum allowed error rate (no. of errors divided by the length of the matching region. Default: see xml).",
 " -O --overlap     Require MINLENGTH overlap between read and adapter for an adapter to be found, (Default: see xml).",
 " -q --quality-filter    Discard reads when it less than specific threshold or ratio. (Format:threshold,ratio).",
 " -Q --quality-low       Cut base less than specific value in anywhere end.",
 " -n --trim-n            Trim N's on ends of reads (Default: see xml).",
 " -r --ratio-n           Discard reads greater than ratio-n (Default: see xml).",
 " -m --min-length        Discard reads shorter than LENGTH (Default: see xml).",
 " -M --max-length        Discard reads longer than LENGTH (Default: see xml).",
 " -c --gc-content        Discard reads less than specific value (Default: see xml).",
 " -E --quality-base      quality base (Default: see xml).",
 " -o --output1     Write trimmed reads1 to FILE.",
 " -p --output2     Write trimmed reads2 to FILE.",
 " -s --split       split file count.",
 " -x --xml         Default configuration file path",
 " -H --html        html report file.",
 " -T --text        text report file.",
 " -j --json        json report file.",
 " -R --title       title of report",
 " -t --threads     threads of program.",
 " -k --reads       the num of request read",
 " -h --help        print help message.",
 " -v --version     print version message.",
 0};
char **p = help_msg;
usage(PROGRAM_NAME);
while (*p) fprintf(stderr, "\033[36m%s\n\033[0m", *p++);
fprintf(stderr,"\033[34m %s %s (%s)\n\033[0m",
    PROGRAM_NAME, VERSION, REVDATE);
}/* for help */
