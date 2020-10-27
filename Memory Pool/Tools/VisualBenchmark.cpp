
/*
Author: Ching-Yen Lin
File: VisualBenchmark.cpp
Detail: To create the visual benchmark on chrome://tracing/.
Important: This code is not Thread-safe.
*/

#include "pch.h"
#include "VisualBenchmark.h"

VisualBenchmark::VisualBenchmark()
	:mCurrentSession(nullptr), mProfileCount(0)
{}
void VisualBenchmark::BeginSession(const std::string& sessionName, const std::string& filePath)
{
	mOutputStream.open(filePath);
	WriteHeader();
	mCurrentSession = new ProfileSession{ sessionName };
}
void VisualBenchmark::EndSession()
{
	WriteFooter();
	mOutputStream.close();
	delete mCurrentSession;
	mCurrentSession = nullptr;
	mProfileCount = 0;
}
void VisualBenchmark::WriteProfile(const ProfileResult& result)
{
	std::lock_guard<std::mutex> lock(Benchmark_Write);
	if (mProfileCount++ > 0)
	{
		mOutputStream << ",";
	}
	std::string name = result.ProfileName;
	std::replace(name.begin(), name.end(), '"', '\'');

	mOutputStream << "{";
	mOutputStream << "\"cat\":\"function\",";
	mOutputStream << "\"dur\":"<<(result.End-result.Start)<<',';
	mOutputStream << "\"name\":\"" << name << "\",";
	mOutputStream << "\"ph\":\"X\",";
	mOutputStream << "\"pid\":0,";
	mOutputStream << "\"tid\":"<<result.ThreadID<<",";
	mOutputStream << "\"ts\":"<<result.Start;
	mOutputStream << "}";
	mOutputStream.flush();
}
VisualBenchmark & VisualBenchmark::Get()
{
	static VisualBenchmark* instance = new VisualBenchmark();
	return *instance;
}
void VisualBenchmark::WriteHeader()
{
	mOutputStream << "{\"otherData\": {},\"traceEvents\":[";
	mOutputStream.flush();
}
void VisualBenchmark::WriteFooter()
{
	mOutputStream << "]}";
	mOutputStream.flush();
}
BenchmarkTimer::BenchmarkTimer(const char* name)
	:mName(name), mStopped(false)
{
	mStartTime = std::chrono::high_resolution_clock::now();
}
BenchmarkTimer::~BenchmarkTimer()
{
	if (!mStopped)
		Stop();
}
void BenchmarkTimer::Stop()
{
	auto endTime = std::chrono::high_resolution_clock::now();
	auto start = std::chrono::time_point_cast<std::chrono::microseconds>(mStartTime).time_since_epoch().count();
	auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch().count();
	auto duration = end - start;
	double ms = duration * 0.001;
	uint32_t threadID = std::hash<std::thread::id>()(std::this_thread::get_id());
	VisualBenchmark::Get().WriteProfile({ mName,start,end,threadID });
	std::cout << mName << ": " << ms << " ms" << std::endl;
	mStopped = true;
}

