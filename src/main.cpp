#include "Renderer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

// Simple helper function to load an image into a OpenGL texture with common settings
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
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable dragging windows outside main window
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

        if (shouldRender) {
            renderer.samplesDone++;
            renderer.Render(data);
            renderedAtLeastOnce = true;
            imageLoaded = false;
        }

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
                shouldRender = true;
                renderOpts.height = tempResHeight;
                renderOpts.width = tempResWidth;
                data = new uint8_t[4 * renderOpts.width * renderOpts.height];
                memset(data, 0, 4 * renderOpts.width * renderOpts.height);
                auto cam = std::shared_ptr<Camera>();
                double aspectRatio = (double)renderOpts.width/renderOpts.height;
                dvec3 position(0, -50, 35);
                dvec3 lookAt(0, 0, 2);
                if (ortho) {
                    cam = std::make_shared<Orthographic>(4, aspectRatio, position, lookAt, dvec3(0, 0, 1));
                } else {
                    cam = std::make_shared<Perspective>(8, aspectRatio, position, lookAt, dvec3(0, 0, 1));
                }
                scene.setCamera(cam);
                renderer.resetBuffer();
                exportedFile = false;
            }
            ImGui::SameLine();
            if (ImGui::Button("Export", ImVec2(100, 25))) {
                if (renderedAtLeastOnce) {
                    Spectra::writeImage("out.png", renderOpts.width, renderOpts.height, data);
                    exportedFile = true;
                }
            }
            if (ImGui::CollapsingHeader("Camera options")) {
                ImGui::Checkbox("Orthographic", &ortho);
            }
            if (ImGui::CollapsingHeader("Object properties")) {
                ImGui::Text("Type: ");
            }
            if (exportedFile) {
                ImGui::Text("Render saved to out.png");
            }
            if (renderedAtLeastOnce)
                ImGui::Text("%d samples finished.", renderer.samplesDone);
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

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

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