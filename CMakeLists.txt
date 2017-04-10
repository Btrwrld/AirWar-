cmake_minimum_required(VERSION 3.6)
project(AirWar__)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp GameFramework/SOIL.h GameFramework/Game.cpp GameFramework/Game.h GameFramework/Shader.cpp
        GameFramework/Shader.h GameFramework/Texture.cpp GameFramework/Texture.h GameFramework/ResourceManager.cpp
        GameFramework/ResourceManager.h GameFramework/SpriteRenderer.cpp GameFramework/SpriteRenderer.h Game/GameObject.cpp
        Game/GameObject.h Game/GameManager.cpp Game/GameManager.h DataStructures/QueueNode.h
        DataStructures/Queue.h)
#add_executable(AirWar__ ${SOURCE_FILES})

# Find glfw header
find_path(GLFW_INCLUDE_DIR GLFW/glfw3.h ${HEADER_SEARCH_PATH})


# Find glfw library
find_library(GLFW_LIBRARIES glfw3 ${LIBRARY_SEARCH_PATH})
# Set additional libs
set( ADDITIONAL_LIBS
        -lGLEW
        -lGLU
        -lGL
        -lglfw3
        -pthread
        -lrt
        -lX11
        -lXi
        -lXrandr
        -lXxf86vm
        -lXinerama
        -lXcursor
        -ldl
        -lSOIL
        )

# Include directories for this project
set(INCLUDE_PATH
        ${OPENGL_INCLUDE_DIR}
        ${GLFW_INCLUDE_DIR}
        )


# Libraries needed on all platforms for this project
set(LIBRARIES
        ${OPENGL_LIBRARIES}
        ${GLFW_LIBRARIES}
        ${X11_LIBRARIES}
        ${ADDITIONAL_LIBS}
        )


# Windows and Linux need GLEW, the OpenGL Extension Wrangler
find_path(GLEW_INCLUDE_DIR GL/glew.h
        ${HEADER_SEARCH_PATH}
        )


set(INCLUDE_PATH ${INCUDE_PATH}
        ${GLEW_INCLUDE_DIR}
        )


# Add a target executable
add_executable(AirWar__ ${SOURCE_FILES})


# Libraries to be linked
target_link_libraries(AirWar__
        ${LIBRARIES}
        )

#find_package(OpenGL REQUIRED)
#find_package(GLUT REQUIRED)
#find_package(GLEW REQUIRED)
#include_directories( ${OPENGL_INCLUDE_DIRS} ${SOIL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} )

#target_link_libraries(AirWar__ ${OPENGL_LIBRARIES} ${SOIL_LIBRARY} ${GLUT_LIBRARY} ${GLEW_LIBRARY})

