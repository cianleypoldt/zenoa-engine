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
 <a href="docs/engine-architecture.md#collision-handling">Collision Handling</a> •
  <a href="docs/engine-architecture.md#physics-model">Physics Model</a> •
  <a href="docs/engine-architecture.md#design-notes">Design Notes</a> •
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

- **GLM** – Vector math
- **SFML** – Debug rendering and input

### Usage

- The API is exposed via the interface header: [`src/engine/interface.h`](src/engine/interface.h).
- Usage examples are provided in the [`examples`](examples/) directory.

---

## About

Read about implementation details [here](docs/engine-architecture.md).

Zenoa was created as an educational project at age 17. While it functions as a working simulation engine, it is not designed to compete with established physics libraries such as Box2D or Chipmunk2D.

---

## License

[MIT License](LICENSE) © Cian Leypoldt
