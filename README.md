# 2D Physics Engine (C++)

A deterministic, real-time 2D rigid-body physics engine built from scratch in modern C++.
Supports convex polygon and circle-shape collisions, accurate impulse resolution, and friction (static and dynamic).

---

# Geometrical Models

- **Separating Axis Theorem** (SAT) for convex collision detection
- **Face projection** for circle-polygon collision detection
- Simple deepest-point check for border collisions

# Physical Models

- Deterministic fixed time step
- **Coulomb friction**
- **Impulse-based solver** incorporating mass and inertia using the following equation:

$$
J = \frac{-(1 + e) \cdot V_{rel} \cdot n}
{\frac{1}{m_A} + \frac{1}{m_B} + \frac{(r_A \times n)^2}{I_A} + \frac{(r_B \times n)^2}{I_B}}
$$

---

## Visual Demos

**Convex + circle impulse and friction resolution**

![convexcircle](media/convex_circle_impulse.gif)

**Real-time piling stability despite mass disparities**

![Piling stability under mass disparity](media/50convex_50circle.gif)

---

## Build (Linux only)

```sh
sudo apt install libsfml-dev
git clone https://github.com/your-username/physics-engine.git
cd physics-engine
make
./bin/physics_demo
```

---

## Dependencies

- **GLM** for vector math
- **SFML** for simple debug rendering
