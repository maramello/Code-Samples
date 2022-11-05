/*
* ==============================================================
* File   : HeapDebugger.cpp
* Purpose: Contains functions for custom heap memory debugger 
		   made for both Windows and Linux
  Author : Rohit Saini
* Date   : 11/2022
* All code is Copyrighted to owner, Rohit Saini
* ==============================================================
*/

#include "HeapDebugger.h"

//include OS Specific functions only
#if defined (_MSC_VER)
#include "WinFunctions.h"   //This file is NOT included, as this is a display sample

#else
#include "LinuxFunctions.h" //This file is NOT included, as this is a display sample

#endif

#include <new>         //for placement new, and overloads
#include <cstdio>
#include <stdexcept>

#define MAXSIZE 16711568

//Zero initialized at load time
static int nifty_counter;

//Memory for the heap debugger singleton object
static typename std::aligned_storage<sizeof(HeapDebugger), alignof (HeapDebugger)>::type
heap_buf;

// global stream object
HeapDebugger& debugger = reinterpret_cast<HeapDebugger&> (heap_buf);

HeapDebugger::HeapDebugger ()
{
	createdFile_ = false;
}

//Write all memory leak data to the CSV on destruction
HeapDebugger::~HeapDebugger ()
{
  //clean-up
	if (!createdFile_)
	{
		//Loop through all memory data
		for (std::list<MemoryData>::iterator it = allocated_list.begin(); it != allocated_list.end(); it++)
		{
			if (it->deleted == false)
			{
				//if new-delete mismatch, preserve error type
				if (it->errorType == ErrorType::NEW_DELETE_MISMATCH)
					printf("New-delete mismatch LEAK detected, not overwriting error type.\n");
				else
				{
					printf("Memory leak detected at %p, of type %s\n", it->address, it->type == MemoryType::SINGLE_BLOCK ? "Single Block" : "Array");
					it->errorType = ErrorType::LEAK;
				}
				WriteCSV(*it);
			}

			else if (it->errorType != ErrorType::NONE)
			{
				WriteCSV(*it);
			}
		}
	}
}

//Create csv file if it does not exist
void HeapDebugger::CreateCSV()
{
	if (!createdFile_)
	{
		std::ofstream csvFile("DebugLog.csv", std::ofstream::out);
		csvFile << "Message, File, Line, Bytes, Address, Additional Info\n";
		csvFile.close(); createdFile_ = true;
	}
}

//Write data to the CSV file
void HeapDebugger::WriteCSV(MemoryData data)
{
	CreateCSV();

	std::ofstream file("DebugLog.csv", std::ios_base::app);

	const char* msg = data.errorType == ErrorType::LEAK ? "Memory Leak" :
		data.errorType == ErrorType::DOUBLE_DELETE ? "Double Delete" : 
		data.errorType == ErrorType::NO_HEAP_POINTER ? "Non Heap Pointer Deletion" : 
		data.errorType == ErrorType::NEW_DELETE_MISMATCH ? "New-delete mismatch" : "None";

	int LineNo = 0;
	const char* fileName;

	bool isDoubleOrHeapDeleteOrMismatch = data.errorType == ErrorType::DOUBLE_DELETE ? true : 
		data.errorType == ErrorType::NO_HEAP_POINTER ? true : data.errorType == ErrorType::NEW_DELETE_MISMATCH ? true : false;

	//Windows SPECIFIC GET FUNCTION 
	#if defined (_MSC_VER)
		Funcs::GetSymFromAddress(data.returnAddress, &LineNo, fileName, isDoubleOrHeapDeleteOrMismatch);
	#else 
		Basic_String str = Funcs::GetSymFromAddress(data.returnAddress, &LineNo, isDoubleOrHeapDeleteOrMismatch);
		fileName = str.c_str();
	#endif

	const char* AddInfo = "None";

	char buff[4096];

	#if defined (_MSC_VER)
		sprintf_s(buff, 4096, "%s,%s,%d,%zu,%p,%s\n", msg, fileName, LineNo, data.bytes, data.address, AddInfo);
	#else 
		sprintf(buff, "%s,%s,%d,%zu,%p,%s\n", msg, fileName, LineNo, data.bytes, data.address, AddInfo);
	#endif

	file << buff;
	file.close();
}

//Close the csv file.
void HeapDebugger::CloseCSV()
{
	createdFile_ = false;
}

//Write final data to the CSV
void HeapDebugger::CreateWriteCloseCSV(MemoryData data)
{
	CreateCSV();
	WriteCSV(data);
	CloseCSV();
}

//Add memory to the observed list
void HeapDebugger::ObserveMemory(MemoryData data)
{
	//std::cout << "Allocated " << data.bytes << " bytes at address " << data.address << std::endl;
	allocated_list.push_back(data);
}

//Free associated memory
void HeapDebugger::FreeMemory(MemoryAddress address, MemoryType type, size_t noBytes, void* ReturnAddr)
{
	if (address == 0)
	{	
		return;
	}

	bool addressFound = false;
	
	//loop through memory
	for (std::list<MemoryData>::iterator it = allocated_list.begin(); it != allocated_list.end(); ++it)
	{
		//find the address
		if (address == it->address)
		{
			addressFound = true;

			//determine type of memory to free
			if (type == it->type)
			{
				//delete the memory if it has notbeen deleted already
				if (!it->deleted)
				{
					it->deleted = true;

					Funcs::FreePageMemory(address, noBytes);
					
					return;
				}
				else
				{
					//this means it was trying to delete memory which was already deleted.
					printf("Double delete detected at %p, of type %s\n", it->address, it->type == MemoryType::SINGLE_BLOCK ? "Single Block" : "Array");
					it->errorType = ErrorType::DOUBLE_DELETE;
					it->returnAddress = ReturnAddr;

					//close the program and break
					CreateWriteCloseCSV(*it);
					DEBUG_BREAKPOINT();
					return;
				}
			}

			//Types are NOT the same
			else
			{
				//If types aren't the same and the memory is not deleted, then it is a mismatch
				if (it->deleted == false)
				{
					printf("Delete mismatch at %p, of type %s\n", it->address, it->type == MemoryType::SINGLE_BLOCK ? "Single Block" : "Array");
					it->errorType = ErrorType::NEW_DELETE_MISMATCH;
					it->returnAddress = ReturnAddr;
					CreateWriteCloseCSV(*it);
					DEBUG_BREAKPOINT();
					return;
				}
			}
		}
	}

	if (addressFound == false)
	{
		MemoryData data(address, noBytes, type);
		data.errorType = ErrorType::NO_HEAP_POINTER;
		data.deleted = true;
		data.returnAddress = ReturnAddr;
	
		printf("Non heap pointer delete detected at %p\n", address);
		CreateWriteCloseCSV(data);

		DEBUG_BREAKPOINT();
		return;

	}
	
}

//Initialize the singleton
HeapDebuggerInitializer::HeapDebuggerInitializer ()
{
	if (nifty_counter++ == 0)
	{
		// placement new of the objects
		new (&debugger) HeapDebugger(); 

		//Windows Sym function only works with MSVC
		#if defined (_MSC_VER)

		//INITIALIZE SYM, the debugger library. Only needed for windows
		Funcs::InitSym();

		#endif		

		printf("Debugger Initialized\n");
	}
}

//Call destructor on singleton
HeapDebuggerInitializer::~HeapDebuggerInitializer ()
{
	if (--nifty_counter == 0)
	{
		(&debugger)->~HeapDebugger();

		printf("Debugger Freed\n");
	}
}

//====================================================================================================
//Below are all the NEW overloads, with appropriate calls to 'Observe Memory' function of the debugger
//Global overloads for tracking

void* operator new(size_t size)
{
	MemoryAddress a = GET_RETURN_ADDR();

	if (size > MAXSIZE)
		throw std::bad_alloc();

	MemoryAddress memory = Funcs::PageAlignedAllocate(size);

	MemoryData data(memory, size, MemoryType::SINGLE_BLOCK);
	data.returnAddress = a;

	debugger.ObserveMemory(data);

	return memory;
}
void* operator new(size_t size, const std::nothrow_t& ) noexcept
{
	MemoryAddress a = GET_RETURN_ADDR();

	if (size > MAXSIZE)
		return nullptr;

	

	MemoryAddress memory = Funcs::PageAlignedAllocate(size);

	MemoryData data(memory, size, MemoryType::SINGLE_BLOCK);
	data.returnAddress = a;

	debugger.ObserveMemory(data);

	return memory;
}

void* operator new[](size_t size)
{
	MemoryAddress a = GET_RETURN_ADDR();

	if (size > MAXSIZE)
		throw std::bad_alloc();

	MemoryAddress memory = Funcs::PageAlignedAllocate(size);

	MemoryData data(memory, size, MemoryType::ARRAY);
	data.returnAddress = a;

	debugger.ObserveMemory(data);

	return memory;
}

void* operator new[](size_t size, const std::nothrow_t& ) noexcept
{
	MemoryAddress a = GET_RETURN_ADDR();

	if (size > MAXSIZE)
		return nullptr;
	

	MemoryAddress memory = Funcs::PageAlignedAllocate(size);

	MemoryData data(memory, size, MemoryType::ARRAY);
	data.returnAddress = a;

	debugger.ObserveMemory(data);

	return memory;
}

//====================================================================================================
//Below are all the DELETE overloads, with appropriate calls to 'Free Memory' function of the debugger

void operator delete(void* address) noexcept
{
	MemoryAddress a = GET_RETURN_ADDR();

	if (address == 0)
		return;

	debugger.FreeMemory(address, MemoryType::SINGLE_BLOCK, 0, a);
}

void operator delete(void* address, size_t size) noexcept
{
	MemoryAddress a = GET_RETURN_ADDR();

	if (address == 0)
		return;

	debugger.FreeMemory(address, MemoryType::SINGLE_BLOCK, size, a);
}

void operator delete(void* address, const std::nothrow_t& ) noexcept
{
	MemoryAddress a = GET_RETURN_ADDR();


	if (address == 0)
		return;

	debugger.FreeMemory(address, MemoryType::SINGLE_BLOCK, 0, a);
}

void operator delete[](void* address) noexcept
{
	MemoryAddress a = GET_RETURN_ADDR();

	if (address == 0)
		return;

	debugger.FreeMemory(address, MemoryType::ARRAY, 0, a);
}

void operator delete[](void* address, size_t size) noexcept
{
	MemoryAddress a = GET_RETURN_ADDR();

	if (address == 0)
		return;

	debugger.FreeMemory(address, MemoryType::ARRAY, size, a);
}

void operator delete[](void* address, const std::nothrow_t&) noexcept
{
	MemoryAddress a = GET_RETURN_ADDR();

	if (address == 0)
		return;

	debugger.FreeMemory(address, MemoryType::ARRAY, 0, a);
}
