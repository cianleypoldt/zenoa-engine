#include "engine/interface.h"
#include "graphics/Renderer.h"
#include "utility/Utility.h"
#include <SFML/Graphics.hpp>

float random(float min, float max) {
    return min + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (max - min);
}

constexpr int convex_count = 50;
constexpr int circle_count = 50;
int main() {

    std::vector<glm::vec2> points = {{0, 0}, {47.5, -34.5}, {29.4, -90.5}, {-29.4, -90.5}, {-47.5, -34.5}};
    float default_density = 0.000001;
    auto simulation_context = rbs::make_context();
    rbs::setBounds(simulation_context, {-500, -250}, {500, 250});
    rbs::setGravity(simulation_context, -9.81);

    simulation_context->fixed_timestep = 0.005;

    Renderer renderer(simulation_context, false);

    auto convex_a_id = rbs::addEntity(simulation_context);
    auto convex_b_id = rbs::addEntity(simulation_context);

    rbs::addConvexCollider(simulation_context, convex_a_id, points, default_density);
    rbs::setPosition(simulation_context, convex_a_id, {0, 0});
    renderer.addConvex(convex_a_id);

    rbs::addConvexCollider(simulation_context, convex_b_id, points, default_density);
    rbs::setPosition(simulation_context, convex_b_id, {20, 70});
    renderer.addConvex(convex_b_id);

    while (true) {
        rbs::step(simulation_context);
        renderer.refresh();
    }

    rbs::drop(simulation_context);
    return 0;
}
