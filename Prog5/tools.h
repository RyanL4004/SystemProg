// file: tools.h ---------------------------------------------------------------
// -----------------------------------------------------------------------------
// header file for the tools library.                                         --
// Please enter your own name on line 10.									  --
// -----------------------------------------------------------------------------
#ifndef TOOLS
#define TOOLS

#define NAME    "Alice E. Fischer"
#define CLASS   "CS 647 / 547"

#include <stdio.h>      
#include <stdlib.h>     // for malloc() and calloc()*/     
#include <math.h>        
#include <string.h>     // for time_t, time() and ctime()*/   
#include <time.h>        
#include <ctype.h>      // for isspace() and isdigit()-- 
#include <limits.h>     // for INT_MAX--   
#include <float.h>      // for DOUBLE_MAX, etc.--
#include <stdarg.h>     // for functions with variable # of arguments--

#include <getopt.h>
#include <sys/types.h>
#include <sys/dir.h>	// for struct dirent
#include <sys/stat.h>	// for struct stat
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <stddef.h>
#include <fcntl.h>		// for setting status flags on file descriptors.

typedef char * cstring;
typedef FILE * cstream;
typedef DIR *  dirstream;

// --------------------------------------------------------------
// Macros to make more convenient use of standard library functions.          --
// --------------------------------------------------------------
#define strequal(a,b)   	!strcmp(a,b)
#define strnequal(a,b,n)  	!strncmp(a,b,n)

// --------------------------------------------------------------
// Routine screen and process management.
// --------------------------------------------------------------
void    clearscreen( void );             
void    fbanner( cstream sout );
void    banner( void );
void    say( cstring format, ... );
void 	delay( int );
void    hold( void );                                    
void    bye( void );                                    

// --------------------------------------------------------------
// Error handling. -------------------------------------------------------------
// --------------------------------------------------------------
int     fatal( cstring format, ... );
void    cleanline( cstream sin );
void 	clean_and_log( cstream sin, cstream sout );

// --------------------------------------------------------------
// Time and date.
// --------------------------------------------------------------
void    when( char date[], char hour[] );
cstring  today( char date[] );
cstring  oclock( char hour[] );

#endif
