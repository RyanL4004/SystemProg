//  File: tools.c --------------------------------------------------------------
//  The tools library.                                                   
//  Assorted utility routines for use in C programs.                      
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#include "tools.h"
// -----------------------------------------------------------------------------
//  Routine screen and process management. 									  --                     
// -----------------------------------------------------------------------------
void 
clearscreen( void ) {   fprintf( stderr, "\f" );  }                                                          
// -----------------------------------------------------------------------------
// Print termination message. 
void 
bye( void ) { fputs( "\nNormal termination.\n", stderr ); }                                                      

// -----------------------------------------------------------------------------  
// Print a neat header on the output.                                        
void    
banner( void ){ fbanner( stdout ); } 

// -----------------------------------------------------------------------------
void 
fbanner( cstream sout )
{   char date[16], time[9];

	setlinebuf( stdin );
	setlinebuf( stdout );
    when(date, time);
    clearscreen();
    fprintf(sout,"\n-------------------------------------------------------\n");
    fprintf(sout,"\t%s \n\t%s \n\t%s  %s\n", "Lang & Mather", "CSCI4547", "February 16th", "2:00PM" );
    fprintf(sout,"-------------------------------------------------------\n");
}

// -----------------------------------------------------------------------------
//  This is a handy function for messages of all sorts.  
//  It formats and prints to stderr.                   
//  It accepts a format followed by a variable number of data items to print.
void
say (cstring format, ...)
{   va_list vargs;                               // optional arguments

    va_start( vargs, format );
    vfprintf( stderr, format, vargs );
    fprintf( stderr, "\n" );
}

// -----------------------------------------------------------------------------
// Delay progress of program for some number of seconds using a "busy wait".   
void 
delay( int seconds )
{   time_t goal;

    goal = time( NULL ) + seconds;  
    do  { /* Nothing */ } while (time(NULL) < goal);    
}

// -----------------------------------------------------------------------------
// Print message and wait for the user to type a newline.                     
void 
hold( void )                                    
{   
    fputs( "\n\n\aPress '.' and 'Enter' to continue", stderr );
    while ( getchar() != '.' ) ; 				// tight loop     
}                                                      

// -----------------------------------------------------------------------------
//  Error handling and error recovery functions.
//-------------------------------------------------------------------------------

// ----------------------------------------------------------------------------- 
// This function is for error messages.  
//    It takes a format argument followed by any number of data arguments.
//    It formats and prints an error message, then exits properly.  
int
fatal (cstring format, ...)
{   va_list vargs;                               // optional arguments

    va_start(vargs, format);
    vfprintf(stderr, format, vargs);
    fprintf(stderr, "\a\a\n");
	hold();
    exit(1);
}

// -----------------------------------------------------------------------------
// Clean out the rest of the current line, discarding all characters up to and
//    including the newline character. Use after an input error to prepare for 
//    resumption of numeric input.                                              
void 
cleanline ( cstream sin )
{   char ch;                      // Character currently under input scanner.

    do { 
		ch = fgetc(sin);           			// Read next character
	} while ( !feof( sin ) && ch != '\n' );	// Quit at first newline
}                                                      

// -----------------------------------------------------------------------------
// Clean out the rest of the current line up to and including the newline 
//    character. Write the characters to an error log file.  Use after an 
//    input error to prepare for resumption of numeric input.                                              
void 	
clean_and_log( cstream sin, cstream sout )
{   char ch;                      // Character currently under input scanner.

    do { 
		ch = fgetc(sin);           			// Read next character
		fputc(ch, sout); 					// Echo to error stream 
	} while ( !feof( sin ) && ch != '\n' );	// Quit at first newline
}                                                      

// ----------------------------------------------------------------------------- 
//  Routines for handling the time and date.                                     
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Store the current date and time in the arguments.  
//      System's date format is: "Fri Jun  9 10:15:55 1995\n"  
//      After extraction, date is: "Fri Jun  9 1995"    hour is: "10:15:55" 
void 
when( char date[], char hour[] )
{   
    time_t now;          // Stores an integer encoding of the date and time. 
    cstring nowstring;    // Stores the date and time in a readable form.

    now = time(NULL);              // Get the date and time from the system.
    nowstring = ctime(&now);                   // Convert to string form.   
    strncpy( date, nowstring, 10);             // Extract day, month, date. 
    strncpy( &date[10], &nowstring[19], 5);    // Extract space and year.   
    date[15]  = '\0';                          // Add the string terminator.       
    strncpy( hour, &nowstring[11], 8);         // Copy hour:minutes:seconds.
    hour[8]  = '\0';                           // Add the string terminator.
}

// -----------------------------------------------------------------------------
// Store the current date in the argument and return a pointer to it. 
//      date format is: "Fri Jun  9 1995"                                    
cstring
today( char date[] )
{   
    time_t now;          // Stores an integer encoding of the date and time. 
    cstring nowstring;    // Stores the date and time in a readable form.

    now = time(NULL);              // Get the date and time from the system.
    nowstring = ctime(&now);                   // Convert to string form.   
    strncpy( date, nowstring, 10);             // Extract day, month, date. 
    strncpy( &date[10], &nowstring[19], 5);    // Extract space and year.   
    date[15]  = '\0';                          // Add the string terminator.       
    return( date );  
}

// -----------------------------------------------------------------------------
// Store the current time in hour and return a pointer to it.   
//      hour format is: "10:15:55"                                           
cstring
oclock( char hour[] )
{   
    time_t now;          // Stores an integer encoding of the date and time. 
    cstring nowstring;    // Stores the date and time in a readable form.

    now = time(NULL);              // Get the date and time from the system.
    nowstring = ctime(&now);              // Convert to string form.        
    strncpy( hour, &nowstring[11], 8);    // Extract hour, minutes, seconds.
    hour[8]  = '\0';                      // Add the string terminator.     
    return( hour );  
}
