#include "engine/interface.h"
#include "graphics/Renderer.h"

#include <cstdlib>
#include <vector>

static float random(float min, float max) {
    return min + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (max - min);
}

constexpr int convex_count = 50;
constexpr int circle_count = 50;

// Demo scene 2 spinning convex + 1 circle:

#if 0
int main() {
    std::vector<glm::vec2> points = {
        { 0,     0      },
        { 95.0,  -69.0  },
        { 58.8,  -181.0 },
        { -58.8, -181.0 },
        { -95.0, -69.0  }
    };
    float default_density    = 0.000001;
    auto  simulation_context = rbs::make_context();
    rbs::set_bounds(simulation_context, { -500, -250 }, { 500, 250 });
    rbs::set_gravity(simulation_context, 0);

    simulation_context->fixed_timestep = 0.07;

    Renderer renderer(simulation_context, false, 25);

    auto convex_id  = rbs::add_entity(simulation_context);
    auto convex2_id = rbs::add_entity(simulation_context);
    auto circle_id  = rbs::add_entity(simulation_context);

    rbs::add_convex_collider(simulation_context, convex_id, points, default_density);
    rbs::set_position(simulation_context, convex_id, { -80, 150 });
    renderer.add_convex(convex_id);
    rbs::add_convex_collider(simulation_context, convex2_id, points, default_density);
    rbs::set_position(simulation_context, convex2_id, { 100, -140 });
    renderer.add_convex(convex2_id);

    rbs::add_circle_collider(simulation_context, circle_id, 50, default_density);
    rbs::set_position(simulation_context, circle_id, { 300, 50 });
    renderer.add_circle(circle_id);

    rbs::apply_torque(simulation_context, convex_id, -2300);
    rbs::apply_torque(simulation_context, convex2_id, 5000);

    rbs::apply_force(simulation_context, circle_id, { -4, 0 });

    while (renderer.active()) {
        rbs::step(simulation_context);
        renderer.refresh();
    }

    rbs::drop(simulation_context);
    return 0;
}
#endif
