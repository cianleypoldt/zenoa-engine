#include "engine/interface.h"
#include "graphics/renderer.h"

int main() {
    // Define a convex polygon shape (pentagon-like)
    std::vector<glm::vec2> points = {
        {0, 0}, {95.0f, -69.0f}, {58.8f, -181.0f},
        {-58.8f, -181.0f}, {-95.0f, -69.0f}
    };

    auto* ctx = rbs::make_context();
    rbs::set_bounds(ctx, {-500, -250}, {500, 250});
    rbs::set_gravity(ctx, 0); // No gravity for controlled testing
    ctx->fixed_timestep = 0.07;

    Renderer renderer(ctx, /* vsync= */ false, /* scale= */ 25);
    float density = 1e-6f;

    // Create and place two convex bodies
    auto convex1 = rbs::add_entity(ctx);
    rbs::add_convex_collider(ctx, convex1, points, density);
    rbs::set_position(ctx, convex1, {-80, 150});
    renderer.add_convex(convex1);

    auto convex2 = rbs::add_entity(ctx);
    rbs::add_convex_collider(ctx, convex2, points, density);
    rbs::set_position(ctx, convex2, {100, -140});
    renderer.add_convex(convex2);

    // Create and place a circle
    auto circle = rbs::add_entity(ctx);
    rbs::add_circle_collider(ctx, circle, 50, density);
    rbs::set_position(ctx, circle, {300, 50});
    renderer.add_circle(circle);

    // Apply torque and force to test dynamics
    rbs::apply_torque(ctx, convex1, -2300);
    rbs::apply_torque(ctx, convex2, 5000);
    rbs::apply_force(ctx, circle, {-4, 0});

    // Run the simulation
    while (renderer.active()) {
        rbs::step(ctx);
        renderer.refresh();
    }

    rbs::drop(ctx);
    return 0;
}