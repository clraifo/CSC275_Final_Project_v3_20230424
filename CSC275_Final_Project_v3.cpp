/*
* Casey L. Raiford
* CSC275 Final Project - Memory Tracking Console Application
* 
* 
This is the main program for the Memory Tracking Console Application. It contains functions that perform 
the tracking of memory usage, user input, and display of the menu options.

The program starts by setting the text color to green, because I was getting REALLY sore eyes from seeing 
black and white all day. 

It then displays the welcome message and instructions. It then enters a loop, allowing the user to 
return to the main menu after each run of the memory tracking program.

The memoryTracker function is responsible for periodically updating the memory data and displaying it 
on the console. It takes a std::shared_ptr of a MemoryDataProvider object as input. The function runs in 
a loop until either the stop flag or return-to-menu flag is set to true. It calls the updateData and 
displayData functions of the MemoryDataProvider object to update and display the memory information, 
respectively.

The userInput function is responsible for detecting user input during runtime. It runs in a loop until 
the stop flag is set to true. It reads the input character from the console and sets the stop flag to 
true if the user enters '3', or sets the return-to-menu flag to true if the user enters '4'.

The startMemoryTracking function displays the menu options, reads user input, and creates a shared pointer 
to a MemoryDataProvider object depending on the user's choice. It then starts the memoryTracker thread 
and the userInput thread to begin tracking memory usage and reading user input, respectively. Finally, it 
waits for both threads to complete using the join() function.

The main function sets the text color to green, displays the welcome message and instructions, and enters 
a do-while loop to allow the user to return to the main menu. It calls the startMemoryTracking function, 
which runs the memory tracking program and waits for the threads to complete. If the user chooses to return 
to the main menu, the loop repeats. When the user chooses to exit, the program sets the text color back 
to default and returns 0.
*/

#include <iostream>     //allows input and output
#include <thread>       //allows multithreading
#include <memory>       //allows shared pointers
#include <atomic>       //allows atomic variables - which I had never heard of before - but 
                        //which are really useful for multithreading
#include "MemoryInfo.h" //allows acces to MemoryDataProvider, MemoryInfo, MemoryInfoFileWriter classes

using namespace std;	//allows use of cout, cin, etc. without having to type std::cout, std::cin, etc.

atomic<bool> stopFlag(false);      // atomic variables are used to ensure that the threads
										// don't try to access the same variable at the same time
atomic<bool> returnToMenu(false);  // and that they don't try to access a variable that is
										// being modified by another thread

void memoryTracker(std::shared_ptr<MemoryDataProvider> memoryDataProvider) { //shared_ptr is used to ensure that the 
																			//memoryDataProvider object is not deleted
																			//while it is being used by the thread
    while (!stopFlag && !returnToMenu) {                        //the thread will continue to run until the user enters 3 or 4
        memoryDataProvider->updateData();                       //the thread will update the data and display it
        memoryDataProvider->displayData();                      //the thread will update the data and display it
        this_thread::sleep_for(chrono::seconds(1));             //the thread will sleep for 1 second
    }
}

void userInput() {                  //this thread will allow the user to enter 3 or 4 to stop the program
    char input;                     //or to return to the main menu
    while (!stopFlag) {             //the thread will continue to run until the user enters 3 or 4
        std::cin >> input;          //the user will enter 3 or 4
        if (input == '3') {         //if the user enters 3, the thread will set the stopFlag to true
            stopFlag = true;        //which will cause the memoryTracker thread to stop
        }
        else if (input == '4') {    //if the user enters 4, the thread will set the returnToMenu to true
            returnToMenu = true;    //which will cause the memoryTracker thread to stop
        }
    }
}

void startMemoryTracking() {        //this function will display the menu options and start the threads
    shared_ptr<MemoryDataProvider> memoryDataProvider; //this shared_ptr will be used to create a MemoryDataProvider object

    char choice;        //this variable will be used to store the user's choice
    while (true) {      //this loop will continue until the user enters a valid choice
        cout << "Choose an option:" << std::endl; //the user will choose an option
        cout << "1. Display memory information on the console" << std::endl;   //the user will choose an option
        cout << "2. Save memory information to a file" << std::endl;           //the user will choose an option
        std::cout << "3. Exit" << std::endl;    //the user will choose an option
        std::cin >> choice;                     //the user will choose an option

        if (choice == '1') {                                        //if the user chooses 1, the program will create a MemoryInfo object
            memoryDataProvider = std::make_shared<MemoryInfo>();    //and store it in the shared_ptr
            break;                                                  //and break out of the loop
        }
        else if (choice == '2') {                           //if the user chooses 2, the program will create a MemoryInfoFileWriter object
            string filename;                           //and store it in the shared_ptr
            cout << "Enter the output file name: ";    //and break out of the loop
            cin >> filename;                           //user will enter the output file name
            memoryDataProvider = std::make_shared<MemoryInfoFileWriter>(filename);  //creates a shared pointer to a MemoryInfoFileWriter 
                                                                                    //object
            break;  //and break out of the loop
        }
        else if (choice == '3') {   //if the user chooses 3, the program will set the stopFlag to true
            stopFlag = true;        //which will cause the memoryTracker thread to stop
            return;                 //and return to the main menu
        }
        else {                      //if the user chooses an invalid option, the program will display an error message
            cerr << "Invalid option. Please choose again." << std::endl; //and the loop will repeat
        }
    }

    thread memoryTrackerThread(memoryTracker, memoryDataProvider); //this thread will run the memoryTracker function
    thread userInputThread(userInput);                             //this thread will run the userInput function

    memoryTrackerThread.join();                                         //the program will wait for the threads to finish
    userInputThread.join();                                             //the program will wait for the threads to finish
}



int main() {                    //this is the main function
    cout << "\033[32m";    // Set text color to green

    //next 4 lines are all output dialogue ot the user
    cout << "Memory Tracking Console Application" << std::endl;
    cout << "This application displays total memory, used memory, and available memory." << std::endl;
    cout << "Press 3 and Enter to exit the application." << std::endl;
    cout << "Press 4 and Enter to return to the main menu." << std::endl << std::endl;

    do { //this do while loop will allow the user to return to the main menu after the program has finished
        returnToMenu = false; //the returnToMenu variable will be set to false
        startMemoryTracking(); //the startMemoryTracking function will be called
    } while (returnToMenu); //the do while loop will continue to run until the user enters 3 or 4

    cout << "\033[0m"; // Reset text color to default
    return 0; //the program will end
}
