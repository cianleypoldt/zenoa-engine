#include "engine/Simulation.h"
#include <stdio.h>

int main()
{
    Simulation::init();
    unsigned int id[100];
    for (int i = 0; i < 100; i++)
    {
        id[i] = Simulation::addRigidConvex({{0, 0}, {1, 0}, {1, 1}, {0, 1}});
    }
    Simulation::free();
}
