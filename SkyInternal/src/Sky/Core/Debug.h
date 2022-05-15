#pragma once

#if _DEBUG

// Macros
#define TYPE_NAME(value) #value
#define SKY_DBG(function)	\
	function

#include <Windows.h>

#include <iostream>
#include <string>
#include <sstream>
#include <memory>

namespace Sky
{
	namespace Core
	{

		class Debug
		{
		private:

		public:
			static bool CreateConsole();
			static bool DestroyConsole();

			// Message boxes
			static void MsgBox(std::string msg);
			static void MsgBox(std::string msg, long code);

			// Helper
			static std::string GetLastErrorAsString();
			static std::string SkyErrorToString(HRESULT hr);
		};

		enum class LogLevel : unsigned int
		{
			//
			// Summary:
			//     Logs that contain the most detailed messages. These messages may contain sensitive
			//     application data. These messages are disabled by default and should never be
			//     enabled in a production environment.
			Trace = 0,
			//
			// Summary:
			//     Logs that are used for interactive investigation during development. These logs
			//     should primarily contain information useful for debugging and have no long-term
			//     value.
			Debug = 1,
			//
			// Summary:
			//     Logs that track the general flow of the application. These logs should have long-term
			//     value.
			Information = 2,
			//
			// Summary:
			//     Logs that highlight an abnormal or unexpected event in the application flow,
			//     but do not otherwise cause the application execution to stop.
			Warning = 3,
			//
			// Summary:
			//     Logs that highlight when the current flow of execution is stopped due to a failure.
			//     These should indicate a failure in the current activity, not an application-wide
			//     failure.
			Error = 4,
			//
			// Summary:
			//     Logs that describe an unrecoverable application or system crash, or a catastrophic
			//     failure that requires immediate attention.
			Critical = 5,
			//
			// Summary:
			//     Not used for writing log messages. Specifies that a logging category should not
			//     write any messages.
			None = 6
		};

		class Logger
		{
		private:
			static const constexpr std::size_t s_bufferSize = 1024 * 16;

			static LogLevel s_defaultLogLevel;

			std::string m_category;
			LogLevel m_logLevel;

			std::string LogLevelToString(LogLevel level);
			std::string getCurrentTimeAsString();

		public:
			static const constexpr HRESULT GetLastError = 0;

			Logger(std::string category, LogLevel logLevel);

			static std::shared_ptr<Logger> Create(std::string category);
			static std::shared_ptr<Logger> Create(std::string category, LogLevel logLevel);

			static void SetDefaultLogLevel(LogLevel logLevel);
			static LogLevel GetDefaultLogLevel();

			bool IsEnabled(LogLevel logLevel);

#pragma region Actual logging logic
			void PrintLog(LogLevel logLevel, std::string message);
			void PrintLog(LogLevel logLevel, const char* _Format, va_list _ArgList);
			void PrintLog(HRESULT hr, LogLevel logLevel, const char* _Format, va_list _ArgList);

			std::string FormatVarArgs(const char* _Format, va_list _ArgList);
#pragma endregion

#pragma region Wrapper

#pragma region Log wrapper
			void Log(LogLevel logLevel, const char* format, ...);
			void Log(HRESULT hr, LogLevel logLevel, const char* format, ...);
#pragma endregion

#pragma region LogTrace wrapper
			void LogTrace(const char* format, ...);
			void LogTrace(HRESULT hr, const char* format, ...);
#pragma endregion

#pragma region LogDebug wrapper
			void LogDebug(const char* format, ...);
			void LogDebug(HRESULT hr, const char* format, ...);
#pragma endregion

#pragma region LogInfo wrapper
			void LogInfo(const char* format, ...);
			void LogInfo(HRESULT hr, const char* format, ...);
#pragma endregion

#pragma region LogWarning wrapper
			void LogWarning(const char* format, ...);
			void LogWarning(HRESULT hr, const char* format, ...);
#pragma endregion

#pragma region LogError wrapper
			void LogError(const char* format, ...);
			void LogError(HRESULT hr, const char* format, ...);
#pragma endregion

#pragma region LogCritical wrapper
			void LogCritical(const char* format, ...);
			void LogCritical(HRESULT hr, const char* format, ...);
#pragma endregion

#pragma endregion
		};

	}
}

#elif

// Macros
#define TYPE_NAME(value) #value
#define SKY_DBG(function) 

#endif