# Zenoa Engine Build System Documentation

This document provides comprehensive information about building the Zenoa Engine on different platforms, configuring the build system, and understanding the build process.

## Table of Contents

- [Quick Start](#quick-start)
- [Dependencies](#dependencies)
- [Platform-Specific Instructions](#platform-specific-instructions)
- [CMake Configuration](#cmake-configuration)
- [Build Scripts](#build-scripts)
- [Advanced Build Options](#advanced-build-options)
- [Troubleshooting](#troubleshooting)

## Quick Start

### Prerequisites
- C++17 compatible compiler (GCC 8+, Clang 7+, MSVC 2019+)
- CMake 3.15 or later
- SFML 3.0+
- GLM (OpenGL Mathematics library)

### Basic Build Process
```bash
# Clone the repository
git clone https://github.com/cianleypoldt/RigidBody-Engine.git
cd RigidBody-Engine

# Create and enter build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build the project
cmake --build .

# Run the executable
./Zenoa  # Linux/macOS
# or
Zenoa.exe  # Windows
```

## Dependencies

### Required Libraries

#### SFML 2.6+ or 3.0+
**Purpose:** Graphics rendering, window management, and input handling for examples and visualization.

**Components used:**
- Graphics - 2D rendering (automatically includes Window and System dependencies)

**Note:** The engine supports both SFML 2.x and 3.x. CMake will automatically detect and use the available version.

#### GLM (OpenGL Mathematics)
**Purpose:** Vector mathematics and linear algebra operations.

**Usage:** All vector operations, transformations, and mathematical calculations in the physics engine.

### Optional Tools
- **clang-tidy** - Static code analysis (used by `scripts/clang-tidy.sh`)
- **Python 3** - For build utilities like `scripts/flatten.py`

## Platform-Specific Instructions

### Linux (Ubuntu/Debian)

#### Install Dependencies
```bash
# Ubuntu 22.04+/Debian 12+
sudo apt update
sudo apt install build-essential cmake git libsfml-dev libglm-dev

# For older versions, you may need to install SFML 3 from source
```

#### Build Commands
```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
./Zenoa
```

### Linux (Arch/Manjaro)

#### Install Dependencies
```bash
sudo pacman -S git clang cmake make sfml glm
```

#### Build Commands
```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
./Zenoa
```

### Linux (CentOS/RHEL/Fedora)

#### Install Dependencies
```bash
# Fedora
sudo dnf install cmake gcc-c++ make SFML-devel glm-devel

# CentOS/RHEL (may require EPEL or building SFML from source)
sudo yum install cmake gcc-c++ make
# Install SFML and GLM manually
```

### macOS

#### Install Dependencies with Homebrew
```bash
# Install Homebrew if not already installed
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install cmake sfml glm
```

#### Install Dependencies with MacPorts
```bash
sudo port install cmake sfml glm
```

#### Build Commands
```bash
mkdir build && cd build
cmake ..
make -j$(sysctl -n hw.ncpu)
./Zenoa
```

### Windows

#### Option 1: Visual Studio with vcpkg

1. **Install Visual Studio 2019 or later** with C++ support
2. **Install vcpkg:**
   ```cmd
   git clone https://github.com/Microsoft/vcpkg.git
   cd vcpkg
   .\bootstrap-vcpkg.bat
   .\vcpkg integrate install
   ```
3. **Install dependencies:**
   ```cmd
   .\vcpkg install sfml:x64-windows glm:x64-windows
   ```
4. **Build with CMake:**
   ```cmd
   mkdir build && cd build
   cmake .. -DCMAKE_TOOLCHAIN_FILE=C:\path\to\vcpkg\scripts\buildsystems\vcpkg.cmake
   cmake --build . --config Release
   ```

#### Option 2: MSYS2/MinGW

1. **Install MSYS2** from https://www.msys2.org/
2. **Install dependencies:**
   ```bash
   pacman -S mingw-w64-x86_64-cmake mingw-w64-x86_64-gcc mingw-w64-x86_64-sfml mingw-w64-x86_64-glm
   ```
3. **Build:**
   ```bash
   mkdir build && cd build
   cmake .. -G "MinGW Makefiles"
   mingw32-make
   ```

#### Option 3: Manual Installation

1. Download and install CMake from https://cmake.org/
2. Download SFML 3.0+ binaries for your compiler
3. Download GLM headers
4. Set environment variables or use CMake flags to point to libraries

## CMake Configuration

### Basic Configuration Options

The project uses standard CMake configuration with several customizable options:

```bash
# Basic configuration
cmake ..

# Specify build type
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake .. -DCMAKE_BUILD_TYPE=Debug

# Specify custom install prefix
cmake .. -DCMAKE_INSTALL_PREFIX=/custom/path

# Use specific compiler
cmake .. -DCMAKE_CXX_COMPILER=clang++
```

### Available Build Types

- **Debug** (default if not specified): Includes debug symbols, no optimization
- **Release**: Full optimization, no debug symbols
- **RelWithDebInfo**: Optimization with debug symbols
- **MinSizeRel**: Optimize for size

### Compiler Flags

The CMakeLists.txt sets the following flags:

#### Standard Flags
```cmake
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
target_compile_options(Zenoa PRIVATE -Wall -Wextra -pedantic)
```

#### Release-Specific Flags
```cmake
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -flto -DNDEBUG")
```

- `-O3`: Maximum optimization
- `-march=native`: Optimize for the current CPU architecture
- `-flto`: Link-time optimization
- `-DNDEBUG`: Disable debug assertions

### Finding Dependencies

The CMake configuration automatically finds dependencies:

```cmake
find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System Audio)
find_package(glm REQUIRED)
```

If dependencies are not found automatically, you can specify paths:

```bash
# Custom SFML path
cmake .. -DSFML_DIR=/path/to/sfml/lib/cmake/SFML

# Custom GLM path  
cmake .. -Dglm_DIR=/path/to/glm/lib/cmake/glm
```

## Build Scripts

The `scripts/` directory contains several utility scripts:

### `scripts/build.sh`

Automated build script for Linux/macOS:

```bash
#!/bin/bash
mkdir build
cd build
cmake ..
if [ $? -ne 0 ]; then
    echo "cmake failed"
    exit 1
fi

make
if [ $? -ne 0 ]; then
    echo "make failed"
    exit 1
fi

echo "Build succeeded"
./Zenoa
```

**Usage:**
```bash
chmod +x scripts/build.sh
./scripts/build.sh
```

### `scripts/clang-tidy.sh`

Runs clang-tidy static analysis on all source files:

```bash
#!/bin/bash
for file in $(find src -name '*.cpp' -o -name '*.h'); do
    clang-tidy "$file" -p build -fix
done
```

**Usage:**
```bash
chmod +x scripts/clang-tidy.sh
./scripts/clang-tidy.sh
```

**Prerequisites:**
- clang-tidy must be installed
- Project must be built first (to generate `compile_commands.json`)

### `scripts/flatten.py`

Python utility to flatten source code into a single file:

**Usage:**
```bash
python3 scripts/flatten.py
# Output: scripts/Zenoa_flattened.txt
```

## Advanced Build Options

### Custom CMake Targets

You can create custom targets for different purposes:

#### Examples Only
```cmake
# Add this to CMakeLists.txt to build examples separately
add_executable(basic_example examples/basic_api_use.cpp ${SOURCES})
target_link_libraries(basic_example PRIVATE SFML::Graphics SFML::Window SFML::System glm::glm)
```

#### Library Only
```cmake
# Build as a library instead of executable
add_library(zenoa_engine ${SOURCES})
target_link_libraries(zenoa_engine PUBLIC SFML::Graphics SFML::Window SFML::System glm::glm)
```

### Cross-Compilation

#### For Windows from Linux
```bash
# Install MinGW cross-compiler
sudo apt install mingw-w64

# Configure CMake for cross-compilation
cmake .. \
    -DCMAKE_TOOLCHAIN_FILE=/path/to/mingw-toolchain.cmake \
    -DCMAKE_SYSTEM_NAME=Windows
```

### Packaging

#### Create DEB Package (Debian/Ubuntu)
```bash
# Install packaging tools
sudo apt install checkinstall

# Build and create package
cmake .. -DCMAKE_INSTALL_PREFIX=/usr
make
sudo checkinstall make install
```

#### Create RPM Package (RHEL/CentOS/Fedora)
```bash
# Create spec file and use rpmbuild
# (Requires creating appropriate .spec file)
```

## Troubleshooting

### Common Issues

#### SFML Not Found
```
Could NOT find SFML (missing: SFML_GRAPHICS_LIBRARY SFML_WINDOW_LIBRARY)
```

**Solutions:**
1. Ensure SFML 3.0+ is installed
2. Set SFML_DIR: `cmake .. -DSFML_DIR=/path/to/sfml`
3. Install SFML development packages: `sudo apt install libsfml-dev`

#### GLM Not Found
```
Could NOT find glm (missing: glm_DIR)
```

**Solutions:**
1. Install GLM: `sudo apt install libglm-dev`
2. Set glm_DIR: `cmake .. -Dglm_DIR=/path/to/glm`
3. GLM is header-only, ensure headers are accessible

#### C++17 Support Issues
```
error: 'std::optional' is not a member of 'std'
```

**Solutions:**
1. Update compiler: GCC 8+, Clang 7+, MSVC 2019+
2. Explicitly set C++ standard: `cmake .. -DCMAKE_CXX_STANDARD=17`

#### Link-Time Optimization Issues
```
error: 'lto1' binary not found
```

**Solutions:**
1. Install LTO support: `sudo apt install gcc-plugin-dev`
2. Disable LTO: Edit CMakeLists.txt and remove `-flto` flag
3. Use a different compiler

#### Missing Graphics Libraries (Linux)
```
error while loading shared libraries: libsfml-graphics.so.3.0
```

**Solutions:**
1. Install SFML runtime: `sudo apt install libsfml-graphics3.0`
2. Update library path: `export LD_LIBRARY_PATH=/path/to/sfml/lib:$LD_LIBRARY_PATH`
3. Run `ldconfig` to refresh library cache

### Performance Issues

#### Slow Debug Builds
Debug builds are naturally slower due to disabled optimizations and debug symbols.

**Solutions:**
- Use Release builds for performance testing: `cmake .. -DCMAKE_BUILD_TYPE=Release`
- Use RelWithDebInfo for debugging with better performance

#### Large Executable Size
**Solutions:**
- Use MinSizeRel build type: `cmake .. -DCMAKE_BUILD_TYPE=MinSizeRel`
- Strip debug symbols: `strip Zenoa`
- Remove unused dependencies from CMakeLists.txt

### Build Cache Issues

If you encounter strange build errors:

```bash
# Clean build directory
rm -rf build/*

# Reconfigure from scratch
cmake ..
make clean
make
```

### Platform-Specific Issues

#### macOS: Command Line Tools
```
xcrun: error: invalid active developer path
```

**Solution:**
```bash
xcode-select --install
```

#### Windows: Visual Studio Version Conflicts
Ensure you're using consistent Visual Studio versions for all dependencies.

#### Linux: Missing Development Headers
```
fatal error: 'SFML/Graphics.hpp' file not found
```

**Solution:**
```bash
# Install development packages (headers)
sudo apt install libsfml-dev  # Ubuntu/Debian
sudo pacman -S sfml           # Arch
sudo dnf install SFML-devel   # Fedora
```

## Integration with IDEs

### Visual Studio Code
1. Install C/C++ extension
2. Install CMake Tools extension
3. Open project folder
4. Use Ctrl+Shift+P → "CMake: Configure"

### CLion
1. Open project folder
2. CLion will automatically detect CMakeLists.txt
3. Configure CMake settings in preferences

### Visual Studio
1. Use "Open Folder" feature
2. Visual Studio will detect CMakeLists.txt
3. Configure CMake settings in CMakeSettings.json

### Debugging Configuration

For debugging, ensure you build with debug symbols:

```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug
make
gdb ./Zenoa  # Linux
lldb ./Zenoa  # macOS
```