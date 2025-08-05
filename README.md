<h1 align="center">Zenoa Engine</h1>

<p align="center">
  2D rigid-body physics engine focused on performance, determinism, and physical stability
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
  <a href="#design-notes">Design Notes</a>
</p>

<img src="media/50convex_50circle.gif" alt="Piling stability under mass disparity" width="100%" />

---

### The engine supports:

- Convex polygon and circle bodies  
- Performant impulse-based collision resolution
- Visual debugging through _SFML_

---

## Quick Start

### Linux (Ubuntu/Debian)
```bash
sudo apt install build-essential cmake libsfml-dev libglm-dev
git clone https://github.com/cianleypoldt/RigidBody-Engine.git
cd RigidBody-Engine
mkdir build && cd build
cmake .. && make
./Zenoa
```

### Linux (Arch/Manjaro)
```bash
sudo pacman -S git clang cmake make sfml glm
git clone https://github.com/cianleypoldt/RigidBody-Engine.git
cd RigidBody-Engine
mkdir build && cd build
cmake .. && make
./Zenoa
```

### Other Platforms
For Windows, macOS, and detailed build instructions, see **[BUILD.md](BUILD.md)**.

### Dependencies

- **GLM** – Vector math and linear algebra
- **SFML 2.6+** – Graphics rendering and window management
- **CMake 3.15+** – Build system
- **C++17** compatible compiler

### API Documentation

- **Complete API Reference:** [`API_REFERENCE.md`](API_REFERENCE.md)
- **Build System Guide:** [`BUILD.md`](BUILD.md)  
- **Interface Header:** [`src/engine/interface.h`](src/engine/interface.h)
- **Usage Examples:** [`examples/`](examples/) directory

---

## Collision Handling

### Convex Shapes

Collision detection between convex polygons uses the **Separating Axis Theorem (SAT)**. Contact points are computed using face clipping, and penetration depth is used to generate response impulses.

### Circle–Polygon Interactions

Circles are handled with a **face projection method**, allowing smooth resolution against polygon edges without excessive branching or shape-specific logic.

<img src="media/convex_circle_impulse.gif" alt="Convex + circle impulse and friction resolution" width="100%" />

### Wall and Border Contacts

A simple deepest-point resolution is used to prevent tunneling and maintain expected object boundaries in confined scenes.

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

---

## License

Zenoa is released under the MIT License.
