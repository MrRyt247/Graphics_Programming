# OpenGL Model Viewer

An OpenGL 3.3 application that loads and renders a 3D model with dynamic point lighting. Built as part of a LearnOpenGL lab (Lab 10: Model Loading).

<hr />
<img width="802" height="625" alt="image" src="https://github.com/user-attachments/assets/0fd13f27-dbbf-44db-853d-b936b9f84ba6" />

## Features

- 3D model loading via Assimp (`.obj` and other formats)
- Two animated point lights that orbit the scene with rainbow colour cycling
- Phong shading (ambient + diffuse + specular) with attenuation
- Textured ground plane
- FPS-style camera with WASD movement and mouse look
- 800×600 window, OpenGL Core Profile 3.3

## Preview

The scene renders a backpack model centred in the world, lit by two orbiting coloured point lights visible as small cubes. A textured container-skin plane sits below the model as a ground surface.

## Project Structure

```
.
├── src/
│   ├── main.cpp              # Entry point and render loop
│   ├── glad.c                # GLAD loader source (generated)
│   ├── shaders/
│   │   ├── model.vert/.frag  # Phong shading for the model and ground
│   │   └── lighting.vert/.frag  # Unlit shader for light cube markers
│   ├── assets/
│   │   ├── backpack/         # backpack.obj + textures (not in repo)
│   │   ├── container.jpg
│   │   └── container_specular.png
│   └── utils/
│       ├── shader_m.h        # Shader compilation helper
│       ├── camera.h          # FPS camera (WASD + mouse)
│       ├── mesh.h            # Mesh class (VAO/VBO/EBO)
│       ├── model.h           # Assimp model loader
│       └── stb_image.h       # Single-header image loader
├── include/
│   ├── glad/glad.h
│   ├── KHR/khrplatform.h
│   └── glm/                  # GLM headers
├── CMakeLists.txt
└── run.sh
```

## Dependencies

| Library | Purpose | Setup |
|---------|---------|-------|
| [GLFW 3](https://www.glfw.org/docs/latest/compile.html) | Window creation and input | See below |
| [GLAD](https://glad.dav1d.de/) | OpenGL function loader | See below |
| [GLM](https://github.com/g-truc/glm) | Math (vectors, matrices) | Header-only, included in `include/glm/` |
| [Assimp](https://github.com/assimp/assimp/blob/master/Build.md) | 3D model loading | See below |
| [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h) | Texture loading | Header-only, included in `src/utils/stb_image.h` |

### Installing GLFW

```bash
git clone https://github.com/glfw/glfw.git /usr/local/lib/glfw
cd /usr/local/lib/glfw
cmake -S . -B build
cmake --build build
```

Full docs: https://www.glfw.org/docs/latest/compile.html

### Installing GLAD

1. Go to https://glad.dav1d.de/
2. Select **Language: C/C++**, **Specification: OpenGL**, **Profile: Core**, **gl: Version 3.3**
3. Click **Generate** and download the zip
4. Extract to `/usr/local/lib/glad/` so you have:
   - `/usr/local/lib/glad/include/glad/glad.h`
   - `/usr/local/lib/glad/include/KHR/khrplatform.h`
   - `/usr/local/lib/glad/src/glad.c`

### Installing Assimp

```bash
git clone https://github.com/assimp/assimp.git /usr/local/lib/assimp
cd /usr/local/lib/assimp
cmake -S . -B build -DASSIMP_BUILD_TESTS=OFF
cmake --build build
cp build/bin/libassimp.so* /usr/local/lib/assimp/bin/
```

Full docs: https://github.com/assimp/assimp/blob/master/Build.md

### stb_image

No installation needed — `stb_image.h` is already bundled in `src/utils/stb_image.h`.

Official repo: https://github.com/nothings/stb/blob/master/stb_image.h

## Build & Run

```bash
# Configure (first time only)
cmake -S .

# Build
cmake --build .

# Run (must be from project root — shaders load relative to cwd)
./app

# Or use the convenience script
./run.sh
```

After the initial `cmake -S .`, only `cmake --build .` is needed for subsequent builds.

## Controls

| Key / Input | Action |
|-------------|--------|
| `W` / `S` | Move forward / backward |
| `A` / `D` | Strafe left / right |
| Mouse move | Look around |
| Scroll wheel | Zoom (FOV) |
| `Esc` | Quit |

Mouse capture is disabled by default. To enable it, uncomment line 64 of `src/main.cpp`:
```cpp
glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
```

## Assets

The backpack model is not included in the repository. Place your `.obj` file and its textures at:

```
src/assets/backpack/backpack.obj
```

The model used is the [survival backpack](https://learnopengl.com/Model-Loading/Model) from LearnOpenGL.

## Shader Uniforms

The `Mesh::Draw` method binds textures automatically using the naming convention:

- `texture_diffuse1`, `texture_diffuse2`, …
- `texture_specular1`, …
- `texture_normal1`, …
- `texture_height1`, …

Additional uniforms (`viewPos`, `pointLights[i].*`) are set explicitly in `main.cpp` before each draw call.

## Platform

Linux (X11). The CMake build links against `X11`, `Xrandr`, `Xinerama`, `Xcursor`, `Xi`, `pthread`, and `dl`.
