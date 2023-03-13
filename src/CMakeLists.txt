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

add_executable(Spectra main.cpp Camera.cpp Spectra.cpp Entity.cpp Scene.cpp)
