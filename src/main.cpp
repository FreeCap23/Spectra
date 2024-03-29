#include "Renderer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/type_ptr.hpp" // for glm::value_ptr()

// Forward declarations

// Error callback function
static void glfw_error_callback(int error, const char* description);

// Simple helper function to load an image into a OpenGL texture with common settings
bool LoadTextureFromData(const uint8_t* data, GLuint* out_texture, int image_width, int image_height);

// Function that initializes the scene
void createScene(Scene& scene);

int main() {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Spectra", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Find the DPI of the screen to scale the font size with
    float xscale, yscale;
    glfwGetMonitorContentScale(glfwGetPrimaryMonitor(), &xscale, &yscale);
    float fontScale = ceilf(16 * xscale);

    // Add font
    // This assumes the directory you launch the app from is build/release/ or build/debug/
    io.Fonts->AddFontFromFileTTF("../../src/res/OpenSans-Regular.ttf", fontScale);

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigDockingWithShift = false;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    options renderOpts;
    renderOpts.samples = 50;
    renderOpts.maxDepth = 50;
    // Need to define these before using them
    renderOpts.height = 1;
    renderOpts.width = 1;
    int renderScale = 100;
    ImVec2 viewport(800, 600);
    bool shouldRender = false;
    bool renderedAtLeastOnce = false;
    bool imageLoaded = false;
    bool exportedFile = false;
    bool ortho = false;
    GLuint imageTexture = 0;
    dvec3 cameraPos(0, -50, 35);
    dvec3 cameraLookAt(0, 3, 0);
    float fov = 8.2;
    std::shared_ptr<Camera> camera;
    
    Scene scene;
    createScene(scene);
    Renderer renderer(scene, renderOpts);
    uint8_t* data = (uint8_t*)malloc(0);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        // Every frame, checks if it needs to render a new sample
        if (shouldRender) {
            renderer.samplesDone++;
            renderer.Render(data);
            renderedAtLeastOnce = true;
            imageLoaded = false;
        }

        // Render result window
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::Begin("Render result");
            viewport = ImGui::GetContentRegionAvail();
            if (renderedAtLeastOnce) {
                if (!imageLoaded) {
                    imageTexture = 0;
                    LoadTextureFromData(data, &imageTexture, renderOpts.width, renderOpts.height);
                    imageLoaded = true;
                }
                ImGui::Image((void*)(intptr_t)imageTexture, viewport);
            }
            ImGui::End();
            ImGui::PopStyleVar();
        }

        // Render options window
        {
            ImGui::Begin("Render options");

            // Temporary values for the render resolution
            int tempResWidth, tempResHeight;
            tempResHeight = static_cast<int>((renderScale / 100.0) * viewport.y);
            tempResWidth = static_cast<int>((renderScale / 100.0) * viewport.x);

            ImGui::SliderInt("Render scale", &renderScale, 1, 100, "%d%%");
            ImGui::Text("%dx%d", tempResWidth, tempResHeight);
            ImGui::InputInt("Max Depth", &renderOpts.maxDepth, 5, 100, 0);

            if (ImGui::Button("Render", ImVec2(100, 25))) {
                // Initialize rendering parameters
                shouldRender = true;
                renderOpts.height = tempResHeight;
                renderOpts.width = tempResWidth;

                // Initialize data buffer
                data = new uint8_t[4 * renderOpts.width * renderOpts.height];
                memset(data, 0, 4 * renderOpts.width * renderOpts.height);

                // Initialize camera
                double aspectRatio = (double)renderOpts.width/renderOpts.height;
                if (ortho) {
                    camera = std::make_shared<Orthographic>(fov, aspectRatio, cameraPos, cameraLookAt, dvec3(0, 0, 1));
                } else {
                    camera = std::make_shared<Perspective>(fov, aspectRatio, cameraPos, cameraLookAt, dvec3(0, 0, 1));
                }
                scene.setCamera(camera);

                // Reset previous render buffer
                renderer.resetBuffer();

                // Stop displaying "Render saved to ..." message
                exportedFile = false;
            }
            ImGui::SameLine();
            if (ImGui::Button("Export", ImVec2(100, 25)))
                if (renderedAtLeastOnce) {
                    Spectra::writeImage("out.png", renderOpts.width, renderOpts.height, data);
                    exportedFile = true;
                }

            if (ImGui::CollapsingHeader("Camera options")) {
                if (ImGui::Checkbox("Orthographic", &ortho)) {
                    double aspectRatio = (double)renderOpts.width / renderOpts.height;
                    if (ortho) {
                        camera = std::make_shared<Orthographic>(fov, aspectRatio, cameraPos, cameraLookAt, dvec3(0, 0, 1));
                    } else {
                        camera = std::make_shared<Perspective>(fov, aspectRatio, cameraPos, cameraLookAt, dvec3(0, 0, 1));
                    }
                    scene.setCamera(camera);
                    renderer.resetBuffer();
                }
                if (
                    ImGui::DragFloat("FOV", &fov, 0.2, 1, 180) || 
                    ImGui::InputDouble("X Position", &cameraPos.x, 0.1, 1) ||
                    ImGui::InputDouble("Y Position", &cameraPos.y, 0.1, 1) ||
                    ImGui::InputDouble("Z Position", &cameraPos.z, 0.1, 1) ||
                    ImGui::InputDouble("X Look At", &cameraLookAt.x, 0.1, 1) ||
                    ImGui::InputDouble("Y Look At", &cameraLookAt.y, 0.1, 1) ||
                    ImGui::InputDouble("Z Look At", &cameraLookAt.z, 0.1, 1) )
                {
                    renderer.resetBuffer(); // reset the image buffer to not get ghosting
                }
            }

            if (ImGui::CollapsingHeader("Object properties")) {
                static int idx = 0, selMat;
                const char* matList[] = {"Lambertian", "Metal", "Dielectric"};
                std::shared_ptr<Material> mat;
                ImGui::InputInt("Index", &idx, 1, 1);
                if (idx >= scene.getEntityCount()) {
                    idx = scene.getEntityCount() - 1;
                } else if (idx < 0) {
                    idx = 0;
                } 
                mat = scene.getEntityAtIdx(idx)->getMat();
                selMat = mat->getType();

                // ImGui::Combo("Material", &selMat, matList, 3, 3);

                if (ImGui::Combo("Material", &selMat, matList, 3, 3)) {
                    dvec3 color = mat->getColor();
                    double rough = mat->getRoughness();
                    double ior = mat->getIor();
                    if (selMat == 0) { // Lambertian
                        if (color.r == 1)
                            color = dvec3(0);
                        mat = std::make_shared<Lambertian>(color);
                    } else if (selMat == 1) { // Metal
                        if (color.r == 1)
                            color = dvec3(0);
                        if (rough == -1)
                            rough = 0;
                        mat = std::make_shared<Metal>(color, rough);
                    } else if (selMat == 2) { // Dielectric
                        if (ior == -1)
                            ior = 2;
                        mat = std::make_shared<Dielectric>(ior);
                    }
                    scene.getEntityAtIdx(idx)->setMat(mat);
                }

                switch (selMat) {
                    case 0: { // Lambertian
                        float color[3];
                        dvec3 vecColor = mat->getColor();
                        color[0] = static_cast<float>(vecColor.r);
                        color[1] = static_cast<float>(vecColor.g);
                        color[2] = static_cast<float>(vecColor.b);
                        if (ImGui::ColorPicker3("Material color", &color[0])) {
                            mat = std::make_shared<Lambertian>(dvec3(color[0], color[1], color[2]));
                            scene.getEntityAtIdx(idx)->setMat(mat);
                            renderer.resetBuffer();
                        }
                        break;
                    }
                    case 1: { // Metal
                        float color[3];
                        dvec3 vecColor = mat->getColor();
                        color[0] = static_cast<float>(vecColor.r);
                        color[1] = static_cast<float>(vecColor.g);
                        color[2] = static_cast<float>(vecColor.b);
                        float rough = static_cast<float>(mat->getRoughness());
                        if (
                            ImGui::ColorPicker3("Material color", &color[0]) || 
                            ImGui::SliderFloat("Roughness", &rough, 0, 1)
                        ) {
                            mat = std::make_shared<Metal>(dvec3(color[0], color[1], color[2]), rough);
                            scene.getEntityAtIdx(idx)->setMat(mat);
                            renderer.resetBuffer();
                        }
                        break;
                    }
                    case 2: { // Dielectric
                        float ior = static_cast<float>(mat->getIor());
                        if (ImGui::SliderFloat("Index of refraction", &ior, 0, 4)) {
                            mat = std::make_shared<Dielectric>(ior);
                            scene.getEntityAtIdx(idx)->setMat(mat);
                            renderer.resetBuffer();
                        }
                        break;
                    }
                    default: {
                        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Error selecting material");
                    }
                }
            }

            if (exportedFile) 
                ImGui::Text("Render saved to out.png");

            if (renderedAtLeastOnce)
                ImGui::Text("%d samples finished.", renderer.samplesDone);

            if (shouldRender)
                if (ImGui::Button("Stop"))
                    shouldRender = false;

            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

bool LoadTextureFromData(const uint8_t* data, GLuint* out_texture, int image_width, int image_height) {
    if (data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    // Delete the texture, if previously created, to stop a memory leak
    glDeleteTextures(1, &image_texture);
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); // Same

    // Upload pixels into texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    *out_texture = image_texture;
    return true;
}

void createScene(Scene& scene) {
    auto matGround = std::make_shared<Lambertian>(dvec3(0.2, 0.8, 0.5));
    auto matMirror = std::make_shared<Metal>(dvec3(1, 1, 1), 0.02);

    scene.addPlane(matGround, dvec3(0, 0, 1), 0);
    scene.addPlane(matMirror, dvec3(0, -1, 0), 3);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++) {
            double picker = Spectra::randomDouble();
            std::shared_ptr<Material> mat;
            if (picker < 0.33) {
                dvec3 color = Spectra::randomVec();
                mat = std::make_shared<Lambertian>(color);
            } else if (picker < 0.66) {
                dvec3 color = Spectra::randomVec();
                double roughness = Spectra::randomDouble();
                mat = std::make_shared<Metal>(color, roughness);
            } else {
                double ior = Spectra::randomDouble();
                mat = std::make_shared<Dielectric>(ior);
            }
            dvec3 position((j-3.5) * 1.25, (i-1.5) * 1.25, 0.5);
            scene.addSphere(mat, position, 0.5);
        }
    }
}