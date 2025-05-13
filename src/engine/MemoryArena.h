#include <cstddef>
#include <cstdlib>

struct arena
{
    char* memory;
    unsigned int capacity;
    unsigned int offset;

    void* allocate(size_t size, size_t alignment = alignof(std::max_align_t))
    {
        // Calculate alignment padding needed
        size_t current_addr = reinterpret_cast<size_t>(memory + offset);
        size_t aligned_addr = (current_addr + alignment - 1) & ~(alignment - 1);
        size_t padding = aligned_addr - current_addr;

        // Check if we have enough space
        if (offset + padding + size > capacity)
        {
            return nullptr; // Not enough space
        }

        // Update offset and return pointer to allocated memory
        offset += padding + size;
        return memory + offset - size;
    }

    void reset()
    {
        offset = 0;
    }
};
