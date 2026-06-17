cmake_minimum_required (VERSION 3.8)

project ("N-Body-Simulation")

include(FetchContent)
FetchContent_Declare(nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(SFML)

add_executable (N-Body-Simulation "src/main.cpp" "src/concurrency/ThreadPool.cpp" "src/physics/integrator.cpp" "src/renderer/camera.cpp" "src/renderer/SFMLrenderer.cpp" "src/simulation/BarnesHutTree.cpp" "src/simulation/simulation.cpp" "src/simulation/scenarios.cpp" )

set_property(TARGET N-Body-Simulation PROPERTY CXX_STANDARD 20)
set_property(TARGET N-Body-Simulation PROPERTY CXX_STANDARD_REQUIRED ON)

target_compile_definitions(N-Body-Simulation PRIVATE SOURCE_DIR="${CMAKE_SOURCE_DIR}")

target_include_directories(N-Body-Simulation PRIVATE
    include/concurrency
    include/physics
    include/renderer
    include/simulation
    include/
    external/glm
)

target_link_libraries(N-Body-Simulation PRIVATE SFML::System SFML::Window SFML::Graphics nlohmann_json::nlohmann_json)