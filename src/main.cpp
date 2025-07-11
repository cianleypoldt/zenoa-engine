#include "engine/interface.h"
#include "graphics/Renderer.h"
#include <SFML/Graphics.hpp>

int main() {
    auto simulation_context = rbs::make_context();
    Renderer debug_renderer(simulation_context, false, 120);

    while (debug_renderer.active()) {
        rbs::step(simulation_context);
        debug_renderer.refresh();
    }

    rbs::drop(simulation_context);
}
