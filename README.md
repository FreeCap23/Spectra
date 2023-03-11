# Spectra

## Rendered image
This is the most recent image rendered.

![Image rendered with Spectra](out.png)

## Motivation
I've been interested in raytracing for some time, so I decided to try to write one myself.

## Resources I used
- ["An Introduction to Ray Tracing" by Andrew S. Glassner](https://www.realtimerendering.com/raytracing/An-Introduction-to-Ray-Tracing-The-Morgan-Kaufmann-Series-in-Computer-Graphics-.pdf)
- [scratchapixel.com](scratchapixel.com)
- [pbrt-v3](pbrt.org)

## Building
To build Spectra, you need CMake and a C++ Compiler, such as g++ or msvc.
```bash
mkdir build
cmake -S src -B build
cmake --build build
```
