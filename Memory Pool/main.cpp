/*
Author: Ching-Yen Lin
*/
#include "pch.h"
#include "Tools\VisualBenchmark.h"
#include "Tools\MemoryPool.h"
using namespace std;
#define TEST_SIZE 1000

/*
	this function take test count
	testing the malloc and free by test count time
*/
void MallocTest(int testCount)
{
	PROFILE_FUNCTION();
	for (int i = 0; i < testCount; i++)
	{
		char *array_ptr = (char *)malloc(TEST_SIZE);
		free(array_ptr);
	}
	return;
}
/*
	this function take test count
	testing the insert and remove in memory pool by test count time
*/
void MemoryPoolTest(int testCount)
{
	PROFILE_FUNCTION();
	MemoryPool mp = MemoryPool(10000, TEST_SIZE/10);
	for (int i = 0; i < testCount; i++)
	{
		char *array_ptr = (char*)mp.GetMemory(TEST_SIZE);
		mp.FreeMemory(array_ptr);
	}
	return;
}
int main()
{
	const int test_count = 1000000;
	VisualBenchmark::Get().BeginSession();
	auto t1 = std::async( MallocTest, test_count);
	auto t2 = std::async(MemoryPoolTest, test_count);
	t1.wait();
	t2.wait();
	VisualBenchmark::Get().EndSession();
	return 0;
}


