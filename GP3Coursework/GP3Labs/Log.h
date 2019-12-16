#pragma once

#ifdef _DEBUG
#define LOG_DEBUG(msg, errorLevel) Log::Debug(msg, __FILE__, __LINE__, errorLevel)
#define LOG_DEBUG_BOOL(msg, errorLevel) Log::DebugBool(msg, __FILE__, __LINE__, errorLevel)
#define GL_ATTEMPT(func) func; \
	if(Log::CheckGLError(__FILE__, __LINE__)) __debugbreak()
#define CHECK_GL_ERROR() Log::CheckGLError(__FILE__, __LINE__)
#else //shouldn't really be outputting to console if this is a release build
#define LOG_DEBUG(msg)
#define GL_ATTEMPT(func) func
#define CHECK_GL_ERROR()
#endif

//Tdifferent log levels
class Log
{
private:
	Log();
public:
	enum LogLevel
	{
		Trace,
		Warning,
		Error
	};
	static void Debug(const std::string& msg, const char* file, int line, LogLevel l);
	static void DebugBool(const bool& msg, const char* file, int line, LogLevel l);
	static void DebugIvec(const glm::ivec2& msg, const char* file, int line, LogLevel l);
	static bool CheckGLError(const char* file, int line);
};


