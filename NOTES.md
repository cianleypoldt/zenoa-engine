## C++/SFML 2D rigitbody solver

```C++
int main()
{
    Simulation::initialize();
    Simulation::useDebugView(true);

    auto convex = utl::convex(
        {0,0}, {323, 3}, {3, 4}, {0,0}
    );

    Simulation::addConvex(convex);
}

```
