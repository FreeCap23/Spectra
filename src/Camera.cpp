#include "Camera.h"

void Camera::render() {
    FILE* outFile = fopen(m_fileName, "w");
    fprintf(outFile, "P3\n%d %d\n255\n", m_width, m_height);
    for (int i = 0; i < m_height; i++) {
        for (int j = 0; j < m_width; j++) {
            fprintf(outFile, "255 0 0 ");
        }
        fprintf(outFile, "\n");
    }
}
