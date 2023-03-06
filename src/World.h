#pragma once
#include "glm/glm.hpp"
#include "Entity.h"
#include <vector>
#include <memory>

using glm::vec3;

struct World {
    vec3 skyColor;
    std::vector<std::unique_ptr<Entity>> entities; 
};
