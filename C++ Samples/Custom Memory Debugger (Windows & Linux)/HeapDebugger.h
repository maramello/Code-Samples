/*
* ==============================================================
* File   : HeapDebugger.h
* Purpose: Contains functions for custom heap memory debugger 
		   made for both Windows and Linux
  Author : Rohit Saini
* Date   : 11/2022
* All code is Copyrighted to owner, Rohit Saini
* ==============================================================
*/

#ifndef HEAPDEBUGGER_H
#define HEAPDEBUGGER_H

//This file is not included, since this is a sample
#include "Common.h"

//Standard C++ list and file stream
#include <list>
#include <fstream>

typedef void* MemoryAddress;


//store type of allocation
enum class MemoryType
{
	SINGLE_BLOCK = 0,
	ARRAY = 1
};

//store types of errors and leaks possible during memory allocation
enum class ErrorType
{
	NONE = 0,
	LEAK = 1,
	DOUBLE_DELETE = 2,
	NO_HEAP_POINTER = 3,
	NEW_DELETE_MISMATCH = 4
};

//store each memory allocation's information
struct MemoryData
{
public:
	MemoryAddress address;
	size_t bytes;

	MemoryType type;
	ErrorType errorType;

	bool deleted;

	MemoryAddress returnAddress;

	MemoryData()
	{
		address = nullptr;
		bytes = 0;
		type = MemoryType::SINGLE_BLOCK;
		errorType = ErrorType::NONE;
		deleted = false;
		returnAddress = 0;
	}

	MemoryData(MemoryAddress add, size_t size, MemoryType memoryType) :
		address(add), bytes(size), type(memoryType), errorType(ErrorType::NONE), deleted(false),
		returnAddress(0)
	{

	}
};

//for storing pointers
namespace Storage {
	template<typename T>
	using list = std::list<T, Mallocator<T>>;
}

//structure of the actual memory debuffer
struct HeapDebugger {
public:
	HeapDebugger();
	~HeapDebugger();

	//Functions to observe and free associated memory
	void ObserveMemory(MemoryData data);
	void FreeMemory(MemoryAddress address, MemoryType type, size_t noBytes, void* ReturnAddr);

	//File logging functions to CSV file
	void CreateCSV();
	void WriteCSV(MemoryData data);
	void CloseCSV();
	void CreateWriteCloseCSV(MemoryData data);

private:
	//All current allocations
	Storage::list<MemoryData> allocated_list;
	//CSV file pointer
	std::ofstream file_;
	//Whether csv file exists
	bool createdFile_;
};

static struct HeapDebuggerInitializer {
	HeapDebuggerInitializer();
	~HeapDebuggerInitializer();
} DebuggerInitializer; // static initializer for every singleton

#endif // HEAPDEBUGGER_H
