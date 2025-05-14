#include "engine/interface.h"

int main()
{
    EntityIDList<10> list;

    for (uint32_t i = 0; i < 10; i++)
        list.add(10 * i);
    list.remove(10);
    list.remove(10);
    list.add(15);
    for (int i = 0; i < 10; i++)
    {
        printf("list item %i = %i\n", i, list.id[i]);
    }
}
