#include "engine/interface.h"
#include "graphics/renderer.h"

int main() {
    auto* simulation_context = rbs::make_context();

    Renderer debug_renderer(simulation_context, false, 120);

    while (debug_renderer.active()) {
        rbs::step(simulation_context);
        debug_renderer.refresh();
    }

    rbs::drop(simulation_context);
}
