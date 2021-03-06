#pragma once

#include <string.h>
#include <iostream>
#include <fstream>
#include <map>
#include <sys/stat.h>
#include <vector>
#include <dirent.h>
#include "../core/ctre.h"
#include <mutex>
#include <thread>

/* forward prototype */
namespace CTRE {
namespace Logger {
class TimestampMsgMap;
}
}

class LoggerDriver {
public:
	void Looping();
	void Open(int language, bool logDriverStation);
	void Close();

	static const int kHierarchyInternal = 0;
	static const int kHierarchyCCI = 1;
	static const int kHierarchyJava = 2;
	static const int kHierarchyAPI = 3;


	CTR_Code Log(CTR_Code code, const std::string & origin, int hierarchy, const char *stacktrace);
	void GetDescription(CTR_Code code, std::string & shrtError, std::string & longError);

	static LoggerDriver & GetInstance();
private:
	LoggerDriver();
	~LoggerDriver();
	static unsigned long GetDirSize(DIR *t);
	static std::vector<std::string> OrderedFiles(DIR *directory);
	
	CTRE::Logger::TimestampMsgMap * _msgMap;

//	unsigned long startOfSeconds = 0;
//	//std::map<std::string, unsigned long> timeStamps;
//	bool writing = false;
//	bool errorF = false;
//	bool threadStarted = false;
//	bool closeThread = false;
//	std::string buf = "";
//	std::mutex thisMutex;
//	std::thread t;
//	FILE *file = NULL;

	static LoggerDriver *_instance;
	
};
