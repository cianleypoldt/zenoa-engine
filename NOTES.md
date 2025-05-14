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
1. Broad Phase: Dynamic AABB Tree
2. Narrow Phase: GJK + EPA + SAT + Voxel BVH
3. Contact Manifolds: Persistent
4. Constraints: MLCP Formulation
5. Solver: Projected Gauss-Seidel (warm-started, block)
6. Position Fix: Nonlinear Gauss-Seidel (split solvers)
7. Integration: Symplectic Euler
8. CCD: Conservative Advancement
