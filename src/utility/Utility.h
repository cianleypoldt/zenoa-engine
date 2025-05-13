#pragma once
#include <cassert>
#include <chrono>
#include <cstdarg>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>

namespace utl
{

//==============================================================================
// Simple logging system
//==============================================================================
enum class LogLevel
{
    Debug,
    Info,
    Warning,
    Error,
    None // Use to disable logging
};

// Initialize the logger - call once at program start
void initLogger(LogLevel minLevel = LogLevel::Info, const char* logFile = nullptr);

// Core logging functions
void logDebug(const char* format, ...);
void logInfo(const char* format, ...);
void logWarning(const char* format, ...);
void logError(const char* format, ...);

// Convenience macros
#ifdef NDEBUG
#define UTL_DEBUG(...)
#else
#define UTL_DEBUG(...) utl::logDebug(__VA_ARGS__)
#endif

#define UTL_INFO(...) utl::logInfo(__VA_ARGS__)
#define UTL_WARNING(...) utl::logWarning(__VA_ARGS__)
#define UTL_ERROR(...) utl::logError(__VA_ARGS__)

//==============================================================================
// Simple assertions
//==============================================================================
#ifdef NDEBUG
#define UTL_ASSERT(condition, message) ((void)0)
#else
#define UTL_ASSERT(condition, message)                                                  \
    do                                                                                  \
    {                                                                                   \
        if (!(condition))                                                               \
        {                                                                               \
            utl::logError("Assertion failed: %s (%s:%d)", message, __FILE__, __LINE__); \
            assert(condition);                                                          \
        }                                                                               \
    } while (0)
#endif

//==============================================================================
// Memory arena allocator
//==============================================================================
struct MemoryArena
{
    char* buffer;
    size_t capacity;
    size_t used;

    // Create a new memory arena with the given capacity
    static MemoryArena create(size_t capacity);

    void destroy();
    void reset();

    void* alloc(size_t size, size_t alignment = alignof(std::max_align_t));

    template <typename T>
    T* allocArray(size_t count)
    {
        return static_cast<T*>(alloc(sizeof(T) * count, alignof(T)));
    }

    template <typename T>
    T* alloc()
    {
        return static_cast<T*>(alloc(sizeof(T), alignof(T)));
    }

    bool canFit(size_t size, size_t alignment = alignof(std::max_align_t)) const;
};

//==============================================================================
// RAII Memory arena scope helper
//==============================================================================
class ScopedArena
{
  public:
    explicit ScopedArena(size_t capacity)
    {
        arena = MemoryArena::create(capacity);
    }

    ~ScopedArena()
    {
        arena.destroy();
    }

    MemoryArena& get() { return arena; }

    // Disallow copying
    ScopedArena(const ScopedArena&) = delete;
    ScopedArena& operator=(const ScopedArena&) = delete;

  private:
    MemoryArena arena;
};

//==============================================================================
// Simple profiling utilities
//==============================================================================
class ScopedTimer
{
  public:
    explicit ScopedTimer(const char* name);
    ~ScopedTimer();

  private:
    const char* name;
    std::chrono::high_resolution_clock::time_point startTime;
};

// Measure execution time of a function
template <typename Func>
float measureExecutionTime(Func&& func)
{
    auto start = std::chrono::high_resolution_clock::now();
    std::forward<Func>(func)();
    auto end = std::chrono::high_resolution_clock::now();

    return std::chrono::duration<float, std::milli>(end - start).count();
}

#ifdef ENABLE_PROFILING
#define UTL_PROFILE_SCOPE(name) utl::ScopedTimer timer##__LINE__(name)
#else
#define UTL_PROFILE_SCOPE(name) ((void)0)
#endif

//==============================================================================
// Error checking helpers
//==============================================================================
// Check if an index is within bounds
template <typename T>
bool isInBounds(T index, T size)
{
    return index >= 0 && index < size;
}

// Safe array access with bounds checking
template <typename T>
T& safeAccess(T* array, size_t index, size_t size, const char* errorMsg = "Array index out of bounds")
{
    UTL_ASSERT(index < size, errorMsg);
    return array[index];
}

} // namespace utl
