# C++ 2D Game Engine

A lightweight 2D game engine built from scratch in C++ using SDL3, developed as a personal project to deepen my understanding of low-level systems programming, game architecture, and real-time rendering.

## Demo

**Pong** — built entirely using this engine as a proof-of-concept to validate core systems.

![Pong Demo](assets/demo.gif)

---

## Features

- **Rendering** — SDL3-based 2D rendering pipeline for drawing shapes each frame
- **Collision Detection** — AABB (Axis-Aligned Bounding Box) collision detection between game objects
- **Input Handling** — Keyboard input system for real-time player controls
- **Game Loop** — Fixed-timestep game loop managing update and render cycles

---

## Getting Started

### Prerequisites

- Windows 10 or later
- [CMake](https://cmake.org/download/) 3.15+
- [SDL3](https://github.com/libsdl-org/SDL/releases) development libraries
- A [C++17](https://en.cppreference.com/w/cpp/17) compatible compiler (MSVC via Visual Studio, or MinGW-w64)

### Building

1. **Clone the repository**
   ```bash
   git clone https://github.com/lawunder/my-engine.git
   cd my-engine
   ```

2. **Configure with CMake**
   ```bash
   mkdir build
   cd build
   cmake ..
   ```

3. **Build**
   ```bash
   cmake --build .
   ```

4. **Run**
   ```bash
   ./Pong.exe
   ```

> **Note:** Make sure SDL3 is installed and its path is correctly set in your environment or `CMakeLists.txt` before building.

---

## Project Structure

```
my-engine/
├── src/              # Engine and game source files
├── assets/           # Font and other assets
└── CMakeLists.txt    # Build configuration
```

---

## Roadmap

- Audio system (SDL3 Mixer)
- Entity Component System (ECS) architecture
- Cross-platform support (Mac/Linux)

---

## Built With

- [C++17](https://en.cppreference.com/w/cpp/17)
- [SDL3](https://www.libsdl.org/)
- [CMake](https://cmake.org/)

---

## Author

**Lucas Wunderlich**
[github.com/lawunder](https://github.com/lawunder) · [linkedin.com/in/lucaswunderlich](https://www.linkedin.com/in/lucaswunderlich)