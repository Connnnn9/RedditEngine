#pragma once

// Include all the external libraries
#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <fstream>
#include <filesystem>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <optional>
#include <sstream> // For std::ostringstream
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <ImGui/Inc/imgui.h>
#include <imgui/Inc/imgui-SFML.h>
#include <windows.h>

// Namespace for constants
namespace KREngine 
{
    constexpr int SCREEN_WIDTH = 800;
    constexpr int SCREEN_HEIGHT = 600;
}

// Utility macros
#define SAFE_DELETE(p) { if (p) { delete (p); (p) = nullptr; } }
#define SAFE_RELEASE(p) { if (p) { (p)->Release(); (p) = nullptr; } }

// Logging function
inline void Log(const std::string& message) {
    std::cout << "[KREngine Log]: " << message << std::endl;
}

// Macro for logging with file and line information
#define LOG(format, ...)                                                       \
    do {                                                                       \
        std::ostringstream oss;                                                \
        oss << "[LOG] (" << __FILE__ << ":" << __LINE__ << ") ";                \
        oss << KREngine::FormatLog(format, ##__VA_ARGS__) << std::endl;         \
        std::cout << oss.str();                                                \
    } while (0)

#ifdef _DEBUG
#define DEBUG_LOG(format, ...) LOG(format, ##__VA_ARGS__)
#else
#define DEBUG_LOG(format, ...)
#endif

// Helper function to format the log message
namespace KREngine {
    inline std::string FormatLog(const std::string& format) {
        return format; // Simple pass-through for single argument
    }

    template <typename... Args>
    inline std::string FormatLog(const std::string& format, Args... args) {
        size_t size = snprintf(nullptr, 0, format.c_str(), args...) + 1;
        if (size <= 0) return "Error formatting log.";
        std::unique_ptr<char[]> buffer(new char[size]);
        snprintf(buffer.get(), size, format.c_str(), args...);
        return std::string(buffer.get(), buffer.get() + size - 1);
    }

    // String utilities
    inline std::string ToLower(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }

    inline std::string ToUpper(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), ::toupper);
        return result;
    }

    // Log redirection
    inline void SetLogFile(const std::string& filename) {
        static std::ofstream logFile(filename, std::ios::app);
        if (logFile.is_open()) {
            std::cout.rdbuf(logFile.rdbuf()); // Redirect std::cout to file
        }
    }
}
