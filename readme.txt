------------------------------------------------------------------
|                                                                |
|        *         * * * *    *    *   * * * *    *              |
|        *         *           *  *    *          *              |
|        *         * * * *      *      * * * *    *              |
|        *         *           *  *    *          *              |
|        * * * *   * * * *    *    *   * * * *    * * * *        |
|                        VERSION 1.7                             |
|          A Lexical Analyzer Written by Jerison Ling            |
|                                                                |
|              Email: lingjie981119@bupt.edu.cn                  |
|      GitHub: github.com/Jerison/Effective-Lexical-Analyzer     |
|                                                                |
------------------------------------------------------------------

<About the Project>:
This is a course project for <Fundamentals of Compiling>, taught by
Prof. Zhang in BUPT in 2020. The program runs well with C/C++ files.
See more details in <Usage>.

<Usage>:
lexical_analyzer.exe [OPTIONS] COMMAND [OPTIONS]

e.g. lexical_analyzer.exe -o temp.txt analyze lexical_analyzer.cpp 

<OPTIONS>:
-o            Redirect all output info to a specific file
-h            Get help information
--pure        Output only token information
--complete    Output complete information

<COMMANDS>
analyze       Specify a C/C++ file and start analysis
token-table   Show all supported token types

<TIPS>
For any messy code existing in output, it's probably caused by some
encoding problem (utf-8 v.s. GBK, you know). Executing "chcp 65001"
before you run the program should help.