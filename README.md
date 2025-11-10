# Raw Ray Tracer

A CPU-based ray tracer written in C++ featuring realistic lighting, materials, and depth of field effects.

Sample renders:

<img width="800" height="400" alt="output" src="https://github.com/user-attachments/assets/aeddfe4d-78c9-402e-8496-eae6ee24d08e" />

<img width="800" height="400" alt="output" src="https://github.com/user-attachments/assets/0068a10e-30c6-4c6a-a0c8-d3226b381e53" />

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

## How to build/run program
- Run build.bat file
- Then run run.exe
- The output image will be in images folder

### Prerequisites
- C++ compiler (GCC, Clang, or MSVC)
