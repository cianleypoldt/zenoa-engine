<h1 align="center">Zenoa Engine</h1>

<p align="center">
  2D rigid-body physics engine focused on modular design and physical stability.<br>
</p>

<p align="center" style="line-height:1.6;">
 <a href="docs/engine-architecture.md#collision-handling">Collision Handling</a> •
  <a href="docs/engine-architecture.md#physics-model">Physics Model</a> •
  <a href="docs/engine-architecture.md#design-notes">Design Notes</a> •
  <a href="#about">About</a>
</p>

<img src="media/50convex_50circle.gif" alt="Piling stability under mass disparity" width="100%" />

---

### Features

- Convex polygon and circle body support  
- Impulse-based collision and friction resolution  
- Semi-implicit Euler integration

> Note: As of [Zenoa v1](https://github.com/cianleypoldt/RigidBody-Engine/releases/tag/latest),  
> collision callbacks are not yet supported.

---

## Build Instructions

Zenoa can be built on most Unix-like systems.<br>
Before building Zenoa, make sure the following are installed on your system:

- **Clang (C++17)** – C++ compiler  
- **CMake** – build system generator  
- **SFML** – for graphics rendering and input  
- **GLM** – for vector and matrix math

### Build

```bash
git clone https://github.com/cianleypoldt/zenoa-engine.git
cd zenoa-engine
mkdir build && cd build
cmake ..
make
./Zenoa
```

### Notes

- The build creates an executable named Zenoa in the build directory.
- Usage examples are provided in the [`examples/`](examples/) directory.
- The API is exposed via the interface header: [`src/engine/interface.h`](src/engine/interface.h).

---

## About

Read about implementation details [here](docs/engine-architecture.md).

---

## License

[MIT License](LICENSE) © Cian Leypoldt
