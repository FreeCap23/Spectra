#pragma once
#include "glm/glm.hpp"

using glm::vec3;

class Camera {
 public:
    Camera() {}
    explicit Camera(vec3 pos) : position(pos) {}

    vec3 getPos();

 private:
    vec3 position;
};
