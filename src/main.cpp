#include "engine/Simulation.h"
#include <stdio.h>

int main()
{
    auto simulation = Simulation::make_context();
    unsigned int id[100];
    for (int i = 0; i < 100; i++)
    {
        id[i] = Simulation::addEntity(simulation, {{0, 0}, {1, 0}, {1, 1}, {0, 1}});
    }
    Simulation::drop(simulation);
}
