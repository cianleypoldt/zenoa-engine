#pragma once
#include <cstdint>

template <uint32_t MaxEntityCapacity>
struct entity_id_list {
    uint32_t count = 0;
    uint32_t id[MaxEntityCapacity];

    entity_id_list() {
        for (uint32_t i = 0; i < MaxEntityCapacity; i++) {
            id[i] = -1;
}
    }

    void add(uint32_t added_id) {
        // check if already in list
        for (uint32_t i = 0; i < count; i++) {
            if (id[i] == added_id) {
                return;
}
        }
        // assign to back, increment count
        id[count++] = added_id;
    }

    void remove(uint32_t removed_id) {
        if (count == 0) { return;
}

        uint32_t index = 0;
        while (index < count) {
            if (id[index++] == removed_id) { break;
}
        }
        if (index == count) { return;
}
        while (index < count) {
            id[index - 1] = id[index];
            index++;
        }
        id[--count] = -1;
    }
};
