#include <cstdlib>

struct arena
{
    char* memory;
    unsigned int capacity;
    unsigned int offset;
};
