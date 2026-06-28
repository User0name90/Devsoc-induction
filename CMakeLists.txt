cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)   
set(CMAKE_CXX_EXTENSIONS OFF)         

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(main 
    src/Center.cpp
    include/button.cpp
    include/sphere.cpp
    include/engine.cpp
    include/particle_splasher.cpp
    include/splasher_engine.cpp)
target_link_libraries(main PRIVATE
    SFML::Graphics
    SFML::Audio
    SFML::Network
)
target_include_directories(main PUBLIC ${CMAKE_SOURCE_DIR} include)
# target_compile_options(main PRIVATE
#     -Wall        # includes -Wreorder and many others
#     -Wextra      # extra warnings Wall doesn't cover
#     -Wpedantic   # strict standard compliance
#     -Wshadow     # warns when variable shadows another (like your count bug earlier)
# )

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)