/*

This header file is for the abstract class MemoryDataProvider. This class has three pure virtual functions: 
updateData(), displayData(), and a default virtual destructor ~MemoryDataProvider(). These functions are meant 
to be implemented by any class that inherits from MemoryDataProvider.

The updateData() function is used to update the memory information, while displayData() is used to display 
the updated memory information. The virtual destructor ~MemoryDataProvider() is necessary to ensure that any 
derived class objects are properly destroyed when they are no longer needed.

By making MemoryDataProvider an abstract class, any derived class must implement these functions, ensuring 
a consistent interface. This allows the rest of the program to work with any MemoryDataProvider object 
regardless of its specific type, making the code more modular and flexible.

*/

#pragma once // This is a preprocessor directive. It tells the compiler to include this file only once.

class MemoryDataProvider {  // creates class MemoryDataProvider
public: // public member functions
    virtual void updateData() = 0;              // creates virtual function updateData
    virtual void displayData() const = 0;       // creates virtual function displayData
    virtual ~MemoryDataProvider() = default;    // creates virtual destructor
};
