cmake_minimum_required(VERSION 3.15)
project(PhysicsEngine LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -flto -DNDEBUG")

# Find SFML 3 (Graphics, Window, System, Audio as needed)
find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System Audio)

# Find GLM
find_package(glm REQUIRED)

# Recursively gather all .cpp files in src/
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link SFML 3 imported targets and GLM
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
        SFML::Audio
        glm::glm
)

# Include directories if needed (not required for SFML 3 or GLM if using targets)
# target_include_directories(${PROJECT_NAME} PRIVATE src)

# Optionally set warnings and other compile options
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
