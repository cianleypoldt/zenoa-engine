# Zenoa Engine (C++)

Zenoa is a real-time 2D rigid-body physics engine built in modern C++17. It was developed as an educational project at age 17, with a focus on clarity, determinism, and stability under physical constraints.

The engine supports convex polygon and circle bodies, impulse-based collision resolution, and visual debugging through SFML.

---

## Contents

- [Collision Handling](#collision-handling)
- [Physics Model](#physics-model)
- [Design Notes](#design-notes)
- [Visual Demos](#demonstration)
- [Build Instructions](#build-instructions-(linux))
- [Dependencies](#dependencies)

---

## Collision Handling

### Convex Shapes

Collision detection between convex polygons uses the **Separating Axis Theorem (SAT)**. Contact points are computed using face clipping, and penetration depth is used to generate response impulses.

### Circle–Polygon Interactions

Circles are handled with a **face projection method**, allowing smooth resolution against polygon edges without excessive branching or shape-specific logic.

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
- The API is exposed via the interface header: [`src/engine/interface.h`](src/engine/interface.h).
- Usage examples are provided in the [`examples`](examples/) directory.

---

## Demonstration

### Convex + Circle Impulse and Friction Resolution

Demonstrates contact resolution, restitution, and friction between convex shapes and circles.

<img src="media/convex_circle_impulse.gif" alt="Convex + circle impulse and friction resolution" width="100%" />

View <a href="examples/collision_demo.cpp" class="code-link">source code</a>

---

### Mass Disparity Stability Test

Tests stability under differences in body mass and geometry.

<img src="media/50convex_50circle.gif" alt="Piling stability under mass disparity" width="100%" />

---

## Build Instructions (Linux)

```bash
sudo pacman -S git clang cmake make sfml glm
git clone https://github.com/cianleypoldt/SAT-Impulse-Physics.git
mkdir Zenoa/build; cd Zenoa/build
cmake ..; make
./Zenoa
```

---

## Dependencies

- **GLM** – Vector math and linear algebra
- **SFML** – Debug rendering and input

---

## License

Zenoa is released under the MIT License.
