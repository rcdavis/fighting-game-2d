project(Game)

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.2
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
    entt
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG v3.16.0
)
FetchContent_MakeAvailable(entt)

set(SPDLOG_HEADER_ONLY ON CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_BENCH OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.16.0
)
FetchContent_MakeAvailable(spdlog)

add_executable(Game
    glad/src/gl.c
    src/Render/OpenGL/GLBuffer.cpp
    src/Render/OpenGL/GLShader.cpp
    src/Render/OpenGL/GLTexture.cpp
    src/Render/OpenGL/GLVertexArray.cpp
    src/Render/OrthographicCamera.cpp
    src/Render/Renderer2D.cpp
    src/Utils/Log.cpp
    src/Game.cpp
    src/Main.cpp
    stb/src/stb_image.c
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res
     DESTINATION ${CMAKE_BINARY_DIR}/Game)

target_compile_definitions(Game PUBLIC
    GLFW_INCLUDE_NONE
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(Game PUBLIC
        DEBUG
        LOGGING_ENABLED
        ASSERTS_ENABLED
    )
endif()

target_include_directories(Game PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/stb/include
)

cmake_policy(SET CMP0072 NEW)
find_package(OpenGL REQUIRED)

target_link_libraries(Game PRIVATE
    EnTT::EnTT
    glm::glm
    glfw
    OpenGL::GL
    spdlog::spdlog_header_only
)
