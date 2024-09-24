cmake_minimum_required(VERSION 3.16)
project(CMakeSFMLProject LANGUAGES CXX)

set(ASSETS_DIR "${CMAKE_SOURCE_DIR}/assets")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_definitions(-DASSETS_DIR="${ASSETS_DIR}")

add_executable(main src/main.cpp
        src/scenes/space.cpp
        src/scenes/space.hpp
        src/objects/player.cpp
        src/objects/player.hpp
        src/objects/meteor.cpp
        src/objects/meteor.hpp
        src/objects/label.cpp
        src/objects/label.hpp
        src/utils/quadtree.cpp
        src/utils/quadtree.hpp
        src/utils/AnimatedSprite.cpp
        src/utils/AnimatedSprite.hpp
        src/objects/projectile.cpp
        src/objects/projectile.hpp
        src/utils/Camera.cpp
        src/utils/Camera.hpp
        src/utils/random.hpp
)
target_link_libraries(main PRIVATE sfml-graphics sfml-audio)
target_compile_features(main PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET main
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:main>
        VERBATIM)
endif()
