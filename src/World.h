#pragma once
#include <vector>
#include <memory>
#include "glm/glm.hpp"
#include "Entity.h"

using glm::vec3;

struct World {
    vec3 skyColor;
    std::vector<std::unique_ptr<Entity>> entities;
};
