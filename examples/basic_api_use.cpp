#include "engine/interface.h"
#include "graphics/renderer.h"

int main() {
    auto* ctx = rbs::make_context();
    rbs::set_bounds(ctx, {-100, -100}, {100, 100});
    rbs::set_gravity(ctx, -9.8f);
    ctx->fixed_timestep = 1.0 / 60.0;

    Renderer renderer(ctx, /* fullscreen= */ true, /* FPS= */ 60);

    auto circle = rbs::add_entity(ctx, {0, 50});
    rbs::add_circle_collider(ctx, circle, 10.0f, 1.0f);
    renderer.add_circle(circle);

    auto ground = rbs::add_entity(ctx, {0, -30}, /* mass= */ 0);
    rbs::add_convex_collider(ctx, ground, {
        {-50, -5}, {50, -5}, {50, 5}, {-50, 5}
    }, 1.0f);
    renderer.add_convex(ground);

    while (renderer.active()) {
        rbs::step(ctx);
        renderer.refresh();
    }

    rbs::drop(ctx);
    return 0;
}