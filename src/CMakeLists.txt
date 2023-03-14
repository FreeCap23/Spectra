cmake_minimum_required(VERSION 3.2)
cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(Spectra)

if ( MSVC )
    message("Detected MSVC.")
    add_compile_options(/Wall /DEBUG:FULL)
else ()
    add_compile_options(-Wall -Wextra -Og -g)

endif ()

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)
find_package(OpenGL REQUIRED)


set(SpectraSources main.cpp Camera.cpp Spectra.cpp Entity.cpp Scene.cpp Renderer.cpp)
set(ImGUISources imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_impl_glfw.cpp imgui/imgui_impl_opengl3.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp)

include_directories(glfw/include/)
add_executable(Spectra ${SpectraSources} ${ImGUISources})

target_link_libraries(Spectra glfw)
target_link_libraries(Spectra OpenGL::GL)
