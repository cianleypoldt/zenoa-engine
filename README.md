<h1 align="center">Zenoa Engine</h1>

<p align="center">
  2D rigid-body physics engine focused on modular design and physical stability.<br>
</p>


<p align="center" style="line-height:1.6;">
 <a href="docs/implementation-details.md#collision-handling">Collision Handling</a> •
  <a href="docs/implementation-details.md#physics-model">Physics Model</a> •
  <a href="docs/implementation-details.md#design-notes">Design Notes</a>
</p>

<img src="media/50convex_50circle.gif" alt="Piling stability demo" width="100%" />

---

## Features

- Convex polygon and circle body support  
- Impulse-based collision and Coulomb friction resolution
- Semi-implicit Euler integration

---

## Build Instructions

Zenoa runs on most Unix-like systems.<br>
Before building, make sure the following are installed on your system:

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

### Documentation

- Usage examples are provided in the [`examples/`](examples/) directory.
- The API is exposed via the interface header: [`src/engine/interface.h`](src/engine/interface.h).

Learn about implementation at [`docs/implementation-details.md`](docs/implementation-details.md).

---

## Project Status
Built in 2025 at age 17 as a learning project in physics simulation and C++ architecture.  
The repository is archived and not under active development.


---

[MIT](LICENSE) © Cian Leypoldt
