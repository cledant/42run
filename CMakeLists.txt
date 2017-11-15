cmake_minimum_required(VERSION 3.8)
project(42run)

#Setup Glad
set(GLAD_API "gl=4.1")
add_subdirectory(glad ${CMAKE_BINARY_DIR}/glad/build)

#Setup GLFW
option(GLFW_BUILD_EXAMPLES "" OFF)
option(GLFW_BUILD_TESTS "" OFF)
option(GLFW_INSTALL "" OFF)
add_subdirectory(glfw ${CMAKE_BINARY_DIR}/glfw/build)

#Setup Assimp
option(BUILD_SHARED_LIBS "" OFF)
add_subdirectory(assimp ${CMAKE_BINARY_DIR}/assimp/build)

#Setup FreeType
add_subdirectory(freetype ${CMAKE_BINARY_DIR}/freetype/build)

#Setup 42Run
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Werror -Wextra -O2")
include_directories(includes glm stb glad/build/includes glfw/include/GLFW assimp/include
		assimp/build/include freetype/include)
link_directories(${CMAKE_BINARY_DIR}/glad/build
		${CMAKE_BINARY_DIR}/glfw/build/src
		${CMAKE_BINARY_DIR}/assimp/build/code
		${CMAKE_BINARY_DIR}/freetype/build)
set(SOURCE_FILES
		srcs/Camera.cpp
		srcs/Cubemap.cpp
		srcs/GeneralException.cpp
		srcs/Glfw_manager.cpp
		srcs/IEntity.cpp
		srcs/IInteractive.cpp
		srcs/Input.cpp
		srcs/main.cpp
		srcs/oGL_module.cpp
		srcs/Shader.cpp
		srcs/Simple_box.cpp
		srcs/Texture.cpp
		srcs/Window.cpp
		srcs/World.cpp
		srcs/Model.cpp
		srcs/Mesh.cpp
		srcs/Prop.cpp
		srcs/FontSet.cpp
		srcs/Ui.cpp
		srcs/CollisionBox.cpp
		srcs/ThirdPersonCamera.cpp)
#		srcs/Player.cpp)

#OS specific libs
if (APPLE)
	find_library(OPEN_GL OpenGL REQUIRED)
	find_library(COCOA COCOA REQUIRED)
	find_library(IOKIT IOkit REQUIRED)
	find_library(COREVIDEO CoreVideo REQUIRED)
	set(EXTRA_LIBS ${OPEN_GL} ${COCOA} ${IOKIT} ${COREVIDEO})
endif ()

add_executable(42run ${SOURCE_FILES})
target_link_libraries(42run glad glfw assimp freetype ${EXTRA_LIBS})
file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})
file(COPY textures DESTINATION ${CMAKE_BINARY_DIR})
file(COPY models DESTINATION ${CMAKE_BINARY_DIR})
file(COPY fonts DESTINATION ${CMAKE_BINARY_DIR})