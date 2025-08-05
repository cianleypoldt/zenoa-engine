# Zenoa Engine API Reference

The Zenoa Engine provides a C++ API for 2D rigid-body physics simulation. All API functions are in the `rbs` namespace.

## Table of Contents

- [Context Management](#context-management)
- [World Configuration](#world-configuration)
- [Entity Management](#entity-management)
- [Physics Properties](#physics-properties)
- [Simulation Control](#simulation-control)
- [Data Retrieval](#data-retrieval)
- [Code Examples](#code-examples)
- [Best Practices](#best-practices)

## Context Management

### `system_context* make_context()`

Creates a new physics simulation context.

**Returns:** Pointer to the new system context

**Example:**
```cpp
auto* ctx = rbs::make_context();
```

### `void drop(system_context* cntx)`

Destroys a physics simulation context and frees memory.

**Parameters:**
- `cntx`: Pointer to the system context to destroy

**Example:**
```cpp
rbs::drop(ctx);
```

## World Configuration

### `void set_bounds(system_context* cntx, glm::vec2 bottom_left, glm::vec2 top_right)`

Sets the world boundaries. Entities will collide with these boundaries.

**Parameters:**
- `cntx`: Pointer to the system context
- `bottom_left`: Bottom-left corner of the world bounds
- `top_right`: Top-right corner of the world bounds

**Example:**
```cpp
rbs::set_bounds(ctx, {-100, -100}, {100, 100});
```

### `void set_gravity(system_context* cntx, float gravity)`

Sets the global gravity value applied to all entities.

**Parameters:**
- `cntx`: Pointer to the system context
- `gravity`: Gravity acceleration (typically negative for downward gravity)

**Example:**
```cpp
rbs::set_gravity(ctx, -9.8f);  // Standard Earth gravity
```

### `uint32_t get_max_entity_count()`

Returns the maximum number of entities supported by the engine.

**Returns:** Maximum entity count

## Entity Management

### `unsigned int add_entity(system_context* cntx, glm::vec2 pos = {0, 0}, float mass = 1)`

Creates a new entity in the simulation.

**Parameters:**
- `cntx`: Pointer to the system context
- `pos`: Initial position (default: {0, 0})
- `mass`: Initial mass (default: 1.0). Set to 0 for static/immovable objects

**Returns:** Unique entity ID

**Example:**
```cpp
auto circle_id = rbs::add_entity(ctx, {0, 50});          // Dynamic object
auto ground_id = rbs::add_entity(ctx, {0, -30}, 0);     // Static object
```

### `void kill_entity(system_context* cntx, uint32_t id)`

Removes an entity from the simulation.

**Parameters:**
- `cntx`: Pointer to the system context
- `id`: Entity ID to remove

**Example:**
```cpp
rbs::kill_entity(ctx, entity_id);
```

## Collider Setup

### `void add_circle_collider(system_context* cntx, unsigned int id, float radius, float density)`

Adds a circular collider to an entity.

**Parameters:**
- `cntx`: Pointer to the system context
- `id`: Entity ID
- `radius`: Circle radius
- `density`: Material density (affects mass calculation)

**Note:** This function automatically calculates and sets the entity's mass and inertia based on the circle's area and density.

**Example:**
```cpp
rbs::add_circle_collider(ctx, circle_id, 10.0f, 1.0f);
```

### `void add_convex_collider(system_context* cntx, unsigned int id, std::vector<glm::vec2> points, float density)`

Adds a convex polygon collider to an entity.

**Parameters:**
- `cntx`: Pointer to the system context
- `id`: Entity ID
- `points`: Vertices of the convex polygon (will be centered around centroid)
- `density`: Material density (affects mass calculation)

**Note:** 
- The polygon vertices are automatically translated so the centroid becomes the origin
- Mass and inertia are calculated based on the polygon area and density
- Vertices should be provided in a consistent winding order

**Example:**
```cpp
std::vector<glm::vec2> box_points = {
    {-25, -5}, {25, -5}, {25, 5}, {-25, 5}
};
rbs::add_convex_collider(ctx, box_id, box_points, 1.0f);
```

## Physics Properties

### `void set_position(system_context* cntx, uint32_t id, glm::vec2 pos)`

Sets the position of an entity.

**Parameters:**
- `cntx`: Pointer to the system context
- `id`: Entity ID
- `pos`: New position

**Example:**
```cpp
rbs::set_position(ctx, entity_id, {50, 100});
```

### `void set_velocity(system_context* cntx, uint32_t id, glm::vec2 vel)`

Sets the linear velocity of an entity.

**Parameters:**
- `cntx`: Pointer to the system context
- `id`: Entity ID
- `vel`: New velocity vector

**Example:**
```cpp
rbs::set_velocity(ctx, entity_id, {10, 0});  // Moving right
```

### `void apply_force(system_context* cntx, uint32_t id, glm::vec2 force)`

Applies a force to an entity (accumulative).

**Parameters:**
- `cntx`: Pointer to the system context
- `id`: Entity ID
- `force`: Force vector to apply

**Note:** Forces accumulate until the next physics step, then are automatically reset.

**Example:**
```cpp
rbs::apply_force(ctx, entity_id, {100, 0});  // Push right
```

### `void apply_torque(system_context* cntx, uint32_t id, float torque)`

Applies rotational torque to an entity (accumulative).

**Parameters:**
- `cntx`: Pointer to the system context
- `id`: Entity ID
- `torque`: Torque value to apply

**Note:** Torques accumulate until the next physics step, then are automatically reset.

**Example:**
```cpp
rbs::apply_torque(ctx, entity_id, 500);  // Rotate counterclockwise
```

## Simulation Control

### `void step(system_context* cntx)`

Advances the physics simulation by one timestep.

**Parameters:**
- `cntx`: Pointer to the system context

**Note:** The timestep is controlled by `cntx->fixed_timestep` (default: 100, which is very large - typically you want 1/60 = 0.0167 for 60 FPS).

**Example:**
```cpp
ctx->fixed_timestep = 1.0 / 60.0;  // 60 FPS
while (running) {
    rbs::step(ctx);
    // ... render and handle input
}
```

## Data Retrieval

### `glm::vec2 get_position(system_context* cntx, uint32_t id)`

Gets the current position of an entity.

**Parameters:**
- `cntx`: Pointer to the system context
- `id`: Entity ID

**Returns:** Position vector, or {0, 0} if entity doesn't exist

**Example:**
```cpp
glm::vec2 pos = rbs::get_position(ctx, entity_id);
```

### `float get_rotation(system_context* cntx, uint32_t id)`

Gets the current rotation of an entity in radians.

**Parameters:**
- `cntx`: Pointer to the system context
- `id`: Entity ID

**Returns:** Rotation in radians, or -1 if entity doesn't exist

**Example:**
```cpp
float rotation = rbs::get_rotation(ctx, entity_id);
```

### `float get_circle_radius(system_context* cntx, uint32_t id)`

Gets the radius of a circle collider.

**Parameters:**
- `cntx`: Pointer to the system context
- `id`: Entity ID

**Returns:** Circle radius, or -1 if entity doesn't exist or isn't a circle

**Example:**
```cpp
float radius = rbs::get_circle_radius(ctx, circle_id);
```

### `std::vector<glm::vec2> get_convex_verticies(system_context* cntx, uint32_t id)`

Gets the vertices of a convex polygon collider in local space.

**Parameters:**
- `cntx`: Pointer to the system context
- `id`: Entity ID

**Returns:** Vector of vertices, or empty vector if entity doesn't exist or isn't a convex polygon

**Example:**
```cpp
auto vertices = rbs::get_convex_verticies(ctx, polygon_id);
```

## Code Examples

### Basic Simulation

```cpp
#include "engine/interface.h"

int main() {
    // Create simulation context
    auto* ctx = rbs::make_context();
    rbs::set_bounds(ctx, {-100, -100}, {100, 100});
    rbs::set_gravity(ctx, -9.8f);
    ctx->fixed_timestep = 1.0 / 60.0;  // 60 FPS

    // Create a falling circle
    auto circle = rbs::add_entity(ctx, {0, 50});
    rbs::add_circle_collider(ctx, circle, 10.0f, 1.0f);

    // Create static ground
    auto ground = rbs::add_entity(ctx, {0, -30}, 0);  // mass = 0 for static
    rbs::add_convex_collider(ctx, ground, {
        {-50, -5}, {50, -5}, {50, 5}, {-50, 5}
    }, 1.0f);

    // Simulation loop
    for (int i = 0; i < 1000; ++i) {
        rbs::step(ctx);
        
        glm::vec2 pos = rbs::get_position(ctx, circle);
        printf("Circle position: %.2f, %.2f\n", pos.x, pos.y);
    }

    rbs::drop(ctx);
    return 0;
}
```

### Forces and Torques

```cpp
// Apply continuous force (like thrust)
for (int i = 0; i < 60; ++i) {  // Apply for 1 second at 60 FPS
    rbs::apply_force(ctx, entity_id, {50, 0});  // Thrust right
    rbs::apply_torque(ctx, entity_id, 10);      // Spin
    rbs::step(ctx);
}
```

### Multiple Shapes

```cpp
// Create various shapes
auto circle = rbs::add_entity(ctx, {-20, 20});
rbs::add_circle_collider(ctx, circle, 8.0f, 1.0f);

auto triangle = rbs::add_entity(ctx, {0, 20});
rbs::add_convex_collider(ctx, triangle, {
    {0, 10}, {-8, -5}, {8, -5}
}, 1.0f);

auto pentagon = rbs::add_entity(ctx, {20, 20});
rbs::add_convex_collider(ctx, pentagon, {
    {0, 0}, {95, -69}, {58, -181}, {-58, -181}, {-95, -69}
}, 1.0f);
```

## Best Practices

### Entity Management
- Always check entity IDs when storing them long-term, as entities can be killed
- Use mass = 0 for static objects (ground, walls, platforms)
- Keep entity counts reasonable for performance

### Timestep Configuration
- Use fixed timesteps for deterministic simulation
- Typical values: 1/60 (60 FPS), 1/120 (120 FPS)
- Smaller timesteps = more accurate but slower simulation

### Collider Design
- Keep convex polygons simple for better performance
- Ensure polygon vertices are in consistent winding order
- Use appropriate density values (typically small, like 0.001-1.0)

### Force Application
- Apply forces every frame for continuous effects
- Use impulses (single frame forces) for instantaneous effects
- Forces and torques accumulate, so apply them once per desired effect

### Memory Management
- Always call `rbs::drop()` to clean up contexts
- The engine manages entity memory automatically
- Vertex pools grow automatically but don't shrink

### Performance Tips
- Limit the number of entities based on your performance requirements
- Use static entities (mass = 0) for non-moving objects
- Consider the complexity of convex shapes in high-entity-count scenarios