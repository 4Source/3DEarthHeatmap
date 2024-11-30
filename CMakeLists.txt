cmake_minimum_required (VERSION 3.5)
project (3DEarthHeatmap)
include(FetchContent) # once in the project to include the module
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenGL REQUIRED)

# find or download stb image lib 
find_path(STB_INCLUDE stb_image.h)
if (NOT STB_INCLUDE_FOUND)
    FetchContent_Declare(
        stb
        GIT_REPOSITORY https://github.com/nothings/stb.git
        GIT_TAG beebb24b945efdea3b9bba23affb8eb3ba8982e7
    )
    FetchContent_MakeAvailable(stb)
    set(STB_INCLUDE ${stb_SOURCE_DIR})
endif()

# find or download glfw lib
find_path(GLFW_INCLUDE glfw3.h)
if (NOT GLFW_INCLUDE_FOUND)
    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw
        GIT_TAG 7b6aead9fb88b3623e3b3725ebb42670cbe4c579
    )
    FetchContent_MakeAvailable(glfw)
    set(GLFW_INCLUDE ${glfw_SOURCE_DIR})
endif()

# find of download glm lib
find_path(GLM_INCLUDE glm.hpp)
if (NOT GLM_INCLUDE_FOUND)
    FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 0af55ccecd98d4e5a8d1fad7de25ba429d60e863
    )
    FetchContent_MakeAvailable(glm)
    set(GLM_INCLUDE ${glm_SOURCE_DIR})
endif()

# Compile external dependencies 
add_subdirectory (external)

include_directories(
    ${STB_INCLUDE} 
    ${GLFW_INCLUDE} 
    external/glew-1.13.0/include/ 
    ${GLM_INCLUDE}
)
add_executable(3DEarthHeatmap 
    src/main.cpp
    src/Shader.cpp
    src/VertexBufferObject.cpp
    src/ElementBufferObject.cpp
    src/VertexArrayObject.cpp
)

include_directories(
    ./include/
)

target_link_libraries(3DEarthHeatmap 
    glfw 
    GLEW_1130  
    glm
)
