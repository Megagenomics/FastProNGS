## FastProNGS

　This tool is designed to preprocess fastq files, it can fast cut adapter, QC, trimming, statistics
　from raw data by all-in-one or step by step. it is developed in c with multithreading supported to 
　be afford high performance.

### Install

　download binary (https://github.com/Megagenomics/FastProNGS/)

　make

　make install

### Dependencies

　libxml2

### Usage

　Usage: FastProNGS [-agbAGBieOqQlnrmMcEopsxHTjRtwh?v] [file ...]<br>
　-a --adapter     The 3' end adapter (if paired data, represent first read). Supporting anchoring '$'.<br>
　-g --front       The 5' end adapter (if paired data, represent first read). Supporting anchoring '^'.<br>
　-b --anywhere    The 3' or 5' end adapter (if paired data, represent first read).<br>
　-A --adapter2    The 3' end adapter (if paired data, represent second read). Supporting anchoring '$'.<br>
　-G --front2      The 5' end adapter (if paired data, represent second read). Supporting anchoring '^'.<br>
　-B --anywhere2   The 3' or 5' end adapter (if paired data, represent second read).<br>
　-i --no-indels   Allow only mismatches in alignments, (Default: allow both mismatches and indels).<br>
　-e --error-rate  Maximum allowed error rate (no. of errors divided by the length of the matching region. Default: see xml).<br>
　-O --overlap     Require MINLENGTH overlap between read and adapter for an adapter to be found, (Default: see xml).<br>
　-q --quality-filter    Discard reads when it less than specific threshold or ratio. (Format:threshold,ratio).<br>
　-Q --quality-low       Cut base less than specific value in anywhere end.<br>
　-n --trim-n            Trim N's on ends of reads (Default: see xml).<br>
　-r --ratio-n           Discard reads greater than ratio-n (Default: see xml).<br>
　-m --min-length        Discard reads shorter than LENGTH (Default: see xml).<br>
　-M --max-length        Discard reads longer than LENGTH (Default: see xml).<br>
　-c --gc-content        Discard reads less than specific value (Default: see xml).<br>
　-E --quality-base      quality base (Default: see xml).<br>
　-o --output1     Write trimmed reads1 to FILE.<br>
　-p --output2     Write trimmed reads2 to FILE.<br>
　-s --split       split file count.<br>
　-x --xml         Default configuration file path<br><br>
　-H --html        html report file.<br>
　-T --text        text report file.<br>
　-j --json        json report file.<br>
　-R --title       title of report<br>
　-t --threads     threads of program.<br>
　-k --reads       the num of request read<br>
　-h --help        print help message.<br>
　-v --version     print version message.<br>

### Author

　BT team, 1 March 2018, http://www.megagenomics.cn/
