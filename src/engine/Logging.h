#ifndef LOGGING_H_
#define LOGGING_H_

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#if defined(__GNUC__)
#define MethodName __PRETTY_FUNCTION__

#elif defined(WIN32)

#define MethodName __FUNCDNAME__

#endif

#define LOG_ALL     9
#define LOG_TRACE   6
#define LOG_INFO    5
#define LOG_DEBUG   4
#define LOG_WARNING 3
#define LOG_ERROR   2
#define LOG_FATAL   1
#define LOG_NONE    0
#define LOG_LEVEL LOG_DEBUG

void LogImpl(int level, const char* function, const char* format, ...);

#define Trace(format, ...) do { LogImpl(LOG_TRACE, MethodName, format, ##__VA_ARGS__); } while(0)
#define Info(format, ...) do { LogImpl(LOG_INFO, MethodName, format, ##__VA_ARGS__); } while(0)
#define Debug(format, ...) do { LogImpl(LOG_DEBUG, MethodName, format, ##__VA_ARGS__); } while(0)
#define Warning(format, ...) do { LogImpl(LOG_WARNING, MethodName, format, ##__VA_ARGS__); } while(0)
#define Error(format, ...) do { LogImpl(LOG_ERROR, MethodName, format, ##__VA_ARGS__); } while(0)
#define Fatal(format, ...) do { LogImpl(LOG_FATAL, MethodName, format, ##__VA_ARGS__); } while(0)


#endif /* LOGGING_H_ */
