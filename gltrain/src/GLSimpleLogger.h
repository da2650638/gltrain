#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <format>
#include <chrono>
#include <iomanip>
#include <sstream>
#ifdef _WIN32
#include <windows.h>
#endif

class GLSimpleLogger {
public:
    enum class LogLevel
    {
        TRACE,
        DEBUG,
        INFO,
        WARNING,
        ERR,
        FATAL
    };

    static GLSimpleLogger& GetInstance()
    {
        static GLSimpleLogger logger;
        return logger;
    }

    template <typename... Args>
    void Log(LogLevel level, std::string_view formatStr, Args&&... args)
    {
        // 使用 std::vformat 动态格式化字符串
        std::string message = std::vformat(formatStr, std::make_format_args(args...));
#ifdef _WIN32
        SetConsoleColor(level);
        std::cout << GetCurrentTime() << FormatPrefix(level) << message << std::endl;
        ResetConsoleColor();
#else
        std::cout << GetColor(level) << GetCurrentTime() << FormatPrefix(level) << message << ResetColor() << std::endl;
#endif

        if (level == LogLevel::FATAL)
        {
            std::exit(EXIT_FAILURE);
        }
    }

    // 各个日志级别的快捷方法
    template<typename... Args>
    void Trace(std::string_view formatStr, Args&&... args)
    {
        Log(LogLevel::TRACE, formatStr, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void Debug(std::string_view formatStr, Args&&... args)
    {
        Log(LogLevel::DEBUG, formatStr, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void Info(std::string_view formatStr, Args&&... args)
    {
        Log(LogLevel::INFO, formatStr, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void Warning(std::string_view formatStr, Args&&... args)
    {
        Log(LogLevel::WARNING, formatStr, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void Error(std::string_view formatStr, Args&&... args)
    {
        Log(LogLevel::ERR, formatStr, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void Fatal(std::string_view formatStr, Args&&... args)
    {
        Log(LogLevel::FATAL, formatStr, std::forward<Args>(args)...);
    }

private:
    GLSimpleLogger() = default; // 私有化构造函数

    // 获取当前时间的格式化字符串
    std::string GetCurrentTime()
    {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);

        struct tm timeInfo;
#ifdef _WIN32
        localtime_s(&timeInfo, &in_time_t); // 使用安全版本localtime_s
#else
        localtime_r(&in_time_t, &timeInfo); // 使用线程安全的localtime_r (POSIX)
#endif

        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

        std::ostringstream oss;
        oss << "[" << std::put_time(&timeInfo, "%Y-%m-%d %H:%M:%S")
            << '.' << std::setw(3) << std::setfill('0') << ms.count() << "] ";
        return oss.str();
    }

    // 根据日志级别返回前缀字符串
    std::string FormatPrefix(LogLevel level)
    {
        switch (level)
        {
        case LogLevel::TRACE: return "[TRACE] ";
        case LogLevel::DEBUG: return "[DEBUG] ";
        case LogLevel::INFO: return "[INFO] ";
        case LogLevel::WARNING: return "[WARNING] ";
        case LogLevel::ERR: return "[ERROR] ";
        case LogLevel::FATAL: return "[FATAL] ";
        default: return "[UNKNOWN] ";
        }
    }

#ifdef _WIN32
    // Windows平台：使用SetConsoleTextAttribute设置控制台颜色
    void SetConsoleColor(LogLevel level)
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        switch (level)
        {
        case LogLevel::TRACE:
            SetConsoleTextAttribute(hConsole, 7); // 白色
            break;
        case LogLevel::DEBUG:
            SetConsoleTextAttribute(hConsole, 11); // 青色
            break;
        case LogLevel::INFO:
            SetConsoleTextAttribute(hConsole, 10); // 绿色
            break;
        case LogLevel::WARNING:
            SetConsoleTextAttribute(hConsole, 14); // 黄色
            break;
        case LogLevel::ERR:
            SetConsoleTextAttribute(hConsole, 12); // 红色
            break;
        case LogLevel::FATAL:
            SetConsoleTextAttribute(hConsole, 79); // 红色背景黑色字体
            break;
        default:
            SetConsoleTextAttribute(hConsole, 7); // 默认白色
        }
    }

    void ResetConsoleColor()
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 7); // 重置为白色
    }
#else
    // Linux/Unix平台：使用ANSI转义码
    std::string GetColor(LogLevel level)
    {
        switch (level)
        {
        case LogLevel::TRACE: return "\033[37m";        // 白色
        case LogLevel::DEBUG: return "\033[36m";        // 青色
        case LogLevel::INFO: return "\033[32m";         // 绿色
        case LogLevel::WARNING: return "\033[33m";      // 黄色
        case LogLevel::ERR: return "\033[31m";          // 红色
        case LogLevel::FATAL: return "\033[41;30m";     // 红色背景黑色字体
        default: return "\033[0m";                      // 默认颜色
        }
    }

    // 重置颜色
    std::string ResetColor()
    {
        return "\033[0m";
    }
#endif
};