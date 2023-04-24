/*
This source file gets information about the system's memory usage and provides a way to write it to a file.
 
This code implements two classes: MemoryInfo and MemoryInfoFileWriter.

The MemoryInfo class is used to store and display information about the system's memory usage. The class 
has a constructor that initializes the total, used, and available memory to 0. The updateData function 
updates the memory information by querying the system for the total, used, and available memory. The 
displayData function outputs the memory information to the console.

The MemoryInfoFileWriter class is a derived class of the MemoryInfo class, which writes the memory 
information to a file in addition to displaying it on the console. The constructor takes a filename argument 
to specify the name of the file to write to. The updateData function first calls the base class updateData 
function to update the memory information and then writes it to the specified file. The displayData function 
outputs a message indicating that the memory information has been saved to the specified file.

*/


#include "MemoryInfo.h" // allows access to the MemoryInfo class
#include "Utils.h"      // allows access to the clearScreen function
#include <iostream>     // allows access to the cout and cin functions
#include <iomanip>      // allows access to the setw function
#include <fstream>      // allows access to the ofstream function
#include <chrono>       // allows access to the chrono functions
#include <thread>       // allows access to the thread functions
#include <limits>       // allows access to the numeric_limits function
#include <cstdlib>      // allows access to the system function

using namespace std; // allows access to the std namespace

#if defined(_WIN32) || defined(_WIN64)  // if the operating system is Windows
#include <windows.h>                    // allows access to the Windows API
#else                                   // if the operating system is not Windows
#include <sys/sysinfo.h>                // allows access to the sysinfo function
#endif                                  // defined(_WIN32) || defined(_WIN64)

MemoryInfo::MemoryInfo() { // constructor
    updatePlatformSpecificMemoryInfo(); // calls the updatePlatformSpecificMemoryInfo function
}

void MemoryInfo::updateData() {                         // function updateData
    updatePlatformSpecificMemoryInfo();                 // calls the updatePlatformSpecificMemoryInfo function
    usedMemoryHistory.push_back(usedMemory);            // adds the usedMemory variable to the usedMemoryHistory deque
    availableMemoryHistory.push_back(availableMemory);  // adds the availableMemory variable to the 
                                                        //availableMemoryHistory deque

    if (usedMemoryHistory.size() > 4) { 			    // if the size of the usedMemoryHistory deque is greater than 4
        usedMemoryHistory.pop_front(); 			        // removes the first element of the usedMemoryHistory deque
        availableMemoryHistory.pop_front(); 		    // removes the first element of the availableMemoryHistory deque
    }
}

void MemoryInfo::displayData() const {                                  // function displayData
    clearScreen();                                                      // calls the clearScreen function
    cout << "Total Memory: " << totalMemory << " KB" << std::endl; // displays the totalMemory variable
    cout << "Used Memory: " << usedMemory << " KB" << std::endl;   // displays the usedMemory variable
    
    // displays the availableMemory variable
    cout << "Available Memory: " << availableMemory << " KB" << std::endl << std::endl; 
    cout << "\t\tPRESS 3 TO EXIT" << std::endl;                // displays the exit message
    cout << "Memory history (past 4 seconds):" << std::endl;       // displays the memory history message
    cout << "Time (s)\tUsed Memory (KB)\tAvailable Memory (KB)" << std::endl; // displays the table headers
    int time = -4;                                                      // creates int variable time and sets it to -4
    
    // for loop that iterates through the usedMemoryHistory deque and the availableMemoryHistory deque
    // the [auto] keyword is used to automatically deduce the iterator types based on the container types
    // The for loop will then iterate through both containers simultaneously, using the iterators 
    // it_used and it_available to access the elements of usedMemoryHistory and availableMemoryHistory, respectively.
    // This whole concept of looping through two containers simultaneously is apparently called a "zipper loop"
    // amd COMPLETELY blew my mind. 
    for (auto it_used = usedMemoryHistory.begin(), it_available = availableMemoryHistory.begin();  
       
        // while the iterators are not at the end of the deques
        it_used != usedMemoryHistory.end() && it_available != availableMemoryHistory.end(); 
        ++it_used, ++it_available, ++time) { // increments the iterators and the time variable

        // displays the time, usedMemory, and availableMemory
        std::cout << "\t" << time << "\t" << *it_used << "\t\t" << *it_available << std::endl; 
    }
}

void MemoryInfo::updatePlatformSpecificMemoryInfo() {   // function updatePlatformSpecificMemoryInfo
#if defined(_WIN32) || defined(_WIN64)                  // if the operating system is Windows
    MEMORYSTATUSEX memInfo;                             // creates a MEMORYSTATUSEX variable memInfo
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);          // sets the dwLength variable of the memInfo variable 
                                                        // to the size of the MEMORYSTATUSEX variable
    GlobalMemoryStatusEx(&memInfo);                     // calls the GlobalMemoryStatusEx function and 
                                                        //passes the memInfo variable as a parameter

    totalMemory = memInfo.ullTotalPhys / 1024;          // sets the totalMemory variable to the ullTotalPhys 
                                                        //variable of the memInfo variable divided by 1024

    availableMemory = memInfo.ullAvailPhys / 1024;      // sets the availableMemory variable to the ullAvailPhys
												        // variable of the memInfo variable divided by 1024
    usedMemory = totalMemory - availableMemory;         // sets the usedMemory variable to the totalMemory 
											            // minues the availableMemory variable
#else                       // if the operating system is not Windows
    struct sysinfo memInfo; // creates a sysinfo variable memInfo
    sysinfo(&memInfo);      // calls the sysinfo function and passes the memInfo variable as a parameter

    totalMemory = memInfo.totalram * memInfo.mem_unit / 1024;    // sets the totalMemory variable to the totalram variable of 
															     // the memInfo variable multiplied by the mem_unit variable of 
															     // the memInfo variable divided by 1024
    availableMemory = memInfo.freeram * memInfo.mem_unit / 1024; // sets the availableMemory variable to the freeram variable 
																 // of the memInfo variable multiplied by the mem_unit variable 
																 // of the memInfo variable divided by 1024
    usedMemory = totalMemory - availableMemory;                  // sets the usedMemory variable to the totalMemory variable minus the 
											                     // availableMemory variable
#endif                                                           // ends if loop
}

//Initializes the 'filename' member variable with the provided 'filename' argument
MemoryInfoFileWriter::MemoryInfoFileWriter(const std::string& filename) : filename(filename) { 
}

void MemoryInfoFileWriter::updateData() {   // function updateData
    MemoryInfo::updateData();               // calls the updateData function of the MemoryInfo class

    ofstream outfile(filename, ios_base::app);    // creates an ofstream variable outfile and opens the 
                                                            //file with the filename variable as a parameter
    if (!outfile) { // if the outfile variable is false

        // displays the error message
        std::cerr << "Error: Unable to open file " << filename << " for writing" << std::endl; 
        return; // returns
    }
    // writes the usedMemory and availableMemory variables to the file
    outfile << "Used Memory: " << usedMemory << " KB, Available Memory: " << availableMemory << " KB\n"; 
}

void MemoryInfoFileWriter::displayData() const {        // function displayData
    if (!messageShown) {                                // if the messageShown variable is false
        cout << "Memory information has been saved to " << filename << std::endl; // displays the message
        cout << "Press 3 to exit " << std::endl;   // displays the exit message

        messageShown = true;                            // sets the messageShown variable to true
    }
}

