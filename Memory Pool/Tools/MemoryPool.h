/*
Author: Ching-Yen Lin
*/
#pragma once

#define MByte unsigned char* 
struct MemoryChunk
{
	MemoryChunk()
		:Data(nullptr), Size(0), UsedSize(0), IsUsed(false), Next(nullptr)
	{
	}
	MByte Data;
	size_t Size;
	size_t UsedSize;
	bool IsUsed;
	MemoryChunk* Next;
};
class MemoryPool
{
public:
	/*
	this function take memory pool size, Chunk size
	it will initial one memory pool for use
	*/
	MemoryPool(const size_t&, const size_t&);
	~MemoryPool();
	/*
	this function take request memory size
	it update the memory pool and return void *
	*/
	void* GetMemory(const size_t&);
	/*
	this function take void*
	free the pointer in the memory pool
	*/
	void FreeMemory(void*);

private:
	/*
	this funcion take memory chunk pointer to chunk array and
	chunk number to create, and memory block pointer
	it will inital chunk array and chunk list
	*/
	void LinkChunkList(MemoryChunk*, unsigned int, MByte);
	/*
	this funcion take total memory block size
	it will creat memory block and initial chunk array and list
	*/
	bool AllocateMemory(const size_t&);
	MemoryChunk* mFirstChunkptr;
	MemoryChunk* mEndChunkptr;
	MemoryChunk* mCurrentChuckptr;
	MByte mMemoryBlock;
	size_t mTotalSize;
	size_t mUsedSize;
	size_t mFreeSize;
	size_t mChunkSize;
	unsigned int mChunkCount;
};