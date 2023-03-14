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
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    *out_texture = image_texture;
    return true;
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
    int samplesDone = 0;
    int samples = 0;
    bool shouldRender = false;
    bool renderedAtLeastOnce = false;
    
    Renderer renderer;
    uint8_t* data = new uint8_t[10];

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin("Render result");
            viewport = ImGui::GetContentRegionAvail();
            if (renderedAtLeastOnce) {
                GLuint imageTexture = 0;
                LoadTextureFromData(data, &imageTexture, renderOpts.width, renderOpts.height);
                ImGui::Image((void*)(intptr_t)imageTexture, viewport);
            }
            ImGui::End();
        }
        {
            ImGui::Begin("Render options");

            // Temporary values for the render resolution
            int tempResWidth, tempResHeight;
            tempResHeight = (renderScale / 100.0) * viewport.y;
            tempResWidth = (renderScale / 100.0) * viewport.x;
            ImGui::SliderInt("Render scale", &renderScale, 1, 100, "%d%%");
            ImGui::Text("%dx%d", tempResWidth, tempResHeight);
            ImGui::InputInt("Samples", &renderOpts.samples, 5, 100, 0);
            ImGui::InputInt("Max Depth", &renderOpts.maxDepth, 5, 100, 0);
            if (ImGui::Button("Render", ImVec2(100, 25))) {
                shouldRender = true;
                samples = renderOpts.samples;
                renderOpts.height = tempResHeight;
                renderOpts.width = tempResWidth;
                data = new uint8_t[3 * renderOpts.width * renderOpts.height];
                samplesDone = 0;
                memset(data, 0, 3 * renderOpts.width * renderOpts.height);
                renderer.Initialize(renderOpts, data);
            }
            ImGui::SameLine();
            if (ImGui::Button("Export", ImVec2(100, 25))) {

            }
            if (renderedAtLeastOnce)
                ImGui::Text("%d / %d samples finished.", samplesDone, samples);
            ImGui::End();
        }
        if (shouldRender && samplesDone < samples) {
            samplesDone++;
            renderer.Render(data);
            renderedAtLeastOnce = true;
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