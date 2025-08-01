# Zenoa Engine (C++)

Zenoa is a real-time 2D rigid-body physics engine built in modern C++17. It was developed as an educational project at age 17, with a focus on clarity, determinism, and stable simulation under simple physical assumptions.

The engine supports convex polygon and circle bodies, impulse-based collision resolution, and visual debugging through SFML. While compact in scope, it demonstrates working solutions to core physics simulation problems like friction, stacking, and energy conservation.

---

## Contents

- [Collision Handling](#collision-handling)  
- [Physics Model](#physics-model)  
- [Design Notes](#design-notes)  
- [Demos](#demos)  
- [Build Instructions](#build-instructions-(linux))  
- [Dependencies](#dependencies)

---

## Collision Handling

### Convex Shapes

Collision detection between convex polygons uses the **Separating Axis Theorem (SAT)**. Contact points are computed using face clipping and penetration depth is used to generate response impulses.

### Circle–Polygon Interactions

Circles are handled with a **face projection method**, allowing smooth resolution against polygon edges without excessive branching or shape-specific logic.

### Wall and Border Contacts

Simple deepest-point resolution is used to prevent tunneling and maintain expected object boundaries in confined scenes.

---

## Physics Model

### Impulse Resolution

Collisions are resolved using a basic impulse solver. Linear and angular velocities are updated using the relative velocity at the contact point and the combined inverse mass and inertia of the two bodies.

Impulse formula:

$$
J =
\frac{
-(1 + e) \cdot (\vec{v}_{rel} \cdot \vec{n})
}{
\frac{1}{m_A} + \frac{1}{m_B} +
\frac{(\vec{r}_A \times \vec{n})^2}{I_A} +
\frac{(\vec{r}_B \times \vec{n})^2}{I_B}
}
$$

Where:
	-	e is the restitution coefficient
	-	\vec{v}_{rel} is the relative velocity at the contact
	-	\vec{n} is the contact normal
	-	m, I are the mass and moment of inertia

### Friction

Basic Coulomb friction is supported. Tangential impulses are clamped relative to the normal impulse, allowing simple sliding and resting contact behavior.

---

## Design Notes

- Uses **Structure of Arrays (SoA)** for better cache behavior during updates.
-	Simulation runs on a fixed timestep for consistency and repeatability.
-	Built to be easy to understand and extend—most components are separated cleanly and avoid unnecessary abstraction.
-	Engine state is stored in a self-contained context, allowing integration into other applications or environments.

---

## Demos

### Convex + Circle Impulse and Friction Resolution

A visual test of contact generation and resolution between convex shapes and circles. Demonstrates working restitution and friction.

![Convex + circle impulse and friction resolution](media/convex_circle_impulse.gif)

---

### Mass Disparity Stability Test

Stress test demonstrating object stacking behavior under significant differences in body mass and shape.

![Piling stability under mass disparity](media/50convex_50circle.gif)

---

## Build Instructions (Linux)

``` bash
sudo pacman -S git clang cmake make sfml glm
git clone https://github.com/cianleypoldt/SAT-Impulse-Physics.git
mkdir Zenoa/build; cd Zenoa/build
cmake ..; make
./Zenoa
```

---

## Dependencies

 - GLM – Vector math and linear algebra
 -	SFML – Debug rendering and input

---

## License

Zenoa is released under the MIT License.