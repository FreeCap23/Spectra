#pragma once
#include <cstdio>

class Camera {
    public:
        Camera() {}
        Camera(int width, int height, const char* fileName) {
            m_width = width;
            m_height = height;
            m_fileName = fileName;
        }

        void render();

    private:
        int m_width, m_height;
        const char* m_fileName;
};
