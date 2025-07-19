# Zenoa Engine (C++)

A deterministic, real-time 2D rigid-body physics engine built in modern C++ as an educational project at the age of 17.

---

## Geometrical Models

- **Separating Axis Theorem** (SAT) for convex collision detection
- **Face projection** method for circle-polygon collisions
- Simple deepest-point penetration impulse resolution for border collisions

## Physical Models

- **Coulomb friction**
- **Impulse-based solver** incorporating mass and inertia using the following equation:

  $$
  \vec{J} = - (1 + e) \frac{\vec{v}_{\text{rel}} \cdot \hat{n}}{
      \frac{1}{m_1} + \frac{1}{m_2} +
      \hat{n} \cdot \left[
          (\mathbf{I}_1^{-1} (\vec{r}_1 \times \hat{n}) ) \times \vec{r}_1 +
          (\mathbf{I}_2^{-1} (\vec{r}_2 \times \hat{n}) ) \times \vec{r}_2
      \right]
  } \hat{n}
  $$


## Architecture

- Custom **SoA (Structure of Arrays)** reduces cache misses
- Reusable, abstracted interface layer with user owned context
- Designed for **deterministic simulation** and compatibility with future physics-based reinforcement learning environments

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

## License

Zenoa engine is released under the MIT License.
