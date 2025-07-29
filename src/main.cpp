#include "engine/interface.h"
#include "graphics/Renderer.h"

int main() {
    auto * simulation_context = rbs::make_context();

    rbs::set_gravity(simulation_context, 0);
    rbs::set_bounds(simulation_context, { -500, -250 }, { 500, 250 });

    Renderer debug_renderer(simulation_context, false, 60);

    while (debug_renderer.active()) {
        rbs::step(simulation_context);
        debug_renderer.refresh();
    }

    rbs::drop(simulation_context);
}
