## Collision Handling

### Convex Polygons

Collisions between convex polygons are determined using the Separating Axis Theorem (SAT). The process involves projecting the vertices of both polygons onto axes aligned with the normals of their edges. If any axis reveals no overlap between the projections, a separating axis exists and the polygons are not in contact. If all projections overlap, the axis with the smallest overlap is used for face clipping, a technique where one polygon’s edge is “clipped” against the other’s edges to trim away non-overlapping parts. This produces the exact region of contact, allowing a precise contact point to be identified.

For a more concrete explaination on SAT, see the [Wikipedia article](https://en.wikipedia.org/wiki/Hyperplane_separation_theorem#Separating_axis_theorem).

### Circle-Polygon Collisions

Collisions between circles and polygons are detected by projecting the circle’s center onto the two closest edges of the polygon and comparing the resulting distances with the circle’s radius to determine whether a contact point exists.

<img src="media/convex_circle_impulse.gif" alt="Convex + circle impulse and friction resolution" width="100%" />

### Walls and Borders

Static boundaries are resolved using the deepest-point method, ensuring that when two polygon vertices contact a wall during the same simulation step, only the deepest contact point receives an impulse.

---

## Physics Model

### Impulse Resolution

Zenoa resolves collisions using an **impulse-based solver** that adjusts linear and angular velocities in proportion to the objects’ inverse mass, moment of inertia, and the relative velocity along the contact normal.

**Impulse formula:**

<p align="center">
  <img src="media/impulse_formula_latex.png" width="300">
</p>

**Where:**
- $e$: coefficient of restitution
- $\vec{v}_{rel}$: relative velocity at the contact point
- $\vec{n}$: collision normal
- $m$: mass of the body
- $I$: moment of inertia
- $\vec{r}$: vector from center of mass to contact point

This resulting impulse $j$ is used to compute the linear and angular forces applied.

### Friction

The engine applies the **Coulomb friction model**, where the friction impulse is calculated from the friction coefficient and the relative tangential velocity at the contact point, then limited by the magnitude of the normal impulse.

---

## Design Notes

- Zenoa implements a **Structure of Arrays (SoA)** layout to improve cache efficiency during simulation steps,
- runs on a **fixed timestep**,
- stores the entire engine state in a single context object, making integration into other systems straightforward and self-contained, and
- uses a custom `entity_list` structure to maintain stable IDs across deletions and avoid costly memory reallocations or shuffling.

---
