/*
Author: Ching-Yen Lin
*/
#pragma once
#include "pch.h"

struct ProfileResult
{
	std::string ProfileName;
	long long Start, End;
	uint32_t ThreadID;
};
struct ProfileSession
{
	std::string SessionName;
};

class VisualBenchmark
{
public:
	VisualBenchmark();
	/*
	this function will take session name and file path
	and create file to radly to write profile
	*/
	void BeginSession(const std::string&  = "", const std::string& = "results.json");
	void EndSession();
	/*
	this function will take Profile Result
	and write file
	*/
	void WriteProfile(const ProfileResult& result);
	/*
	 this function will return this class' instance for other function to use
	*/
	static VisualBenchmark& Get();
private:
	void WriteHeader();
	void WriteFooter();
private:
	ProfileSession* mCurrentSession;
	std::ofstream mOutputStream;
	int mProfileCount;
};

class BenchmarkTimer
{
public:
	/*
	this function will take function name
	and start benchmart
	*/
	BenchmarkTimer(const char* ="");
	~BenchmarkTimer();
	void Stop();
private:
	std::chrono::time_point<std::chrono::steady_clock>mStartTime;
	const char* mName;
	bool mStopped;
};
static std::mutex Benchmark_Write;
#define PROFILING 1
#if PROFILING
#define PROFILE_SCOPE(name) BenchmarkTimer timer##__LINE__(name)
#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCSIG__)
#else
#define PROFILE_SCOPE(name)
#endif