cmake_minimum_required(VERSION 3.2)
cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(Spectra)

# Set the build type to release by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Set compiler flags for release mode 
if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /W4")
else ()
    set(CMAKE_CXX_FLAGS_RELEASE "-Wall -Wextra -O3")
endif()

# Set compiler flags for debug mode 
if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "/W4 /DEBUG:FULL /Od")
else ()
    set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -Og -g")
endif()

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)
find_package(OpenGL REQUIRED)


set(SpectraSources main.cpp Camera.cpp Spectra.cpp Entity.cpp Scene.cpp Renderer.cpp)
set(ImGUISources imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_impl_glfw.cpp imgui/imgui_impl_opengl3.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp)

include_directories(glfw/include/)

# Create a subdirectory for the executable
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})

# Add the executable and set its output directory
add_executable(Spectra ${SpectraSources} ${ImGUISources})
set_target_properties(Spectra PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})

target_link_libraries(Spectra glfw)
target_link_libraries(Spectra OpenGL::GL)
