# Raw Ray Tracer

A CPU-based ray tracer written in C++ featuring realistic lighting, materials, and depth of field effects.

![Ray Traced Scene](images/output.png)

## Features

- **Physically Based Rendering**: Accurate light transport simulation
- **Multiple Materials**: 
  - Lambertian (diffuse) surfaces
  - Metallic surfaces with fuzzy reflections
  - Dielectric (glass) materials with refraction
- **Depth of Field**: Camera with adjustable aperture and focus distance
- **Anti-aliasing**: Multi-sample per-pixel rendering
- **Stochastic Scene Generation**: Randomly generated scenes with hundreds of objects

## Scene Details

- **100+ objects** including spheres with various materials
- **Realistic lighting** with sky gradient background
- **Depth of field** effects for cinematic rendering
- **Gamma correction** for accurate color representation

## Quick Start

### Prerequisites
- C++ compiler (GCC, Clang, or MSVC)

### Building
```bash
# Clone the repository
git clone https://github.com/yourusername/RawRayTracer.git
cd RawRayTracer

# Build the project
mkdir build && cd build
cmake ..
make

# Or compile directly
g++ -O3 -std=c++11 src/*.cpp -o raytracer
