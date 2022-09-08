#pragma once
#define _CRTDBG_MAP_ALLOC 
#include <crtdbg.h>

//https://stackoverflow.com/questions/3994035/what-is-aligned-memory-allocation
//CPU 캐시 최적화..? 

template<size_t T>
class AlignedAllocationPolicy
{
public:
	static void* operator new(size_t size)
	{
		return _aligned_malloc(size, T);
	}

	static void operator delete(void* memory)
	{
		_aligned_free(memory);
	}
};

