#include "Camera.h"

#define WIDTH 400
#define HEIGHT 200

// TODO: Add argument for file name
int main() {

    Camera camera(WIDTH, HEIGHT, "out.ppm");

    camera.render();

    return 0;
}
