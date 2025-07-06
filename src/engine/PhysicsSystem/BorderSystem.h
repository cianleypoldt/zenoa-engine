#pragma once
#include <glm/glm.hpp>

class SystemContext;
class BorderSystem {
  public:
    float border_elasticity = 1;
    float border_friction = 1;
    glm::vec2 bottom_left_corner = {-250, 500};
    glm::vec2 top_right_corner = {250, 500};

    void apply(SystemContext* cntx);

    void sphereBorderCollision(SystemContext* cntx, uint32_t id);

    void convexBorderCollision(SystemContext* cntx, uint32_t id);
};
