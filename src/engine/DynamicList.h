#include <glm/glm.hpp>

template <uint32_t max_entity_count>
class DynamicList
{
    uint32_t count;
    uint32_t id[max_entity_count];
};
