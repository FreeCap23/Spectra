# Spectra

## Rendered image
This is the most recent image rendered.

![Image rendered with Spectra](out.png)

## Features
- Stochastic Multi-Sampling anti-aliasing
- Materials

## Motivation
I've been interested in raytracing for some time, so I decided to try to write one myself.

## Resources I used
- ["An Introduction to Ray Tracing" by Andrew S. Glassner](https://www.realtimerendering.com/raytracing/An-Introduction-to-Ray-Tracing-The-Morgan-Kaufmann-Series-in-Computer-Graphics-.pdf)
- [scratchapixel.com](https://www.scratchapixel.com)
- [pbrt-v3](https://www.pbrt.org)
- [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html)

## Building
To build Spectra, you need CMake 3.2+ and a C++ Compiler, such as g++ or MSVC.
```bash
mkdir build
cmake -S src -B build
cmake --build build
```
