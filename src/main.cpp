#include "Camera.h"
#include "Ray.h"
#include <cstdio>

#define WIDTH 400
#define HEIGHT 200

vec3 getColor(Ray& ray) {
    return vec3(255, 0, 0);
}

// TODO: Add argument for file name
int main() {
    // Setup output file
    FILE* outFile = fopen("out.ppm", "w");
    fprintf(outFile, "P3\n%d %d\n255\n", WIDTH, HEIGHT);

    // Setup camera
    Camera camera(vec3(0, 0, 0));

    // Setup ray
    Ray ray;
    ray.origin = camera.getPos();
    ray.direction.z = 1;

    // Render loop
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            // Map pixel coordinates to ray direction
            ray.direction.y = i / (HEIGHT / 2.0) - 1.0;
            ray.direction.x = j / (WIDTH / 2.0) - 1.0;

            // Get color for this pixel
            vec3 color = getColor(ray);

            // Write the color to the output file
            fprintf(outFile, "%d %d %d ", (int)color.x, (int)color.y, (int)color.z);
        }
        // Write newline at the end of the row
        fprintf(outFile, "\n");
    }

    return 0;
}
