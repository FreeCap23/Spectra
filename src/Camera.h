#pragma once
#include "glm/glm.hpp"

using glm::vec3;

class Camera {
    public:
        Camera() {}
        Camera(vec3 pos) {
            m_pos = pos;
        }

        vec3 getPos();

    private:
        vec3 m_pos;
};
