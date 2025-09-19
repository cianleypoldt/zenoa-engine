<h1 align="center">Zenoa Engine</h1>

<p align="center">
  2D rigid-body physics engine focused on performance, determinism, and physical stability.<br>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Physics-8E44AD?style=for-the-badge&logo=physics&logoColor=white" />
  <img src="https://img.shields.io/badge/C++17-1E90FF?style=for-the-badge&logo=c%2B%2B&logoColor=white" />
  <img src="https://img.shields.io/badge/SFML-34495E?style=for-the-badge&logo=sfml&logoColor=white" />
  <img src="https://img.shields.io/badge/GLM-3D9970?style=for-the-badge&logo=circle&logoColor=white" />
</p>

<p align="center" style="line-height:1.6;">
  <a href="#collision-handling">Collision Handling</a> •
  <a href="#physics-model">Physics Model</a> •
  <a href="#design-notes">Design Notes</a> •
  <a href="#about">About</a>
</p>

<img src="media/50convex_50circle.gif" alt="Piling stability under mass disparity" width="100%" />

---

### The engine supports:

- Convex polygon and circle bodies  
- Impulse-based collision and friction resolution
- Visual debugging through _SFML_

> As of now ([Zenoa v1](https://github.com/cianleypoldt/RigidBody-Engine/releases/tag/latest)), collision callbacks are not supported.

---

## Build Instructions (Unix-like)

```bash
sudo pacman -S git clang cmake make sfml glm
git clone https://github.com/cianleypoldt/SAT-Impulse-Physics.git
mkdir Zenoa/build; cd Zenoa/build
cmake ..; make
./Zenoa
```

### Dependencies

- **GLM** – Vector math and linear algebra
- **SFML** – Debug rendering and input

### Usage

- The API is exposed via the interface header: [`src/engine/interface.h`](src/engine/interface.h).
- Usage examples are provided in the [`examples`](examples/) directory.

---

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

## About

Zenoa was created as an educational project at age 17. While it functions as a working simulation engine, it is not designed to compete with established physics libraries such as Box2D or Chipmunk2D.

---

## License

[MIT License](LICENSE) © Cian Leypoldt
