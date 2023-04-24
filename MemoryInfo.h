/*

This code defines two classes, MemoryInfo and MemoryInfoFileWriter, both derived from the abstract base class 
MemoryDataProvider.

The MemoryInfo class represents the current memory state of the computer and stores three data members: 
totalMemory, usedMemory, and availableMemory, all of which are of type unsigned long long. It also contains a 
deque for storing the last four seconds of usedMemory and availableMemory values.

The MemoryInfo class has two virtual functions: updateData() and displayData(), which are used for updating 
and displaying the memory information, respectively. The updateData() function calls the protected function 
updatePlatformSpecificMemoryInfo() which updates the totalMemory, usedMemory, and availableMemory data 
members with platform-specific code. The displayData() function displays the memory information on the 
console, including the last four seconds of usedMemory and availableMemory.

The MemoryInfoFileWriter class represents the memory state of the computer and writes the memory information 
to a specified file. It has a data member filename of type std::string, representing the output file name. 
The updateData() function updates the memory information and writes it to the specified file. The 
displayData() function displays a message indicating where the memory information has been saved.

*/


#pragma once // tells the compiler to include this file only once.

#include <deque>                    // double ended queue - deque is used to store the memory information in a queue
#include <string>                   // string is used to store the name of the file to which the 
                                    //memory information will be saved
#include "MemoryDataProvider.h"     // allows access to the MemoryDataProvider class

using namespace std; // allows access to the std namespace

class MemoryInfo : public MemoryDataProvider { // creates class MemoryInfo that inherits from the 
											  //MemoryDataProvider class
public: // public member functions
    MemoryInfo(); // creates constructor
    void updateData() override; // creates function updateData
    void displayData() const override; // creates function displayData

protected: // protected member functions
    void updatePlatformSpecificMemoryInfo(); // creates function updatePlatformSpecificMemoryInfo

    unsigned long long totalMemory;     // creates unsigned long long variable totalMemory
    unsigned long long usedMemory;      // creates unsigned long long variable usedMemory
    unsigned long long availableMemory; // creates unsigned long long variable availableMemory

private: // private member functions
    deque<unsigned long long> usedMemoryHistory;	  // creates deque variable usedMemoryHistory
    deque<unsigned long long> availableMemoryHistory; // creates deque variable availableMemoryHistory
};

class MemoryInfoFileWriter : public MemoryInfo { // creates class MemoryInfoFileWriter that inherits 
												 //from the MemoryInfo class
public: // public member functions
    MemoryInfoFileWriter(const std::string& filename); // creates constructor
    void updateData() override; // creates function updateData
    void displayData() const override; // creates function displayData

private: // private member functions
    string filename; // creates string variable filename
    mutable bool messageShown = false; // creates bool variable messageShown
};

