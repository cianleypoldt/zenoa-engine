#pragma once
#include <glm/glm.hpp>

template <uint32_t max_entity_capacity>
struct EntityIDList
{
    uint32_t count = 0;
    uint32_t id[max_entity_capacity];

    EntityIDList()
    {
        for (int i = 0; i < max_entity_capacity; i++)
            id[i] = -1;
    }

    void add(uint32_t added_id)
    {
        // check if already in list
        for (int i = 0; i < count; i++)
        {
            if (id[i] == added_id)
                return;
        }
        // assign to back, increment count
        id[count++] = added_id;
    }

    void remove(uint32_t removed_id)
    {
        // search list for corresponding id
        for (int i = 0; i < count; i++)
        {
            if (id[i] == removed_id)
            {
                // overwrite corresponding id with last item in list and shrink list by one
                id[i] = id[--count];
                id[count] = -1;
                return;
            }
        }
    }
};
