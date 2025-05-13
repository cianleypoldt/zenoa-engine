#include "Utility.h"
#include <cstdio>
#include <ctime>
#include <iostream>

namespace utl
{

//==============================================================================
// Logger implementation
//==============================================================================
static LogLevel g_logLevel = LogLevel::Info;
static std::ofstream g_logFile;

void initLogger(LogLevel minLevel, const char* logFile)
{
    g_logLevel = minLevel;
}
static std::string getCurrentTimestamp()
{
    char buffer[64];
    time_t now = time(nullptr);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return buffer;
}
// Internal function to get level name
static const char* getLevelName(LogLevel level)
{
    switch (level)
    {
    case LogLevel::Debug:
        return "DEBUG";
    case LogLevel::Info:
        return "INFO";
    case LogLevel::Warning:
        return "WARNING";
    case LogLevel::Error:
        return "ERROR";
    default:
        return "UNKNOWN";
    }
}
static void log(LogLevel level, const char* format, va_list args)
{
    if (level < g_logLevel)
        return;

    char message[1024];
    vsnprintf(message, sizeof(message), format, args);

    std::string timestamp = getCurrentTimestamp();
    std::string output = timestamp + " [" + getLevelName(level) + "] " + message;

    std::cout << output << std::endl;
}

void logDebug(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    log(LogLevel::Debug, format, args);
    va_end(args);
}

void logInfo(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    log(LogLevel::Info, format, args);
    va_end(args);
}

void logWarning(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    log(LogLevel::Warning, format, args);
    va_end(args);
}

void logError(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    log(LogLevel::Error, format, args);
    va_end(args);
}

//==============================================================================
// Memory arena implementation
//==============================================================================
MemoryArena MemoryArena::create(size_t capacity)
{
    MemoryArena arena;
    arena.buffer = static_cast<char*>(malloc(capacity));
    arena.capacity = capacity;
    arena.used = 0;

    if (!arena.buffer)
    {
        UTL_ERROR("Failed to allocate memory arena of size %zu", capacity);
        arena.capacity = 0;
    }

    return arena;
}

void MemoryArena::destroy()
{
    free(buffer);
    buffer = nullptr;
    capacity = 0;
    used = 0;
}

void MemoryArena::reset()
{
    used = 0;
}

void* MemoryArena::alloc(size_t size, size_t alignment)
{
    if (!buffer)
        return nullptr;

    // Calculate aligned address
    size_t current = reinterpret_cast<size_t>(buffer + used);
    size_t aligned = (current + alignment - 1) & ~(alignment - 1);
    size_t padding = aligned - current;

    // Check if there's enough space
    if (used + padding + size > capacity)
    {
        UTL_ERROR("Memory arena out of space: %zu requested, %zu available",
                  size, capacity - used);
        return nullptr;
    }

    // Update used amount and return pointer
    used += padding + size;
    return buffer + aligned - reinterpret_cast<size_t>(buffer);
}

bool MemoryArena::canFit(size_t size, size_t alignment) const
{
    size_t current = reinterpret_cast<size_t>(buffer + used);
    size_t aligned = (current + alignment - 1) & ~(alignment - 1);
    size_t padding = aligned - current;

    return (used + padding + size <= capacity);
}

//==============================================================================
// Profiling implementation
//==============================================================================
ScopedTimer::ScopedTimer(const char* name)
    : name(name)
{
    startTime = std::chrono::high_resolution_clock::now();
}

ScopedTimer::~ScopedTimer()
{
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<float, std::milli>(endTime - startTime).count();
    UTL_DEBUG("%s: %.3f ms", name, duration);
}

} // namespace utl
