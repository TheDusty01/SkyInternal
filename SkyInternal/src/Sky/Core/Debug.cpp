#if _DEBUG

#include "Debug.h"

#include "../../ExitCodes.h"
#include "Detours.h"

#include <ctime>

using namespace Sky::Core;

#pragma region Debug Class
bool Debug::CreateConsole()
{
    if (!AllocConsole())
    {
        // Add some error handling here.
        // You can call GetLastError() to get more info about the error.
        return false;
    }

    // std::cout, std::clog, std::cerr, std::cin
    FILE* fstout = stdout, *fstdin = stdin, *fstderr = stderr;
    freopen_s(&fstout, "CONOUT$", "w", stdout);
    freopen_s(&fstderr, "CONOUT$", "w", stderr);
    freopen_s(&fstdin, "CONIN$", "r", stdin);
    std::cout.clear();
    std::clog.clear();
    std::cerr.clear();
    std::cin.clear();

    return true;
}

bool Debug::DestroyConsole()
{
    fclose(stdout);
    fclose(stderr);
    fclose(stdin);

    if (!FreeConsole())
    {
        return false;
    }

    return false;
}

void Debug::MsgBox(std::string msg)
{
    MessageBoxA(0, msg.c_str(), "Sky", MB_OK | MB_ICONINFORMATION);
}

void Debug::MsgBox(std::string msg, long code)
{
    std::stringstream ss;
    ss << msg << ": " << code;
    std::string s = ss.str();

    MessageBoxA(0, s.c_str(), "Sky", MB_OK | MB_ICONINFORMATION);
}

std::string Debug::GetLastErrorAsString()
{
    // Get the error message ID, if any.
    DWORD errorMessageId = ::GetLastError();
    if (errorMessageId == 0)
    {
        // No error message has been recorded
        return std::string(); 
    }

    LPSTR messageBuffer = nullptr;

    // Ask Win32 to give us the string version of that message ID.
    // The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be).
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, errorMessageId, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    // Copy the error message into a std::string.
    std::string message(messageBuffer, size);

    // Free the Win32's string's buffer.
    LocalFree(messageBuffer);
    
    return message;
}

std::string Debug::SkyErrorToString(HRESULT hr)
{
#define HR_TO_STR(x)    \
    case x:             \
        return #x

    switch (hr)
    {
        HR_TO_STR(SKY_ERROR);
        HR_TO_STR(SKY_ERROR_WINDOW);
        HR_TO_STR(SKY_ERROR_WINDOW_SIZE);
        HR_TO_STR(SKY_ERROR_WINDOW_PROC);
        HR_TO_STR(SKY_ERROR_DIRECT3D_CREATE);
        HR_TO_STR(SKY_ERROR_DIRECT3D_CREATE_DEVICE);
        HR_TO_STR(SKY_ERROR_DETOURS_INIT);
        HR_TO_STR(SKY_ERROR_DETOURS_ENABLE);
        HR_TO_STR(SKY_ERROR_DETOURS_HOOK_ENDSCENE);
        HR_TO_STR(SKY_ERROR_DETOURS_HOOK_RESET);
        HR_TO_STR(SKY_ERROR_GET_CLIENT);
        HR_TO_STR(SKY_ERROR_GET_ENGINE);
    }

#undef HR_TO_STR

    //std::string detourStatus = Detours::StatusToString(static_cast<MH_STATUS>(hr));
    

    return std::string();
}
#pragma endregion

#pragma region Logger Class
LogLevel Logger::s_defaultLogLevel = LogLevel::Information;

std::string Logger::LogLevelToString(LogLevel level)
{
    switch (level)
    {
    case LogLevel::Trace:
        return "TRACE";
    case LogLevel::Debug:
        return "DEBUG";
    case LogLevel::Information:
        return "INFO";
    case LogLevel::Warning:
        return "WARN";
    case LogLevel::Error:
        return "ERROR";
    case LogLevel::Critical:
        return "CRITICAL";
    default:
        return "NONE";
    }
}

std::string Logger::getCurrentTimeAsString()
{
    time_t rawtime;
    struct tm timeinfo;

    time(&rawtime);
    localtime_s(&timeinfo, &rawtime);

    // "2021-09-30 23:59:59"  -> 19 chars
    char buffer[20];    // Max date length is 19 + 1 for zero terminator
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    std::string dateTime(buffer);

    return dateTime;
}

Logger::Logger(std::string category, LogLevel logLevel) : m_category(category), m_logLevel(logLevel)
{
}

std::shared_ptr<Logger> Logger::Create(std::string category)
{
    return Create(category, Logger::s_defaultLogLevel);
}

std::shared_ptr<Logger> Logger::Create(std::string category, LogLevel logLevel)
{
    return std::make_shared<Logger>(category, logLevel);
}

void Logger::SetDefaultLogLevel(LogLevel logLevel)
{
    s_defaultLogLevel = logLevel;
}

LogLevel Logger::GetDefaultLogLevel()
{
    return Logger::s_defaultLogLevel;
}

bool Logger::IsEnabled(LogLevel logLevel)
{
    return logLevel >= m_logLevel;
}

#pragma region Actual Logging Logic
void Logger::PrintLog(LogLevel logLevel, std::string formattedMessage)
{
    // Get current timestamp
    std::string timestamp = getCurrentTimeAsString();

    // actual log output
    std::cout << "[" << timestamp << "]" << " " << "[" << LogLevelToString(logLevel) << "]" << " " << "[" << m_category << "]" << " " << formattedMessage << std::endl;
}

void Logger::PrintLog(LogLevel logLevel, const char* _Format, va_list _ArgList)
{
    // only print if logLevel is enabled
    if (!IsEnabled(logLevel))
        return;

    std::string formattedMessage = FormatVarArgs(_Format, _ArgList);
    PrintLog(logLevel, formattedMessage);
}


void Logger::PrintLog(HRESULT hr, LogLevel logLevel, const char* _Format, va_list _ArgList)
{
    // only print if logLevel is enabled
    if (!IsEnabled(logLevel))
        return;

    std::string errMsg;
    if (hr == GetLastError)
    {
        errMsg = Debug::GetLastErrorAsString();
    }
    else
    {
        errMsg = Debug::SkyErrorToString(hr);
    }

    // If error message is empty set default value
    if (errMsg.empty())
    {
        errMsg = "Unknown";
    }

    std::string formattedMessage = FormatVarArgs(_Format, _ArgList) +
        "\nError: " + errMsg;

    PrintLog(logLevel, formattedMessage);
}


std::string Logger::FormatVarArgs(const char *_Format, va_list _ArgList)
{
    char* buffer = new char[s_bufferSize];

    vsnprintf(buffer, s_bufferSize, _Format, _ArgList);

    std::string formattedMessage(buffer);

    delete[] buffer;

    return formattedMessage;
}
#pragma endregion

#pragma region Wrapper

// Macros
#define LOG(logLevel, format) \
		va_list args; \
		va_start(args, format); \
		PrintLog(logLevel, format, args); \
		va_end(args)

#define LOG_HR(hr, logLevel, format) \
		va_list args; \
		va_start(args, format); \
		PrintLog(hr, logLevel, format, args); \
		va_end(args)


#pragma region Log wrapper
void Logger::Log(LogLevel logLevel, const char* format, ...)
{
    LOG(logLevel, format);
}

void Logger::Log(HRESULT hr, LogLevel logLevel, const char* format, ...)
{
    LOG_HR(hr, logLevel, format);
}
#pragma endregion

#pragma region LogTrace wrapper
void Logger::LogTrace(const char* format, ...)
{
    LOG(LogLevel::Trace, format);
}

void Logger::LogTrace(HRESULT hr, const char* format, ...)
{
    LOG_HR(hr, LogLevel::Trace, format);
}
#pragma endregion

#pragma region LogDebug wrapper
void Logger::LogDebug(const char* format, ...)
{
    LOG(LogLevel::Debug, format);
}

void Logger::LogDebug(HRESULT hr, const char* format, ...)
{
    LOG_HR(hr, LogLevel::Debug, format);
}
#pragma endregion

#pragma region LogInfo wrapper
void Logger::LogInfo(const char* format, ...)
{
    LOG(LogLevel::Information, format);
}

void Logger::LogInfo(HRESULT hr, const char* format, ...)
{
    LOG_HR(hr, LogLevel::Information, format);
}
#pragma endregion

#pragma region LogWarning wrapper
void Logger::LogWarning(const char* format, ...)
{
    LOG(LogLevel::Warning, format);
}

void Logger::LogWarning(HRESULT hr, const char* format, ...)
{
    LOG_HR(hr, LogLevel::Warning, format);
}
#pragma endregion

#pragma region LogError wrapper
void Logger::LogError(const char* format, ...)
{
    LOG(LogLevel::Error, format);
}

void Logger::LogError(HRESULT hr, const char* format, ...)
{
    LOG_HR(hr, LogLevel::Error, format);
}
#pragma endregion

#pragma region LogCritical wrapper
void Logger::LogCritical(const char* format, ...)
{
    LOG(LogLevel::Critical, format);
}

void Logger::LogCritical(HRESULT hr, const char* format, ...)
{
    LOG_HR(hr, LogLevel::Critical, format);
}
#pragma endregion


// Remove macros
#undef LOG
#undef LOG_HR

#pragma endregion

#pragma endregion

#endif