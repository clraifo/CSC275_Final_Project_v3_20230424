/*
This is the implementation file for the clearScreen function, which is defined in the Utils.h header file. 
The purpose of this function is to clear the console screen to make the display of information easier to read.

The implementation of the clearScreen function uses preprocessor directives to determine which platform 
the program is running on. If it is running on a Windows platform, the function uses the Windows Console API 
to clear the console screen. Otherwise, it uses the tigetstr function from the term.h header file to send 
the appropriate terminal control sequence to clear the screen.
*/

#include "Utils.h"  // includes Utils.h
#include <iostream> // includes iostream

#if defined(_WIN32) || defined(_WIN64)  // if the operating system is Windows
#include <windows.h>                    // includes windows.h
#else                                   // if the operating system is not Windows
#include <unistd.h>                     // includes unistd.h
#include <term.h>                       // includes term.h
#endif                                  // defined(_WIN32) || defined(_WIN64)

void clearScreen() {                                    // creates function clearScreen
#if defined(_WIN32) || defined(_WIN64)                  // if the operating system is Windows
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);   // creates HANDLE variable hStdout
    COORD coordScreen = { 0, 0 };                       // creates COORD variable coordScreen
    DWORD cCharsWritten;                                // creates DWORD variable cCharsWritten
    CONSOLE_SCREEN_BUFFER_INFO csbi;                    // creates CONSOLE_SCREEN_BUFFER_INFO variable csbi
    DWORD dwConSize;                                    // creates DWORD variable dwConSize

    GetConsoleScreenBufferInfo(hStdout, &csbi); 	   // gets the console screen buffer info
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;		   // gets the console size
    FillConsoleOutputCharacter(hStdout, (TCHAR)' ', dwConSize, coordScreen, &cCharsWritten); // fills the console with spaces
    GetConsoleScreenBufferInfo(hStdout, &csbi); 	   // gets the console screen buffer info
    FillConsoleOutputAttribute(hStdout, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten); // fills the console with attributes
    SetConsoleCursorPosition(hStdout, coordScreen);	    // sets the console cursor position
#else 												    // if the operating system is not Windows
    if (!cur_term) { 								    // if the terminal is not defined
        int result; 								    // creates int variable result
        setupterm(NULL, STDOUT_FILENO, &result); 	    // sets up the terminal
        if (result <= 0) return; 					    // if the result is less than or equal to 0, return
    }
    putp(tigetstr("clear")); 						    // puts the terminal clear string
#endif 												    // defined(_WIN32) || defined(_WIN64)
}
