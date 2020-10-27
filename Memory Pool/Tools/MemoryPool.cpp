/*
Author: Ching-Yen Lin
File: MemoryPool.cpp
Detail: To create stack like memory pool
Important: This code is not Thread-safe.
It is stack like memory pool so need to use as stack.
*/
#include "pch.h"
#include"MemoryPool.h"
MemoryPool::MemoryPool(const size_t& totalsize, const size_t& chunksize)
	:mMemoryBlock(nullptr), mFirstChunkptr(nullptr), mEndChunkptr(nullptr), mCurrentChuckptr(nullptr),
	mTotalSize(totalsize), mChunkSize(chunksize), mUsedSize(0), mFreeSize(0),
	mChunkCount(0)
{
	bool result = AllocateMemory(totalsize);
}
MemoryPool::~MemoryPool()
{
	if (mMemoryBlock)
		free(mMemoryBlock);
	mMemoryBlock = nullptr;
	if (mFirstChunkptr)
		free(mMemoryBlock);
	mFirstChunkptr = nullptr;
	mEndChunkptr = nullptr;
	mCurrentChuckptr = nullptr;
	mTotalSize = 0;
	mChunkSize = 0;
	mUsedSize = 0;
	mFreeSize = 0;
	mChunkCount = 0;
}
bool MemoryPool::AllocateMemory(const size_t& memorySize)
{
	MByte  new_mem_block_ptr = (MByte)malloc(memorySize);
	if (!new_mem_block_ptr)
		return false;
	mMemoryBlock = new_mem_block_ptr;
	size_t chunk_num = memorySize / mChunkSize + ((memorySize%mChunkSize) ? 1 : 0);

	MemoryChunk* chunks = new MemoryChunk[chunk_num];
	if (!chunks)
	{
		free(mMemoryBlock);
		mMemoryBlock = nullptr;
		return false;
	}
	mChunkCount = chunk_num;
	LinkChunkList(chunks, mChunkCount, mMemoryBlock);
	mFreeSize = memorySize;
}
void MemoryPool::LinkChunkList(MemoryChunk* chunksPtr, unsigned int chunkNum, MByte memBlockPtr)
{
	//link chunk list like link list
	for (unsigned int i = 0; i < chunkNum; i++)
	{
		if (i == 0)
		{
			mFirstChunkptr = chunksPtr;
			mEndChunkptr = chunksPtr;
			mCurrentChuckptr = chunksPtr;
		}
		else
		{
			mEndChunkptr->Next = chunksPtr + i;
			mEndChunkptr = mEndChunkptr->Next;
		}
		mEndChunkptr->Data = memBlockPtr + i * mChunkSize;
		mEndChunkptr->Size = mTotalSize - i * mChunkSize;
	}

}
void* MemoryPool::GetMemory(const size_t& memSize)
{
	//find enough spase and mark chunk list
	if (mCurrentChuckptr->Size >= memSize)
	{
		void* return_mem = mCurrentChuckptr->Data;
		size_t accum = 0;
		MemoryChunk* chuck_ptr = mCurrentChuckptr;
		while (accum < memSize)
		{
			chuck_ptr->UsedSize = memSize - accum;
			chuck_ptr->IsUsed = true;
			accum += mChunkSize;
			chuck_ptr = chuck_ptr->Next;
		}
		mCurrentChuckptr->IsUsed = true;
		mCurrentChuckptr->Next = chuck_ptr;
		mCurrentChuckptr = chuck_ptr;
		mUsedSize += memSize;
		mFreeSize -= memSize;
		return return_mem;
	}
	return nullptr;
}

void MemoryPool::FreeMemory(void * input)
{
	//find which chunk hold that pointer 
	MemoryChunk* target_chuck_ptr = mFirstChunkptr;
	while (target_chuck_ptr->Data != input)
	{
		target_chuck_ptr = target_chuck_ptr->Next;
	} 
	MemoryChunk* begin_ptr = target_chuck_ptr;
	MemoryChunk* end_ptr= target_chuck_ptr->Next;
	size_t accum = 0;
	//mark the related chunk to free
	while (begin_ptr!= end_ptr)
	{
		begin_ptr->IsUsed = false;
		accum += begin_ptr->UsedSize;
		begin_ptr->UsedSize = 0;
		begin_ptr++;
	}

	target_chuck_ptr->IsUsed = false;
	target_chuck_ptr->Next = target_chuck_ptr + 1;
	mCurrentChuckptr = target_chuck_ptr;
	mUsedSize -= accum;
	mFreeSize += accum;
}
