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

## Build Instructions (Linux)

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

Collisions between convex polygons are detected using the **Separating Axis Theorem (SAT)**. Each shape is projected onto axes defined by the **normals of its edges**. If a separating axis exists—i.e., an axis where the projections don’t overlap—the shapes are not colliding. Otherwise, the axis with the smallest overlap is used to compute the **minimum translation vector (MTV)** for resolving the collision.

For a visual overview, see the [Wikipedia article on SAT](https://en.wikipedia.org/wiki/Hyperplane_separation_theorem#Separating_axis_theorem).

If a collision is detected, **face clipping** is used to find precise **contact points**. This involves selecting a reference and incident face, and clipping the incident face against the reference’s side planes.

The resulting contact data, along with **penetration depth**, is used to compute **collision response impulses**, resolving interpenetration while applying realistic forces. Impulse resolution accounts for **mass, restitution**, and **friction**.

### Circle-Polygon Collisions

Collisions between circles and polygons use a **face projection method**, projecting the circle center onto polygon edges to identify contact. This avoids excessive branching while maintaining consistent behavior across shape types.

<img src="media/convex_circle_impulse.gif" alt="Convex + circle impulse and friction resolution" width="100%" />

### Walls and Borders

Static boundaries are handled with a **deepest-point resolution** strategy. This ensures stable contact, prevents tunneling, and keeps objects confined to the expected play area.

---

## Physics Model

### Impulse Resolution

Collisions are resolved using a basic impulse solver. Linear and angular velocities are updated using the relative velocity at the contact point and the combined inverse mass and inertia of the two bodies.

**Impulse formula:**

$$
J = \frac{-(1 + e) \cdot (\vec{v}_{rel} \cdot \vec{n})}{\frac{1}{m_A} + \frac{1}{m_B} + \frac{(\vec{r}_A \times \vec{n})^2}{I_A} + \frac{(\vec{r}_B \times \vec{n})^2}{I_B}}
$$

**Where:**

- $e$ is the restitution coefficient
- $\vec{v}_{rel}$ is the relative velocity at the contact
- $\vec{n}$ is the contact normal
- $m$, $I$ are the mass and moment of inertia

### Friction

Basic Coulomb friction is supported. Tangential impulses are clamped relative to the normal impulse, allowing simple sliding and resting contact behavior.

---

## Design Notes

- Uses **Structure of Arrays (SoA)** for better cache behavior during updates.
- Simulation runs on a **fixed timestep** for consistency and repeatability.
- Engine **state** is stored in a **self-contained context**, allowing integration into other applications or environments.
- Custom `entity_list` structure keeps all IDs consistent through deletions and avoids large memory moves or resizes.

---

## About

Zenoa is a physics engine project developed at age 17 with a focus on performance, determinism, and low-level system design—built both as a functional engine and as a deeper study in real-time simulation.

---

## License

[MIT License](LICENSE) © Cian Leypoldt
